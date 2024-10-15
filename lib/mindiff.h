/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#ifndef LIB_MINDIFF_H_
#define LIB_MINDIFF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../bkr_common.h"

/**
 * @brief Color palette statistics structure.
 *
 * This structure is used to store information about a color in the palette
 * and its frequency of occurrence in the image being processed.
 */
typedef struct _bkr_palette_stats {
  bkr_rgb* color; /**< Pointer to the RGB color. */
  uint32_t count; /**< Frequency of the color's occurrence. */
} _bkr_palette_stats;

/**
 * @brief Calculate color statistics using the minimum difference method.
 *
 * This function processes an array of pixels and calculates color statistics
 * based on a given or default color palette. It uses the minimum difference
 * method to map each pixel to the closest color in the palette.
 *
 * The function separates colors into colored and grayscale palettes based on
 * the gray_offset parameter. It then processes each pixel, finding the closest
 * color in the appropriate palette, and updates the count for that color.
 *
 * After processing all pixels, it compiles the final color statistics,
 * including only colors with non-zero counts, and sorts them for consistent
 * output.
 *
 * @param pixels Pointer to an array of bkr_rgb structures representing the
 * pixels to process.
 * @param pixel_count The number of pixels in the pixels array.
 * @param stats Pointer to an array of bkr_color_stats structures where the
 * results will be stored.
 * @param param Pointer to a bkr_mindiff_parameter structure containing
 * processing parameters.
 *
 * @return The number of unique colors found in the statistics, or -1 if an
 * error occurred.
 */
extern int bkr_mindiff_calculate_color_stats(bkr_rgb* pixels,
                                             uint32_t pixel_count,
                                             bkr_color_stats stats[],
                                             bkr_mindiff_parameter* param);

#ifdef __cplusplus
}
#endif

#endif  // LIB_MINDIFF_H_
