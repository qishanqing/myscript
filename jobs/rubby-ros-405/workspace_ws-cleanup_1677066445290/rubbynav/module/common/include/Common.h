#ifndef RUBBYDECISION_COMMON_H
#define RUBBYDECISION_COMMON_H

#include <iostream>
#include <vector>
#include "nav_log.h"
#include "enumInfo.h"

#include <opencv2/highgui/highgui.hpp>

#define LOGCOMMON(INFO) LOG(INFO) << "[NAV] [COMMON] "
#define LOGMOTION(INFO) LOG(INFO) << "[NAV] [MOTION] "
#define LOGPLANNING(INFO) LOG(INFO) << "[NAV] [PLANNING] "
#define LOGHSM(INFO) LOG(INFO)<<"[NAV] [HSM] "
#define LOGMOTION_RECOVERY(INFO) LOG(INFO) << "[NAV] [MOTION] [RECOVERY] "

//#define ROSDEBUG
#define RUBBYNAV
//#define RUBBYSIMULATION
// #define SLAMTEST    // SLAM 测试手动建图
#define NEWCONTROL // PID进行控制


#define Pi_ 3.1415926535898f

#define ROBOTRADIUS 0.175f                   //机器人半径
#define OFFSETANGLE_FOLLOW_L 1.308996939f    // 75/180*PI
#define OFFSETANGLE_FOLLOW_R (-1.308996939f) // -75/180*PI
#define OFFSETRADIUS_FOLLOW 0.160229565936f  //沿边传感器安装半径

#ifdef RUBBYNAV
#define OFFSETDEPTH_FOLLOW 0.018f //沿边传感器安装深度
#define MAXVALID_FOLLOW 0.5f              //沿边传感器最远有效距离
#endif

#ifdef RUBBYSIMULATION
#define OFFSETDEPTH_FOLLOW 0.185f //沿边传感器安装深度
#define MAXVALID_FOLLOW 0.5f      //沿边传感器最远有效距离
#endif

#define OFFSETANGLE_CLIFF_L 1.134464014f      // 65/180*PI
#define OFFSETRADIUS_CLIFF_L 0.159f           //
#define OFFSETANGLE_CLIFF_LF 0.3490658504f    // 20/180*PI
#define OFFSETRADIUS_CLIFF_LF 0.155f          //
#define OFFSETANGLE_CLIFF_RF (-0.3490658504f) // 20/180*PI
#define OFFSETRADIUS_CLIFF_RF 0.155f          //
#define OFFSETANGLE_CLIFF_R (-1.134464014f)   // 65/180*PI
#define OFFSETRADIUS_CLIFF_R (-0.159f)        //

#define OFFSETANGLE_CAMERA_L 0.161f  //双目摄像头左目相对正前方偏移角度 9.21°　
#define OFFSETRADIUS_CAMERA_L 0.156f //双目摄像头左目距离机器人中心半径
#define OFFSETANGLE_CAMERA_M 0.0f    //双目摄像头中心相对正前方偏移角度 0°　
#define OFFSETRADIUS_CAMERA_M 0.154f //双目摄像头中心距离机器人中心半径
#define OFFSETANGLE_CAMERA_R         //双目摄像头右目相对正前方偏移角度
#define OFFSETRADIUS_CAMERA_R        //双目摄像头右目距离机器人中心半径
#define CAMERABLIND 0.175f           //双目视觉盲区
#define CAMERAVIEW 0.75f             //双目视觉范围

struct Header_
{
    uint64_t seq = 0;          //增量计数
    uint64_t stamp = 0;        //时间戳
    std::string frame_id = ""; //帧id（可忽略）
    bool status = false;       // true:有更新　, false:无更新
};

/*碰撞传感器数据结构*/
struct BumperInfo
{
    Header_ header;
    bool Bumper_Left = false;                                           /*左边碰撞传感器被触发*/
    bool Bumper_Right = false;                                          /*右边碰撞传感器被触发*/
    bool Bumper_Front = false;                                          /*正前方碰撞传感器被触发*/
    bool BumperTriggered = Bumper_Left || Bumper_Right || Bumper_Front; /*碰撞传感器被触发*/
};

