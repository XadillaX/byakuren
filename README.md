# Byakuren

A theme color extracting library implemented by C.

> This library is used in a related company for years.

> **TODO:** Using KD-Tree to find some colors.

## 聖 白蓮

![聖 白蓮](byakuren.png)

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

## Compile Static Library

This project can be compiled to a static libary (byakuren.a) for using.

```shell
$ make byakuren
```

After compiling, you may use this library just by including `byakuren.h` in your project.

```c
#include "byakuren.h"
```

## Supported Algorithm

- [x] Octree Algorithm
- [x] Min-diff Algorithm
- [x] Mix Algorithm

## APIs

### `bkr_rgb` Structure

```c
typedef struct bkr_rgb {
    uint8_t red;
    uint8_t green;
    uint8_t blue
} bkr_rgb;
```

RGB pixel structure.

| name | type | description |
|------|------|-------------|
| red | uint8_t | the RED value (0-255) |
| green | uint8_t | the GREEN value (0-255) |
| blue | uint8_t | the BLUE value (0-255) |

### `bkr_color_stats` Structure

```c
typedef struct bkr_color_stats {
    bkr_rgb color;
    uint32_t value;
    uint32_t count;
} bkr_color_stats;
```

Stats of theme color result.

| name | type | description |
|------|------|-------------|
| color | bkr_rgb | a color pixel to indicate a theme color |
| value | uint32_t | a color pixel's INT32 value to indicates the theme color |
| count | uint32_t | stats of this theme color in the picture |

```c
typedef struct bkr_palette_array {
    uint32_t count;
    bkr_rgb* colors;
} bkr_palette_array;
```

A crowd of colors to indicate a theme color palette.

| name | type | description |
|------|------|-------------|
| count | uint32_t | color count in this palette |
| colors | bkr_rgb* | each color in this palette |

### `bkr_mindiff_parameter` Structure

```c
typedef struct bkr_mindiff_parameter {
    bkr_palette_array* palette;
    int16_t gray_offset;
} bkr_mindiff_parameter;
```

A parameter passes to Min-diff Algorithm.

| name | type | description |
|------|------|-------------|
| palette | bkr_palette_array* | a palette to calculate the theme color, left for `NULL` to indicate the default palette |
| gray_offset | int16_t | the offset to judge whether a color is gray, recommand to be `5` |

### Initialization & Release

Before the whole work you should initialize the Byakuren environment:

```c
int bkr_init();
```

And after all the work you should release the environment:

```c
void bkr_destroy();
```

### Octree Algorithm

#### Build Octree

```c
bkr_octree_node* bkr_build_octree(
        bkr_rgb* pixels,
        uint32_t pixel_count,
        uint32_t max_colors);
```

| parameter | type | description |
|-----------|------|-------------|
| pixels | bkr_rgb* | the RGB pixels of a picture |
| pixel_count | uint32_t | pixel count of the picture |
| max_colors | uint32_t | maximum theme color count this octree will have |

+ Return an octree

#### Calculate

```c
int bkr_octree_calculate_color_stats(
        bkr_octree_node* node,
        bkr_color_stats stats[]);
```

| parameter | type | description |
|-----------|------|-------------|
| node | bkr_octree_node* | the octree which `bkr_build_octree` returned |
| stats | bkr_color_stats | an array to receive each theme color stats |

+ Return the count of theme colors.

#### Release Octree

```c
void bkr_release_octree(bkr_octree_node* node);
```

| parameter | type | description |
|-----------|------|-------------|
| node | bkr_octree_node* | the octree to be released |

#### Example

```c
bkr_rgb* rgb = GET_PICTURE_RGB(); // implement by yourself
uint32_t color_count = GET_PICTURE_PIXEL_COUNT(); // implement by yourself
bkr_color_stats stats[256];
bkr_octree_node* root = bkr_build_octree(rgb, color_count, 256);
int colors = bkr_octree_calculate_color_stats(root, stats);
```

