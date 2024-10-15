/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#ifndef LIB_OCTREE_H_
#define LIB_OCTREE_H_
#include "../bkr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Builds an octree color quantization structure from an array of pixels.
 *
 * This function constructs an octree representation of the color space for the
 * given pixel array. The octree is used to efficiently quantize colors and
 * reduce the color palette to a specified maximum number of colors.
 *
 * The octree is built by iteratively adding colors and reducing the tree when
 * necessary to maintain the maximum color limit. This process involves creating
 * new nodes, updating leaf counts, and merging nodes when the tree exceeds the
 * specified size.
 *
 * @param pixels Pointer to an array of bkr_rgb structures representing the
 * input pixels.
 * @param pixel_count The number of pixels in the input array.
 * @param max_colors The maximum number of colors allowed in the final quantized
 * palette.
 * @return A pointer to the root node of the constructed octree, or NULL if an
 * error occurred.
 */
extern bkr_octree_node* bkr_build_octree(bkr_rgb* pixels,
                                         uint32_t pixel_count,
                                         uint32_t max_colors);

/**
 * @brief Releases the memory allocated for an octree structure.
 *
 * This function recursively frees all memory associated with the given octree,
 * including all its child nodes. It should be called when the octree is no
 * longer needed to prevent memory leaks.
 *
 * The function traverses the entire tree structure, freeing each node in a
 * depth-first manner. After calling this function, the passed node pointer and
 * all its children become invalid and should not be accessed.
 *
 * @param node Pointer to the root node of the octree to be released.
 */
extern void bkr_release_octree(bkr_octree_node* node);

/**
 * @brief Calculates color statistics from a built octree.
 *
 * This function traverses the octree structure and computes color statistics
 * for each leaf node. It aggregates information such as color values and pixel
 * counts, providing a summary of the quantized color palette.
 *
 * The function performs the following steps:
 * 1. For each leaf node, it calculates the average color.
 * 2. It stores the color information and pixel count in the provided stats
 * array.
 * 3. If the current node is the root, it sorts the final statistics for
 * consistent output.
 *
 * @param node Pointer to the root node of the octree to analyze.
 * @param stats Pointer to an array of bkr_color_stats structures where the
 * results will be stored.
 * @return The number of unique colors found in the octree (i.e., the number of
 * leaf nodes).
 */
extern int bkr_octree_calculate_color_stats(bkr_octree_node* node,
                                            bkr_color_stats stats[]);

#ifdef __cplusplus
}
#endif

#endif  // LIB_OCTREE_H_