/*红外传感器数据结构*/
struct FollowIrInfo
{
    Header_ header;
    float Ir_Right = 0.0f; /*右侧沿边传感器数据,单位：米*/
    float Ir_Left = 0.0f;  /*左侧沿边传感器数据,单位：米*/
    int Ir_Right_mm = 0; /*右侧沿边传感器数据,单位：毫米*/
    int Ir_Left_mm = 0;  /*左侧沿边传感器数据,单位：毫米*/
};

struct StationIrInfo
{
    Header_ header;
    uint8_t Ir_ChargingPile = 0; /*充电桩信号*/
};

/*尘盒水箱传感器数据结构*/
struct BoxTankInfo
{
    Header_ header;
    bool DustCollectionbox = false; /*集尘盒传感器数据*/
    bool WaterTank = false;         /*水箱传感器数据*/
};

/*下视传感器数据结构*/
struct CliffInfo
{
    Header_ header;
    bool Cliff_Right = false;                                                       /*右侧下视传感器被触发*/
    bool Cliff_RightF = false;                                                      /*右前下视传感器被触发*/
    bool Cliff_LeftF = false;                                                       /*左前下视传感器被触发*/
    bool Cliff_Left = false;                                                        /*左侧下视传感器被触发*/
    bool CliffTriggered = Cliff_Right || Cliff_RightF || Cliff_LeftF || Cliff_Left; /*下视传感器被触发*/
};

//电源管理信息
struct BMS
{
    enum batteryState
    {
        Not_charging = 0, //未充电
        Battery_low,      //电池电量低
        Charging,         //充电中
        Charge_complete   //充满电
    };

    enum ledCode
    {
        Outside_region = 0, //区域之外；
        A_region = 1,       //区域A；
        B_region = 2,       //区域B；
        C_region = 4,       //区域C；
        E_region = 6,       //区域E；
        D_region = 8,       //区域D；
    };

    Header_ header;
    uint8_t Battery = 0; // 电池电量
    int state = 0;         // 充电状态
    bool isInPile = false; // 是否在桩上

    uint8_t Ir_left = 0;
    uint8_t Ir_leftB = 0;
    uint8_t Ir_right = 0;
    uint8_t Ir_rightB = 0;
};

/*IMU数据*/
struct IMUInfo
{ // imu
    Header_ header;
    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;
};

//四元数
struct Quaternion
{
    Quaternion() : x(0.0), y(0.0), z(0.0), w(0.0) {}
    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    float x;
    float y;
    float z;
    float w;
};

//欧拉角
struct EulerAngles
{
    EulerAngles() : roll(0.0), pitch(0.0), yaw(0.0) {}
    EulerAngles(float _roll, float _pitch, float _yaw) : roll(_roll), pitch(_pitch), yaw(_yaw) {}
    double roll;
    double pitch;
    double yaw;
};

//位姿
struct PoseStamped
{
    PoseStamped() : pose(cv::Point2f(0.0, 0.0)), point(cv::Point(0, 0)), orientation(Quaternion()), euler(EulerAngles()) {}
    PoseStamped(const cv::Point2f &_pose, const cv::Point &_point, const Quaternion &_orientation = Quaternion(), const EulerAngles &_euler = EulerAngles()) : pose(_pose), point(_point), orientation(_orientation), euler(_euler) {}

    void toEulerAngle(const Quaternion q, EulerAngles &e)
    {
        // roll (x-axis rotation)
        double sinr_cosp = +2.0f * (q.w * q.x + q.y * q.z);
        double cosr_cosp = +1.0f - 2.0f * (q.x * q.x + q.y * q.y);
        e.roll = atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        double sinp = +2.0f * (q.w * q.y - q.z * q.x);
        if (fabs(sinp) >= 1)
            e.pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
        else
            e.pitch = asin(sinp);

        // yaw (z-axis rotation)
        double siny_cosp = +2.0f * (q.w * q.z + q.x * q.y);
        double cosy_cosp = +1.0f - 2.0f * (q.y * q.y + q.z * q.z);
        e.yaw = atan2(siny_cosp, cosy_cosp);
        //    return yaw;
    }

    Header_ header;
    cv::Point2f pose; /*世界坐标*/
    cv::Point point;  /*世界栅格坐标*/
    Quaternion orientation;
    EulerAngles euler;
};

