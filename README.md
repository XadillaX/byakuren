# Byakuren

A theme color extracting library implemented by C.

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

- [o] Octree Algorithm
- [o] Min-diff Algorithm
- [o] Mix Algorithm

## Test Command

```shell
$ make ./test/bkr_test
```

After `make` command, you should generate a binary file named `test.rgb`. Then run:

```shell
$ cd test && ./bkr_test ALGORITHM
```

> `ALGORITHM` is a parameter means algorithm you want to test.
>
> support `octree`、`mindiff` and `mix` so far.

## Test Helper

If you want to test quickly, you may use a simple script.

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
