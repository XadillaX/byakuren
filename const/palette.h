/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#ifndef CONST_PALETTE_H_
#define CONST_PALETTE_H_
#include "../bkr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Default color palette used for color quantization and analysis.
 *
 * This constant represents a predefined color palette containing 256 RGB
 * colors. It is used as the default palette in various color processing
 * functions throughout the Byakuren library when a custom palette is not
 * provided.
 *
 * The palette includes a wide range of colors, including:
 * - Basic colors (black, white, primary and secondary colors)
 * - Shades of gray
 * - Various hues and saturations to cover a broad spectrum of the color space
 *
 * This default palette is designed to provide a good balance between color
 * accuracy and computational efficiency for most general-purpose color
 * quantization tasks.
 */
extern const bkr_palette_array _default_palette;

#ifdef __cplusplus
}
#endif

#endif  // CONST_PALETTE_H_
