# Byakuren

Byakuren is a C library for extracting theme colors from images.

> This library has been used in production by a related company for years.

> **TODO:** Implement KD-Tree for color finding.

## 聖 白蓮

![聖 白蓮](https://github.com/XadillaX/byakuren/blob/master/assets/byakuren.png?raw=true)

> 聖 白蓮（ひじり びゃくれん，Hiziri Byakuren）是系列作品《东方project》中的角色，首次登场于《东方星莲船》。
>
> + 种族：魔法使
> + 能力：使用魔法程度的能力（酣畅增强身体能力的）
> + 危险度：不明
> + 人类友好度：中
> + 主要活动场所：命莲寺之类
>
> 命莲寺的住持。虽然原本是人类，不过由于常年的修行已经完全超越了人类。现在已经属于人们常说的魔法使了。
>
> 虽然已经入了佛门，但是不知道什么原因却被妖怪敬仰着。她从来没有像童话故事中的魔法使那样，念诵着咒语治退妖怪。使用的力量完全是邪恶的，一点都不像是圣人，虽然并没有人目击到她与人类为敌，但其实已彻底成为妖怪的同伴了。

## Features

- Supports multiple color extraction algorithms:
  - Octree algorithm
  - Min-diff algorithm
  - Mix-in algorithm (combining Octree and Min-diff)
- Provides a simple C API
- Can be compiled into a static library for easy integration

## Installation

Clone the repository with submodules:

```shell
git clone --recurse-submodules https://github.com/XadillaX/byakuren.git
```

Compile the static library:

```shell
make byakuren
```

After compilation, include `byakuren.h` in your project to use the library:

```c
#include "byakuren.h"
```

## Usage

1. Initialize the Byakuren environment:

```c
int result = bkr_init();
```

2. Use one of the color extraction algorithms:
   - Octree Algorithm
   - Min-diff Algorithm
   - Mix Algorithm

3. Release the environment when done:

```c
bkr_destroy();
```

For detailed API usage, refer to the [API Documentation](#apis) section below.

## Testing

Compile the test binary:

```shell
make ./test/bkr_test
```

Run the test with a specific algorithm:

```shell
cd test && ./bkr_test ALGORITHM
```

Replace `ALGORITHM` with `octree`, `mindiff`, or `mix`.

### Test Helper

A Node.js-based test helper is available for quick testing without generating `.rgb` files manually.

1. Install dependencies:

```shell
npm install
```

2. Run the helper:

```shell
./test/run.js -u IMAGE_URL -a ALGORITHM
```

For example:

```shell
./test/run.js -u http://example.com/image.jpg -a octree
```

Use `-a all` to test all three algorithms at once.

## APIs

Here's a brief overview of the main APIs provided by Byakuren. For detailed parameter descriptions and usage, please refer to the full API documentation in the header files.

### Initialization & Cleanup

```c
int bkr_init();
void bkr_destroy();
```

### Octree Algorithm

```c
bkr_octree_node* bkr_build_octree(bkr_rgb* pixels, uint32_t pixel_count, uint32_t max_colors);
int bkr_octree_calculate_color_stats(bkr_octree_node* node, bkr_color_stats stats[]);
void bkr_release_octree(bkr_octree_node* node);
```

### Min-diff Algorithm

```c
int bkr_mindiff_calculate_color_stats(bkr_rgb* pixels, uint32_t pixel_count, bkr_color_stats stats[], bkr_mindiff_parameter* param);
```

### Mix Algorithm

```c
int bkr_mix_calculate_color_stats(bkr_rgb* pixels, uint32_t pixel_count, uint32_t octree_max_colors, bkr_mindiff_parameter* mindiff_param, bkr_color_stats stats[]);
```

For full API documentation, including detailed descriptions of parameters and return values, please refer to the following header files:

- `bkr_common.h`: Common structures and definitions
- `lib/octree.h`: Octree algorithm functions
- `lib/mindiff.h`: Min-diff algorithm functions
- `lib/mix.h`: Mix algorithm functions

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

「雖然我覺得不怎麼可能有人會關注我」
