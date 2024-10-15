/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#include "mindiff.h"
#include <string.h>

#include "../const/palette.h"

static uint32_t bkr_rgb_size = sizeof(bkr_rgb);
static uint32_t stats_size = sizeof(bkr_color_stats);

uint32_t _bkr_diff_square(bkr_rgb* a, bkr_rgb* b) {
  return (a->red - b->red) * (a->red - b->red) +
         (a->green - b->green) * (a->green - b->green) +
         (a->blue - b->blue) * (a->blue - b->blue);
}

int32_t _bkr_find_mindiff(bkr_rgb* a,
                          _bkr_palette_stats* palette,
                          uint32_t palette_count) {
  uint32_t min_diff = MAX_INT;
  uint32_t temp_diff = 0;
  int32_t min_diff_idx = -1;

  for (uint32_t i = 0; i < palette_count; i++) {
    temp_diff = _bkr_diff_square(a, palette[i].color);

    if (min_diff_idx == -1 || min_diff > temp_diff) {
      min_diff_idx = i;
      min_diff = temp_diff;
    }
  }

  return min_diff_idx;
}

static inline int _bkr_mindiff_calc(bkr_rgb* pixels,
                                    uint32_t pixel_count,
                                    int16_t gray_offset,
                                    _bkr_palette_stats* colored,
                                    uint32_t colored_count,
                                    _bkr_palette_stats* grayed,
                                    uint32_t grayed_count,
                                    bkr_color_stats stats[]) {
  int32_t index;
  _bkr_palette_stats* current_palette;
  for (uint32_t i = 0; i < pixel_count; i++) {
    // Determine whether to use the colored or grayed palette based on the
    // pixel's properties and the gray_offset.
    current_palette = (gray_offset < 0 || !BKR_IS_GRAY(pixels + i, gray_offset))
                          ? colored
                          : grayed;
    index = _bkr_find_mindiff(
        pixels + i,
        current_palette,
        current_palette == colored ? colored_count : grayed_count);
    current_palette[index].count++;
  }

  // Compile the final color statistics by combining results from both palettes.
  // Only colors with non-zero counts are included in the final statistics.
  int answer_count = 0;
  for (uint32_t i = 0; i < colored_count; i++) {
    if (!colored[i].count) continue;
    memcpy(&stats[answer_count].color, (colored[i].color), bkr_rgb_size);
    stats[answer_count].value = BKR_RGB_TO_INT32(
        colored[i].color->red, colored[i].color->green, colored[i].color->blue);
    stats[answer_count++].count = colored[i].count;
  }
  for (uint32_t i = 0; i < grayed_count; i++) {
    if (!grayed[i].count) continue;
    memcpy(&stats[answer_count].color, (grayed[i].color), bkr_rgb_size);
    stats[answer_count].value = BKR_RGB_TO_INT32(
        grayed[i].color->red, grayed[i].color->green, grayed[i].color->blue);
    stats[answer_count++].count = grayed[i].count;
  }

  // Sort the final color statistics to ensure consistent output.
  qsort(stats, answer_count, stats_size, _stats_cmp);

  return answer_count;
}

uint8_t _bkr_generate_colored_and_gray_palette(bkr_palette_array* palette,
                                               _bkr_palette_stats** p_colored,
                                               _bkr_palette_stats** p_grayed,
                                               uint32_t* colored_count,
                                               uint32_t* grayed_count,
                                               int16_t gray_offset) {
  _bkr_palette_stats* colored;
  _bkr_palette_stats* grayed;

  *colored_count = *grayed_count = 0;

  // Allocate memory for colored and grayscale palettes.
  colored = (_bkr_palette_stats*)  // NOLINT(readability/casting)
      calloc(palette->count, sizeof(_bkr_palette_stats));
  grayed = (_bkr_palette_stats*)  // NOLINT(readability/casting)
      calloc(palette->count, sizeof(_bkr_palette_stats));

  // Check if memory allocation was successful. If not, free any allocated
  // memory and return 0 to indicate failure.
  if (!colored || !grayed) {
    SAFE_FREE(colored);
    SAFE_FREE(grayed);
    return 0;
  }

  *p_colored = colored;
  *p_grayed = grayed;

  if (gray_offset < 0) {
    // If gray offset is unlimited, treat all colors as colored.
    // This effectively disables grayscale processing.
    for (uint32_t i = 0; i < palette->count; i++) {
      colored[i].color = palette->colors + i;
      colored[i].count = 0;
    }
    *colored_count = palette->count;
    grayed_count = 0;
  } else {
    // Separate colors into colored and grayscale palettes based on their
    // properties.
    bkr_rgb* temp_palette = palette->colors;
    for (uint32_t i = 0; i < palette->count; i++) {
      if (temp_palette[i].blue == temp_palette[i].red &&
          temp_palette[i].red == temp_palette[i].green) {
        // If R=G=B, the color is considered grayscale.
        grayed[*grayed_count].color = temp_palette + i;
        grayed[(*grayed_count)++].count = 0;
      } else {
        // Otherwise, it's considered a color.
        colored[*colored_count].color = temp_palette + i;
        colored[(*colored_count)++].count = 0;
      }
    }

    // Handle edge cases where all colors are either colored or grayscale.
    // In such cases, treat all colors as colored and set gray_offset to -1.
    if (!grayed_count || !colored_count) {
      gray_offset = -1;
      if (grayed_count) {
        memcpy(colored, grayed, sizeof(_bkr_palette_stats) * palette->count);
        memset(grayed, 0, sizeof(_bkr_palette_stats) * palette->count);

        colored_count = grayed_count;
        grayed_count = 0;
      }
    }
  }

  return 1;
}

int bkr_mindiff_calculate_color_stats(bkr_rgb* pixels,
                                      uint32_t pixel_count,
                                      bkr_color_stats stats[],
                                      bkr_mindiff_parameter* param) {
  bkr_palette_array* total_palette = 0;
  _bkr_palette_stats* colored = 0;
  _bkr_palette_stats* grayed = 0;

  uint32_t colored_count = 0;
  uint32_t grayed_count = 0;
  // Use the provided palette if available, otherwise use the default palette.
  total_palette = param->palette
                      ? param->palette
                      : (bkr_palette_array*)  // NOLINT(readability/casting)
                      (&_default_palette);

  if (!total_palette->count) return -1;

  // Generate separate palettes for colored and grayscale colors.
  if (!_bkr_generate_colored_and_gray_palette(total_palette,
                                              &colored,
                                              &grayed,
                                              &colored_count,
                                              &grayed_count,
                                              param->gray_offset)) {
    return -1;
  }

  // Perform the main calculation of color statistics.
  int result = _bkr_mindiff_calc(pixels,
                                 pixel_count,
                                 param->gray_offset,
                                 colored,
                                 colored_count,
                                 grayed,
                                 grayed_count,
                                 stats);

  // Clean up allocated memory to prevent memory leaks.
  SAFE_FREE(colored);
  SAFE_FREE(grayed);

  return result;
}