### Min-diff Algorithm

#### Calculate

```c
int bkr_mindiff_calculate_color_stats(
        bkr_rgb* pixels,
        uint32_t pixel_count,
        bkr_color_stats stats[],
        bkr_mindiff_parameter* param);
```

| parameter | type | description |
|-----------|------|-------------|
| pixels | bkr_rgb* | the RGB pixels of a picture |
| pixel_count| uint32_t | pixel count of the picture |
| stats | bkr_color_stats | an array to receive each theme color stats |
| param | bkr_mindiff_parameter* | the parameter passes to Min-diff Algorithm for calculating |

+ Return the count of theme colors.

#### Example

```c
bkr_rgb* rgb = GET_PICTURE_RGB(); // implement by yourself
uint32_t color_count = GET_PICTURE_PIXEL_COUNT(); // implement by yourself
bkr_color_stats stats[256];
bkr_mindiff_parameter param;
param.gray_offset = 5;
param.palette = NULL;
int colors = bkr_mindiff_calculate_color_stats(rgb, color_count, stats, &param);
```

### Mix Algorithm

Mix Octree and Min-diff up.

#### Calculate

```c
int bkr_mix_calculate_color_stats(
        bkr_rgb* pixels,
        uint32_t pixel_count,
        uint32_t octree_max_colors,
        bkr_mindiff_parameter* mindiff_param,
        bkr_color_stats stats[]);
```

| parameter | type | description |
|-----------|------|-------------|
| pixels | bkr_rgb* | the RGB pixels of a picture |
| pixel_count | uint32_t | pixel count of the picture |
| octree_max_colors | uint32_t | maximum theme color count this octree will have |
| param | bkr_mindiff_parameter* | the parameter passes to Min-diff Algorithm for calculating |
| stats | bkr_color_stats | an array to receive each theme color stats |

+ Return the count of theme colors.

#### Example

```c
bkr_rgb* rgb = GET_PICTURE_RGB(); // implement by yourself
uint32_t color_count = GET_PICTURE_PIXEL_COUNT(); // implement by yourself
bkr_color_stats stats[256];
bkr_mindiff_parameter param;
param.gray_offset = -1;
param.palette = NULL;
int colors = bkr_mix_calculate_color_stats(rgb, color_count, 256, &param, stats);
print_stats(stats, colors);
```

## Test Command

```shell
$ make ./test/bkr_test
```

After `make` command, you should generate a binary file named `test.rgb`. Then run:

```shell
$ cd test && ./bkr_test ALGORITHM
```

> You may create your own `.rgb` file by referring **test/run.js** or **test/test.c**. Or you may have a look at [Test Helper](#test-helper).
>
> `ALGORITHM` is a parameter means algorithm you want to test.
>
> support `octree`、`mindiff` and `mix` so far.

## Test Helper

If you want to test quickly (no *.rgb), you may use a simple script.

Install [Node.js](https://nodejs.org/) first and come to `test` folder. Then run:

```shell
$ cd test
$ npm install
```

> **NOTICE:** You can start test helper directly under OSX. Otherwise, you should compile a test binary executor before testing, that is `$ make ./test/bkr_test`.

After installing, run `--help`.

```shell
$ ./run.js --help
```

You will see some introduction. `-u` means a URL.

For an example:

```shell
$ ./run.js -u http://cdn.duitang.com/uploads/item/201205/22/20120522224448_43nFu.thumb.600_0.jpeg -a octree
```

After command above, a browser will be open to display the result.

If you want to test three algorithm at one time, you should make `-a` be `all`.

For an example:

```shell
$ ./run.js -u http://cdn.duitang.com/uploads/item/201205/22/20120522224448_43nFu.thumb.600_0.jpeg -a all
```

## Contribution

You're welcome to make Pull Requests.

「雖然我覺得不怎麼可能有人會關注我」
