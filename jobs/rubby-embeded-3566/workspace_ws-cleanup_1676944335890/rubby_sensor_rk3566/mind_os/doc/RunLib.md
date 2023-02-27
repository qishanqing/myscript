# 使用RunLib

MindOS提供了一种机制，可以将MindOS的核心，嵌入到其他应用程序中运行，即RunLib模式。

需要特别注意的是，使用此模式时，要单独引用<mind_os/runlib/runlib.h>头文件。

此模式主要用于单个模块在其他环境，如一些QtCreator等一些IDE中的Debug。

## Project

[示例项目: runlib](./tutorial/runlib)

## Build & Run

特别地，此示例不需要基于mosrun来运行。

```
$ mkdir build
$ cmake .. && make
$ ./ExampleXXX
```