struct MotorCurrent
{
    float mainBrushMotorCurrent = 0.0f;        //主刷电机
    float leftBrushMotorsCurrent = 0.0f;       //左边刷电机
    float rightBrushMotorsCurrent = 0.0f;      //右边刷电机
    float dustSuctionFanMotorsCurrent = 0.0f;  //吸尘风机电机
    float leftWheelMotorsCurrent = 0.0f;       //左轮电机
    float rightWheelMotorsCurrent = 0.0f;      //右轮电机
};

struct VisionInfo
{
    Header_ header;
    std::vector<cv::Point2f> data;
};

struct TofInfo
{
    Header_ header;
    PoseStamped posebase;
    PoseStamped posecamera;
    PoseStamped posemin;
    PoseStamped posemax;
    std::vector<float> data;
    std::vector<bool> flg;
    std::vector<PoseStamped> pose1;
    std::vector<PoseStamped> pose2;
	PoseStamped poseNearest;
};

struct workingMode
{
    Header_ header;
    workingMode_ workMode;
    workingMode_ lastWorkMode;
};

struct Line
{
    cv::Point2f pose1;
    cv::Point2f pose2;

    cv::Point point1;
    cv::Point point2;
};

struct Lines
{
    Header_ header;
    std::vector<Line> lines;
    void release()
    {
        if (!lines.empty())
        {
            lines.clear();
            std::vector<Line>().swap(lines);
        }
    }
};

struct Rectangle
{
    cv::Point2f pose1;
    cv::Point2f pose2;

    cv::Point point1;
    cv::Point point2;
};

struct Rectangles
{
    Header_ header;
    std::vector<std::vector<PoseStamped>> rectangles;
    void release()
    {
        if (!rectangles.empty())
        {
            rectangles.clear();
            std::vector<std::vector<PoseStamped>>().swap(rectangles);
        }
    }
};

struct APPForbiden
{
    Header_ header;

    Lines wall;
    Rectangles closed;
};

struct APPDelimit
{
    Header_ header;

    Lines limit;
    Rectangles opened;
};

//禁区,划区信息
struct APPInfo
{
    Header_ header;

    APPForbiden forbiden;
    APPDelimit delimit;
    void release()
    {
        forbiden.closed.release();
        forbiden.wall.release();
        delimit.opened.release();
        delimit.limit.release();
    }
};

struct PerceptionDelimit
{
    Header_ header;
    std::vector<std::vector<PoseStamped>> delimit; //深度学习分区数据
    // std::vector<std::vector<PoseStamped>> Obs;//深度学习障碍物数据
    void release()
    {
        delimit.clear();
        std::vector<std::vector<PoseStamped>>().swap(delimit);
        //        Obs.clear();
        //        std::vector<std::vector<PoseStamped>>().swap(Obs);
    }
};

//清扫框
struct BoxInfo
{
    cv::Point point_min; /*小端角点*/
    cv::Point point_max; /*小端角点*/

    cv::Point2f pose_min;
    cv::Point2f pose_max;

    int id;

    void resetBox()
    {
        point_min = cv::Point(0, 0);
        point_max = cv::Point(0, 0);

        pose_min = cv::Point2f(0.0f, 0.0f);
        pose_max = cv::Point2f(0.0f, 0.0f);
        id = 0;
    }
};

//清扫报告
struct CleanReportInfo
{
    Header_ header;
    float Area = 0.0f;     /*清扫面积*/
    float Duration = 0.0f; /*清扫时长*/
};

struct APPMap
{
    Header_ header;

    int width;          //宽
    int height;         //高
    float resolving;    //分辨率
    cv::Point2f origin; //
    cv::Mat map;        //地图

    cv::Point2f origin_real; //世界坐标系下最大包络小端角点
    int width_real;          // 世界坐标系下最大包络(宽栅格数)
    int height_real;         // 世界坐标系下最大包络(高栅格数)

    enum Cell
    {
        Obs = ObsCell,
        Unknow = UnknowCell,
        Free = FreeCell
    };

    //栅格坐标转世界坐标
    cv::Point2f MapToWorld(cv::Point m)
    {
        return cv::Point2f((origin.x + (m.x + 0.5f) * resolving), (origin.y + (m.y + 0.5f) * resolving));
    }

    //世界坐标转栅格坐标
    cv::Point WorldToMap(cv::Point2f w)
    {
        return cv::Point((int)((w.x - origin.x) / resolving), (int)((w.y - origin.y) / resolving));
    }
};

//速度控制
struct SpeedControlInfo
{
    Header_ header;
    float vl = 0.0;
    float va = 0.0;
};

