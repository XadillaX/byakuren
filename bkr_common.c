/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#include "bkr_common.h"

extern int _bkr_init_octree_env();
extern void _bkr_destroy_octree_env();

xmem_pool_handle bkr_rgb_pool = 0;

int bkr_init() {
  if (!bkr_rgb_pool) {
    bkr_rgb_pool = xmem_create_pool(sizeof(bkr_rgb));
  }

  if (!bkr_rgb_pool) return 0;

  if (!_bkr_init_octree_env()) {
    return 0;
  }

  return 1;
}

void bkr_destroy() {
  SAFE_DESTROY_POOL(bkr_rgb_pool);
  _bkr_destroy_octree_env();
}

int _stats_cmp(const void* a, const void* b) {
  if (((bkr_color_stats*)a)->count !=        // NOLINT(readability/casting)
      ((bkr_color_stats*)b)->count) {        // NOLINT(readability/casting)
    return -(((bkr_color_stats*)a)->count -  // NOLINT(readability/casting)
             ((bkr_color_stats*)b)->count);  // NOLINT(readability/casting)
  }

  return -(((bkr_color_stats*)a)->value -  // NOLINT(readability/casting)
           ((bkr_color_stats*)b)->value);  // NOLINT(readability/casting)
}
