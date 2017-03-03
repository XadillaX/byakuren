/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description:  common header.
 *
 *        Version:  1.0
 *        Created:  2014/12/09 17时55分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX Scarlet
 *   Organization:  Gensokyo
 *
 * =====================================================================================
 */
#ifndef __BKR_COMMON_H__
#define __BKR_COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "third-party/xmempool/xmempool.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL (0)
#endif

#define SAFE_DESTROY_POOL(pool)     if(pool) { xmem_destroy_pool(pool); pool = NULL; }
#define SAFE_FREE(pointer)          if(pointer) { free(pointer); pointer = NULL; }
#define MAX_INT                     (2147483647)
#define BKR_RGB_TO_INT32(r, g, b)   (((r) << 16) + ((g) << 8) + (b))
#define BKR_IS_GRAY(a, offset) (abs((a)->red - (a)->green) <= offset && \
        abs((a)->red - (a)->blue) <= offset && \
        abs((a)->green - (a)->blue) <= offset)

/**
 * RGB 颜色结构体
 */
typedef struct bkr_rgb {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} bkr_rgb;

/**
 * 主题色结果（颜色、色值、像素数）结构体
 */
typedef struct bkr_color_stats {
    bkr_rgb  color;
    uint32_t value;
    uint32_t count;
} bkr_color_stats;

/**
 * 八叉树节点结构体
 */
typedef struct bkr_octree_node {
    uint32_t red_components;
    uint32_t green_components;
    uint32_t blue_components;

    uint8_t  is_leaf;
    uint32_t pixel_count;

    struct bkr_octree_node* children[8];
} bkr_octree_node;

/**
 * 八叉树可删减链表节点结构体
 */
typedef struct bkr_octree_reducible_list_node {
    struct bkr_octree_node*                node;
    struct bkr_octree_reducible_list_node* next;
} bkr_octree_reducible_list_node;

/**
 * 标准色板结构体
 */
typedef struct bkr_palette_array {
    uint32_t count;
    bkr_rgb* colors;
} bkr_palette_array;

/**
 * 最小差值法参数结构体
 */
typedef struct bkr_mindiff_parameter {
    bkr_palette_array* palette;
    int16_t            gray_offset;
} bkr_mindiff_parameter;


// *          *    *          *
// ** 注意没有对多线程作支持 **
//    主要是因为内存池没支持
// ** ---------------------- **
// *                          *
extern xmem_pool_handle bkr_rgb_pool;
extern int              bkr_init();
extern void             bkr_destroy();

// 该函数为颜色结果排序的函数
extern int _stats_cmp(const void* a, const void* b);

#ifdef __cplusplus
}
#endif

#endif

