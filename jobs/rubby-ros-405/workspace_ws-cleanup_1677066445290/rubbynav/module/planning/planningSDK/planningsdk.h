#ifndef RUBBYPLANNING_LIBRARY_H
#define RUBBYPLANNING_LIBRARY_H

#include <iostream>
#include <unistd.h>

#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <list>
#include <thread>

#include "Common.h"
#include "Log.h"
#include "searchCharging.h"

using namespace std;
using namespace cv;

class PlanningSdk : public findChargingpile::searchCharging{

public:
    PlanningSdk() = default;;
    ~PlanningSdk(){
#ifdef ROSDEBUG
        kill_thread_planning();
#endif
        Destroy();
        if(T_planning != nullptr){
            T_planning = nullptr;
        }
    };

    bool planningInit(SensorInfo* SensorInfo_, DataDown* DataDown_);
    bool routineCleanInit();
    bool DelimitCleanInit();
    bool SegmentCleanInit();
    bool getCleanBox(BoxInfo &BoxInfo_);
    bool getCleanDelimit(BoxInfo &BoxInfo_);
    bool getCleanSegment(BoxInfo &BoxInfo_){ return false;}
    int getCleanStart(PoseStamped &PoseStamped_);
    int getCleanStartDelimit(PoseStamped &PoseStamped_);
    int getCleanStartSegment(PoseStamped &PoseStamped_){ return false;}
    int nav(const PoseStamped &start_, const PoseStamped &end_, vector<PoseStamped> &navPth_);
    bool bowInit(const PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_);
    bool bowInitDelimit(const PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_);
    bool bowInitSegment(const PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_){ return false;}
    bool getBowPath(PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_, vector<PoseStamped>& bowPath_);
    bool getBowPathDelimit(PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_, vector<PoseStamped>& bowPath_);
    bool getBowPathSegment(PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_, vector<PoseStamped>& bowPath_){ return false;}
    void updateMap();
    void completeTask();
    bool getSlamPath(BoxInfo BoxInfo_, PoseStamped& slamPose_);
    bool getSlamPathDelimit(BoxInfo BoxInfo_, PoseStamped& slamPose_);
    bool getSlamPathSegment(BoxInfo BoxInfo_, PoseStamped& slamPose_){ return false;}
    bool mergeSlamPath(vector<PoseStamped> navPath_, vector<PoseStamped> &slamPath__);
    void Destroy();
    bool CleanDataRese();
    bool getNearSafePoint(PoseStamped p1 , PoseStamped &p2);

    int getFollowObsPoint(PoseStamped & p_nav , PoseStamped & p_obs);
    bool getSearchChargingPoint(PoseStamped &p);

    bool autoSegment();
    bool autoSegmentOperation();

private:

    int Init();

    /**
     *
     * @param dataup
     * @param motion_state
     * @return
     */
    int  UploadSensor(SensorInfo & dataup , int MotionControlMode);
    int  UploadApp(const APPInfo & data);
    int  DownloadData();

    bool SetCleanTask(int WorkingTask);

    bool CleanBoxInit(const PoseStamped & start);
    bool GetCurCleanBox(const PoseStamped & start , BoxInfo & box);

    bool CleanMarkingInit(const PoseStamped & start);
    bool GetCurCleanMark(const PoseStamped & start , BoxInfo & box);

    bool CleanSegmentInit(const PoseStamped & start);

    int CleanAgain();
    int CleanReport(CleanReportInfo & info);

    int SearchUncleanPoint(const PoseStamped & start , PoseStamped & target);
    int SearchUncleanPoint_checkStart(const PoseStamped & start , vector<Point> vc2);
    int SearchUncleanPoint_remainder(const PoseStamped & start , PoseStamped & target);
    int SearchUncleanPointMarking(const PoseStamped & start , PoseStamped &target);
    int ExploreAround(Point2f start , Point2f &target);

    int SearchObsPoint(const PoseStamped & start , PoseStamped & target_nav , PoseStamped & target_obs);
    int SearchObsPointMarking(const PoseStamped & start , PoseStamped & target_nav , PoseStamped & target_obs);
    int SearchObsPointAutoMarking(const PoseStamped & start , PoseStamped & target_nav , PoseStamped & target_obs);

    int Nav(const PoseStamped & start , const PoseStamped & target , vector<PoseStamped> & path);

    bool GetCleanPath(PoseStamped & start , vector<PoseStamped> & path , int type);
    bool GetCleanPathDelimit(PoseStamped & start , vector<PoseStamped> & path , int type);

    bool Slampath(BoxInfo BoxInfo_, PoseStamped& slamPose_s , PoseStamped& slamPose_e);
    bool GetNearPoint(PoseStamped start , PoseStamped pose_1 , PoseStamped pose_2 , PoseStamped& target);
    int OutObs(Point2f start);

#ifdef ROSDEBUG
    int creat_thread_planning();
    int kill_thread_planning();

#endif
    int creat_thread_buildmap();
    int kill_thread_buildmap();
private:
    FILE *fp = nullptr;//文件指针
    FILE *fpplanning = nullptr;//文件指针
    std::thread *T_planning = nullptr;
    bool flg_thread_planning = true;

    std::thread *T_buildmap = nullptr;
    bool flg_thread_buildmap = true;

    SensorInfo * sensorinfo = nullptr;
    DataDown * datadown = nullptr;
    int workingtask = WorkingTask_Init;
    PoseStamped uncleanpoint = PoseStamped();
    vector<Point> p_history_search;
    int dir_shory = 0;//flg_cleanturn
    int dir_long = 0;//flg_cleango

    PoseStamped slamp1,slamp2;

    std::chrono::steady_clock::time_point clean_time_start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point clean_time_end = std::chrono::steady_clock::now();

    void savemap(string name , const Mat &map);

};

#endif