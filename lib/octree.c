/**
 * Byakuren - A theme color extracting library implemented by C.
 *
 * Copyright (c) 2024 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/byakuren/blob/master/LICENSE>
 */
#include "octree.h"
#include <stdio.h>
#include <stdlib.h>

xmem_pool_handle _node_pool = 0;
xmem_pool_handle _reducible_pool = 0;

int _bkr_init_octree_env() {
  if (!_node_pool) {
    _node_pool = xmem_create_pool(sizeof(bkr_octree_node));
  }

  if (!_reducible_pool) {
    _reducible_pool = xmem_create_pool(sizeof(bkr_octree_reducible_list_node));
  }

  return _node_pool && _reducible_pool;
}

void _bkr_destroy_octree_env() {
  SAFE_DESTROY_POOL(_node_pool);
  SAFE_DESTROY_POOL(_reducible_pool);
}

int _bkr_octree_add_color(bkr_octree_node* node,
                          bkr_rgb* color,
                          int level,
                          uint32_t* leaf_count,
                          bkr_octree_reducible_list_node* reducible[]) {
  if (node->is_leaf && node->is_leaf != 0xff) {
    node->pixel_count++;
    node->red_components += color->red;
    node->green_components += color->green;
    node->blue_components += color->blue;
    return 1;
  } else {
    // Determine the index of the child node based on the color's RGB values.
    // For example, if we have:
    //
    //   R: 10101101
    //   G: 00101101
    //   B: 10010010
    //
    // At level 0, we look at the leftmost bit of each color component.
    // This gives us: R=1, G=0, B=1, which translates to index 5 (binary 101).
    // As we descend the tree, we shift our focus to the next bit for each
    // level.
    uint8_t red = (color->red >> (7 - level)) & 1;
    uint8_t green = (color->green >> (7 - level)) & 1;
    uint8_t blue = (color->blue >> (7 - level)) & 1;
    int idx = (red << 2) + (green << 1) + blue;

    if (!node->children[idx]) {
      bkr_octree_node* tmp = node->children[idx] =
          (bkr_octree_node*)  // NOLINT(readability/casting)
          xmem_alloc(_node_pool);

      // If memory allocation fails, return 0 to indicate an error.
      if (!tmp) return 0;

      if (level == 7) {
        tmp->is_leaf = 1;
        (*leaf_count)++;
      } else {
        // Add this node to the current level's reducible list. This list is
        // used later for color reduction if needed.
        bkr_octree_reducible_list_node* reducible_node =
            (bkr_octree_reducible_list_node*)  // NOLINT(readability/casting)
            xmem_alloc(_reducible_pool);

        // If we can't create a new reducible node, return an error.
        if (!reducible_node) return 0;

        reducible_node->node = tmp;
        reducible_node->next = reducible[level];
        reducible[level] = reducible_node;
      }
    }

    // Recursively add the color to the appropriate child node.
    return _bkr_octree_add_color(
        node->children[idx], color, level + 1, leaf_count, reducible);
  }
}

void _bkr_octree_reduce(uint32_t* leaf_count,
                        bkr_octree_reducible_list_node* reducible[]) {
  int lv = 6;

  // Find the lowest non-empty reducible list.
  while (!reducible[lv] && lv >= 0) lv--;
  if (lv < 0) return;

  // Get the first reducible node from the lowest level, and update the list
  // head to the next node.
  bkr_octree_node* node = reducible[lv]->node;
  reducible[lv] = reducible[lv]->next;

  int r = 0, g = 0, b = 0;
  int count = 0;
  for (int i = 0; i < 8; i++) {
    if (!node->children[i]) continue;
    r += node->children[i]->red_components;
    g += node->children[i]->green_components;
    b += node->children[i]->blue_components;
    count += node->children[i]->pixel_count;

    // Reduce the leaf count as we're merging children into their parent.
    (*leaf_count)--;

    // Free the memory of the child node.
    xmem_free(_node_pool,
              (char*)(node->children[i]));  // NOLINT(readability/casting)
    node->children[i] = 0;
  }

  // Set the values for the parent node, which now becomes a leaf.
  node->is_leaf = 1;
  node->red_components = r;
  node->green_components = g;
  node->blue_components = b;
  node->pixel_count = count;

  // Increment the leaf count for the new leaf node.
  (*leaf_count)++;
}

