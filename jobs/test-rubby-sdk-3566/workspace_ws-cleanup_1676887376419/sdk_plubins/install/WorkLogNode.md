# WorkLog Node

用于监控数据帧率和延迟的节点。

## 1. Published Topics

暂无

## 2. Subcribed Topics

暂无

## 3. Services

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /work_log/record_work_run | [RecordWorkRunSrv](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/work_log/RecordWorkRunSrv.h) | 记录运行日志到文件，返回执行成功/失败。 |
| /work_log/load_work_run | [LoadWorkRunSrv](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/work_log/LoadWorkRunSrv.h) | 从文件中加载运行日志，返回运行日志列表。 |

## 4. Calls

暂无

## 5. Parameters

暂无
