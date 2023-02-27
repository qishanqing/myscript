# HMI Node

HMI节点是负责人机交互功能的节点，主要包含了操作获取的接口。  
该节点通过监听9002端口，通过websocket/flatbuffer 与客户端交互。  

## 1. Published Topics

暂无

## 2. Subcribed Topics
| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /state/event/test/result | [ResultTest](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/server/state/ResultTest.h) | 接收Test事件结果。|
| /key_event | [KeyEvent](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/sensor/KeyEvent.h) | 硬件按钮键状态。|
| /hmi/KeyEvent | [KeyEvent](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/msg/hmi/KeyEvent.h) | hmi 模拟按键状态。|
| /state/switch/type | [KeyEvent](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/msg/state/CurrentState.h) | 接受状态机状态。|

## 3. Services

暂无

## 4. Calls

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /state/event/test| [EventTest](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/state/EventTest.h) | 调用服务接口发布请求。 |
| /sensor/cpu_id| [CpuIdSrv](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/sensor/CpuIdSrv.h) | get cpuid |
| /state/type| [StateTypeSrv](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/state/StateTypeSrv.h) | 获取当前状态 |

## 5. Parameters

暂无
