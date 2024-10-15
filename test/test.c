/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../byakuren.h"

#define RGB_FILENAME "test.rgb"

void print_stats(bkr_color_stats* stats, int count) {
  printf("{\n");
  printf("\"count\": %d,\n", count);
  printf("\"colors\": [\n");
  for (int i = 0; i < count; i++) {
    printf("{\"color\": %d, \"count\": %d}%c\n",
           stats[i].value,
           stats[i].count,
           i == count - 1 ? ' ' : ',');
  }
  printf("]\n");
  printf("}\n");
}

void calc_octree(bkr_rgb* rgb, int color_count) {
  bkr_color_stats stats[256];
  bkr_octree_node* root = bkr_build_octree(rgb, color_count, 256);
  int colors = bkr_octree_calculate_color_stats(root, stats);
  bkr_release_octree(root);
  print_stats(stats, colors);
}

void calc_mindiff(bkr_rgb* rgb, int color_count) {
  bkr_color_stats stats[256];
  bkr_mindiff_parameter param;
  param.gray_offset = 5;
  param.palette = NULL;

  int colors =
      bkr_mindiff_calculate_color_stats(rgb, color_count, stats, &param);
  print_stats(stats, colors);
}

void calc_mix(bkr_rgb* rgb, int color_count) {
  bkr_color_stats stats[256];
  bkr_mindiff_parameter param;
  param.gray_offset = -1;
  param.palette = NULL;

  int colors =
      bkr_mix_calculate_color_stats(rgb, color_count, 256, &param, stats);
  print_stats(stats, colors);
}

int main(int argc, char* argv[]) {
  bkr_init();

  if (argc <= 1) {
    printf("need algorithm.\n");
    return 4;
  }

  // open the file & read RGB value
  struct stat file_stat;
  if (lstat(RGB_FILENAME, &file_stat) < 0) {
    printf("lstat error\n");
    return 4;
  }

  int file_length = file_stat.st_size;
  if (file_length % 3) {
    printf("broken RGB file length.\n");
    return 4;
  }

  // read the file
  int color_count = file_length / 3;
  bkr_rgb* rgb = (bkr_rgb*)  // NOLINT(readability/casting)
      malloc(sizeof(bkr_rgb) * color_count);
  FILE* fp = fopen(RGB_FILENAME, "rb");
  if (!fp) {
    printf("can't open RGB file.\n");
    return 4;
  }
  fread(rgb, sizeof(bkr_rgb), color_count, fp);
  fclose(fp);

  // start!
  if (!strcmp("octree", argv[1])) {
    calc_octree(rgb, color_count);
  } else if (!strcmp("mindiff", argv[1])) {
    calc_mindiff(rgb, color_count);
  } else if (!strcmp("mix", argv[1])) {
    calc_mix(rgb, color_count);
  } else {
    printf("no such algorithm yet.\n");
    return 4;
  }

  bkr_destroy();

  return 0;
}
