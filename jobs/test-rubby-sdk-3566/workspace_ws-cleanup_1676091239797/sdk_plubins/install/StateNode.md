# State Node

State节点是整个系统的业务层状态控制节点，负责系统事件和异常的处理。

## 1. Published Topics

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /state/event/test/result | [ResultTest](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/server/state/ResultTest.h) | 发布测试事件处理结果，可根据resultId来区分是否为自己的事件处理结果。 |
| /state/switch/type | [CurrentState](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/msg/state/CurrentState.h) | 发布当前状态机状态。 |

## 2. Subcribed Topics

暂无


## 3. Services

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /state/event/test | [EventTest](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/state/EventTest.h) | 接收测试事件，返回事件结果resultId。 |
| /state/event/speed_control | [SetRemoteCmdSrv](http://192.168.50.191:85/abby/source/sdk_plubins/-/tree/master/msgs/srv/state/SetRemoteCmdSrv.h) | 接收remote hmi转发的控制指令发送给底盘。 |
| /state/event/keyboard | [KeyboardCmdSrv](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/state/KeyboardCmdSrv.h) | 接收扫地机的按键指令。 |
| /state/clean/region | [CleanRegionSrv](http://192.168.50.191:85/abby/source/sdk_plubins/-/tree/master/msgs/srv/state/CleanRegionSrv.h) | 接收划区清扫。 |

## 4. Calls

## 5. Parameters

暂无
