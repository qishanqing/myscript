# Senmatic Node

State节点是整个系统的业务层状态控制节点，负责系统事件和异常的处理。

## 1. Published Topics

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /senmatic_map/convert | [SenmaticMap](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/perception/SenmaticMap.h) | 接受perception模块发布的结果，按配置文件添加扩展后发布。 |

## 2. Subcribed Topics

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /senmatic_map | [SenmaticMap](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/perception/SenmaticMap.h) | 接受perception模块发布的结果 |