struct SlamState
{
    Header_ header;
    int state;
};

struct ChassisControlInfo
{
    bool SideFan_L;           /*左边刷*/
    bool SideFan_R;           /*右边刷*/
    bool Roller_brush;        /*滚刷*/
    uint8_t Suction_Rating;   /*吸力等级*/
    uint8_t WaterPump_Rating; /*水泵等级*/
    bool switch_Bumper;       /*碰撞传感器电源开关*/
    bool switch_Cliff;        /*下视传感器电源开关*/
    bool switch_Ir;           /*红外传感器电源开关*/
    bool switch_WaterPump;    /*水泵电源开关*/
    bool switch_Charge;       /*充电开关*/
};

struct perceptionObject
{
    enum ClassID
    {
        UNKNOWN = 0,
        SHOES,
        BIN,
        PEDESTAL,
        WIRE,
        SOCKET,
        CAT,
        DOG,
        DESK_RECT,
        DESK_CIRCLE
    };

    enum Status
    {
        ENABLE = 0,
        DISABLE
    };

    int id = -1;
    int classID = UNKNOWN; // ClassID
    float detectExtend;    /*清扫（避障）外扩的距离*/
    std::vector<PoseStamped> locationWorld;
    std::uint64_t time;
    bool status = DISABLE; // Status
    void release()
    {
        locationWorld.clear();
        std::vector<PoseStamped>().swap(locationWorld);
        status = DISABLE;
    }
};

struct perception
{
    Header_ header;

    enum Status
    {
        NO_UPDATE = 0,
        UPDATE
    };
    std::vector<perceptionObject> objects;
    Status status = NO_UPDATE;
    void release()
    {
        for (auto i : objects)
        {
            i.release();
        }
        objects.clear();
        std::vector<perceptionObject>().swap(objects);
        status = NO_UPDATE;
    }
};

struct SegmentInfo//自动分区结果
{
    Header_ header;

    cv::Mat SegmentMap;  // 输出的分区图
    std::vector< std::vector<cv::Point> > border;  // 轮廓列表
    std::vector< std::vector<cv::Point> > door;//门列表
    int SegNumber;  // 分区数量
};
struct SegmentOperationInfo{//自动分区操作
    Header_ header;

    int flag;
    std::vector<int> merge;//合并
    std::pair<int , std::pair<cv::Point , cv::Point>> split;//拆分
};
/*上传数据结构*/
struct SensorInfo
{
    Header_ header;
    workingMode workMode; //工作模式

    PoseStamped Posture;     //机器人当前位姿
    BumperInfo Bumper;       //碰撞传感器信息
    FollowIrInfo Followir;   //沿边传感器信息
    StationIrInfo Stationir; //充电桩信息
    CliffInfo Cliff;         //下视传感器信息
    TofInfo tofdata;         //双目转tof数据

    perception perceptionInfo;               // 深度学习障碍物信息
    PerceptionDelimit PerceptionDelimitInfo; //深度学习划区数据

    SegmentInfo Segment;                     //自动分区数据
    SegmentOperationInfo Segmentoperation;   //自动分区操作数据

    SlamState Slamflg; // slam状态信息
    IMUInfo Imu;       //陀螺仪
    BMS Bms;           //电池管理

    APPInfo appinfo; // app　下发数据

    std::string maploadpath;//地图加载地址
    MapLoadState mapload = MapLoadState::MapLoad_INIT; //地图加载标记
    MotorCurrent motorCur;  //电流数据

    RecoveryStyle recoveryStyle = RecoveryStyle::Recovery_INIT;
    PoseStamped HomePose = PoseStamped(cv::Point2f(-30.0f , -30.0f) , cv::Point(0 , 0));      //充电桩位姿
    PoseStamped StartPose = PoseStamped(cv::Point2f(-30.0f , -30.0f) , cv::Point(0 , 0));     //开始工作位姿
};

/*下发数据结构*/
struct DataDown
{
    SpeedControlInfo cmd;              //轮速控制
    APPMap appmap;                     // app显示地图数据
    ChassisControlInfo ChassisControl; //底盘控制数据
    uint8_t FlgSlam;                   // slam状态
    uint8_t MotionState;               //运动状态
    float area = 0.0f;                 //清扫面积
    int clean_du = 0;                  //清扫时长
};
#endif
