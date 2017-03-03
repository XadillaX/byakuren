/*
 * =====================================================================================
 *
 *       Filename:  octree.h
 *
 *    Description:  Octree of cthmclrx.
 *
 *        Version:  1.0
 *        Created:  2014/12/09 17时43分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadilalX Scarlet
 *   Organization:  Gensokyo
 *
 * =====================================================================================
 */
#ifndef __BKR_OCTREE_H__
#define __BKR_OCTREE_H__
#include "../common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bkr_octree_node* bkr_build_octree(bkr_rgb* pixels, uint32_t pixel_count, uint32_t max_colors);
extern void bkr_release_octree(bkr_octree_node* node);
extern int bkr_octree_calculate_color_stats(bkr_octree_node* node, bkr_color_stats stats[]);

#ifdef __cplusplus
}
#endif

#endif

