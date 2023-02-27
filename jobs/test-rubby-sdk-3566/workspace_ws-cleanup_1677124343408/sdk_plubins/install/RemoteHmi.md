# Remote Hmi Node

用于接入中科的app，接收转发app下发的指令。

## 1. Published Topics

暂无

## 2. Subcribed Topics

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /state/event/speed_control/result | [SetRemoteCmdRlt](http://192.168.50.191:85/abby/source/sdk_plubins/-/tree/master/msgs/msg/remote_hmi/SetRemoteCmdRlt.h) | 接收remote state的处理结果。|

## 3. Services

暂无

## 4. Calls

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /state/event/speed_control | [SetRemoteCmdSrv](http://192.168.50.191:85/abby/source/sdk_plubins/-/tree/master/msgs/srv/state/SetRemoteCmdSrv.h) | 接收APP下发的控制指令发送给remote state。|

## 5. Parameters

暂无