void bkr_release_octree(bkr_octree_node* node) {
  // Recursively release memory for all children nodes.
  for (int i = 0; i < 8; i++) {
    if (node->children[i]) bkr_release_octree(node->children[i]);
  }

  // Release the memory for the current node.
  xmem_free(_node_pool, (char*)node);  // NOLINT(readability/casting)
}

/**
static inline void _bkr_print_reducible(bkr_octree_reducible_list_node* list[])
{
    printf("[");
    for(int i = 0; i < 7; i++)
    {
        int count = 0;
        bkr_octree_reducible_list_node* node = list[i];
        while(node)
        {
            count++;
            node = node->next;
        }
        printf("%d", count);
        if(i != 0) printf(",");
    }
    printf("]");
}
*/

bkr_octree_node* bkr_build_octree(bkr_rgb* pixels,
                                  uint32_t pixel_count,
                                  uint32_t max_colors) {
  bkr_octree_node* root =
      (bkr_octree_node*)xmem_alloc(_node_pool);  // NOLINT(readability/casting)

  // If memory allocation fails for the root node, return 0 to indicate an
  // error.
  if (!root) return 0;
  root->is_leaf = 0xff;

  uint32_t leaf_count = 0;

  // Create an array to hold 7 reducible lists, one for each level of the octree
  // (excluding the root and leaf levels).
  bkr_octree_reducible_list_node* _reducible_list[7];

  // Initialize all reducible lists to empty (null).
  for (int i = 0; i < 7; i++) _reducible_list[i] = 0;

  for (uint32_t i = 0; i < pixel_count; i++) {
    // For each pixel, add its color to the octree using the
    // _bkr_octree_add_color function. This function updates the leaf_count and
    // _reducible_list as needed.
    if (!_bkr_octree_add_color(
            root, pixels + i, 0, &leaf_count, _reducible_list)) {
      bkr_release_octree(root);
      return 0;
    }

    // After adding a color, check if the leaf count exceeds the maximum allowed
    // colors. If so, reduce the octree by merging nodes until we're back within
    // the limit.
    while (leaf_count > max_colors) {
      _bkr_octree_reduce(&leaf_count, _reducible_list);
      // Uncomment the following line to print the state of reducible lists for
      // debugging:
      //
      // _bkr_print_reducible(_reducible_list);
    }
  }

  return root;
}

int bkr_octree_calculate_color_stats(bkr_octree_node* node,
                                     bkr_color_stats stats[]) {
  static uint32_t stats_size = sizeof(bkr_color_stats);
  if (node->is_leaf && node->is_leaf != 0xff) {
    // Calculate the average color for this leaf node.
    int r = node->red_components / node->pixel_count;
    int g = node->green_components / node->pixel_count;
    int b = node->blue_components / node->pixel_count;

    // Store the color statistics for this leaf node.
    stats[0].count = node->pixel_count;
    stats[0].color.red = r;
    stats[0].color.green = g;
    stats[0].color.blue = b;
    stats[0].value = BKR_RGB_TO_INT32(r, g, b);

    return 1;
  }

  int _count = 0;
  // Recursively calculate color statistics for all non-null children.
  for (int i = 0; i < 8; i++) {
    if (NULL != node->children[i]) {
      _count +=
          bkr_octree_calculate_color_stats(node->children[i], stats + _count);
    }
  }

  // If this is the root node (indicated by is_leaf == 0xff), sort the final
  // color statistics.
  if (node->is_leaf == 0xff) {
    qsort(stats, _count, stats_size, _stats_cmp);
  }

  return _count;
}
