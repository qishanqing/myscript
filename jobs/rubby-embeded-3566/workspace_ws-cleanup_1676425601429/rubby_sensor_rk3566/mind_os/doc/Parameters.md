# 参数服务器

MindOS提供了一个通用的参数服务器，可用于储存不同类型的参数。需要注意的是，只能存储能够与std::string进行相互转化的数据类型。而一些自定义类型是无法存储的。

## Project

[示例项目: parameters](./tutorial/parameters)

## Build & Run

```
$ mkdir build
$ cmake .. && make
$ source ../scripts/env.sh 
$ mosrun ../run/xxx.run.yaml
```
