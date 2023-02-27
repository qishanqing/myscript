# Map Node

用于管理地图。

## 1. Services

| 名称 | 参数 | 说明 |
| --- | --- | --- |
| /map/costmap/save | [CostmapSave](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map/CostmapSrv.h) | 保存栅格地图。 |
| /map/costmap/get_url | [CostmapGetUrl](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map/CostmapSrv.h) | 根据URL加载栅格地图。 |
| /map/costmap/get_id | [CostmapGetId](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map/CostmapSrv.h) | 根据sceneid加载栅格地图。 |
| /map/outline/save | [OutlineSave](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/map/OutlineSrv.h) | 保存轮廓点集信息。 |
| /map/outline/get | [OutlineGet](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/map/OutlineSrv.h) | 根据sceneid/type获取对应的轮廓点集。  |
| /map/outline/del | [OutlineDel](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/map/OutlineSrv.h) | 根据sceneid/type删除对应的轮廓点集。  |
| /map/outline/del/ids | [OutlineDelById](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/map/OutlineSrv.h) | 根据id删除对应的轮廓点集。  |
| /map/pointcloud/new | [PointCloudNew](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map/PointCloudSrv.h) | 新建地图。  |
| /map/pointcloud/delete | [PointCloudDel](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map/PointCloudSrv.h) | 根据id删除对应的地图。  |
| /map/pointcloud/update | [PointCloudUpdate](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map/PointCloudSrv.h) | 根据id更新对应的地图。  |
| /map/pointcloud/query | [PointCloudQuery](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map/PointCloudSrv.h) | 根据id查询对应的地图。  |
| /map/pointcloud/default | [PointCloudDefault](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map/PointCloudSrv.h) | 获取默认地图。  |
| /map/key/set | [MapKeySet](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/map/MapKeySrv.h) | 以scene/map为key保存在内存中。  |
| /map/key/get | [MapKeyGet](http://192.168.50.191:85/abby/source/sdk_plubins/-/blob/master/msgs/srv/map/MapKeySrv.h) | 获取scene/map为key保存在内存中的状态。  |
| /map/rotate_map/set| [RotateParamSet](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map//RotateParamSet.h) | 保存slam的旋转地图的参数。  |
| /map/rotate_map/get | [RotateParamGet](http://192.168.50.191:85/abby/source/abby_msg/-/blob/master/srv/server/map//RotateParamGet.h) | 获取旋转地图的参数。  |
## 2. Calls

暂无

## 3. Parameters

暂无
