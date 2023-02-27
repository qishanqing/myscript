#ifndef ENUMINFO_H_
#define ENUMINFO_H_

//工作模式
enum workingMode_
{
    WORK_INIT = 0,
    WORK_ROUTINECLEAN, /*常规清扫模式*/
    WORK_DELIMITCLEAN, /*划区清扫模式*/
    WORK_AUTOPARTITIONSCLEAN, /*自动分区清扫模式*/
    WORK_RECHARGE,     /*回充模式*/
    WORK_RECOVERY,    /*脱困模式*/
    WORK_PAUSE,        /*暂停*/
    WORK_RESUME,       /*取消暂停*/
    NO_WORK            /*非工作模式*/
};

//清扫任务
enum WorkingTask
{
    WorkingTask_Init = 0,
    WorkingTask_BuildMap = 1,      //快速建图
    WorkingTask_BuildMap_Clean,    //边建图边清扫,常规清扫
    WorkingTask_BuildMap_Support,  //地图教学,不启动清扫设备,补充建图
    WorkingTask_Clean,             //常规清扫
    WorkingTask_Clean_Room,        //自动分区+常规清扫
    WorkingTask_Clean_SemanticMap, //语义区域清扫+常规清扫
    WorkingTask_Clean_Manual,      //划区清扫+常规清扫
    WorkingTask_Clean_History,     //端点续扫
};

//地图栅格属性
enum CellType
{
    //二值化图
    FreeCell = 0,
    ObsCell = 100,
    UnknowCell = 255,
    //禁区图
    ForbiddenCell = 0,                        //  虚拟墙、虚拟禁区
    ForbiddenMarkCell = 100,         //  困境所标记的区域
    ForbiddenUnknowCell = 255,  // 非禁区和非困境等区域, 脱困成功
    //分区图
    RegionareaCell = 0,
    RegionCell = 50,
    RegionsideCell = 100,
    RegionUnknowCell = 255,
    //搜索-清扫路径的生成图
    SearchFreeCell = 0,
    SearchCovCell = 50,
    SearchObsCell = 100,
    SearchUnknowCell = 255,
    //覆盖图
    CoverCell = 0,
    CoverIgnoreCell = 1,
    CoverUnknowCell = 255,
    //沿边图
    FollowObsIdMaxCell = 200,
    FollowObsCell = 252,
    FollowFreeCell = 253,
    FollowIgnoreCell = 254,
    FollowUnknowCell = 255,
    //路径图
    PathFreeCell = 0,
    PathUnknowCell = 255,
    //导航图
    NavFreeCell = 0,                           // 自由通行区域
    NavObsExpandCell = 100,        // 膨胀层
    NavCoverCell = 200,                   //  ...
    NavSensorObsCell = 253,        //  碰撞传感器建出的障碍物
    NavObsCell = 254,                      //  墙体等障碍物
    NavUnknowCell = 255              //  未知区域
};

//任务状态
enum motionState
{
    INIT = 0,
    LEAVEPILE,     /*下桩*/
    RELOCATION,    /*重定位*/
    FIRSTBUILDMAP, /*首次建图*/
    NAVIGATION,    /*导航*/
    BOW,           /*弓字形清扫*/
    FEBCLEANRIGHT, /*清扫右沿边*/
    FEBCLEANLEFT,  /*清扫左沿边*/
    FENAVRIGHT,    /*导航右沿边*/
    FENAVLEFT,     /*导航左沿边*/
    EXPLOREPILE,   /*探桩*/
    UPPERPILE,     /*上桩*/
    CHARGE,        /*充电*/
    CHARGEDONE,    /*充电完成*/
    AWAYDILEMMA,   /*脱困*/
    FINISHEDTASK   /*任务完成*/
};

//定位状态
enum SigLocation
{
    Location_init = 0,

    Location_map_relocation_start = 1, // 地图重定位开始
    Location_map_relocation_fail = 2,  //地图重定位失败
    Location_map_relocation_ok = 3,    //地图重定位成功

