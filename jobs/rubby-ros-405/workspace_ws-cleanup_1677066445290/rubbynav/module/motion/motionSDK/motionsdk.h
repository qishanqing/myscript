#ifndef RUBBYMOTION_LIBRARY_H
#define RUBBYMOTION_LIBRARY_H

#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <list>
#include <thread>

#include "Common.h"
#include "Log.h"

#include "PathTrack.h"
#include "dock.h"
#include "FollowObs.h"
#include "Relocation.h"
#include "MotionMapManage.h"
#include "Operation.h"
#include "ObstacleAvoid.h"
#include "OutDock.h"
#include "Recovery.h"

using namespace std;
using namespace cv;

class MotionSdk : virtual public MotionMapManage, public PathTrack, public Dock, public FollowObs, public Relocation, public ObstacleAvoid, public OutDock, public Recovery
{
public:
    MotionSdk() = default;
    ~MotionSdk()
    {
        Destory();
        if (T_motion != nullptr)
        {
            T_motion = nullptr;
        }
    };

    bool motionInit(SensorInfo *SensorInfo_, DataDown *DataDown_);

    bool reLocationInit();
    int reLocationRun();

    bool firstbuildmapInit();
    bool firstbuildmapRun();

    bool followNavPathInit(vector<PoseStamped> &navPath_);
    int followNavPath();

    bool followBowInit(vector<PoseStamped> &bowPath_);
    int followBowPath();
    int followBowPathDelimit();
    int followBowPathSegment(){ return 0;}

    bool followObsInit(PoseStamped p_nav, PoseStamped p_obs);

    bool followEdgeTypeInit();
    EdgeType followEdgeTypeRun();

    bool followObsEdgeRightInit();
    int followObsEdgeRight();
    bool getObsEdgeLeftStart(PoseStamped &PoseStamped_);
    bool followObsEdgeLeftInit();
    int followObsEdgeLeft();

    bool followVirtualEdgeInit();
    NodeStatus followVirtualEdgeRun();

    void completeTask();

    bool cleanBoxInit(BoxInfo BoxInfo_);
    bool cleanBoxInitDelimit(BoxInfo BoxInfo_);
    bool cleanBoxInitSegment(BoxInfo BoxInfo_){ return false;}

    void Destory();

    int ObstacleAvoidanceInit();
    int ObstacleAvoidance();

    bool dockInit();
    int dockRun();

    bool outDockInit();
    bool outDockRun();

    bool recoveryInit(const RecoveryStyle& recoveryStyle);
    NodeStatus recoveryRun(const RecoveryStyle& recoveryStyle);

    int RunParking();

    bool setRotateAngle(double angle_);

private:

#ifdef ROSDEBUG
    int creat_thread_motion();
    int kill_thread_motion();
#endif

private:
    FILE *fp = nullptr;       //文件指针
    FILE *fpmotion = nullptr; //文件指针
    std::thread *T_motion = nullptr;
    bool flg_thread_motion = true;

    SensorInfo *sensor = nullptr;
    DataDown *datadown = nullptr;
    BoxInfo boxinfo;
    std::vector<BoxInfo> CleanBoxCur_history; /*历史清扫框信息*/
    vector<PoseStamped> cleanpath;
    vector<PoseStamped> navpath;
    PoseStamped followObs_nav;
    PoseStamped followObs_obs;
	PoseStamped followObs_start;
    int flg_navfollowobs = 0;
	int motionstate_last = 0;
	list<Point> followpath;
	list<Point> leftFollowResetPose;
#ifdef NEWCONTROL
    vector<PoseStamped> insertPath;
#endif

    float Relocation_go = 0.5;                  //重定位前进距离
    float dis_overlook = 0.15f;
};

#endif