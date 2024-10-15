/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#ifndef LIB_MIX_H_
#define LIB_MIX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../bkr_common.h"

/**
 * @brief Calculate color statistics using a mixed approach of octree
 * quantization and minimum difference method.
 *
 * This function combines the efficiency of octree color quantization with the
 * accuracy of the minimum difference method to produce high-quality color
 * statistics. The process involves several steps:
 *
 * 1. Octree quantization is applied to the input pixels to reduce the number of
 * colors to a manageable set.
 * 2. The reduced color set is then mapped to a predefined palette using the
 * minimum difference method.
 * 3. The results are compiled into color statistics, sorted, and returned.
 *
 * This mixed approach allows for faster processing of large images while still
 * maintaining color accuracy and adhering to a specific color palette.
 *
 * @param pixels Pointer to an array of bkr_rgb structures representing the
 * pixels to process.
 * @param pixel_count The number of pixels in the pixels array.
 * @param octree_max_colors The maximum number of colors to use in the octree
 * quantization step.
 * @param mindiff_param Pointer to a bkr_mindiff_parameter structure containing
 * parameters for the minimum difference calculation.
 * @param stats Pointer to an array of bkr_color_stats structures where the
 * results will be stored.
 *
 * @return The number of unique colors found in the statistics, or -1 if an
 * error occurred.
 */
extern int bkr_mix_calculate_color_stats(bkr_rgb* pixels,
                                         uint32_t pixel_count,
                                         uint32_t octree_max_colors,
                                         bkr_mindiff_parameter* mindiff_param,
                                         bkr_color_stats stats[]);

#ifdef __cplusplus
}
#endif

#endif  // LIB_MIX_H_