    Location_kinnap_relocation_start = 4, //机器人绑架后放在地上，开始重定位
    Location_kinnap_relocation_fail = 5,  //机器人绑架重定位失败
    Location_kinnap_relocation_ok = 6,    //机器人绑架重定位成功

    Location_SLAM_start = 7,   // 无地图启动信号
    Location_KINNAPPED = 8,    //机器人被绑架抬起
    Location_MERGE_MAP_END = 9 //机器人位姿有效标记
};

//重定位动作状态
enum SigRepositioning
{
    Reposition_RELOCATIONINIT = 0, /*重定位动作闲时状态*/
    Reposition_RUNNING = 1,        /*重定位动作正在进行*/
    Reposition_FINISHEND = 2,      /*重定位动作三角走完结束*/
    Reposition_NOFINISHEND = 3,    /*重定位动作三角未走完结束*/
    Reposition_STARTSUCCEED = 4    /*NAV start成功*/
};

//机器人运动控制模式
enum MotionControlMode
{
    Motion_Init = 0,
    Motion_Stop = 1,
    Motion_Brake,
    Motion_LeaveStation,
    Motion_EnterStation,
    Motion_Nav,
    Motion_Cleaning,
    Motion_FollowObs_L,
    Motion_FollowObs_R,
    Motion_LeaveObs,
    Motion_Relocation
};

//机器人动作模式
enum MotionActionmode
{
    MotionAction_Init = 0,
    MotionAction_FollowObs_Right = 1,
    MotionAction_FollowObs_Left,
    MotionAction_Rotate,
    MotionAction_Reverse,
    MotionAction_Line,
    MotionAction_Arc
};

//常规清扫搜索未清扫点状态
enum SiggetCleanStart
{
    FindUncleanPoint_Null = 0,
    FindUncleanPoint_Out = 1,
    FindUncleanPoint_Inside,
    FindUncleanPoint_InObs
};

//划区清扫搜索未清扫点状态
enum SiggetCleanStartDelimit
{
    FindUncleanPointDelimit_Null = 0,
    FindUncleanPointDelimit_Out = 1,
    FindUncleanPointDelimit_Inside,
    FindUncleanPointDelimit_InObs
};

//搜索障碍物
enum SigSearchObsPoint
{
    FindObs_Null = 0,
    FindObs_success = 1,
    FindObs_InObs,
};

//导航状态
enum SigNav
{
    Nav_fail = 0,
    Nav_success = 1,
    Nav_InObs
//    Nav_TargetInObs
};

//探索状态
enum SigSearchExplore
{
    Search_Null = 0,
    Search_Get = 1,
    Search_StartInObs = 2
};

//跟踪导航路径状态
enum SigNavFollowPath
{
    Nav_Follow_Run = 0,
    Nav_Follow_Finish = 1,
    Nav_Follow_Fail
};

//跟踪清扫路径状态
enum SigcleanFollowPath
{
    Clean_Follow_Run = 0,
    Clean_Follow_Finish = 1,
    Clean_Follow_Fail,

};

enum SigRobotInDesignatedArea
{
    InDesignatedArea_Inside = 0,
    InDesignatedArea_Outside = 1
};

//加载地图状态
enum MapLoadState
{
    MapLoad_INIT = 0,
    MapLoad_SUCCESS = 1,
    MapLoad_Fail = 2
};

//动作状态
enum NodeStatus
{
    DEFAULT = -1,
    RUNNING = 0,
    SUCCESS,
    FAILURE
};

//脱困类型
enum RecoveryStyle
{
    Recovery_INIT = 0,
    Recovery_Forbiden,//机器人当前在禁区
    Recovery_Obs,//机器人当前在障碍物内
    Recovery_Surrounded,//机器人当前被障碍物环绕
    Recovery_Physics //机器人物理被困
};

//边类型
enum EdgeType
{
    Edge_DEFAULT = 0,
    Edge_Actual,  // 实体边
    Edge_Virtual  // 虚拟边
};

#endif