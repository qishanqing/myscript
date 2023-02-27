# ROS的内嵌使用

可以通过建立ROS的spin处理节点，来支持ROS的内嵌使用。实际上，有两种方式去处理ROS的spin，一种是通过将事件发送到主线程上去执行，另一种，则是建立一个新的子线程执行。

如果想借助rviz等显示数据，则可以在订阅某数据后，将其转为ROS的消息格式，再通过ROS的接口发布出去即可。

## Project

[示例项目: ros](./tutorial/ros)

## Build & Run

```
$ mkdir build
$ cmake .. && make
$ source ../scripts/env.sh 
$ mosrun ../run/xxx.run.yaml
```
