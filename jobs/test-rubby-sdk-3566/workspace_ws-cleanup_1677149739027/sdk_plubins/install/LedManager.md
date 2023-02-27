# LedManager Node

该节点是管理扫地机的led显示。


## 1. Calls

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /sensor/led_config| [LedSrv](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/sensor/LedSrv.h) | 用于设置LED。 | 

## 2. Subcribed Topics
| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /led_manager/fault | [bool] | 接收fault事件。|
| /led_manager/clear | [bool] | 清楚故障。|
| /battery_event | [BatteryEvent](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/sensor/BatteryEvent.h) | 电源管理充电状态。|
| /wifi_status | [WifiStatus](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/sensor/WifiStatus.h) | wifi状态。|

