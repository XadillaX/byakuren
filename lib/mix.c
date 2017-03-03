/*
 * =====================================================================================
 *
 *       Filename:  mix.c
 *
 *    Description:  mix of cthmclrx.
 *
 *        Version:  1.0
 *        Created:  2014/12/17 20时32分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX Scarlet
 *   Organization:  Gensokyo
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mix.h"
#include "octree.h"
#include "mindiff.h"
#include "../const/palette.h"

extern uint8_t  _bkr_generate_colored_and_gray_pelette(bkr_palette_array* palette, _bkr_palette_stats** p_colored,
        _bkr_palette_stats** p_grayed, uint32_t* colored_count, uint32_t* grayed_count, int16_t gray_offset);
extern uint32_t _bkr_diff_square(bkr_rgb* a, bkr_rgb* b);
extern int32_t  _bkr_find_mindiff(bkr_rgb* a, _bkr_palette_stats* palette, uint32_t palette_count);

static uint32_t bkr_rgb_size = sizeof(bkr_rgb);
static uint32_t stats_size   = sizeof(bkr_color_stats);

static inline int _bkr_true_mix_calculate(bkr_color_stats octree_stats[], uint32_t octree_stats_count, int16_t gray_offset,
        _bkr_palette_stats* colored, uint32_t colored_count, _bkr_palette_stats* grayed, uint32_t grayed_count, bkr_color_stats stats[])
{
    int32_t index;
    _bkr_palette_stats* current_palette;
    for(uint32_t i = 0; i < octree_stats_count; i++)
    {
        // get current palette and the neareast one
        // color.
        current_palette = (gray_offset < 0 || !BKR_IS_GRAY(&(octree_stats + i)->color, gray_offset)) ? colored : grayed;
        index = _bkr_find_mindiff((bkr_rgb*)((bkr_color_stats*)(octree_stats + i)), current_palette,
                current_palette == colored ? colored_count : grayed_count);
        current_palette[index].count += octree_stats[i].count;
    }

    // get answer!
    int answer_count = 0;
    for(uint32_t i = 0; i < colored_count; i++)
    {
        if(!colored[i].count) continue;
        memcpy(&stats[answer_count].color, (colored[i].color), bkr_rgb_size);
        stats[answer_count].value   = BKR_RGB_TO_INT32(colored[i].color->red, colored[i].color->green, colored[i].color->blue);
        stats[answer_count++].count = colored[i].count;
    }
    for(uint32_t i = 0; i < grayed_count; i++)
    {
        if(!grayed[i].count) continue;
        memcpy(&stats[answer_count].color, (grayed[i].color), bkr_rgb_size);
        stats[answer_count].value   = BKR_RGB_TO_INT32(grayed[i].color->red, grayed[i].color->green, grayed[i].color->blue);
        stats[answer_count++].count = grayed[i].count;
    }

    // sort for answer
    qsort(stats, answer_count, stats_size, _stats_cmp);

    return answer_count;
}

int bkr_mix_calculate_color_stats(bkr_rgb* pixels, uint32_t pixel_count,
        uint32_t octree_max_colors, bkr_mindiff_parameter* mindiff_param, bkr_color_stats stats[])
{
    // let's octree first!
    bkr_color_stats* octree_stats = malloc(sizeof(bkr_color_stats) * octree_max_colors);
    bkr_octree_node* root         = bkr_build_octree(pixels, pixel_count, octree_max_colors);
    if(!root) return -1;
    int colors = bkr_octree_calculate_color_stats(root, octree_stats);
    bkr_release_octree(root);

    // let's get the `mindiff` parameters!
    bkr_palette_array* total_palette = 0;
    _bkr_palette_stats* colored      = 0;
    _bkr_palette_stats* grayed       = 0;

    uint32_t colored_count = 0;
    uint32_t grayed_count  = 0;
    total_palette          = mindiff_param->palette ? mindiff_param->palette : (bkr_palette_array*)(&_default_palette);

    // if it's wrong palette
    if(!total_palette->count)
    {
        free(octree_stats);
        return -1;
    }

    // if we can't generate colored and gray palettes
    if(!_bkr_generate_colored_and_gray_pelette(total_palette, &colored,
                &grayed, &colored_count, &grayed_count, mindiff_param->gray_offset))
    {
        return -1;
    }

    int answer_count = _bkr_true_mix_calculate(octree_stats, colors, mindiff_param->gray_offset, colored,
            colored_count, grayed, grayed_count, stats);

    free(octree_stats);

    SAFE_FREE(colored);
    SAFE_FREE(grayed);

    return answer_count;
}

