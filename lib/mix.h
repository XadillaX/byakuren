/*
 * =====================================================================================
 *
 *       Filename:  mix.h
 *
 *    Description:  mix in cthmclrx.
 *
 *        Version:  1.0
 *        Created:  2014/12/17 17时58分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX Scarlet
 *   Organization:  Gensokyo
 *
 * =====================================================================================
 */
#ifndef __BKR_MIX_H__
#define __BKR_MIX_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "../common.h"

extern int bkr_mix_calculate_color_stats(bkr_rgb* pixels, uint32_t pixel_count, uint32_t octree_max_colors, bkr_mindiff_parameter* mindiff_param, bkr_color_stats stats[]);

#ifdef __cplusplus
}
#endif
#endif

