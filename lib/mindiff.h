/*
 * =====================================================================================
 *
 *       Filename:  mindiff.h
 *
 *    Description:  Mindiff of cthmclrx.
 *
 *        Version:  1.0
 *        Created:  2014/12/17 10时13分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX Scarlet
 *   Organization:  Gensokyo
 *
 * =====================================================================================
 */
#ifndef __BKR_MINDIFF_H__
#define __BKR_MINDIFF_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "../common.h"

/**
 * 色板统计结构体
 */
typedef struct _bkr_palette_stats {
    bkr_rgb* color;
    uint32_t count;
} _bkr_palette_stats;

extern int bkr_mindiff_calculate_color_stats(bkr_rgb* pixels, uint32_t pixel_count, bkr_color_stats stats[], bkr_mindiff_parameter* param);

#ifdef __cplusplus
}
#endif
#endif

