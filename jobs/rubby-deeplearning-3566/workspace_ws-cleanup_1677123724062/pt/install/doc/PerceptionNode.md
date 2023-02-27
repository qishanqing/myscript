# Perception Node 
功能介绍：节点主要负责障碍物及环境感知


## 1. Published Topics
| 名称 |参数| 说明 |
| --- |---| --- |
| /senmatic_map|[SenmaticMap](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/perception/SenmaticMap.h) | 感知预测结果，按照固定帧率发送；用于导航模块避障，需要 slam 进行位姿修正；  |
| /perception/error|[ErrorMessage](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/common/error/ErrorMessage.h)  | 感知模块异常信息 |  


## 2. Subcribed Topics
| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /img_ir  | [ImgMsg](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/sensor/ImgMsg.h) | 双目数据 |
| /pose  | [SlamResult](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/slam/SlamResult.h) | 位姿信息 |
| /opt_senmatic_map  | [SenmaticMap](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/msg/perception/SenmaticMap.h) | 修正后的地图 |

##  3. Services
| 名称 |参数| 说明 |
| --- | --- |---|
| /perception/init|[Init](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/perception/Init.h)   | 初始化， 模块配置文件等加载 |
| /perception/start|[Start](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/perception/Start.h)   | 启动 |
| /perception/stop|[Stop](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/perception/Stop.h)  | 停止 |
| /perception/pause|[Pause](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/perception/Pause.h)   | 暂停 |
| /perception/restart|[Restart](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/perception/Restart.h)  | 重启 |
| /perception/save_map|[SaveMap](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/perception/SaveMap.h)   | 清扫结束前，需保存地图，保存地图前需 slam 对地图进行位置修正 |
| /perception/load_map|[LoadMap](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/perception/LoadMap.h)  | 启动时加载地图，传入地图的路径； |
| /perception/rotate_map|[RotateMap](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/perception/RotateMap.h)  | 地图旋转修正 |

## 4. Calls
| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /slam/get_pose | [GetPoseSrv](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/slam/GetPoseSrv.h)| 获取位姿信息 |


## 5. Parameters
| 名称 | 参数 | 说明 |
| --- | --- | --- |
| system.runtime.configdir.perception | std::string| perception的配置参数目录 |