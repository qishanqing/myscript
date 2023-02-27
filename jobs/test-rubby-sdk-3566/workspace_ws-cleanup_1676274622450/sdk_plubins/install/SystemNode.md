# System Node

System节点是用来配置一些全局通用性的参数的节点。如各模块参数目录等。

## 1. Published Topics

暂无

## 2. Subcribed Topics

暂无


## 3. Services

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /system/log/clean | [CleanLogSrv](http://192.168.50.191:85/abby/source/sdk_plubins/-/tree/master/msgs/srv/system/CleanLogSrv.h) | 清理日志 |

## 4. Calls

暂无

## 5. Parameters

| 名称 | 类型 | 说明 |
| --- | --- | --- |
| system.runtime.configdir.server | std::string | server的配置参数目录 |
| system.runtime.configdir.sensor | std::string | sensor的配置参数目录 |
| system.runtime.configdir.slam | std::string | slam的配置参数目录 |
| system.runtime.configdir.depth | std::string | depth的配置参数目录 |
| system.runtime.configdir.perception | std::string | perception的配置参数目录 |
| system.runtime.configdir.navigation | std::string | navigation的配置参数目录 |
