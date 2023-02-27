# 主线程事件机制

为了能支持一些只能在主线程中进行的操作，和与一些必须使用主线程的库进行兼容，MindOS提供了主线程事件机制。可以通过相应的接口，将事件从子线程发送到主线程去执行。

## Project

[示例项目: main_thread_event](./tutorial/main_thread_event)

## Build & Run

```
$ mkdir build
$ cmake .. && make
$ source ../scripts/env.sh 
$ mosrun ../run/xxx.run.yaml
```
