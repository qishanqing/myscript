# 远程通信RemoteNodeHandle

RemoteNodeHandle是一个支持远程节点通信的工具。它可以像本地节点一样，进行消息的发布/订阅，服务的声明/调用。它有两种工作模式：手动模式和自动模式。

手动模式时，RemoteNodeHandle与NodeHandle完全分离，各自维护自己的消息与服务，想让二者产生联系的话，需要使用者手动编写转发程序。无参的RemoteNodeHandle构造即激活手动模式。

自动模式时，不需要对消息和服务进行转发，只需要声明哪些消息与服务允许远程调用即可。将NodeHandle的引用或指针作为参数的RemoteNodeHandle构造即激活自动模式。

当前只支持点对点通信方式。

## Project

[示例项目: remote](./tutorial/remote)

## Build & Run

```
$ mkdir build
$ cmake .. && make
$ source ../scripts/env.sh 
$ mosrun ../run/xxx.run.yaml
