/*
 * =====================================================================================
 *
 *       Filename:  common.c
 *
 *    Description:  common c file.
 *
 *        Version:  1.0
 *        Created:  2014/12/09 18时14分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX Scarlet
 *   Organization:  Gensokyo
 *
 * =====================================================================================
 */
#include "common.h"

extern int  _bkr_init_octree_env();
extern void _bkr_destroy_octree_env();

xmem_pool_handle bkr_rgb_pool = 0;

int bkr_init()
{
    if(!bkr_rgb_pool)
    {
        bkr_rgb_pool = xmem_create_pool(sizeof(bkr_rgb));
    }

    if(!bkr_rgb_pool) return 0;

    if(!_bkr_init_octree_env())
    {
        return 0;
    }

    return 1;
}

void bkr_destroy()
{
    SAFE_DESTROY_POOL(bkr_rgb_pool);
    _bkr_destroy_octree_env();
}

int _stats_cmp(const void* a, const void* b)
{
    if(((bkr_color_stats*)a)->count != ((bkr_color_stats*)b)->count)
    {
        return -(((bkr_color_stats*)a)->count - ((bkr_color_stats*)b)->count);
    }
    
    return -(((bkr_color_stats*)a)->value - ((bkr_color_stats*)b)->value);
}

