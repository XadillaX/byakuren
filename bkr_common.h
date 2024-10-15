/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#ifndef BKR_COMMON_H_
#define BKR_COMMON_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "third-party/xmempool/xmempool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Version string of the Byakuren library.
 */
#define BKR_VERSION "1.0.1"

#ifndef NULL
#define NULL (0)
#endif

/**
 * @brief Safely destroy a memory pool.
 *
 * This macro checks if the pool exists before destroying it and sets the
 * pointer to NULL afterwards.
 */
#define SAFE_DESTROY_POOL(pool)                                                \
  if (pool) {                                                                  \
    xmem_destroy_pool(pool);                                                   \
    pool = NULL;                                                               \
  }

/**
 * @brief Safely free allocated memory.
 *
 * This macro checks if the pointer is not NULL before freeing it and sets
 * the pointer to NULL afterwards.
 */
#define SAFE_FREE(pointer)                                                     \
  if (pointer) {                                                               \
    free(pointer);                                                             \
    pointer = NULL;                                                            \
  }

/**
 * @brief Maximum value for a 32-bit signed integer.
 */
#define MAX_INT (2147483647)

/**
 * @brief Convert RGB values to a 32-bit integer representation.
 */
#define BKR_RGB_TO_INT32(r, g, b) (((r) << 16) + ((g) << 8) + (b))

/**
 * @brief Check if the difference between two color components is within a given
 * offset.
 */
#define DIFF_A_B_GTE_OFFSET(item, a, b, offset)                                \
  (abs((item)->a - (item)->b) <= offset)

#define BKR_IS_GRAY_INNER(item, offset)                                        \
  DIFF_A_B_GTE_OFFSET(item, red, green, offset) &&                             \
      DIFF_A_B_GTE_OFFSET(item, red, blue, offset) &&                          \
      DIFF_A_B_GTE_OFFSET(item, green, blue, offset)

/**
 * @brief Check if a color is considered grayscale based on a given offset.
 *
 * This macro checks if the differences between red, green, and blue components
 * are all within the specified offset.
 */
#define BKR_IS_GRAY(item, offset) (BKR_IS_GRAY_INNER(item, offset))

/**
 * @brief RGB color structure.
 *
 * This structure represents a color in the RGB color space.
 * Each component (red, green, blue) is an 8-bit unsigned integer,
 * allowing for 256 levels of intensity for each color channel.
 */
typedef struct bkr_rgb {
  uint8_t red;   /**< Red component of the color (0-255) */
  uint8_t green; /**< Green component of the color (0-255) */
  uint8_t blue;  /**< Blue component of the color (0-255) */
} bkr_rgb;

/**
 * @brief Color statistics structure.
 *
 * This structure holds information about a specific color, including
 * its RGB values, a 32-bit integer representation, and its frequency count.
 * It is used to store results of color analysis operations.
 */
typedef struct bkr_color_stats {
  bkr_rgb color;  /**< RGB color values */
  uint32_t value; /**< 32-bit integer representation of the color */
  uint32_t count; /**< Frequency count of the color */
} bkr_color_stats;

/**
 * @brief Octree node structure for color quantization.
 *
 * This structure represents a node in the octree used for color quantization.
 * It stores color component sums, leaf status, pixel count, and pointers to
 * child nodes.
 */
typedef struct bkr_octree_node {
  uint32_t red_components;   /**< Sum of red components */
  uint32_t green_components; /**< Sum of green components */
  uint32_t blue_components;  /**< Sum of blue components */

  uint8_t is_leaf;      /**< Flag indicating if this node is a leaf */
  uint32_t pixel_count; /**< Number of pixels represented by this node */

  struct bkr_octree_node* children[8]; /**< Pointers to child nodes */
} bkr_octree_node;

/**
 * @brief Reducible list node structure for octree color quantization.
 *
 * This structure is used to maintain a list of reducible nodes in the octree.
 * It is part of the mechanism for merging nodes during color reduction.
 */
typedef struct bkr_octree_reducible_list_node {
  struct bkr_octree_node* node; /**< Pointer to the octree node */
  struct bkr_octree_reducible_list_node*
      next; /**< Pointer to the next list node */
} bkr_octree_reducible_list_node;

/**
 * @brief Color palette array structure.
 *
 * This structure represents a color palette, containing a count of colors
 * and a pointer to an array of RGB color structures.
 */
typedef struct bkr_palette_array {
  uint32_t count;  /**< Number of colors in the palette */
  bkr_rgb* colors; /**< Pointer to an array of RGB colors */
} bkr_palette_array;

/**
 * @brief Parameters for the minimum difference color quantization method.
 *
 * This structure holds parameters used in the minimum difference method
 * for color quantization, including a custom palette and grayscale offset.
 */
typedef struct bkr_mindiff_parameter {
  bkr_palette_array* palette; /**< Pointer to a custom color palette */
  int16_t gray_offset;        /**< Offset for determining grayscale colors */
} bkr_mindiff_parameter;

/**
 * @brief Memory pool handle for RGB color structures.
 *
 * This global variable holds the handle to a memory pool used for
 * allocating bkr_rgb structures. It improves memory allocation efficiency
 * for frequently used RGB color objects.
 *
 * @note This library does not support multi-threading, primarily due to
 * limitations of the memory pool implementation.
 */
extern xmem_pool_handle bkr_rgb_pool;

/**
 * @brief Initialize the Byakuren library.
 *
 * @brief This function initializes the necessary resources for the Byakuren
 * library.
 *
 * It creates a memory pool for RGB color structures and initializes the octree
 * environment. This function should be called before using any other Byakuren
 * library functions.
 *
 * @return 1 if initialization is successful, 0 otherwise.
 */
extern int bkr_init();

/**
 * @brief Clean up and release resources used by the Byakuren library.
 *
 * @brief This function destroys the memory pools and frees any allocated
 * resources.
 *
 * It should be called when the Byakuren library is no longer needed to prevent
 * memory leaks and ensure proper cleanup of resources.
 */
extern void bkr_destroy();

/**
 * @brief Comparison function for sorting color statistics.
 *
 * @brief This function is used as a comparator for qsort to sort
 * bkr_color_stats structures.
 *
 * It first compares the count of colors, sorting in descending order. If the
 * counts are equal, it then compares the color values, also in descending
 * order.
 *
 * @param a Pointer to the first color statistic to compare.
 * @param b Pointer to the second color statistic to compare.
 * @return Negative if a should come before b, positive if b should come before
 * a, and zero if they are equivalent.
 */
extern int _stats_cmp(const void* a, const void* b);

#ifdef __cplusplus
}
#endif

#endif  // BKR_COMMON_H_
