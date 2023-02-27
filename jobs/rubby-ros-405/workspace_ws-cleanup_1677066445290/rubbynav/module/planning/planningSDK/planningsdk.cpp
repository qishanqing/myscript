#include <utility>


#include<time.h>
#include <zconf.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include "planningsdk.h"
#include "Planner.h"
#include "Map.h"

bool PlanningSdk::planningInit(SensorInfo* SensorInfo_, DataDown* DataDown_){
    s_log.storageTypeInit(LOG_PLANNING);

    if(SensorInfo_ != nullptr){
        sensorinfo = SensorInfo_;
    }else{
        LOGPLANNING(INFO) << "SensorInfo_ is nullptr !!!";
        return false;
    }
    if(DataDown_ != nullptr){
        datadown = DataDown_;
    }else{
        LOGPLANNING(INFO) << "DataDown_ is nullptr !!!";
        return false;
    }
    s_planner.init();

#ifdef ROSDEBUG
    s_map.creat_thread_map();
    creat_thread_planning();
#endif
    creat_thread_buildmap();

    return true;
}
bool PlanningSdk::routineCleanInit(){
    clean_time_start = std::chrono::steady_clock::now();
    SetCleanTask(sensorinfo->workMode.workMode);
    sensorinfo->StartPose = sensorinfo->Posture;
//    findChargingpile::searchCharging::searchCharginginit();
    return CleanBoxInit(sensorinfo->Posture);
}
bool PlanningSdk::DelimitCleanInit(){
    clean_time_start = std::chrono::steady_clock::now();
    SetCleanTask(sensorinfo->workMode.workMode);
    sensorinfo->StartPose = sensorinfo->Posture;
//    if(sensorinfo->workMode.workMode == workingMode_::WORK_AUTOPARTITIONSCLEAN){
//        CleanSegmentInit(sensorinfo->Posture);
//    }
//    findChargingpile::searchCharging::searchCharginginit();
    return CleanMarkingInit(sensorinfo->Posture);
}
bool PlanningSdk::SegmentCleanInit(){
    clean_time_start = std::chrono::steady_clock::now();
    SetCleanTask(sensorinfo->workMode.workMode);
    sensorinfo->StartPose = sensorinfo->Posture;
//    findChargingpile::searchCharging::searchCharginginit();
    return CleanSegmentInit(sensorinfo->Posture);
}
bool PlanningSdk::getCleanBox(BoxInfo &BoxInfo_){
    return GetCurCleanBox(sensorinfo->Posture , BoxInfo_);
}
bool PlanningSdk::getCleanDelimit(BoxInfo &BoxInfo_){
    return GetCurCleanMark(sensorinfo->Posture , BoxInfo_);
}
int PlanningSdk::getCleanStart(PoseStamped &PoseStamped_){
    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();
    sensorinfo->recoveryStyle = RecoveryStyle::Recovery_INIT;
    int flg = SearchUncleanPoint(sensorinfo->Posture , PoseStamped_);

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = time_end - time_start; // std::micro 表示以微秒为时间单位

    LOGPLANNING(INFO) << "time cost getCleanStart:" <<elapsed.count();
    return flg;
}
int PlanningSdk::getCleanStartDelimit(PoseStamped &PoseStamped_){
    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();
    sensorinfo->recoveryStyle = RecoveryStyle::Recovery_INIT;
    int flg = SearchUncleanPointMarking(sensorinfo->Posture , PoseStamped_);
    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = time_end - time_start; // std::micro 表示以微秒为时间单位

    LOGPLANNING(INFO) << "time cost getCleanStartDelimit:" <<elapsed.count();
    return flg;
}
int PlanningSdk::nav(const PoseStamped &start_, const PoseStamped &end_, vector<PoseStamped> &navPth_){
    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();
    sensorinfo->recoveryStyle = RecoveryStyle::Recovery_INIT;
    int flg = Nav(start_ , end_ , navPth_);
    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = time_end - time_start; // std::micro 表示以微秒为时间单位

    LOGPLANNING(INFO) << "time cost nav:" <<elapsed.count();
    return flg;
}
bool PlanningSdk::bowInit(const PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_){
    return true;
}
bool PlanningSdk::bowInitDelimit(const PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_){
    return true;
}
bool PlanningSdk::getBowPath(PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_, vector<PoseStamped>& bowPath_){
    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

    bool flg = GetCleanPath(PoseStamped_ , bowPath_ , 0);
    if(!flg){
        datadown->cmd.va = 0.0f;
        datadown->cmd.vl = 0.0f;
    }

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = time_end - time_start; // std::micro 表示以微秒为时间单位

    LOGPLANNING(INFO) << "time cost getBowPath:" <<elapsed.count();
    return flg;
}
bool PlanningSdk::getBowPathDelimit(PoseStamped &PoseStamped_, const BoxInfo &BoxInfo_, vector<PoseStamped>& bowPath_){
    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

    bool flg = GetCleanPathDelimit(PoseStamped_ , bowPath_ , 0);
    if(!flg){
        datadown->cmd.va = 0.0f;
        datadown->cmd.vl = 0.0f;
    }

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = time_end - time_start; // std::micro 表示以微秒为时间单位

    LOGPLANNING(INFO) << "time cost getBowPathDelimit:" <<elapsed.count();
    return flg;
}
void PlanningSdk::updateMap(){
#if 0
    if(sensorinfo != nullptr){
        UploadSensor(*sensorinfo , datadown->MotionState);
    }
#endif
}
void PlanningSdk::completeTask(){
//    Destroy();
//    s_map.datarelease();
    return;
}
bool PlanningSdk::getSlamPath(BoxInfo BoxInfo_, PoseStamped& slamPose_){
    bool flg = Slampath(std::move(BoxInfo_), slamp1 , slamp2);
    if(flg){
        if(GetNearPoint(sensorinfo->Posture , slamp1 , slamp2 , slamPose_)){
        }else{
            slamPose_ = slamp1;
        }
    }else{
        slamp1 = PoseStamped();
        slamp2 = PoseStamped();
    }
    return flg;
}
bool PlanningSdk::getSlamPathDelimit(BoxInfo BoxInfo_, PoseStamped& slamPose_){
    return false;
}
bool PlanningSdk::mergeSlamPath(vector<PoseStamped> navPath_, vector<PoseStamped> &slamPath__){
    if(navPath_.empty() || slamp1.point == Point(0,0) || slamp2.point == Point(0,0) || navPath_.back().point != slamp1.point){
        LOGPLANNING(INFO) << "navPath_.size:" << navPath_.size();
        LOGPLANNING(INFO) << "navPath_.back():(" << navPath_.back().point.x << ","<<navPath_.back().point.y << ")-(" << navPath_.back().pose.x << "," <<navPath_.back().pose.y <<")";
        LOGPLANNING(INFO) << "slamp1:(" << slamp1.point.x << ","<< slamp1.point.y << ")-(" << slamp1.pose.x << "," <<slamp1.pose.y <<"),slamp2:("<< slamp2.point.x << ","<< slamp2.point.y << ")-(" << slamp2.pose.x << "," <<slamp2.pose.y <<")";
        slamp1 = PoseStamped();
        slamp2 = PoseStamped();
        return false;
    }else{
        slamPath__.clear();
        slamPath__.swap(navPath_);
        slamPath__.emplace_back(slamp2);
        slamp1 = PoseStamped();
        slamp2 = PoseStamped();
        return true;
    }
}

int PlanningSdk::Init(){
    s_planner.init();
//    s_planner.creat_thread_planning();
}
void PlanningSdk::Destroy(){
#ifdef ROSDEBUG
    s_map.kill_thread_map();
#endif
    kill_thread_buildmap();
    s_planner.destroy();
    s_map.datarelease();
    if(!p_history_search.empty()){
        p_history_search.clear();
        vector<Point>().swap(p_history_search);
    }
//    findChargingpile::searchCharging::dataRelease();
    if(sensorinfo != nullptr){
        sensorinfo = nullptr;
    }
    if(datadown != nullptr){
        datadown = nullptr;
    }
}
bool PlanningSdk::CleanDataRese(){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * clean_box = s_planner.getcleanbox();
    if(map_manage == nullptr || clean_box == nullptr){
        LOGPLANNING(INFO) << "* map_manage || clean_box is nullptr !!!";
        return true;
    }
    if(!p_history_search.empty()){
        p_history_search.clear();
        vector<Point>().swap(p_history_search);
    }
    clean_box->datareset();
    s_map.init();
    map_manage->flg_firstfill = true;
    return true;
}
bool PlanningSdk::getNearSafePoint(PoseStamped p1 , PoseStamped &p2){
    if(s_map.NavMap.at<uint8_t >(p1.point) == NavFreeCell &&
        s_map.ForbidenMap.at<uint8_t >(p1.point) == ForbiddenUnknowCell &&
        s_map.DeepLearningMap.at<uint8_t >(p1.point) != ObsCell){
        p2 = p1;
        LOGPLANNING(INFO) << "getNearSafePoint p1 is safe (" << p1.point.x << " , " << p1.point.y << ")";
        return true;
    }else{
        LOGPLANNING(INFO) << "getNearSafePoint p1 is not safe (" << p1.point.x << " , " << p1.point.y << ")";
        Point ptmp;
        for(int i = 1 ; i <= 10 ; i++)
            for(int j = 0 ; j < s_map.GetExpandSingleElement(i)->size() ; j++){
                ptmp = *(s_map.GetExpandSingleElement(i , j)) + p1.point;
                if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                    continue;
                }
                if(s_map.NavMap.at<uint8_t >(ptmp) == NavFreeCell &&
                   s_map.ForbidenMap.at<uint8_t >(ptmp) == ForbiddenUnknowCell &&
                   s_map.DeepLearningMap.at<uint8_t >(ptmp) != ObsCell){
                    p2.point = ptmp;
                    p2.pose = s_map.MapToWorld(p2.point);
                    LOGPLANNING(INFO) << "getNearSafePoint p2 is get (" << p2.point.x << " , " << p2.point.y << ")";
                    return true;
                }
        }
    }
    p2 = p1;
    LOGPLANNING(INFO) << "getNearSafePoint p2 is not get";
    return false;
}
int PlanningSdk::getFollowObsPoint(PoseStamped & p_nav , PoseStamped & p_obs){
    int flg = 0;
    if(sensorinfo->workMode.workMode == workingMode_::WORK_ROUTINECLEAN){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_INIT;
        flg = SearchObsPoint(sensorinfo->Posture , p_nav , p_obs);
    }else if(sensorinfo->workMode.workMode == workingMode_::WORK_DELIMITCLEAN){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_INIT;
        flg = SearchObsPointMarking(sensorinfo->Posture , p_nav , p_obs);
//		flg = SearchObsPoint(sensorinfo->Posture , p_nav , p_obs);
    }else if(sensorinfo->workMode.workMode == workingMode_::WORK_AUTOPARTITIONSCLEAN){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_INIT;
        flg = SearchObsPoint(sensorinfo->Posture , p_nav , p_obs);
    }else{
        LOGPLANNING(INFO) << "workMode is error :" << sensorinfo->workMode.workMode;
    }

    if(flg == 1){
        LOGPLANNING(INFO) << "getFollowObsPoint: p_nav(" << p_nav.point.x << " , " << p_nav.point.y << ") , p_obs(" << p_obs.point.x << " , " << p_obs.point.y << ")";
    }
    return flg;
}
bool PlanningSdk::getSearchChargingPoint(PoseStamped &p){
    return false;
    if(findChargingpile::searchCharging::chargingPilePosition(p)){
        p.point = s_map.WorldToMap(p.pose);
        LOGPLANNING(INFO) << "getSearchChargingPoint:("<< p.pose.x << " , " << p.pose.y << ")-(" << p.point.x << " , " << p.point.y << ")";
        return true;
    }else{
        return false;
    }
}
bool PlanningSdk::autoSegment(){
    s_log.storageTypeInit(LOG_BOX);
    LOGPLANNING(INFO) << "autoSegment";
    RoomSegment * segment = s_planner.getroomsegment();
    if(segment == nullptr){
        LOGPLANNING(INFO) << "* segment is nullptr !!!";
    }

    imwrite(s_log.pathFive_ + "Nav.png",s_map.NavMap);
    segment->SegmentInit(s_map.NavMap , nullptr);
    segment->SegmentRun(s_map.NavMap , sensorinfo->Segment);
    imwrite(s_log.pathFive_ + "SegmentMap.png",sensorinfo->Segment.SegmentMap);
    return true;
}
bool PlanningSdk::autoSegmentOperation(){
    s_log.storageTypeInit(LOG_BOX);
    LOGPLANNING(INFO) << "autoSegment";
    RoomSegment * segment = s_planner.getroomsegment();
    if(segment == nullptr){
        LOGPLANNING(INFO) << "* segment is nullptr !!!";
    }
    segment->SegmentOperation(sensorinfo->Segmentoperation , sensorinfo->Segment);
    imwrite(s_log.pathFive_ + "SegmentMap.png",sensorinfo->Segment.SegmentMap);
    return true;
}
bool PlanningSdk::CleanBoxInit(const PoseStamped &  start){
    s_log.storageTypeInit(LOG_BOX);

    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * clean_box = s_planner.getcleanbox();
    if(map_manage == nullptr || clean_box == nullptr){
        LOGPLANNING(INFO) << "* map_manage || clean_box is nullptr !!!";
        return false;
    }
    if(!p_history_search.empty()){
        p_history_search.clear();
        vector<Point>().swap(p_history_search);
    }
    uncleanpoint.pose = Point2f(-30.0f,-30.0f);
    uncleanpoint.point = Point(0,0);

    clean_box->CleanBoxInit(s_map.CleanBoxMap , start.point);
    LOGPLANNING(INFO) << "box size is:" << clean_box->boxarray.size();
    imwrite(s_log.pathFive_ + "cleanBoxMap.png",s_map.CleanBoxMap);

    BoxInfo box;
    bool flg = clean_box->GetPointInBox(start.point,box);
    if(flg){
        box.pose_min = s_map.MapToWorld(box.point_min);
        box.pose_max = s_map.MapToWorld(box.point_max);

        if(sensorinfo->mapload == MapLoadState::MapLoad_SUCCESS){
            clean_box->SetCurBox_loadmap(box);
        }else{
            clean_box->SetCurBox(box);
        }
    }
    return true;
}
bool PlanningSdk::CleanMarkingInit(const PoseStamped & start){
    s_log.storageTypeInit(LOG_BOX);

    LOGPLANNING(INFO) << "CleanMarkingInit";

    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * clean_box = s_planner.getcleanbox();
    Search * search = s_planner.getsearch();
    if(map_manage == nullptr || clean_box == nullptr || search == nullptr){
        LOGPLANNING(INFO) << "* map_manage || clean_boxis || search is nullptr !!!";
        return false;
    }
    if(!p_history_search.empty()){
        p_history_search.clear();
        vector<Point>().swap(p_history_search);
    }
    uncleanpoint.pose = Point2f(-30.0f,-30.0f);
    uncleanpoint.point = Point(0,0);

    if(!s_map.perceptiondata.delimit.empty()){
        LOGPLANNING(INFO) << "perceptiondata.delimit size:"<<s_map.perceptiondata.delimit.size();
#ifdef RUBBYSIMULATION
        for(int i = 0 ; i < s_map.perceptiondata.delimit.size() ; i++){
            for(int j = 0 ; j < s_map.perceptiondata.delimit[i].size() ; j++){
                s_map.perceptiondata.delimit[i][j].point = s_map.perceptiondata.delimit[i][j].point + start.point;
                s_map.perceptiondata.delimit[i][j].pose = s_map.MapToWorld(s_map.perceptiondata.delimit[i][j].point);
                LOGPLANNING(INFO) << "perceptiondata.delimit: " << i << " - " << j << ":(" <<s_map.perceptiondata.delimit[i][j].point.x << " , " <<s_map.perceptiondata.delimit[i][j].point.y <<")";
            }
        }
#endif
#ifdef RUBBYNAV
        for(int i = 0 ; i < s_map.perceptiondata.delimit.size() ; i++){
            for(int j = 0 ; j < s_map.perceptiondata.delimit[i].size() ; j++){
                LOGPLANNING(INFO) << "perceptiondata.delimit: " << i << " - " << j << ":(" <<s_map.perceptiondata.delimit[i][j].point.x << " , " <<s_map.perceptiondata.delimit[i][j].point.y <<")";
            }
        }
#endif
        map_manage->fill_perception_delimit(s_map.perceptiondata);
    }else{
        LOGPLANNING(INFO) << "perceptiondata.delimit is empty !!!";
    }
    imwrite(s_log.pathFive_ + "PartitionMap.png",s_map.RegionSegmentationMap);

    BoxInfo box;
    bool flg = clean_box->GetPointInDelimit(start.point , box);
    if(box.id == -1){
        Point p_region;
        search->Search_point_out_set(s_map.RegionSegmentationMap,start.point,RegionUnknowCell,p_region);
        box.id = s_map.RegionSegmentationMap.at<uint8_t >(p_region);
        LOGPLANNING(INFO) << "GetCurCleanMark boxinfo->curbox.id == -1 , p_region:(" << p_region.x << " , " << p_region.y << ") - id :" << box.id;
    }
    if(flg){
        if(sensorinfo->mapload == MapLoadState::MapLoad_SUCCESS){
            clean_box->SetCurBox_loadmap(box);
        }else{
            clean_box->SetCurBox(box);
        }
    }
    return true;
}
bool PlanningSdk::CleanSegmentInit(const PoseStamped & start){
    s_log.storageTypeInit(LOG_BOX);
    LOGPLANNING(INFO) << "CleanSegmentInit";

    RoomSegment * segment = s_planner.getroomsegment();
    if(segment == nullptr){
        LOGPLANNING(INFO) << "* segment is nullptr !!!";
    }
	imwrite(s_log.pathFive_ + "Nav.png",s_map.NavMap);
    segment->SegmentInit(s_map.NavMap , nullptr);
    segment->SegmentRun(s_map.NavMap , sensorinfo->Segment);
    if(!sensorinfo->Segment.SegmentMap.empty()){
        imwrite(s_log.pathFive_ + "Segment.png",sensorinfo->Segment.SegmentMap);
    }else{
        LOGPLANNING(INFO) << "sensorinfo->Segment.SegmentMap is empty !!!";
    }
	s_map.perceptiondata.delimit.clear();
    std::vector<PoseStamped> vc;
    for(int i = 0 ; i < sensorinfo->Segment.border.size() ; i++){
        for(int j = 0 ; j < sensorinfo->Segment.border[i].size() ; j++){
            vc.emplace_back(PoseStamped(s_map.MapToWorld(sensorinfo->Segment.border[i][j]) , sensorinfo->Segment.border[i][j]));
        }
        s_map.perceptiondata.delimit.emplace_back(vc);
        vc.clear();
        std::vector<PoseStamped>().swap(vc);
    }

}
int PlanningSdk::CleanAgain(){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * clean_box = s_planner.getcleanbox();
    if(map_manage == nullptr || clean_box == nullptr){
        LOGPLANNING(INFO) << "* map_manage || clean_box is nullptr !!!";
        return 0;
    }
    if(!p_history_search.empty()){
        p_history_search.clear();
        vector<Point>().swap(p_history_search);
    }
    map_manage->cleanagain();
    clean_box->datareset();
}
int PlanningSdk::CleanReport(CleanReportInfo & info){
    info.Area = 0.0f;
    info.Duration = 0.0f;
    return true;
}
bool PlanningSdk::GetCurCleanBox(const PoseStamped & start , BoxInfo & box){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * clean_box = s_planner.getcleanbox();
    if(map_manage == nullptr || clean_box == nullptr){
        LOGPLANNING(INFO) << "* map_manage || clean_box is nullptr !!!";
        return false;
    }
    Point start_;
    if((uncleanpoint.pose.x <= -30.0 || uncleanpoint.pose.y <= -30.0) || (uncleanpoint.pose.x >= 30.0 || uncleanpoint.pose.y >= 30.0)){
        start_ = start.point;
    }else{
        start_ = uncleanpoint.point;
    }

    LOGPLANNING(INFO) << "check clean box:(" << start.pose.x << "," << start.pose.y << ")-("<< start_.x<< "," <<start_.y << ")";
    bool flg = clean_box->GetPointInBox(start_,box);
    if(flg){
        box.pose_min = s_map.MapToWorld(box.point_min);
        box.pose_max = s_map.MapToWorld(box.point_max);

        if(sensorinfo->mapload == MapLoadState::MapLoad_SUCCESS){
            clean_box->SetCurBox_loadmap(box);
        }else{
            clean_box->SetCurBox(box);
        }
        LOGPLANNING(INFO) << "cleanbox start_:(" << start_.x << "," << start_.y << ") box:id-" << box.id << ", pose:(" <<box.pose_min.x << "," << box.pose_min.y << ")-(" << box.pose_max.x << "," << box.pose_max.y << ") , point:(" << box.point_min.x << "," << box.point_min.y << ")-(" << box.point_max.x << "," << box.point_max.y << ")";
    }
    return flg;
}
int PlanningSdk::SearchUncleanPoint(const PoseStamped & start , PoseStamped & target){
    s_log.storageTypeInit(LOG_SEARCH);

    Search * search = s_planner.getsearch();
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * boxinfo = s_planner.getcleanbox();
    CleanPath * clean_path = s_planner.getcleanpath();
    if(search == nullptr || map_manage == nullptr || boxinfo == nullptr || clean_path == nullptr){
        LOGPLANNING(INFO) << "* search || map_manage || boxinfo || clean_path is nullptr !!!";
        return SiggetCleanStart::FindUncleanPoint_Null;
    }
    LOGPLANNING(INFO) << "SearchUncleanPoint start:(" << start.point.x << " , " << start.point.y << ")-(" << start.pose.x << " , " << start.pose.y << ")";

    if(s_map.ForbidenMap.at<uint8_t >(start.point) != ForbiddenUnknowCell){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Forbiden;
        imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
        LOGPLANNING(INFO) << "start in forbiden";
        return SiggetCleanStart::FindUncleanPoint_InObs;//机器人当前在禁区内。
    }

    imwrite(s_log.pathFive_ + "Navmap.png",s_map.NavMap);
    imwrite(s_log.pathFive_ + "Appmap.png",s_map.APPMap);
    imwrite(s_log.pathFive_ + "Covermap.png",s_map.CoverMap);
    imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
    imwrite(s_log.pathFive_ + "DeepLearningmap.png",s_map.DeepLearningMap);
    imwrite(s_log.pathFive_ + "Pathmap.png",s_map.PathMap);

    Mat SearchMap = cv::Mat::ones(s_map.height,s_map.width,CV_8UC1)*255;
    uint8_t nav_v;
    uint8_t cov_v;
    uint8_t forbiden_v;
    LOGPLANNING(INFO) << "SearchUncleanPoint s_map.originPointOccupyMin:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")-(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")"<< " , s_map.originPointOccupyMax:(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")-(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";

    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            nav_v = s_map.NavMap.at<uint8_t >(i,j);
            cov_v = s_map.CoverMap.at<uint8_t >(i,j);
            forbiden_v = s_map.ForbidenMap.at<uint8_t >(i,j);

            if(nav_v == NavFreeCell){
                /*if(cov_v == CoverUnknowCell){
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
                if(cov_v == CoverCell || cov_v == CoverIgnoreCell){
                    SearchMap.at<uint8_t >(i,j) = SearchCovCell;
                }*/
				SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
            }else{
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }
            if(forbiden_v == ForbiddenCell || s_map.DeepLearningMap.at<uint8_t >(i,j) == ObsCell){
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }
        }

    rectangle(SearchMap , s_map.originPointOccupyMin , s_map.originPointOccupyMax , Scalar(SearchObsCell) , 1 , LINE_4);
    imwrite(s_log.pathFive_ + "SearchMap.png",SearchMap);


    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();
    vector<Point> vc;
    vector<Point> vc1;
    vector<Point> vc2;
    if(SearchMap.at<uint8_t >(start.point) != SearchFreeCell){
        Point p;
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                if(SearchMap.at<uint8_t>(p) == SearchFreeCell){
                    vc.emplace_back(p);
                }
            }

        if(vc.empty()){
            if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
            }
            for (int n = 0; n < 6; n++)
                for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                    p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
					if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
						s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
					}
                }
            sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
            LOGPLANNING(INFO) << "start in obs";
            return SiggetCleanStart::FindUncleanPoint_InObs;//机器人当前在障碍物内。
        }else{
            vector<Point>::iterator iter;
            vc1.emplace_back(vc.front());
            iter = find(vc.begin() , vc.end() , vc1.back());
            vc.erase(iter);
            int num_min = 81;
            list<Point> list_out;
            bool sig = false;
            while(true){
                LOGPLANNING(INFO) << "vc:"<< vc.size() << " vc1:" << vc1.size() << " vc2:" << vc2.size();
                for(int i = 0 ; i < vc.size() ; i++){
                    LOGPLANNING(INFO) << "vc:"<< i << "  :(" << vc[i].x << " , " << vc[i].y << ")";
                }
                for(int i = 0 ; i < vc1.size() ; i++){
                    LOGPLANNING(INFO) << "vc1:"<< i << "  :(" << vc1[i].x << " , " << vc1[i].y << ")";
                }
                for(int i = 0 ; i < vc2.size() ; i++){
                    LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
                }
                sig = false;
                for(int i = 0 ; i < vc1.size() ; i++)
                    for(int j = 0 ; j < vc.size() ; j++){
                        if(abs(vc1[i].x - vc[j].x) + abs(vc1[i].y - vc[j].y) == 1){
                            sig = true;
                            vc1.emplace_back(vc[j]);
                            i = (int)vc1.size();
                            break;
                        }
                    }
                if(!sig){
                    search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                    if(list_out.empty() || list_out.size() < 81){
                        vc1.clear();
                    }else{
                        vc2.emplace_back(vc1.front());
                        vc1.clear();
                    }
                    list_out.clear();

                    if(!vc.empty()){
                        vc1.emplace_back(vc.front());
                        iter = find(vc.begin() , vc.end() , vc1.back());
                        vc.erase(iter);

                        if(vc.empty()){
                            search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                            if(list_out.empty() || list_out.size() < 81){
                                vc1.clear();
                            }else{
                                vc2.emplace_back(vc1.front());
                                vc1.clear();
                            }
                            list_out.clear();
                        }
                    }
                }else{
                    iter = find(vc.begin() , vc.end() , vc1.back());
                    vc.erase(iter);

                    if(vc.empty()){
                        search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                        if(list_out.empty() || list_out.size() < 81){
                            vc1.clear();
                        }else{
                            vc2.emplace_back(vc1.front());
                            vc1.clear();
                        }
                        list_out.clear();
                    }
                }
                if(!vc.empty()){
                }else{
                    break;
                }
            }

            vc.clear();
            vc1.clear();

            if(vc2.empty()){
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                        if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                        }
                    }
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                LOGPLANNING(INFO) << "start in obs";
                return SiggetCleanStart::FindUncleanPoint_InObs;//机器人当前在障碍物内。
            }
        }
    }

	for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){

            cov_v = s_map.CoverMap.at<uint8_t >(i,j);

            if(SearchMap.at<uint8_t >(i,j) == SearchFreeCell){
                if(cov_v == CoverUnknowCell){
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
                if(cov_v == CoverCell || cov_v == CoverIgnoreCell){
                    SearchMap.at<uint8_t >(i,j) = SearchCovCell;
                }
			}
        }

    Point target_;

#if 0
    int flg = FindUncleanPoint_Null;
    p_history_search.emplace_back(start.point);

    for(int i = 0 ; i < p_history_search.size() ; i++){
        if(s_map.NavMap.at<uint8_t >(p_history_search[i]) != NavFreeCell){
            LOGPLANNING(INFO) << "remove != Free:(" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
            p_history_search.erase(p_history_search.begin() + i);
            i--;
        }
    }

    vector<Point> p_ignore;
    flg = search->Search_box_point(SearchMap,boxinfo,start.point,target_,p_ignore);
    LOGPLANNING(INFO) << "p_ignore size 0:" << p_ignore.size();
    while(!p_ignore.empty()){
        s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverIgnoreCell;
        p_ignore.pop_back();
    }

    if(flg == FindUncleanPoint_Null){
        for(int k = s_map.originPointOccupyMin.y ; k < s_map.originPointOccupyMax.y ; k++)
            for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
                if(s_map.PathMap.at<uint8_t >(k,j) == PathFreeCell){
                    if(s_map.ForbidenMap.at<uint8_t >(k,j) != ForbiddenCell || s_map.DeepLearningMap.at<uint8_t >(k,j) != ObsCell){
                        s_map.NavMap.at<uint8_t >(k,j) = NavFreeCell;
                        s_map.PathMap.at<uint8_t >(k,j) = PathUnknowCell;
                        if(SearchMap.at<uint8_t >(k,j) == SearchUnknowCell || SearchMap.at<uint8_t >(k,j) == SearchObsCell){
                            SearchMap.at<uint8_t >(k,j) = SearchCovCell;
                        }
                    }
                }
            }

        flg = search->Search_box_point(SearchMap,boxinfo,start.point,target_,p_ignore);
        LOGPLANNING(INFO) << "p_ignore size 1:" << p_ignore.size();
        while(!p_ignore.empty()){
            s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverIgnoreCell;
            p_ignore.pop_back();
        }
        imwrite(s_log.pathFive_ + "SearchMap_1.png",SearchMap);
    }

    if(flg == FindUncleanPoint_Null) {
        if(p_history_search.size() == 1){
            LOGPLANNING(INFO) << "p_history_search.size() == 1";
        }else {
            list<Point> plist;
            Point p_tmp;
            for (int i = (int) p_history_search.size() - 1; i >= 0; i--) {
                p_tmp = p_history_search[i];
                flg = search->Search_box_point(SearchMap,boxinfo,p_tmp,target_,p_ignore);
                if (flg == FindUncleanPoint_Null) {
                    LOGPLANNING(INFO) << "remove FindUncleanPoint_Null (" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
                    p_history_search.erase(p_history_search.begin() + i);
                } else {
                    if (!plist.empty()) {
                        plist.clear();
                    }
                    if (p_tmp != start.point) {
                        int num_tmp = 0;
                        int num_start = 0;
                        if (search->count_set_type(SearchMap , p_tmp , s_map.originPointOccupyMin , s_map.originPointOccupyMax , plist) == 0) {
                            num_tmp = (int) plist.size();
                        }
                        plist.clear();
                        list<Point>().swap(plist);
                        if (search->count_set_type(SearchMap , start.point , s_map.originPointOccupyMin , s_map.originPointOccupyMax , plist) == 0) {
                            num_start = (int) plist.size();
                        }
                        if (num_tmp < num_start) {
                            p_history_search.erase(p_history_search.begin() + i);
                        } else {
                            list<Point>::iterator iter;
                            for (iter = plist.begin(); iter != plist.end(); iter++) {
                                if(s_map.ForbidenMap.at<uint8_t>(*iter) == ForbiddenUnknowCell){
                                    s_map.ForbidenMap.at<uint8_t>(*iter) = ForbiddenMarkCell;
                                }
                                Point p;
                                for (int n = 0; n < 5; n++)
                                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                        p = *((StructuringElement->begin() + n)->begin() + k) + *iter;
                                        if(s_map.ForbidenMap.at<uint8_t>(p) == ForbiddenUnknowCell){
                                            s_map.ForbidenMap.at<uint8_t>(p) = ForbiddenMarkCell;
                                        }
                                    }
                            }
                            imwrite(s_log.pathFive_ + "FindUncleanPoint_InObs.png",s_map.ForbidenMap);
                            plist.clear();
                            list<Point>().swap(plist);
                            SearchMap.release();
                            LOGPLANNING(INFO) << "inobs";
                            return 0;//3,返回值为3表示被困。
                        }
                    } else {
                        break;
                    }
                }
            }
            plist.clear();
            list<Point>().swap(plist);
        }
    }
    vector<Point>().swap(p_ignore);
#endif
#if 1
    int flg = 0;
    vector<Point> p_ignore;
    if(vc2.empty()){
        flg = search->Search_box_point(SearchMap,boxinfo,start.point,target_,p_ignore);
        while(!p_ignore.empty()){
            s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverIgnoreCell;
            p_ignore.pop_back();
        }
        if(flg == 0){
            if(search->Search_p2p(SearchMap , start.point , sensorinfo->StartPose.point)){
                list<Point> list_out;
                search->count_set_type(SearchMap , start.point , s_map.originPointOccupyMin , s_map.originPointOccupyMax , list_out);
                if(!list_out.empty()){
                    while(!list_out.empty()){
                        if(s_map.ForbidenMap.at<uint8_t >(list_out.front()) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(list_out.front()) = ForbiddenMarkCell;
                        }
                        Point p;
                        for (int n = 0; n < 6; n++)
                            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                p = *((StructuringElement->begin() + n)->begin() + k) + list_out.front();
                                if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                                    s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                                }
                            }
                        list_out.pop_front();
                    }
                }
                list_out.clear();
                SearchMap.release();
                LOGPLANNING(INFO) << "Start surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Surrounded;
                return SiggetCleanStart::FindUncleanPoint_InObs;//机器人当前被障碍物环绕，需要脱困。
            }
        }
    }else{
        for(int i = 0 ; i < vc2.size() ; i++){
            LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
            flg = search->Search_box_point(SearchMap,boxinfo,vc2[i],target_,p_ignore);
            while(!p_ignore.empty()){
                s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverIgnoreCell;
                p_ignore.pop_back();
            }
            if(flg > 0){
                break;
            }
        }
        if(flg == 0){
            bool sig = false;
            for(int i = 0 ; i < vc2.size() ; i++){
                if(search->Search_p2p(SearchMap , vc2[i] , sensorinfo->StartPose.point)){
                    sig = true;
                    break;
                }
            }
            vc2.clear();
            if(!sig){
                Point p;
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
						if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
							s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
						}
                    }
                SearchMap.release();
                LOGPLANNING(INFO) << "Start nearby surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                return SiggetCleanStart::FindUncleanPoint_InObs;//机器人当前被陷入障碍物，临近点被障碍物环绕，需要脱困。
            }
        }
    }

#endif

    if(flg > 0){
        if(flg == SiggetCleanStart::FindUncleanPoint_Inside){
            Point p_ = target_;
            clean_path->find_Endpoint(SearchMap,p_,boxinfo->curbox.point_min,boxinfo->curbox.point_max,target_);
            LOGPLANNING(INFO) << "p_:(" << p_.x << " , " << p_.y << ") , target_(" << target_.x << " , " << target_.y << ")";
        }
//        Point p;
//        clean_path->find_corner_point_0(SearchMap,target_,boxinfo->curbox.point_min,boxinfo->curbox.point_max,p);
        clean_path->find_corner_point(SearchMap,target_,boxinfo->curbox.point_min,boxinfo->curbox.point_max);
        if(clean_path->dir_clean_lock == 1){
            dir_shory = 1;
        }else if(clean_path->dir_clean_lock == 2){
            dir_shory = 0;
        }
        if(clean_path->dir_clean_path_first == 1){
            dir_long = 1;
        }else if(clean_path->dir_clean_path_first == 2){
            dir_long = 0;
        }
        map_manage->fill_cover_rectangle(target_,boxinfo->curbox,3);
        target.point = target_;
        target.pose = s_map.MapToWorld(target_);
        uncleanpoint = target;

        LOGPLANNING(INFO) << "SearchUncleanPoint target:(" << target.point.x << " , " << target.point.y << ")-(" << target.pose.x << " , " << target.pose.y << ")";
    }else{
//        p_history_search.clear();
//        vector<Point>().swap(p_history_search);
        imwrite(s_log.pathFive_ + "cleanfinish_Navmap.png",s_map.NavMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Appmap.png",s_map.APPMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Covermap.png",s_map.CoverMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Forbidenmap.png",s_map.ForbidenMap);
        imwrite(s_log.pathFive_ + "cleanfinish_DeepLearningmap.png",s_map.DeepLearningMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Pathmap.png",s_map.PathMap);
        imwrite(s_log.pathFive_ + "cleanfinish_CleanBoxMap.png",s_map.CleanBoxMap);
    }
    SearchMap.release();
    LOGPLANNING(INFO) << "SearchUncleanPoint:flg-" << flg;
    return flg;
}
int PlanningSdk::SearchUncleanPoint_checkStart(const PoseStamped & start , vector<Point> vc2){
    LOGPLANNING(INFO) << "checkStart";
    Search * search = s_planner.getsearch();
    if(search == nullptr){
        LOGPLANNING(INFO) << "* search is nullptr !!!";
        return 0;
    }
    if(s_map.ForbidenMap.at<uint8_t >(start.point) != ForbiddenUnknowCell){
        LOGPLANNING(INFO) << "start in forbiden";
        return 1;//机器人当前在禁区内，不能移动，报警。
    }
    Mat SearchMap;
    if(vc2.empty()){
        LOGPLANNING(INFO) << "start in obs";
        return 2;//机器人当前在障碍物内，需要脱困。
    }else {
        SearchMap = cv::Mat::ones(s_map.height,s_map.width,CV_8UC1)*255;
        uint8_t nav_v;
        uint8_t learn_v;
        uint8_t forbiden_v;
        for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
            for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
                nav_v = s_map.NavMap.at<uint8_t >(i,j);
                learn_v = s_map.DeepLearningMap.at<uint8_t >(i,j);
                forbiden_v = s_map.ForbidenMap.at<uint8_t >(i,j);

                if(forbiden_v != ForbiddenUnknowCell || learn_v != UnknowCell || nav_v != NavFreeCell){
                    SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
                }else{
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
            }
        imwrite(s_log.pathFive_ + "checkStart.png",SearchMap);
        if(search->Search_p2p(SearchMap , start.point , sensorinfo->StartPose.point)){
            SearchMap.release();
            LOGPLANNING(INFO) << "start and sensorinfo->StartPose is connected";
            return 0;
        }
    }

    if(s_map.DeepLearningMap.at<uint8_t>(sensorinfo->StartPose.point) != UnknowCell &&
       s_map.NavMap.at<uint8_t >(sensorinfo->StartPose.point) != NavFreeCell &&
       s_map.ForbidenMap.at<uint8_t >(sensorinfo->StartPose.point) != ForbiddenUnknowCell){
        LOGPLANNING(INFO) << "sensorinfo->StartPose in obs　, can not check start surround with obs";
        return 0;//机器人开始工作位姿在障碍物内，无法判断机器人当前位置是否被障碍物环绕。
    }

    bool flg = false;
    for(int i = 0 ; i < vc2.size() ; i++) {
        LOGPLANNING(INFO) << "vc2:(" << vc2[i].x << " , " << vc2[i].y <<")";
        if(search->Search_p2p(SearchMap , vc2[i] , sensorinfo->StartPose.point)){
            flg = true;
            break;
        }
    }

    if(!flg){
        LOGPLANNING(INFO) << "Start surrounded with obs";
        return 3;//机器人当前被障碍物环绕，需要脱困。
    }

    return 0;
}
int PlanningSdk::SearchUncleanPoint_remainder(const PoseStamped & start , PoseStamped & target){
    s_log.storageTypeInit(LOG_SEARCH);

    Search * search = s_planner.getsearch();
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * boxinfo = s_planner.getcleanbox();
    CleanPath * clean_path = s_planner.getcleanpath();
    if(search == nullptr || map_manage == nullptr || boxinfo == nullptr || clean_path == nullptr){
        LOGPLANNING(INFO) << "* search || map_manage || boxinfo || clean_path is nullptr !!!";
        return 0;
    }
    LOGPLANNING(INFO) << "SearchUncleanPoint_remainder start:(" << start.point.x << " , " << start.point.y << ")-(" << start.pose.x << " , " << start.pose.y << ")";

    imwrite(s_log.pathFive_ + "Navmap.png",s_map.NavMap);
    imwrite(s_log.pathFive_ + "Appmap.png",s_map.APPMap);
    imwrite(s_log.pathFive_ + "Covermap.png",s_map.CoverMap);
    imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
    imwrite(s_log.pathFive_ + "DeepLearningmap.png",s_map.DeepLearningMap);
    imwrite(s_log.pathFive_ + "Pathmap.png",s_map.PathMap);

    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();
    Mat SearchMap = cv::Mat::ones(s_map.height,s_map.width,CV_8UC1)*255;
    uint8_t nav_v;
    uint8_t cov_v;
    uint8_t forbiden_v;
    LOGPLANNING(INFO) << "SearchUncleanPoint_remainder s_map.originPointOccupyMin:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")-(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")"<< " , s_map.originPointOccupyMax:(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")-(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";

    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            nav_v = s_map.NavMap.at<uint8_t >(i,j);
            cov_v = s_map.CoverMap.at<uint8_t >(i,j);
            forbiden_v = s_map.ForbidenMap.at<uint8_t >(i,j);

            if(nav_v == NavFreeCell){
                if(cov_v == CoverUnknowCell || cov_v == CoverIgnoreCell){
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
                if(cov_v == CoverCell){
                    SearchMap.at<uint8_t >(i,j) = SearchCovCell;
                }
            }else{
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }
            if(forbiden_v == ForbiddenCell || s_map.DeepLearningMap.at<uint8_t >(i,j) == ObsCell){
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }
        }
#if 0
    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i ++){//边界封堵
        SearchMap.at<uint8_t >(i,s_map.originPointOccupyMin.x) = SearchObsCell;
        SearchMap.at<uint8_t >(i,s_map.originPointOccupyMax.x) = SearchObsCell;
    }
    for(int i = s_map.originPointOccupyMin.x ; i < s_map.originPointOccupyMax.x ; i ++){//边界封堵
        SearchMap.at<uint8_t >(s_map.originPointOccupyMin.y,i) = SearchObsCell;
        SearchMap.at<uint8_t >(s_map.originPointOccupyMax.y,i) = SearchObsCell;
    }
#endif
    rectangle(SearchMap , s_map.originPointOccupyMin , s_map.originPointOccupyMax , Scalar(SearchObsCell) , 1 , LINE_4);

    Point target_;

    imwrite(s_log.pathFive_ + "SearchMap.png",SearchMap);

    int flg = FindUncleanPoint_Null;
    p_history_search.emplace_back(start.point);

    for(int i = 0 ; i < p_history_search.size() ; i++){
        if(s_map.NavMap.at<uint8_t >(p_history_search[i]) != NavFreeCell){
            LOGPLANNING(INFO) << "remove != Free:(" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
            p_history_search.erase(p_history_search.begin() + i);
            i--;
        }
    }

    vector<Point> p_ignore;
    flg = search->Search_box_point_remainder(SearchMap,boxinfo,start.point,target_,p_ignore);
    LOGPLANNING(INFO) << "p_ignore size 0:" << p_ignore.size();
    while(!p_ignore.empty()){
        s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverCell;
        p_ignore.pop_back();
    }

    if(flg == FindUncleanPoint_Null){
        for(int k = s_map.originPointOccupyMin.y ; k < s_map.originPointOccupyMax.y ; k++)
            for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
                if(s_map.PathMap.at<uint8_t >(k,j) == PathFreeCell){
                    if(s_map.ForbidenMap.at<uint8_t >(k,j) != ForbiddenCell || s_map.DeepLearningMap.at<uint8_t >(k,j) != ObsCell){
                        s_map.NavMap.at<uint8_t >(k,j) = NavFreeCell;
                        s_map.PathMap.at<uint8_t >(k,j) = PathUnknowCell;
                        if(SearchMap.at<uint8_t >(k,j) == SearchUnknowCell || SearchMap.at<uint8_t >(k,j) == SearchObsCell){
                            SearchMap.at<uint8_t >(k,j) = SearchCovCell;
                        }
                    }
                }
            }

        flg = search->Search_box_point_remainder(SearchMap,boxinfo,start.point,target_,p_ignore);
        LOGPLANNING(INFO) << "p_ignore size 1:" << p_ignore.size();
        while(!p_ignore.empty()){
            s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverCell;
            p_ignore.pop_back();
        }
        imwrite(s_log.pathFive_ + "SearchMap_1.png",SearchMap);
    }

    if(flg == FindUncleanPoint_Null) {
        if(p_history_search.size() == 1){
            LOGPLANNING(INFO) << "p_history_search.size() == 1";
        }else {
            list<Point> plist;
            Point p_tmp;
            for (int i = (int) p_history_search.size() - 1; i >= 0; i--) {
                p_tmp = p_history_search[i];
                flg = search->Search_box_point_remainder(SearchMap,boxinfo,p_tmp,target_,p_ignore);
                if (flg == FindUncleanPoint_Null) {
                    LOGPLANNING(INFO) << "remove FindUncleanPoint_Null (" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
                    p_history_search.erase(p_history_search.begin() + i);
                } else {
                    if (!plist.empty()) {
                        plist.clear();
                    }
                    if (p_tmp != start.point) {
                        int num_tmp = 0;
                        int num_start = 0;
                        if (search->count_set_type(SearchMap, p_tmp, Point(0, 0), Point(SearchMap.cols - 1, SearchMap.rows - 1),plist) == 0) {
                            num_tmp = (int) plist.size();
                        }
                        plist.clear();
                        list<Point>().swap(plist);
                        if (search->count_set_type(SearchMap, start.point, Point(0, 0),Point(SearchMap.cols - 1, SearchMap.rows - 1),plist) == 0) {
                            num_start = (int) plist.size();
                        }
                        if (num_tmp < num_start) {
                            p_history_search.erase(p_history_search.begin() + i);
                        } else {

                            list<Point>::iterator iter;
                            for (iter = plist.begin(); iter != plist.end(); iter++) {

                                s_map.ForbidenMap.at<uint8_t>(*iter) = ForbiddenMarkCell;
                                for (int n = 0; n < 5; n++)
                                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                        Point p = *((StructuringElement->begin() + n)->begin() + k) + *iter;

                                        s_map.ForbidenMap.at<uint8_t>(p) = ForbiddenMarkCell;
                                    }
                            }
                            imwrite(s_log.pathFive_ + "FindUncleanPoint_InObs.png",s_map.ForbidenMap);
                            plist.clear();
                            list<Point>().swap(plist);
                            SearchMap.release();
                            return 0;
                        }
                    } else {
                        break;
                    }
                }
            }
            plist.clear();
            list<Point>().swap(plist);
        }
    }

    vector<Point>().swap(p_ignore);

    if(flg > 0){
        if(flg == FindUncleanPoint_Inside){
            Point p_ = target_;
            clean_path->find_Endpoint(SearchMap,p_,boxinfo->curbox.point_min,boxinfo->curbox.point_max,target_);
            LOGPLANNING(INFO) << "p_:(" << p_.x << " , " << p_.y << ") , target_(" << target_.x << " , " << target_.y << ")";
        }

        clean_path->find_corner_point(SearchMap,target_,boxinfo->curbox.point_min,boxinfo->curbox.point_max);
        if(clean_path->dir_clean_lock == 1){
            dir_shory = 1;
        }else if(clean_path->dir_clean_lock == 2){
            dir_shory = 0;
        }
        if(clean_path->dir_clean_path_first == 1){
            dir_long = 1;
        }else if(clean_path->dir_clean_path_first == 2){
            dir_long = 0;
        }
        map_manage->fill_cover_rectangle(target_,boxinfo->curbox,3);
        target.point = target_;
        target.pose = s_map.MapToWorld(target_);
        uncleanpoint = target;

        LOGPLANNING(INFO) << "SearchUncleanPoint_remainder target:(" << target.point.x << " , " << target.point.y << ")-(" << target.pose.x << " , " << target.pose.y << ")";
    }else{
        p_history_search.clear();
        vector<Point>().swap(p_history_search);
        imwrite(s_log.pathFive_ + "cleanfinish_Navmap.png",s_map.NavMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Appmap.png",s_map.APPMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Covermap.png",s_map.CoverMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Forbidenmap.png",s_map.ForbidenMap);
        imwrite(s_log.pathFive_ + "cleanfinish_DeepLearningmap.png",s_map.DeepLearningMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Pathmap.png",s_map.PathMap);
        imwrite(s_log.pathFive_ + "cleanfinish_CleanBoxMap.png",s_map.CleanBoxMap);
    }
    SearchMap.release();
    LOGPLANNING(INFO) << "SearchUncleanPoint_remainder:flg-" << flg;
    return flg;
}

bool PlanningSdk::GetCurCleanMark(const PoseStamped & start , BoxInfo & box){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * clean_box = s_planner.getcleanbox();
    Search * search = s_planner.getsearch();
    if(map_manage == nullptr || clean_box == nullptr || search == nullptr){
        LOGPLANNING(INFO) << "* map_manage || clean_box || search is nullptr !!!";
        return false;
    }
    LOGPLANNING(INFO) << "start: (" << start.pose.x << ", " << start.pose.y << ")-(" << start.point.x << ", " << start.point.y << ")";
    if(start.point.x < 0 || start.point.y < 0 || start.point.x >= s_map.width || start.point.y >= s_map.height){
        return false;
    }else{
        bool flg = clean_box->GetPointInDelimit(start.point , box);
        if(box.id == -1){
            Point p_region;
            search->Search_point_out_set(s_map.RegionSegmentationMap,start.point,RegionUnknowCell,p_region);
            box.id = s_map.RegionSegmentationMap.at<uint8_t >(p_region);
            LOGPLANNING(INFO) << "GetCurCleanMark boxinfo->curbox.id == -1 , p_region:(" << p_region.x << " , " << p_region.y << ") - id :" << box.id;
        }
        if(flg){
            if(sensorinfo->mapload == MapLoadState::MapLoad_SUCCESS){
                clean_box->SetCurBox_loadmap(box);
            }else{
                clean_box->SetCurBox(box);
            }
            LOGPLANNING(INFO) << "box id:" <<box.id << " ,(" << box.point_min.x << "," << box.point_min.y<< ") "<< " ,(" << box.point_max.x << "," << box.point_max.y<< ") ";
        }
    }
    return true;
}
int PlanningSdk::SearchUncleanPointMarking(const PoseStamped & start , PoseStamped &target){
    s_log.storageTypeInit(LOG_SEARCH);

    Search * search = s_planner.getsearch();
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * boxinfo = s_planner.getcleanbox();
    CleanPath * clean_path = s_planner.getcleanpath();
    if(search == nullptr || map_manage == nullptr || boxinfo == nullptr || clean_path == nullptr){
        LOGPLANNING(INFO) << "* search || map_manage || boxinfo || clean_path is nullptr !!!";
        return SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;
    }
    LOGPLANNING(INFO) << "SearchUncleanPointMarking start:(" << start.point.x << " , " << start.point.y << ")-(" << start.pose.x << " , " << start.pose.y << ")";
    LOGPLANNING(INFO) << "SearchUncleanPoint s_map.originPointOccupyMin:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")-(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")"<< " , s_map.originPointOccupyMax:(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")-(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";

    if(s_map.ForbidenMap.at<uint8_t >(start.point) != ForbiddenUnknowCell){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Forbiden;
        imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
        LOGPLANNING(INFO) << "start in forbiden";
        return SiggetCleanStartDelimit::FindUncleanPointDelimit_InObs;//机器人当前在禁区内。
    }

    imwrite(s_log.pathFive_ + "Navmap.png",s_map.NavMap);
    imwrite(s_log.pathFive_ + "Appmap.png",s_map.APPMap);
    imwrite(s_log.pathFive_ + "Covermap.png",s_map.CoverMap);
    imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
    imwrite(s_log.pathFive_ + "DeepLearningmap.png",s_map.DeepLearningMap);
    imwrite(s_log.pathFive_ + "Pathmap.png",s_map.PathMap);
    imwrite(s_log.pathFive_ + "RegionSegmentationMap.png",s_map.RegionSegmentationMap);

    Mat SearchMap = cv::Mat::ones(s_map.height, s_map.width, CV_8UC1) * 255;
    uint8_t nav_v;
    uint8_t cov_v;
    uint8_t forbiden_v;
    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            nav_v = s_map.NavMap.at<uint8_t >(i,j);
            cov_v = s_map.CoverMap.at<uint8_t >(i,j);
            forbiden_v = s_map.ForbidenMap.at<uint8_t >(i,j);
            if(nav_v == NavFreeCell){
                /*if(cov_v == CoverUnknowCell){
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
                if(cov_v == CoverCell || cov_v == CoverIgnoreCell){
                    SearchMap.at<uint8_t >(i,j) = SearchCovCell;
                }*/
				SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
            }else{
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }
            if(forbiden_v == ForbiddenCell || s_map.DeepLearningMap.at<uint8_t >(i,j) == ObsCell){
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }
        }

    rectangle(SearchMap , s_map.originPointOccupyMin , s_map.originPointOccupyMax , Scalar(SearchObsCell) , 1 , LINE_4);
    imwrite(s_log.pathFive_ + "SearchMap.png",SearchMap);


    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();
    vector<Point> vc;
    vector<Point> vc1;
    vector<Point> vc2;
    if(SearchMap.at<uint8_t >(start.point) != SearchFreeCell){
        Point p;
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                if(SearchMap.at<uint8_t>(p) == SearchFreeCell){
                    vc.emplace_back(p);
                }
            }

        if(vc.empty()){
            if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
            }
            for (int n = 0; n < 6; n++)
                for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                    p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
					if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
						s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
					}
                }
            sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
            LOGPLANNING(INFO) << "start in obs";
            return SiggetCleanStartDelimit::FindUncleanPointDelimit_InObs;//机器人当前在障碍物内。
        }else{
            vector<Point>::iterator iter;
            vc1.emplace_back(vc.front());
            iter = find(vc.begin() , vc.end() , vc1.back());
            vc.erase(iter);
            int num_min = 81;
            list<Point> list_out;
            bool sig = false;
            while(true){
                LOGPLANNING(INFO) << "vc:"<< vc.size() << " vc1:" << vc1.size() << " vc2:" << vc2.size();
                for(int i = 0 ; i < vc.size() ; i++){
                    LOGPLANNING(INFO) << "vc:"<< i << "  :(" << vc[i].x << " , " << vc[i].y << ")";
                }
                for(int i = 0 ; i < vc1.size() ; i++){
                    LOGPLANNING(INFO) << "vc1:"<< i << "  :(" << vc1[i].x << " , " << vc1[i].y << ")";
                }
                for(int i = 0 ; i < vc2.size() ; i++){
                    LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
                }
                sig = false;
                for(int i = 0 ; i < vc1.size() ; i++)
                    for(int j = 0 ; j < vc.size() ; j++){
                        if(abs(vc1[i].x - vc[j].x) + abs(vc1[i].y - vc[j].y) == 1){
                            sig = true;
                            vc1.emplace_back(vc[j]);
                            i = (int)vc1.size();
                            break;
                        }
                    }
                if(!sig){
                    search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                    if(list_out.empty() || list_out.size() < 81){
                        vc1.clear();
                    }else{
                        vc2.emplace_back(vc1.front());
                        vc1.clear();
                    }
                    list_out.clear();

                    if(!vc.empty()){
                        vc1.emplace_back(vc.front());
                        iter = find(vc.begin() , vc.end() , vc1.back());
                        vc.erase(iter);

                        if(vc.empty()){
                            search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                            if(list_out.empty() || list_out.size() < 81){
                                vc1.clear();
                            }else{
                                vc2.emplace_back(vc1.front());
                                vc1.clear();
                            }
                            list_out.clear();
                        }
                    }
                }else{
                    iter = find(vc.begin() , vc.end() , vc1.back());
                    vc.erase(iter);

                    if(vc.empty()){
                        search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                        if(list_out.empty() || list_out.size() < 81){
                            vc1.clear();
                        }else{
                            vc2.emplace_back(vc1.front());
                            vc1.clear();
                        }
                        list_out.clear();
                    }
                }
                if(!vc.empty()){
                }else{
                    break;
                }
            }

            vc.clear();
            vc1.clear();

            if(vc2.empty()){
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                        if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                        }
                    }
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                LOGPLANNING(INFO) << "start in obs";
                return SiggetCleanStart::FindUncleanPoint_InObs;//机器人当前在障碍物内。
            }
        }
    }

	for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){

            cov_v = s_map.CoverMap.at<uint8_t >(i,j);

            if(SearchMap.at<uint8_t >(i,j) == SearchFreeCell){
                if(cov_v == CoverUnknowCell){
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
                if(cov_v == CoverCell || cov_v == CoverIgnoreCell){
                    SearchMap.at<uint8_t >(i,j) = SearchCovCell;
                }
			}
        }

    Point target_;
#if 0
    int flg = FindUncleanPoint_Null;
    p_history_search.emplace_back(start.point);

    for(int i = 0 ; i < p_history_search.size() ; i++){
        if(s_map.NavMap.at<uint8_t >(p_history_search[i]) != NavFreeCell){
            LOGPLANNING(INFO) << "remove != Free:(" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
            p_history_search.erase(p_history_search.begin() + i);
            i--;
        }
    }

    vector<Point> p_ignore;
    flg = search->Search_mark_point(SearchMap, s_map.RegionSegmentationMap, boxinfo, start.point, target_,p_ignore);
    LOGPLANNING(INFO) << "p_ignore size 0:" << p_ignore.size();
    while(!p_ignore.empty()){
        s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverIgnoreCell;
        p_ignore.pop_back();
    }

    if(flg == FindUncleanPoint_Null){
        for(int k = s_map.originPointOccupyMin.y ; k < s_map.originPointOccupyMax.y ; k++)
            for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
                if(s_map.PathMap.at<uint8_t >(k,j) == PathFreeCell){
                    if(s_map.ForbidenMap.at<uint8_t >(k,j) != ForbiddenCell || s_map.DeepLearningMap.at<uint8_t >(k,j) != ObsCell){
                        s_map.NavMap.at<uint8_t >(k,j) = NavFreeCell;
                        s_map.PathMap.at<uint8_t >(k,j) = PathUnknowCell;
                        if(SearchMap.at<uint8_t >(k,j) == SearchUnknowCell || SearchMap.at<uint8_t >(k,j) == SearchObsCell){
                            SearchMap.at<uint8_t >(k,j) = SearchCovCell;
                        }
                    }
                }
            }

        flg = search->Search_mark_point(SearchMap, s_map.RegionSegmentationMap, boxinfo, start.point, target_,p_ignore);
        LOGPLANNING(INFO) << "p_ignore size 1:" << p_ignore.size();
        while(!p_ignore.empty()){
            s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverIgnoreCell;
            p_ignore.pop_back();
        }
        imwrite(s_log.pathFive_ + "SearchMap_1.png",SearchMap);
    }

    if(flg == FindUncleanPoint_Null) {
        if(p_history_search.size() == 1){
            LOGPLANNING(INFO) << "p_history_search.size() == 1";
        }else {
            list<Point> plist;
            Point p_tmp;
            for (int i = (int) p_history_search.size() - 1; i >= 0; i--) {
                p_tmp = p_history_search[i];
                flg = search->Search_mark_point(SearchMap, s_map.RegionSegmentationMap, boxinfo, start.point, target_,p_ignore);
                if (flg == FindUncleanPoint_Null) {
                    LOGPLANNING(INFO) << "remove FindUncleanPoint_Null (" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
                    p_history_search.erase(p_history_search.begin() + i);
                } else {
                    if (!plist.empty()) {
                        plist.clear();
                    }
                    if (p_tmp != start.point) {
                        int num_tmp = 0;
                        int num_start = 0;
                        if (search->count_set_type(SearchMap , p_tmp , s_map.originPointOccupyMin , s_map.originPointOccupyMax , plist) == 0) {
                            num_tmp = (int) plist.size();
                        }
                        plist.clear();
                        list<Point>().swap(plist);
                        if (search->count_set_type(SearchMap , start.point , s_map.originPointOccupyMin , s_map.originPointOccupyMax , plist) == 0) {
                            num_start = (int) plist.size();
                        }
                        if (num_tmp < num_start) {
                            p_history_search.erase(p_history_search.begin() + i);
                        } else {
                            list<Point>::iterator iter;
                            for (iter = plist.begin(); iter != plist.end(); iter++) {
                                if(s_map.ForbidenMap.at<uint8_t>(*iter) == ForbiddenUnknowCell){
                                    s_map.ForbidenMap.at<uint8_t>(*iter) = ForbiddenMarkCell;
                                }
                                Point p;
                                for (int n = 0; n < 5; n++)
                                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                        p = *((StructuringElement->begin() + n)->begin() + k) + *iter;
                                        if(s_map.ForbidenMap.at<uint8_t>(p) == ForbiddenUnknowCell){
                                            s_map.ForbidenMap.at<uint8_t>(p) = ForbiddenMarkCell;
                                        }
                                    }
                            }
                            imwrite(s_log.pathFive_ + "FindUncleanPoint_InObs.png",s_map.ForbidenMap);
                            plist.clear();
                            list<Point>().swap(plist);
                            SearchMap.release();
                            LOGPLANNING(INFO) << "inobs";
                            return 0;//3,返回值为3表示被困。
                        }
                    } else {
                        break;
                    }
                }
            }
            plist.clear();
            list<Point>().swap(plist);
        }
    }
    vector<Point>().swap(p_ignore);
#endif
#if 1
    int flg = FindUncleanPoint_Null;
    vector<Point> p_ignore;

    if(vc2.empty()){
        flg = search->Search_mark_point(SearchMap, s_map.RegionSegmentationMap, boxinfo, start.point, target_,p_ignore);
        while(!p_ignore.empty()){
            s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverIgnoreCell;
            p_ignore.pop_back();
        }
        /*if(flg == 0){
            if(search->Search_p2p(SearchMap , start.point , sensorinfo->StartPose.point) == 0){
				LOGPLANNING(INFO) << "start.point:(" << start.point.x << " , " << start.point.y << ")";
				LOGPLANNING(INFO) << "sensorinfo->StartPose.point:(" << sensorinfo->StartPose.point.x << " , " << sensorinfo->StartPose.point.y << ")";
                list<Point> list_out;
                search->count_set_type(SearchMap , start.point , s_map.originPointOccupyMin , s_map.originPointOccupyMax , list_out);
                if(!list_out.empty()){
                    while(!list_out.empty()){
                        if(s_map.ForbidenMap.at<uint8_t >(list_out.front()) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(list_out.front()) = ForbiddenMarkCell;
                        }
                        Point p;
                        for (int n = 0; n < 6; n++)
                            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                p = *((StructuringElement->begin() + n)->begin() + k) + list_out.front();
                                if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                                    s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                                }
                            }
                        list_out.pop_front();
                    }
                }
                list_out.clear();
                SearchMap.release();
                LOGPLANNING(INFO) << "Start surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Surrounded;
                return SiggetCleanStartDelimit::FindUncleanPointDelimit_InObs;//机器人当前被障碍物环绕，需要脱困。
            }
        }*/
    }else{
        for(int i = 0 ; i < vc2.size() ; i++){
            LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
            flg = search->Search_mark_point(SearchMap, s_map.RegionSegmentationMap, boxinfo, vc2[i], target_,p_ignore);
            while(!p_ignore.empty()){
                s_map.CoverMap.at<uint8_t >(p_ignore.back()) = CoverIgnoreCell;
                p_ignore.pop_back();
            }
            if(flg > 0){
                break;
            }
        }
        if(flg == 0){
            bool sig = false;
            for(int i = 0 ; i < vc2.size() ; i++){
                if(search->Search_p2p(SearchMap , vc2[i] , sensorinfo->StartPose.point)){
                    sig = true;
                    break;
                }
            }
            vc2.clear();
            if(!sig){
                Point p;
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
						if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
							s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
						}
                    }
                SearchMap.release();
                LOGPLANNING(INFO) << "Start nearby surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                return SiggetCleanStartDelimit::FindUncleanPointDelimit_InObs;//机器人当前被陷入障碍物，临近点被障碍物环绕，需要脱困。
            }
        }
    }

#endif
    if(flg > 0){
        if(flg == SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside){
            Point p_ = target_;
            clean_path->find_Endpoint_mark(SearchMap,s_map.RegionSegmentationMap,boxinfo->curbox.id,p_,target_);
            printf("p_:(%d,%d) , target_(%d,%d)\n",p_.x,p_.y,target_.x,target_.y);
            LOGPLANNING(INFO) << "p_:(" << p_.x << " , " << p_.y << ") , target_(" << target_.x << " , " << target_.y << ")";
        }
        clean_path->find_corner_point_mark(SearchMap,s_map.RegionSegmentationMap,target_,boxinfo->curbox.id);
        if(clean_path->dir_clean_lock == 1){
            dir_shory = 1;
        }else if(clean_path->dir_clean_lock == 2){
            dir_shory = 0;
        }
        if(clean_path->dir_clean_path_first == 1){
            dir_long = 1;
        }else if(clean_path->dir_clean_path_first == 2){
            dir_long = 0;
        }
        map_manage->fill_cover_delimit_rectangle(target_,s_map.RegionSegmentationMap,boxinfo->curbox,3);
        target.point = target_;
        target.pose = s_map.MapToWorld(target_);
        uncleanpoint = target;

        LOGPLANNING(INFO) << "SearchUncleanPointMarking target:(" << target.point.x << " , " << target.point.y << ")-(" << target.pose.x << " , " << target.pose.y << ")";
    }else{
        p_history_search.clear();
        vector<Point>().swap(p_history_search);
        imwrite(s_log.pathFive_ + "cleanfinish_RegionSegmentationMap.png",s_map.RegionSegmentationMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Navmap.png",s_map.NavMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Appmap.png",s_map.APPMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Covermap.png",s_map.CoverMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Forbidenmap.png",s_map.ForbidenMap);
        imwrite(s_log.pathFive_ + "cleanfinish_DeepLearningmap.png",s_map.DeepLearningMap);
        imwrite(s_log.pathFive_ + "cleanfinish_Pathmap.png",s_map.PathMap);

    }
    SearchMap.release();
    LOGPLANNING(INFO) << "SearchUncleanPointMarking:flg-" << flg;
    return flg;
}
int PlanningSdk::ExploreAround(Point2f start , Point2f &target){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    Search * search = s_planner.getsearch();
    if(map_manage == nullptr || search == nullptr){
        LOGPLANNING(INFO) << "* nav || map_manage || search nullptr !!!";
        return Search_Null;
    }
    Point p_start,p_target;
    p_start = s_map.WorldToMap(start);
    LOGPLANNING(INFO) << "p_start: (" << p_start.x << ", " << p_start.y << ")";
    p_target = s_map.WorldToMap(start);

    Mat map;
    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();
//    s_map.NavMapMutex.writeLock();
//    imwrite(logpath + "/explor/explor_src.png",s_map.NavMap);

//    s_map.PathMapMutex.readLock();
    for(int i = 0 ; i < s_map.NavMap.rows ; i++)
        for(int j = 0 ; j < s_map.NavMap.cols ; j++){
            if(s_map.PathMap.at<uint8_t >(i,j) == PathFreeCell && s_map.NavMap.at<uint8_t >(i,j) != NavSensorObsCell){
                s_map.NavMap.at<uint8_t >(i,j) = NavFreeCell;
            }
        }
//    s_map.PathMapMutex.readUnlock();
    for(int n = 0 ; n < 7 ; n ++)
        for(int i = 0 ; i < (StructuringElement->begin() + n)->size() ; i++){
            Point p = *((StructuringElement->begin() + n)->begin()+i) + p_start;
            if(s_map.NavMap.at<uint8_t >(p) == NavUnknowCell){
                s_map.NavMap.at<uint8_t >(p) = NavFreeCell;
            }
        }
    for(int n = 0 ; n < 1 ; n ++)
        for(int i = 0 ; i < (StructuringElement->begin() + n)->size() ; i++){
            Point p = *((StructuringElement->begin() + n)->begin()+i) + p_start;
            if(s_map.NavMap.at<uint8_t >(p) != NavFreeCell){
                s_map.NavMap.at<uint8_t >(p) = NavFreeCell;
            }
        }
    if(s_map.NavMap.at<uint8_t >(p_start) != NavFreeCell){
        s_map.NavMap.at<uint8_t >(p_start) = NavFreeCell;
    }
    map_manage->setexplormap(s_map.NavMap,map);
//    s_map.NavMapMutex.writeUnlock();

//    imwrite(logpath + "/explor/explor.png",map);

    int flg = Search_Null;
    p_history_search.emplace_back(p_start);
//    s_map.NavMapMutex.readLock();
    for(int i = 0 ; i < p_history_search.size() ; i++){
        if(s_map.NavMap.at<uint8_t >(p_history_search[i]) != NavFreeCell){
            p_history_search.erase(p_history_search.begin() + i);
            i--;
        }
    }
//    s_map.NavMapMutex.readUnlock();
    list<Point> plist;
    Point p_tmp;
    for(int i = (int)p_history_search.size() - 1 ; i >= 0  ; i--) {
        p_tmp = p_history_search[i];
        LOGPLANNING(INFO) << "history point " << i << ": (" << p_tmp.x << ", " << p_tmp.y << ")";

        flg = search->Search_point_explore(map, p_tmp, p_target);
        if(flg == Search_Null){
            p_history_search.erase(p_history_search.begin() + i);
        }else{
            plist.clear();
            list<Point>().swap(plist);
            if(p_tmp != p_start){
                int num_tmp = 0;
                int num_start = 0;
                if(search->count_set_type(map, p_tmp, Point(0, 0), Point(map.cols - 1, map.rows - 1) , plist) == 0){
                    num_tmp = (int)plist.size();
                }
                plist.clear();
                list<Point>().swap(plist);
                if(search->count_set_type(map, p_start, Point(0, 0), Point(map.cols - 1, map.rows - 1) , plist) == 0){
                    num_start = (int)plist.size();
                }
                if(num_tmp < num_start){
                    //删掉当前点
                    p_history_search.erase(p_history_search.begin() + i);
                }else{
                    //机器人被困
                    //1. 填充 可通行的小区域,以机器人当前位置为中心,边长为2m的方形范围内
                    //2. 查找被困区域[边界点]
                    //3. 查找每个[边界点]与被困区域以外的可通行区域的[最近点]
                    //4. 找到[边界点]与[最近点]距离最近的[点对]
                    //5. 把[点对]联通.
                    //6. 再次搜索.

                    list<Point>::iterator iter;
//                    s_map.ForbidenMapMutex.writeLock();
                    for(iter = plist.begin() ; iter != plist.end() ; iter ++){
                        s_map.ForbidenMap.at<uint8_t >(*iter) = ForbiddenMarkCell;
                        for(int n = 0 ; n < 5 ; n ++)
                            for(int k = 0 ; k < (StructuringElement->begin() + n)->size() ; k++){
                                Point p = *((StructuringElement->begin() + n)->begin() + k) + *iter;
//                                for(int m = 0 ; m < p_inobs.size() ; m++){
//                                    if(p_inobs[m] == p){
//                                        break;
//                                    }else if( m == p_inobs.size() - 1){
//                                        p_inobs.emplace_back(p);
//                                        s_map.NavMap.at<uint8_t >(p) = NavSensorObsCell;
//                                    }
//                                }
                                s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                            }
                    }
//                    s_map.ForbidenMapMutex.writeUnlock();
                    plist.clear();
                    list<Point>().swap(plist);
                    map.release();
                    return Search_StartInObs;
                }
            }else{
                break;
            }
        }
    }
    plist.clear();
    list<Point>().swap(plist);

    if(flg){
        target = s_map.MapToWorld(p_target);
    }else{
        p_history_search.clear();
        vector<Point>().swap(p_history_search);
        int num_ = 0;
//        s_map.NavMapMutex.readLock();
//        imwrite(logpath + "/explor/explor_finished.png",s_map.NavMap);
        for(int i = 0 ; i < s_map.height ; i++)
            for(int j = 0 ; j < s_map.width ; j++){
                if(s_map.NavMap.at<uint8_t >(i,j) != NavUnknowCell){
                    num_ ++;
                }
            }
//        s_map.NavMapMutex.readUnlock();

        LOGPLANNING(INFO) << "探明区域大小为: " << num_*0.04*0.04<< "(M^2) , 栅格数: " << num_ ;
        // 基于当前系统的当前日期/时间
        time_t now = time(nullptr);
        // 把 now 转换为字符串形式
        char* dt = ctime(&now);
        cout << "结束时间：" << dt << endl;

    }

    LOGPLANNING(INFO) << "Explore flg: " << flg << ", p_start(" << p_start.x << ", " << p_start.y << ")-(" << start.x << ", " << start.y << "),explore target(" << p_target.x << ", " << p_target.y << ")-(" << target.x << ", " << target.y << ")";

    map.release();
    return flg;
}

int PlanningSdk::SearchObsPoint(const PoseStamped & start , PoseStamped & target_nav , PoseStamped & target_obs){
    s_log.storageTypeInit(LOG_SEARCH);

    Search * search = s_planner.getsearch();
    if(search == nullptr){
        LOGPLANNING(INFO) << "* search is nullptr !!!";
        return SigSearchObsPoint::FindObs_Null;
    }
    target_nav.point = Point(0 , 0);
    target_obs.point = Point(0 , 0);
    LOGPLANNING(INFO) << "SearchObsPoint start:(" << start.point.x << " , " << start.point.y << ")-(" << start.pose.x << " , " << start.pose.y << ")";

    if(s_map.ForbidenMap.at<uint8_t >(start.point) != ForbiddenUnknowCell){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Forbiden;
        imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
        LOGPLANNING(INFO) << "start in forbiden";
        return SigSearchObsPoint::FindObs_InObs;//机器人当前在禁区内。
    }

    imwrite(s_log.pathFive_ + "Navmap.png",s_map.NavMap);
    imwrite(s_log.pathFive_ + "Appmap.png",s_map.APPMap);
//    imwrite(s_log.pathFive_ + "Covermap.png",s_map.CoverMap);
    imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
//    imwrite(s_log.pathFive_ + "DeepLearningmap.png",s_map.DeepLearningMap);
//    imwrite(s_log.pathFive_ + "Pathmap.png",s_map.PathMap);
    imwrite(s_log.pathFive_ + "FollowMap.png",s_map.FollowMap);

    Mat SearchMap = cv::Mat::ones(s_map.height,s_map.width,CV_8UC1)*255;
    uint8_t nav_v;
    uint8_t forbiden_v;
    uint8_t follow_v;
    uint8_t learning_v;
    LOGPLANNING(INFO) << "SearchObsPoint s_map.originPointOccupyMin:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")"<< " , s_map.originPointOccupyMax:(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";

    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            nav_v = s_map.NavMap.at<uint8_t >(i,j);
            forbiden_v = s_map.ForbidenMap.at<uint8_t >(i,j);
            follow_v = s_map.FollowMap.at<uint8_t >(i,j);
            //learning_v = s_map.DeepLearningMap.at<uint8_t >(i,j);

            if(follow_v != FollowUnknowCell || forbiden_v == ForbiddenCell){
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }else{
                if(nav_v == NavObsExpandCell){
                    SearchMap.at<uint8_t >(i,j) = SearchCovCell;
                }else if(nav_v == NavFreeCell){
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
                if(nav_v == NavObsCell || nav_v == NavSensorObsCell){
                    SearchMap.at<uint8_t >(i,j) = SearchObsCell;
                }
            }
        }

    rectangle(SearchMap , s_map.originPointOccupyMin , s_map.originPointOccupyMax , Scalar(SearchUnknowCell) , 1 , LINE_4);
    imwrite(s_log.pathFive_ + "SearchMap.png",SearchMap);

    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();
    vector<Point> vc;
    vector<Point> vc1;
    vector<Point> vc2;
    if(SearchMap.at<uint8_t >(start.point) != SearchFreeCell){
        Point p;
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                if(SearchMap.at<uint8_t>(p) == SearchFreeCell){
                    vc.emplace_back(p);
                }
            }

        if(vc.empty()){
            if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
            }
            for (int n = 0; n < 6; n++)
                for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                    p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
					if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
						s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
					}
                }
            sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
            LOGPLANNING(INFO) << "start in obs";
            return SigSearchObsPoint::FindObs_InObs;//机器人当前在障碍物内。
        }else{
            vector<Point>::iterator iter;
            vc1.emplace_back(vc.front());
            iter = find(vc.begin() , vc.end() , vc1.back());
            vc.erase(iter);
            int num_min = 81;
            list<Point> list_out;
            bool sig = false;
            while(true){
                LOGPLANNING(INFO) << "vc:"<< vc.size() << " vc1:" << vc1.size() << " vc2:" << vc2.size();
                for(int i = 0 ; i < vc.size() ; i++){
                    LOGPLANNING(INFO) << "vc:"<< i << "  :(" << vc[i].x << " , " << vc[i].y << ")";
                }
                for(int i = 0 ; i < vc1.size() ; i++){
                    LOGPLANNING(INFO) << "vc1:"<< i << "  :(" << vc1[i].x << " , " << vc1[i].y << ")";
                }
                for(int i = 0 ; i < vc2.size() ; i++){
                    LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
                }
                sig = false;
                for(int i = 0 ; i < vc1.size() ; i++)
                    for(int j = 0 ; j < vc.size() ; j++){
                        if(abs(vc1[i].x - vc[j].x) + abs(vc1[i].y - vc[j].y) == 1){
                            sig = true;
                            vc1.emplace_back(vc[j]);
                            i = (int)vc1.size();
                            break;
                        }
                    }
                if(!sig){
					num_min = 81;
                    search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                    if(list_out.empty() || list_out.size() < 81){
                        vc1.clear();
                    }else{
                        vc2.emplace_back(vc1.front());
                        vc1.clear();
                    }
                    list_out.clear();

                    if(!vc.empty()){
                        vc1.emplace_back(vc.front());
                        iter = find(vc.begin() , vc.end() , vc1.back());
                        vc.erase(iter);

                        if(vc.empty()){
							num_min = 81;
                            search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                            if(list_out.empty() || list_out.size() < 81){
                                vc1.clear();
                            }else{
                                vc2.emplace_back(vc1.front());
                                vc1.clear();
                            }
                            list_out.clear();
                        }
                    }
                }else{
                    iter = find(vc.begin() , vc.end() , vc1.back());
                    vc.erase(iter);

                    if(vc.empty()){
						num_min = 81;
                        search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                        if(list_out.empty() || list_out.size() < 81){
                            vc1.clear();
                        }else{
                            vc2.emplace_back(vc1.front());
                            vc1.clear();
                        }
                        list_out.clear();
                    }
                }
                if(!vc.empty()){
                }else{
                    break;
                }
            }

            vc.clear();
            vc1.clear();

			if(vc2.empty()){
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
				for (int n = 0; n < 6; n++)
					for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
						p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
						if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
							s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
						}
					}
				sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
				LOGPLANNING(INFO) << "start in obs";
				return SigSearchObsPoint::FindObs_InObs;//机器人当前在障碍物内。
			}
        }
    }

#if 0
    for(int i = 0 ; i < p_history_search.size() ; i++){
        if(SearchMap.at<uint8_t >(p_history_search[i]) != SearchFreeCell){
            LOGPLANNING(INFO) << "remove != Free:(" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
            p_history_search.erase(p_history_search.begin() + i);
            i--;
        }
    }

    Mat map_search = Mat::zeros(SearchMap.rows , SearchMap.cols , CV_8UC1);
    bool flg = false;
	if(SearchMap.at<uint8_t >(start.point) == SearchFreeCell){
		flg = search->Search_obs_point(SearchMap,s_map.originPointOccupyMin , s_map.originPointOccupyMax,start.point,target_obs.point,map_search);
	}else{
		Point p_;
        for(int i = 1 ; i < 5 ; i++)
            for(int j = 0 ; j < s_map.GetExpandSingleElement(i)->size() ; j++){
                p_ = *s_map.GetExpandSingleElement(i,j) + start.point;
				if(SearchMap.at<uint8_t >(p_) == SearchFreeCell){
					flg = search->Search_obs_point(SearchMap,s_map.originPointOccupyMin , s_map.originPointOccupyMax,p_,target_obs.point,map_search);
					if(flg){
						LOGPLANNING(INFO) << "start in obs , use replace point:(" << p_.x << " , " << p_.y << ") , target_obs.point:(" << target_obs.point.x << " , " << target_obs.point.y << ")";
						i = 5;
						break;
					}
				}
            }
	}

    if(!flg){
        Point tmp_start;
        for (int i = (int) p_history_search.size() - 1; i >= 0; i--) {
            tmp_start = p_history_search[i];
            flg = search->Search_obs_point(SearchMap,s_map.originPointOccupyMin , s_map.originPointOccupyMax,tmp_start,target_obs.point,map_search);
//            map_history = map_search.clone();
//            map_history = map_history * 255;
//            imwrite(s_log.pathFive_ + to_string(i) + "_map_history.png",map_history);

            if(flg){
                break;
            }
        }
    }else{
        p_history_search.emplace_back(start.point);
    }
#endif
#if 1
    bool flg = false;
    Mat map_search = Mat::zeros(SearchMap.rows , SearchMap.cols , CV_8UC1);
    if(vc2.empty()){
        flg = search->Search_obs_point(SearchMap,s_map.originPointOccupyMin , s_map.originPointOccupyMax,start.point,target_obs.point,map_search);
        if(flg == 0){
            if(search->Search_p2p(SearchMap , start.point , sensorinfo->StartPose.point)){
                list<Point> list_out;
                search->count_set_type(SearchMap , start.point , s_map.originPointOccupyMin , s_map.originPointOccupyMax , list_out);
                if(!list_out.empty()){
                    while(!list_out.empty()){
                        if(s_map.ForbidenMap.at<uint8_t >(list_out.front()) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(list_out.front()) = ForbiddenMarkCell;
                        }
                        Point p;
                        for (int n = 0; n < 6; n++)
                            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                p = *((StructuringElement->begin() + n)->begin() + k) + list_out.front();
                                if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                                    s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                                }
                            }
                        list_out.pop_front();
                    }
                }
                list_out.clear();
                SearchMap.release();
                LOGPLANNING(INFO) << "Start surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Surrounded;
                return SigSearchObsPoint::FindObs_InObs;//机器人当前被障碍物环绕，需要脱困。
            }
        }
    }else{
        for(int i = 0 ; i < vc2.size() ; i++){
            LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
            flg = search->Search_obs_point(SearchMap,s_map.originPointOccupyMin , s_map.originPointOccupyMax,vc2[i],target_obs.point,map_search);
            if(flg > 0){
                break;
            }
        }
        if(flg == 0){
            bool sig = false;
            for(int i = 0 ; i < vc2.size() ; i++){
                if(search->Search_p2p(SearchMap , vc2[i] , sensorinfo->StartPose.point)){
                    sig = true;
                    break;
                }
            }
            vc2.clear();
            if(!sig){
                Point p;
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
						if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
							s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
						}
                    }
                SearchMap.release();
                LOGPLANNING(INFO) << "Start nearby surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                return SigSearchObsPoint::FindObs_InObs;//机器人当前被陷入障碍物，临近点被障碍物环绕，需要脱困。
            }
        }
    }
#endif

    if(flg){
		LOGPLANNING(INFO) << "target_obs:("<< target_obs.point.x << " , " << target_obs.point.y << ")";
        Point p_;
        for(int i = 1 ; i < 10 ; i++)
            for(int j = 0 ; j < s_map.GetExpandSingleElement(i)->size() ; j++){
                p_ = *s_map.GetExpandSingleElement(i,j) + target_obs.point;
                if(map_search.at<uint8_t >(p_)){
                    if(SearchMap.at<uint8_t >(p_) == SearchFreeCell){
						list<Point> list_out;
						int num_ = 81;
						search->count_set_type(SearchMap , p_ , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_ , list_out);
						if(list_out.size() < 81){
							list_out.clear();
						}else{
							list_out.clear();
							target_nav.point = p_;
							i = 10;
							break;
						}
                    }
                }
            }
    }

    map_search.release();

    if(target_nav.point == Point(0 , 0) || target_obs.point == Point(0 , 0)){
        SearchMap.release();
        LOGPLANNING(INFO) << "SearchObsPoint:not get target !!!";
        LOGPLANNING(INFO) << "target_nav:(" << target_nav.point.x << " , " << target_nav.point.y << ") , target_obs:(" << target_obs.point.x << " , " << target_obs.point.y << ")";
        return SigSearchObsPoint::FindObs_Null;
    }else{
        Point p;
		if(SearchMap.at<uint8_t>(target_obs.point) == SearchObsCell){
			s_map.FollowMap.at<uint8_t >(target_obs.point) = FollowIgnoreCell;
		}
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + target_obs.point;
                if(SearchMap.at<uint8_t>(p) == SearchObsCell){
                    s_map.FollowMap.at<uint8_t >(p) = FollowIgnoreCell;
                }
            }
        SearchMap.release();
        target_nav.pose = s_map.MapToWorld(target_nav.point);
        target_obs.pose = s_map.MapToWorld(target_obs.point);
        LOGPLANNING(INFO) << "target_nav:(" << target_nav.point.x << " , " << target_nav.point.y << ")-(" << target_nav.pose.x << " , " << target_nav.pose.y << ")";
        LOGPLANNING(INFO) << "target_obs:(" << target_obs.point.x << " , " << target_obs.point.y << ")-(" << target_obs.pose.x << " , " << target_obs.pose.y << ")";
        return SigSearchObsPoint::FindObs_success;
    }
}
int PlanningSdk::SearchObsPointMarking(const PoseStamped & start , PoseStamped & target_nav , PoseStamped & target_obs){
    s_log.storageTypeInit(LOG_SEARCH);

    Search * search = s_planner.getsearch();
    CleanBox * boxinfo = s_planner.getcleanbox();
    if(search == nullptr || boxinfo == nullptr){
        LOGPLANNING(INFO) << "* search || boxinfo is nullptr !!!";
        return SigSearchObsPoint::FindObs_Null;
    }
    target_nav.point = Point(0 , 0);
    target_obs.point = Point(0 , 0);
    LOGPLANNING(INFO) << "SearchObsPointMarking start:(" << start.point.x << " , " << start.point.y << ")-(" << start.pose.x << " , " << start.pose.y << ")";

    if(s_map.ForbidenMap.at<uint8_t >(start.point) != ForbiddenUnknowCell){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Forbiden;
        imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
        LOGPLANNING(INFO) << "start in forbiden";
        return SigSearchObsPoint::FindObs_InObs;//机器人当前在禁区内。
    }

    imwrite(s_log.pathFive_ + "Navmap.png",s_map.NavMap);
    imwrite(s_log.pathFive_ + "Appmap.png",s_map.APPMap);
//    imwrite(s_log.pathFive_ + "Covermap.png",s_map.CoverMap);
    imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
    imwrite(s_log.pathFive_ + "DeepLearningmap.png",s_map.DeepLearningMap);
//    imwrite(s_log.pathFive_ + "Pathmap.png",s_map.PathMap);
    imwrite(s_log.pathFive_ + "FollowMap.png",s_map.FollowMap);
	imwrite(s_log.pathFive_ + "RegionSegmentationMap.png",s_map.RegionSegmentationMap);


    Mat SearchMap = cv::Mat::ones(s_map.height,s_map.width,CV_8UC1)*255;
    uint8_t nav_v;
    uint8_t forbiden_v;
    uint8_t follow_v;
    LOGPLANNING(INFO) << "SearchObsPointMarking s_map.originPointOccupyMin:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")"<< " , s_map.originPointOccupyMax:(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";

    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            nav_v = s_map.NavMap.at<uint8_t >(i,j);
            forbiden_v = s_map.ForbidenMap.at<uint8_t >(i,j);
            follow_v = s_map.FollowMap.at<uint8_t >(i,j);

            if(follow_v != FollowUnknowCell || forbiden_v == ForbiddenCell){
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }else{
                if(nav_v == NavObsExpandCell){
                    SearchMap.at<uint8_t >(i,j) = SearchCovCell;
                }else if(nav_v == NavFreeCell){
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
                if(nav_v == NavObsCell || nav_v == NavSensorObsCell){
                    SearchMap.at<uint8_t >(i,j) = SearchObsCell;
                }
            }
        }

    rectangle(SearchMap , s_map.originPointOccupyMin , s_map.originPointOccupyMax , Scalar(SearchUnknowCell) , 1 , LINE_4);
    imwrite(s_log.pathFive_ + "SearchMap.png",SearchMap);


    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();
    vector<Point> vc;
    vector<Point> vc1;
    vector<Point> vc2;
    if(SearchMap.at<uint8_t >(start.point) != SearchFreeCell){
        Point p;
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                if(SearchMap.at<uint8_t>(p) == SearchFreeCell){
                    vc.emplace_back(p);
                }
            }

        if(vc.empty()){
            if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
            }
            for (int n = 0; n < 6; n++)
                for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                    p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
					if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
						s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
					}
                }
            sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
            LOGPLANNING(INFO) << "start in obs";
            return SigSearchObsPoint::FindObs_InObs;//机器人当前在障碍物内。
        }else{
            vector<Point>::iterator iter;
            vc1.emplace_back(vc.front());
            iter = find(vc.begin() , vc.end() , vc1.back());
            vc.erase(iter);
            int num_min = 81;
            list<Point> list_out;
            bool sig = false;
            while(true){
                LOGPLANNING(INFO) << "vc:"<< vc.size() << " vc1:" << vc1.size() << " vc2:" << vc2.size();
                for(int i = 0 ; i < vc.size() ; i++){
                    LOGPLANNING(INFO) << "vc:"<< i << "  :(" << vc[i].x << " , " << vc[i].y << ")";
                }
                for(int i = 0 ; i < vc1.size() ; i++){
                    LOGPLANNING(INFO) << "vc1:"<< i << "  :(" << vc1[i].x << " , " << vc1[i].y << ")";
                }
                for(int i = 0 ; i < vc2.size() ; i++){
                    LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
                }
                sig = false;
                for(int i = 0 ; i < vc1.size() ; i++)
                    for(int j = 0 ; j < vc.size() ; j++){
                        if(abs(vc1[i].x - vc[j].x) + abs(vc1[i].y - vc[j].y) == 1){
                            sig = true;
                            vc1.emplace_back(vc[j]);
                            i = (int)vc1.size();
                            break;
                        }
                    }
                if(!sig){
                    search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                    if(list_out.empty() || list_out.size() < 81){
                        vc1.clear();
                    }else{
                        vc2.emplace_back(vc1.front());
                        vc1.clear();
                    }
                    list_out.clear();

                    if(!vc.empty()){
                        vc1.emplace_back(vc.front());
                        iter = find(vc.begin() , vc.end() , vc1.back());
                        vc.erase(iter);

                        if(vc.empty()){
                            search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                            if(list_out.empty() || list_out.size() < 81){
                                vc1.clear();
                            }else{
                                vc2.emplace_back(vc1.front());
                                vc1.clear();
                            }
                            list_out.clear();
                        }
                    }
                }else{
                    iter = find(vc.begin() , vc.end() , vc1.back());
                    vc.erase(iter);

                    if(vc.empty()){
                        search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                        if(list_out.empty() || list_out.size() < 81){
                            vc1.clear();
                        }else{
                            vc2.emplace_back(vc1.front());
                            vc1.clear();
                        }
                        list_out.clear();
                    }
                }
                if(!vc.empty()){
                }else{
                    break;
                }
            }

            vc.clear();
            vc1.clear();

            if(vc2.empty()){
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                        if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                        }
                    }
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                LOGPLANNING(INFO) << "start in obs";
                return SigSearchObsPoint::FindObs_InObs;//机器人当前在障碍物内。
            }
        }
    }

#if 0
    for(int i = 0 ; i < p_history_search.size() ; i++){
        if(SearchMap.at<uint8_t >(p_history_search[i]) != SearchFreeCell){
            LOGPLANNING(INFO) << "remove != Free:(" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
            p_history_search.erase(p_history_search.begin() + i);
            i--;
        }
    }
    BoxInfo box = boxinfo->GetCurDelimit();
    LOGPLANNING(INFO) << "CurDelimit id:" << box.id;
    Mat map_search = Mat::zeros(SearchMap.rows , SearchMap.cols , CV_8UC1);
    bool flg = search->Search_obs_point_marking(SearchMap,s_map.RegionSegmentationMap,box.id,s_map.originPointOccupyMin , s_map.originPointOccupyMax,start.point,target_obs.point,map_search);
//    Mat map_history = map_search.clone();
//    map_history = map_history * 255;
//    imwrite(s_log.pathFive_ + "map_history.png",map_history);

    if(!flg){
        Point tmp_start;
        for (int i = (int) p_history_search.size() - 1; i >= 0; i--) {
            tmp_start = p_history_search[i];
            flg = search->Search_obs_point_marking(SearchMap,s_map.RegionSegmentationMap,box.id,s_map.originPointOccupyMin , s_map.originPointOccupyMax,tmp_start,target_obs.point,map_search);
//            map_history = map_search.clone();
//            map_history = map_history * 255;
//            imwrite(s_log.pathFive_ + to_string(i) + "_map_history.png",map_history);

            if(flg){
                break;
            }
        }
    }else{
        p_history_search.emplace_back(start.point);
    }
#endif
#if 1
    bool flg = false;
    BoxInfo box = boxinfo->GetCurDelimit();
    LOGPLANNING(INFO) << "CurDelimit id:" << box.id;
    Mat map_search = Mat::zeros(SearchMap.rows , SearchMap.cols , CV_8UC1);
    if(vc2.empty()){
        flg = search->Search_obs_point_marking(SearchMap,s_map.RegionSegmentationMap,box.id,s_map.originPointOccupyMin , s_map.originPointOccupyMax,start.point,target_obs.point,map_search);
        if(flg == 0){
            if(search->Search_p2p(SearchMap , start.point , sensorinfo->StartPose.point)){
                list<Point> list_out;
                search->count_set_type(SearchMap , start.point , s_map.originPointOccupyMin , s_map.originPointOccupyMax , list_out);
                if(!list_out.empty()){
                    while(!list_out.empty()){
                        if(s_map.ForbidenMap.at<uint8_t >(list_out.front()) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(list_out.front()) = ForbiddenMarkCell;
                        }
                        Point p;
                        for (int n = 0; n < 6; n++)
                            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                p = *((StructuringElement->begin() + n)->begin() + k) + list_out.front();
                                if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                                    s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                                }
                            }
                        list_out.pop_front();
                    }
                }
                list_out.clear();
                SearchMap.release();
                LOGPLANNING(INFO) << "Start surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Surrounded;
                return SigSearchObsPoint::FindObs_InObs;//机器人当前被障碍物环绕，需要脱困。
            }
        }
    }else{
        for(int i = 0 ; i < vc2.size() ; i++){
            LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
            flg = search->Search_obs_point_marking(SearchMap,s_map.RegionSegmentationMap,box.id,s_map.originPointOccupyMin , s_map.originPointOccupyMax,vc2[i],target_obs.point,map_search);
            if(flg > 0){
                break;
            }
        }
        if(flg == 0){
            bool sig = false;
            for(int i = 0 ; i < vc2.size() ; i++){
                if(search->Search_p2p(SearchMap , vc2[i] , sensorinfo->StartPose.point)){
                    sig = true;
                    break;
                }
            }
            vc2.clear();
            if(!sig){
                Point p;
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
						if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
							s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
						}
                    }
                SearchMap.release();
                LOGPLANNING(INFO) << "Start nearby surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                return SigSearchObsPoint::FindObs_InObs;//机器人当前被陷入障碍物，临近点被障碍物环绕，需要脱困。
            }
        }
    }
#endif

    if(flg){
        Point p_;
        for(int i = 1 ; i < 10 ; i++)
            for(int j = 0 ; j < s_map.GetExpandSingleElement(i)->size() ; j++){
                p_ = *s_map.GetExpandSingleElement(i,j) + target_obs.point;
                if(map_search.at<uint8_t >(p_)){
                    if(SearchMap.at<uint8_t >(p_) == SearchFreeCell && s_map.RegionSegmentationMap.at<uint8_t >(p_) == box.id){
                        target_nav.point = p_;
                    }
                }
            }
    }

    map_search.release();

    if(target_nav.point == Point(0 , 0) || target_obs.point == Point(0 , 0)){
        SearchMap.release();
        LOGPLANNING(INFO) << "SearchObsPointMarking:not get target !!!";
        LOGPLANNING(INFO) << "target_nav:(" << target_nav.point.x << " , " << target_nav.point.y << ") , target_obs:(" << target_obs.point.x << " , " << target_obs.point.y << ")";
        return SigSearchObsPoint::FindObs_Null;
    }else{
        Point p;
        if(SearchMap.at<uint8_t>(target_obs.point) == SearchObsCell){
            s_map.FollowMap.at<uint8_t >(target_obs.point) = FollowIgnoreCell;
        }
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + target_obs.point;
                if(SearchMap.at<uint8_t>(p) == SearchObsCell){
                    s_map.FollowMap.at<uint8_t >(p) = FollowIgnoreCell;
                }
            }
        SearchMap.release();
        target_nav.pose = s_map.MapToWorld(target_nav.point);
        target_obs.pose = s_map.MapToWorld(target_obs.point);
        LOGPLANNING(INFO) << "SearchObsPointMarking target_nav:(" << target_nav.point.x << " , " << target_nav.point.y << ")-(" << target_nav.pose.x << " , " << target_nav.pose.y << ")";
        LOGPLANNING(INFO) << "SearchObsPointMarking target_obs:(" << target_obs.point.x << " , " << target_obs.point.y << ")-(" << target_obs.pose.x << " , " << target_obs.pose.y << ")";
        return SigSearchObsPoint::FindObs_success;
    }
}
int PlanningSdk::SearchObsPointAutoMarking(const PoseStamped & start , PoseStamped & target_nav , PoseStamped & target_obs){
    s_log.storageTypeInit(LOG_SEARCH);

    Search * search = s_planner.getsearch();
    CleanBox * boxinfo = s_planner.getcleanbox();
    if(search == nullptr || boxinfo == nullptr){
        LOGPLANNING(INFO) << "* search || boxinfo is nullptr !!!";
        return SigSearchObsPoint::FindObs_Null;
    }
    target_nav.point = Point(0 , 0);
    target_obs.point = Point(0 , 0);
    LOGPLANNING(INFO) << "SearchObsPointMarking start:(" << start.point.x << " , " << start.point.y << ")-(" << start.pose.x << " , " << start.pose.y << ")";

    if(s_map.ForbidenMap.at<uint8_t >(start.point) != ForbiddenUnknowCell){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Forbiden;
        imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
        LOGPLANNING(INFO) << "start in forbiden";
        return SigSearchObsPoint::FindObs_InObs;//机器人当前在禁区内。
    }

    imwrite(s_log.pathFive_ + "Navmap.png",s_map.NavMap);
    imwrite(s_log.pathFive_ + "Appmap.png",s_map.APPMap);
//    imwrite(s_log.pathFive_ + "Covermap.png",s_map.CoverMap);
    imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
    imwrite(s_log.pathFive_ + "DeepLearningmap.png",s_map.DeepLearningMap);
//    imwrite(s_log.pathFive_ + "Pathmap.png",s_map.PathMap);
    imwrite(s_log.pathFive_ + "FollowMap.png",s_map.FollowMap);
    imwrite(s_log.pathFive_ + "RegionSegmentationMap.png",s_map.RegionSegmentationMap);


    Mat SearchMap = cv::Mat::ones(s_map.height,s_map.width,CV_8UC1)*255;
    uint8_t nav_v;
    uint8_t forbiden_v;
    uint8_t follow_v;
    LOGPLANNING(INFO) << "SearchObsPointMarking s_map.originPointOccupyMin:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")"<< " , s_map.originPointOccupyMax:(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";

    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            nav_v = s_map.NavMap.at<uint8_t >(i,j);
            forbiden_v = s_map.ForbidenMap.at<uint8_t >(i,j);
            follow_v = s_map.FollowMap.at<uint8_t >(i,j);

            if(follow_v != FollowUnknowCell || forbiden_v == ForbiddenCell){
                SearchMap.at<uint8_t >(i,j) = SearchUnknowCell;
            }else{
                if(nav_v == NavObsExpandCell){
                    SearchMap.at<uint8_t >(i,j) = SearchCovCell;
                }else if(nav_v == NavFreeCell){
                    SearchMap.at<uint8_t >(i,j) = SearchFreeCell;
                }
                if(nav_v == NavObsCell || nav_v == NavSensorObsCell){
                    SearchMap.at<uint8_t >(i,j) = SearchObsCell;
                }
            }
        }

    rectangle(SearchMap , s_map.originPointOccupyMin , s_map.originPointOccupyMax , Scalar(SearchUnknowCell) , 1 , LINE_4);
    imwrite(s_log.pathFive_ + "SearchMap.png",SearchMap);


    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();
    vector<Point> vc;
    vector<Point> vc1;
    vector<Point> vc2;
    if(SearchMap.at<uint8_t >(start.point) != SearchFreeCell){
        Point p;
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                if(SearchMap.at<uint8_t>(p) == SearchFreeCell){
                    vc.emplace_back(p);
                }
            }

        if(vc.empty()){
            if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
            }
            for (int n = 0; n < 6; n++)
                for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                    p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                    if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                        s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                    }
                }
            sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
            LOGPLANNING(INFO) << "start in obs";
            return SigSearchObsPoint::FindObs_InObs;//机器人当前在障碍物内。
        }else{
            vector<Point>::iterator iter;
            vc1.emplace_back(vc.front());
            iter = find(vc.begin() , vc.end() , vc1.back());
            vc.erase(iter);
            int num_min = 81;
            list<Point> list_out;
            bool sig = false;
            while(true){
                LOGPLANNING(INFO) << "vc:"<< vc.size() << " vc1:" << vc1.size() << " vc2:" << vc2.size();
                for(int i = 0 ; i < vc.size() ; i++){
                    LOGPLANNING(INFO) << "vc:"<< i << "  :(" << vc[i].x << " , " << vc[i].y << ")";
                }
                for(int i = 0 ; i < vc1.size() ; i++){
                    LOGPLANNING(INFO) << "vc1:"<< i << "  :(" << vc1[i].x << " , " << vc1[i].y << ")";
                }
                for(int i = 0 ; i < vc2.size() ; i++){
                    LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
                }
                sig = false;
                for(int i = 0 ; i < vc1.size() ; i++)
                    for(int j = 0 ; j < vc.size() ; j++){
                        if(abs(vc1[i].x - vc[j].x) + abs(vc1[i].y - vc[j].y) == 1){
                            sig = true;
                            vc1.emplace_back(vc[j]);
                            i = (int)vc1.size();
                            break;
                        }
                    }
                if(!sig){
                    search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                    if(list_out.empty() || list_out.size() < 81){
                        vc1.clear();
                    }else{
                        vc2.emplace_back(vc1.front());
                        vc1.clear();
                    }
                    list_out.clear();

                    if(!vc.empty()){
                        vc1.emplace_back(vc.front());
                        iter = find(vc.begin() , vc.end() , vc1.back());
                        vc.erase(iter);

                        if(vc.empty()){
                            search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                            if(list_out.empty() || list_out.size() < 81){
                                vc1.clear();
                            }else{
                                vc2.emplace_back(vc1.front());
                                vc1.clear();
                            }
                            list_out.clear();
                        }
                    }
                }else{
                    iter = find(vc.begin() , vc.end() , vc1.back());
                    vc.erase(iter);

                    if(vc.empty()){
                        search->count_set_type(SearchMap , vc1.front() , s_map.originPointOccupyMin , s_map.originPointOccupyMax , num_min , list_out);
                        if(list_out.empty() || list_out.size() < 81){
                            vc1.clear();
                        }else{
                            vc2.emplace_back(vc1.front());
                            vc1.clear();
                        }
                        list_out.clear();
                    }
                }
                if(!vc.empty()){
                }else{
                    break;
                }
            }

            vc.clear();
            vc1.clear();

            if(vc2.empty()){
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                        if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                        }
                    }
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                LOGPLANNING(INFO) << "start in obs";
                return SigSearchObsPoint::FindObs_InObs;//机器人当前在障碍物内。
            }
        }
    }

#if 0
    for(int i = 0 ; i < p_history_search.size() ; i++){
        if(SearchMap.at<uint8_t >(p_history_search[i]) != SearchFreeCell){
            LOGPLANNING(INFO) << "remove != Free:(" << p_history_search[i].x << " , " << p_history_search[i].y << ")" ;
            p_history_search.erase(p_history_search.begin() + i);
            i--;
        }
    }
    BoxInfo box = boxinfo->GetCurDelimit();
    LOGPLANNING(INFO) << "CurDelimit id:" << box.id;
    Mat map_search = Mat::zeros(SearchMap.rows , SearchMap.cols , CV_8UC1);
    bool flg = search->Search_obs_point_marking(SearchMap,s_map.RegionSegmentationMap,box.id,s_map.originPointOccupyMin , s_map.originPointOccupyMax,start.point,target_obs.point,map_search);
//    Mat map_history = map_search.clone();
//    map_history = map_history * 255;
//    imwrite(s_log.pathFive_ + "map_history.png",map_history);

    if(!flg){
        Point tmp_start;
        for (int i = (int) p_history_search.size() - 1; i >= 0; i--) {
            tmp_start = p_history_search[i];
            flg = search->Search_obs_point_marking(SearchMap,s_map.RegionSegmentationMap,box.id,s_map.originPointOccupyMin , s_map.originPointOccupyMax,tmp_start,target_obs.point,map_search);
//            map_history = map_search.clone();
//            map_history = map_history * 255;
//            imwrite(s_log.pathFive_ + to_string(i) + "_map_history.png",map_history);

            if(flg){
                break;
            }
        }
    }else{
        p_history_search.emplace_back(start.point);
    }
#endif
#if 1
    bool flg = false;
    BoxInfo box = boxinfo->GetCurDelimit();
    LOGPLANNING(INFO) << "CurDelimit id:" << box.id;
    Mat map_search = Mat::zeros(SearchMap.rows , SearchMap.cols , CV_8UC1);
    if(vc2.empty()){
        flg = search->Search_obs_point_marking(SearchMap,s_map.RegionSegmentationMap,box.id,s_map.originPointOccupyMin , s_map.originPointOccupyMax,start.point,target_obs.point,map_search);
        if(flg == 0){
            if(search->Search_p2p(SearchMap , start.point , sensorinfo->StartPose.point)){
                list<Point> list_out;
                search->count_set_type(SearchMap , start.point , s_map.originPointOccupyMin , s_map.originPointOccupyMax , list_out);
                if(!list_out.empty()){
                    while(!list_out.empty()){
                        if(s_map.ForbidenMap.at<uint8_t >(list_out.front()) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(list_out.front()) = ForbiddenMarkCell;
                        }
                        Point p;
                        for (int n = 0; n < 6; n++)
                            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                p = *((StructuringElement->begin() + n)->begin() + k) + list_out.front();
                                if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                                    s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                                }
                            }
                        list_out.pop_front();
                    }
                }
                list_out.clear();
                SearchMap.release();
                LOGPLANNING(INFO) << "Start surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Surrounded;
                return SigSearchObsPoint::FindObs_InObs;//机器人当前被障碍物环绕，需要脱困。
            }
        }
    }else{
        for(int i = 0 ; i < vc2.size() ; i++){
            LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
            flg = search->Search_obs_point_marking(SearchMap,s_map.RegionSegmentationMap,box.id,s_map.originPointOccupyMin , s_map.originPointOccupyMax,vc2[i],target_obs.point,map_search);
            if(flg > 0){
                break;
            }
        }
        if(flg == 0){
            bool sig = false;
            for(int i = 0 ; i < vc2.size() ; i++){
                if(search->Search_p2p(SearchMap , vc2[i] , sensorinfo->StartPose.point)){
                    sig = true;
                    break;
                }
            }
            vc2.clear();
            if(!sig){
                Point p;
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                        if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                        }
                    }
                SearchMap.release();
                LOGPLANNING(INFO) << "Start nearby surrounded with obs";
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                return SigSearchObsPoint::FindObs_InObs;//机器人当前被陷入障碍物，临近点被障碍物环绕，需要脱困。
            }
        }
    }
#endif

    if(flg){
        Point p_;
        for(int i = 1 ; i < 10 ; i++)
            for(int j = 0 ; j < s_map.GetExpandSingleElement(i)->size() ; j++){
                p_ = *s_map.GetExpandSingleElement(i,j) + target_obs.point;
                if(map_search.at<uint8_t >(p_)){
                    if(SearchMap.at<uint8_t >(p_) == SearchFreeCell && s_map.RegionSegmentationMap.at<uint8_t >(p_) == box.id){
                        target_nav.point = p_;
                    }
                }
            }
    }

    map_search.release();

    if(target_nav.point == Point(0 , 0) || target_obs.point == Point(0 , 0)){
        SearchMap.release();
        LOGPLANNING(INFO) << "SearchObsPointMarking:not get target !!!";
        LOGPLANNING(INFO) << "target_nav:(" << target_nav.point.x << " , " << target_nav.point.y << ") , target_obs:(" << target_obs.point.x << " , " << target_obs.point.y << ")";
        return SigSearchObsPoint::FindObs_Null;
    }else{
        Point p;
        if(SearchMap.at<uint8_t>(target_obs.point) == SearchObsCell){
            s_map.FollowMap.at<uint8_t >(target_obs.point) = FollowIgnoreCell;
        }
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + target_obs.point;
                if(SearchMap.at<uint8_t>(p) == SearchObsCell){
                    s_map.FollowMap.at<uint8_t >(p) = FollowIgnoreCell;
                }
            }
        SearchMap.release();
        target_nav.pose = s_map.MapToWorld(target_nav.point);
        target_obs.pose = s_map.MapToWorld(target_obs.point);
        LOGPLANNING(INFO) << "SearchObsPointMarking target_nav:(" << target_nav.point.x << " , " << target_nav.point.y << ")-(" << target_nav.pose.x << " , " << target_nav.pose.y << ")";
        LOGPLANNING(INFO) << "SearchObsPointMarking target_obs:(" << target_obs.point.x << " , " << target_obs.point.y << ")-(" << target_obs.pose.x << " , " << target_obs.pose.y << ")";
        return SigSearchObsPoint::FindObs_success;
    }
}
int PlanningSdk::Nav(const PoseStamped & start , const PoseStamped & target , vector<PoseStamped> & path){
    s_log.storageTypeInit(LOG_NAV);

    global_planner::NavPath * nav = s_planner.getnav();
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    Search * search = s_planner.getsearch();

    if(nav == nullptr || map_manage == nullptr || search == nullptr){
        LOGPLANNING(INFO) << "* nav || map_manage || search nullptr !!!" ;
        return SigNav::Nav_fail;
    }
    path.clear();
    std::vector<PoseStamped>().swap(path);

    LOGPLANNING(INFO) << "nav start:(" << start.point.x << " , " << start.point.y << ")-(" << start.pose.x << " , " << start.pose.y << ")"<< " , target:(" << target.point.x << " , " << target.point.y << ")-(" << target.pose.x << " , " << target.pose.y << ")";
    LOGPLANNING(INFO) << "nav s_map.originPointOccupyMin:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")-(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")"<< " , s_map.originPointOccupyMax:(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")-(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";

    if(start.point == target.point){
        path.emplace_back(start);
        path.emplace_back(target);
        LOGPLANNING(INFO) << "nav start is target";
        return SigNav::Nav_success;
    }
    if(s_map.ForbidenMap.at<uint8_t >(start.point) != ForbiddenUnknowCell){
        sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Forbiden;
        imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
        LOGPLANNING(INFO) << "start in forbiden";
        return SigNav::Nav_InObs;//机器人当前在禁区内。
    }

    Point start_ = start.point;
    Point target_ = target.point;
    imwrite(s_log.pathFive_ + "Navmap.png",s_map.NavMap);
    imwrite(s_log.pathFive_ + "Forbidenmap.png",s_map.ForbidenMap);
    imwrite(s_log.pathFive_ + "DeepLearningmap.png",s_map.DeepLearningMap);
    imwrite(s_log.pathFive_ + "Pathmap.png",s_map.PathMap);


    Mat src = s_map.NavMap.clone();

    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            if(src.at<uint8_t >(i,j) == NavSensorObsCell || src.at<uint8_t >(i,j) == NavObsExpandCell || src.at<uint8_t >(i,j) == NavObsCell){
                src.at<uint8_t >(i,j) = NavObsCell;
            }
            if(s_map.ForbidenMap.at<uint8_t >(i,j) != ForbiddenUnknowCell || s_map.DeepLearningMap.at<uint8_t >(i,j) == ObsCell ){
                src.at<uint8_t >(i,j) = NavObsCell;
            }
        }

    imwrite(s_log.pathFive_ + "nav_src.png",src);
    if(src.at<uint8_t>(target_) != NavFreeCell){
        LOGPLANNING(INFO) << "target point is in obs!!!" ;
        return SigNav::Nav_fail;
    }

    bool sig = false;
    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();
    vector<Point> vc;
    vector<Point> vc1;
    vector<Point> vc2;
    if(s_map.DeepLearningMap.at<uint8_t >(start.point) != UnknowCell ||
       s_map.NavMap.at<uint8_t >(start.point) != NavFreeCell){
        Point p;
        for (int n = 0; n < 5; n++)
            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                if(s_map.DeepLearningMap.at<uint8_t>(p) == UnknowCell &&
                   s_map.NavMap.at<uint8_t >(p) == NavFreeCell &&
                   s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                    vc.emplace_back(p);
                }
            }

        if(vc.empty()){
            if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
            }
            for (int n = 0; n < 6; n++)
                for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                    p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
					if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
						s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
					}
                }
            sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
            LOGPLANNING(INFO) << "start in obs";
            return SigNav::Nav_InObs;//机器人当前在障碍物内。
        }else{
            vector<Point>::iterator iter;
            vc1.emplace_back(vc.front());
            iter = find(vc.begin() , vc.end() , vc1.back());
            vc.erase(iter);
            bool sig = false;
            while(true){
                LOGPLANNING(INFO) << "vc:"<< vc.size() << " vc1:" << vc1.size() << " vc2:" << vc2.size();
                for(int i = 0 ; i < vc.size() ; i++){
                    LOGPLANNING(INFO) << "vc:"<< i << "  :(" << vc[i].x << " , " << vc[i].y << ")";
                }
                for(int i = 0 ; i < vc1.size() ; i++){
                    LOGPLANNING(INFO) << "vc1:"<< i << "  :(" << vc1[i].x << " , " << vc1[i].y << ")";
                }
                for(int i = 0 ; i < vc2.size() ; i++){
                    LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
                }
                sig = false;
                for(int i = 0 ; i < vc1.size() ; i++)
                    for(int j = 0 ; j < vc.size() ; j++){
                        if(abs(vc1[i].x - vc[j].x) + abs(vc1[i].y - vc[j].y) == 1){
                            sig = true;
                            vc1.emplace_back(vc[j]);
                            i = (int)vc1.size();
                            break;
                        }
                    }
                if(!sig){
                    vc2.emplace_back(vc1.front());
                    vc1.clear();

                    if(!vc.empty()){
                        vc1.emplace_back(vc.front());
                        iter = find(vc.begin() , vc.end() , vc1.back());
                        vc.erase(iter);

                        if(vc.empty()){
                            vc2.emplace_back(vc1.front());
                            vc1.clear();
                        }
                    }
                }else{
                    iter = find(vc.begin() , vc.end() , vc1.back());
                    vc.erase(iter);

                    if(vc.empty()){
                        vc2.emplace_back(vc1.front());
                        vc1.clear();
                    }
                }
                if(!vc.empty()){
                }else{
                    break;
                }
            }

            vc.clear();
            vc1.clear();

            if(vc2.empty()){
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
                        if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                        }
                    }
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                LOGPLANNING(INFO) << "start in obs";
                return SigNav::Nav_InObs;//机器人当前在障碍物内。
            }
        }
    }

    if(!vc2.empty()){
        sig = false;
        for(int i = 0 ; i < vc2.size() ; i++){
            LOGPLANNING(INFO) << "vc2:"<< i << "  :(" << vc2[i].x << " , " << vc2[i].y << ")";
            sig = nav->connect_p2p(src , vc2[i] , target_);
            if(sig){
                start_ = vc2[i];
                LOGPLANNING(INFO) << "start in obs , use nearby";
                path.emplace_back(start);
                break;
            }
        }
        if(!sig){
            for(int i = 0 ; i < vc2.size() ; i++){
                sig = nav->connect_p2p(src , vc2[i] , sensorinfo->StartPose.point);
                if(sig){
                    src.release();
                    LOGPLANNING(INFO) << "vc2 start and target not connect !!!" ;
                    return SigNav::Nav_fail;
                }
            }
            if(!sig){
                Point p;
                if(s_map.ForbidenMap.at<uint8_t >(start.point) == ForbiddenUnknowCell){
                    s_map.ForbidenMap.at<uint8_t >(start.point) = ForbiddenMarkCell;
                }
                for (int n = 0; n < 6; n++)
                    for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                        p = *((StructuringElement->begin() + n)->begin() + k) + start.point;
						if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
						}
                    }
                src.release();
                LOGPLANNING(INFO) << "start nearby surrounded with obs !!!" ;
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Obs;
                return SigNav::Nav_InObs;
            }
        }
    }else{
        sig = nav->connect_p2p(src , start_ , target_);
        if(!sig){
            sig = nav->connect_p2p(src , start_ , sensorinfo->StartPose.point);
            if(sig){
                src.release();
                LOGPLANNING(INFO) << "start and target not connect !!!" ;
                return SigNav::Nav_fail;
            }else{
                list<Point> list_out;
                search->count_set_type(src , start.point , s_map.originPointOccupyMin , s_map.originPointOccupyMax , list_out);
                if(!list_out.empty()){
                    while(!list_out.empty()){
                        if(s_map.ForbidenMap.at<uint8_t >(list_out.front()) == ForbiddenUnknowCell){
                            s_map.ForbidenMap.at<uint8_t >(list_out.front()) = ForbiddenMarkCell;
                        }
                        Point p;
                        for (int n = 0; n < 6; n++)
                            for (int k = 0; k < (StructuringElement->begin() + n)->size(); k++) {
                                p = *((StructuringElement->begin() + n)->begin() + k) + list_out.front();
                                if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
                                    s_map.ForbidenMap.at<uint8_t >(p) = ForbiddenMarkCell;
                                }
                            }
                        list_out.pop_front();
                    }
                }
                list_out.clear();
                src.release();
                LOGPLANNING(INFO) << "start surrounded with obs !!!" ;
                sensorinfo->recoveryStyle = RecoveryStyle::Recovery_Surrounded;
                return SigNav::Nav_InObs;
            }
        }
    }

    if(start_ == target_){
        path.emplace_back(start);
        path.emplace_back(PoseStamped(s_map.MapToWorld(start_),start_));

        vc2.clear();
        src.release();
        LOGPLANNING(INFO) << "nav find path out nav";
        return SigNav::Nav_success;
    }

    src = src(Rect(s_map.originPointOccupyMin.x,s_map.originPointOccupyMin.y,(s_map.originPointOccupyMax.x - s_map.originPointOccupyMin.x),(s_map.originPointOccupyMax.y - s_map.originPointOccupyMin.y))).clone();
    start_= start_ - s_map.originPointOccupyMin;
    target_ = target_ - s_map.originPointOccupyMin;
    std::vector<Point> path_;
    nav->aStartest(src,start_,target_,path_);

    for(int j = 0 ; j < path_.size() ; j++){
        path_[j] = path_[j] + s_map.originPointOccupyMin;
        path.emplace_back(PoseStamped(s_map.MapToWorld(path_[j]),path_[j]));
    }

    path_.clear();
    std::vector<Point>().swap(path_);
#if 0
    Mat map_tmp = src.clone();
    Point start_tmp = start_;
    list<Point> plist;
    if(map_tmp.at<uint8_t >(start_) != NavFreeCell){
        if(search->Search_point_set(map_tmp,start_,NavFreeCell,start_tmp)){
        }else{
            src.release();
            map_tmp.release();
            LOGPLANNING(INFO) << "start point is in obs , can not replace!!!" ;
//            return Nav_StartInObs;
            return false;//起点在障碍物里面。
        }
    }

    int num_nim = 81;
    while(true){
        num_nim = 81;
        int flg = search->count_set_type(map_tmp,start_tmp,Point(start_.x - 50,start_.y - 50),Point(start_.x + 50,start_.y + 50),num_nim,plist);
        if(flg == 0){
            if(num_nim < 81){
                LOGPLANNING(INFO) << "start_tmp　is surrounded with obs :(" << start_tmp.x << " , " << start_tmp.y << ")";

                while (!plist.empty()){
                    map_tmp.at<uint8_t >(plist.front()) = NavUnknowCell;
                    plist.pop_front();
                }
                plist.clear();
                list<Point>().swap(plist);
                if(search->Search_point_set(map_tmp,start_,NavFreeCell,start_tmp)){
                }else{
                    src.release();
                    map_tmp.release();
                    LOGPLANNING(INFO) << "start point is surrounded with obs , can not replace!!!" ;
//                    return Nav_StartInObs;
                    return false;//起点被障碍物包围。
                }
            }else{
                if(start_ == start_tmp){

                }else{
                    if(pow(start_tmp.x - start_.x , 2) + pow(start_tmp.y - start_.y , 2) <= pow(ROBOTRADIUS/s_map.resolving + 1 , 2)){
                        LOGPLANNING(INFO) << "start_:(" << start_.x << " , " << start_.y << ") start_tmp:(" << start_tmp.x << " , " << start_tmp.y << ")" ;
                        start_ = start_tmp;
//                        path.emplace_back(PoseStamped(Point2f(100.0,100.0),s_map.MapToWorld(Point2f(100.0,100.0))));
                        LOGPLANNING(INFO) << "start is in obs , can replace:(" << start_tmp.x << " , " << start_tmp.y << ")" ;
                    }
                }
                plist.clear();
                list<Point>().swap(plist);
                break;
            }
        }else{
            plist.clear();
            list<Point>().swap(plist);
            break;
        }
    }
    imwrite(s_log.pathFive_ + "nav_dilate.png",map_tmp);
#endif
    vc2.clear();
    src.release();
    LOGPLANNING(INFO) << "nav find path" ;
    return SigNav::Nav_success;
}

bool PlanningSdk::GetCleanPath(PoseStamped & start , vector<PoseStamped> & path , int type){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * boxinfo = s_planner.getcleanbox();
    CleanPath * clean_path = s_planner.getcleanpath();
    if(map_manage == nullptr || boxinfo == nullptr || clean_path == nullptr){
        LOGPLANNING(INFO) << "* map_manage || boxinfo || clean_path is nullptr !!!";

        datadown->cmd.vl = 0.0f;
        datadown->cmd.va = 0.0f;
        return false;
    }
    LOGPLANNING(INFO) << "robotpose(" << sensorinfo->Posture.point.x <<","<< sensorinfo->Posture.point.y << ") , dir_long:" << dir_long << ", dir_shory:" << dir_shory;

    bool flg = false;
    PoseStamped p_tmp  = start;
    PoseStamped ps,pe;

    if(dir_long == 1){
        flg = clean_path->clean_down(boxinfo->curbox,p_tmp,ps,pe);
        dir_long = 0;
    }else if(dir_long == 0){
        flg = clean_path->clean_up(boxinfo->curbox,p_tmp,ps,pe);
        dir_long = 1;
    }
    if(flg){
        path.clear();
        path.emplace_back(ps);
        path.emplace_back(pe);

        p_tmp = pe;
        if(dir_shory == 1){
            if(dir_long == 1){
                if(clean_path->clean_leftObtuse(boxinfo->curbox , p_tmp , ps , pe , 1)){
                    p_tmp = ps;
                    if(clean_path->clean_down(boxinfo->curbox,p_tmp,ps,pe)){
                        path.emplace_back(ps);
                        path.emplace_back(pe);
                    }
                }else if(clean_path->clean_leftAcute(boxinfo->curbox , p_tmp , ps , pe , 1)){
                    if(ps.point.x < path.front().point.x){
                        p_tmp = ps;
                        if(clean_path->clean_down(boxinfo->curbox,p_tmp,ps,pe)){
                            path.emplace_back(ps);
                            path.emplace_back(pe);
                        }
                    }
                }
            }else if(dir_long == 0){
                if(clean_path->clean_rightObtuse(boxinfo->curbox , p_tmp , ps , pe , 1)){
                    p_tmp = ps;
                    if(clean_path->clean_up(boxinfo->curbox,p_tmp,ps,pe)){
                        path.emplace_back(ps);
                        path.emplace_back(pe);
                    }
                }else if(clean_path->clean_rightAcute(boxinfo->curbox , p_tmp , ps , pe , 1)){
                    if(ps.point.x > path.front().point.x){
                        p_tmp = ps;
                        if(clean_path->clean_up(boxinfo->curbox,p_tmp,ps,pe)){
                            path.emplace_back(ps);
                            path.emplace_back(pe);
                        }
                    }
                }
            }
        }else if(dir_shory == 0){
            if(dir_long == 1){
                if(clean_path->clean_rightObtuse(boxinfo->curbox , p_tmp , ps , pe , 0)){
                    p_tmp = ps;
                    if(clean_path->clean_down(boxinfo->curbox,p_tmp,ps,pe)){
                        path.emplace_back(ps);
                        path.emplace_back(pe);
                    }
                }else if(clean_path->clean_rightAcute(boxinfo->curbox , p_tmp , ps , pe , 0)){
                    if(ps.point.x < path.front().point.x){
                        p_tmp = ps;
                        if(clean_path->clean_down(boxinfo->curbox,p_tmp,ps,pe)){
                            path.emplace_back(ps);
                            path.emplace_back(pe);
                        }
                    }
                }
            }else if(dir_long == 0){
                if(clean_path->clean_leftObtuse(boxinfo->curbox , p_tmp , ps , pe , 0)){
                    p_tmp = ps;
                    if(clean_path->clean_up(boxinfo->curbox,p_tmp,ps,pe)){
                        path.emplace_back(ps);
                        path.emplace_back(pe);
                    }
                }else if(clean_path->clean_leftAcute(boxinfo->curbox , p_tmp , ps , pe , 0)){
                    if(ps.point.x > path.front().point.x){
                        p_tmp = ps;
                        if(clean_path->clean_up(boxinfo->curbox,p_tmp,ps,pe)){
                            path.emplace_back(ps);
                            path.emplace_back(pe);
                        }
                    }
                }
            }
        }
    }
    if(flg){
        if(path.size() == 4){
            if(path[2].pose.x > path[3].pose.x){
                path[3].pose.x = path[2].pose.x - 0.1f;
                path[3].point = s_map.WorldToMap(path[3].pose);
            }else {
                path[3].pose.x = path[2].pose.x + 0.1f;
                path[3].point = s_map.WorldToMap(path[3].pose);
            }
        }else if(path.size() == 2){
            if((abs(path[0].point.x - path[1].point.x) + abs(path[0].point.y - path[1].point.y)) < 3){
                LOGPLANNING(INFO) << "clean path is too short";
                flg = false;
            }
        }
    }
    if(!flg){
        map_manage->fill_cover_uncover(sensorinfo->Posture.point,boxinfo->curbox,3);
        datadown->cmd.vl = 0.0f;
        datadown->cmd.va = 0.0f;
    }
    if(flg){
        LOGPLANNING(INFO) << "dir_long:" <<dir_long << ",dir_shory:" << dir_shory;
        vector<PoseStamped>::iterator iter;
        int i = 0;
        for(iter = path.begin() ; iter != path.end() ; iter++){
            LOGPLANNING(INFO) << "clean path:" << i << "-(" << iter->pose.x << "," << iter->pose.y << ")-(" << iter->point.x << "," << iter->point.y <<")";
            i++;
        }
        start = path.back();
    }
    return flg;
}
bool PlanningSdk::GetCleanPathDelimit(PoseStamped & start , vector<PoseStamped> & path , int type){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * boxinfo = s_planner.getcleanbox();
    CleanPath * clean_path = s_planner.getcleanpath();
    if(map_manage == nullptr || boxinfo == nullptr || clean_path == nullptr){
        LOGPLANNING(INFO) << "* map_manage || boxinfo || clean_path is nullptr !!!";
        return false;
    }

    LOGPLANNING(INFO) << "robotpose(" << sensorinfo->Posture.point.x <<","<< sensorinfo->Posture.point.y << ") , dir_long:" << dir_long << ", dir_shory:" << dir_shory;

    bool flg = false;
    PoseStamped p_tmp  = start;
    PoseStamped ps,pe;

    if(dir_long == 1){
        flg = clean_path->clean_down_mark(boxinfo->curbox,p_tmp,ps,pe);
        dir_long = 0;
    }else if(dir_long == 0){
        flg = clean_path->clean_up_mark(boxinfo->curbox,p_tmp,ps,pe);
        dir_long = 1;
    }
    if(flg){
        path.clear();
        path.emplace_back(ps);
        path.emplace_back(pe);

        p_tmp = pe;
        if(dir_shory == 1){
            if(dir_long == 1){
                if(clean_path->clean_leftObtuse_mark(boxinfo->curbox , p_tmp , ps , pe , 1)){
                    p_tmp = ps;
                    if(clean_path->clean_down_mark(boxinfo->curbox,p_tmp,ps,pe)){
                        path.emplace_back(ps);
                        path.emplace_back(pe);
                    }
                }else if(clean_path->clean_leftAcute_mark(boxinfo->curbox , p_tmp , ps , pe , 1)){
                    if(ps.point.x < path.front().point.x){
                        p_tmp = ps;
                        if(clean_path->clean_down_mark(boxinfo->curbox,p_tmp,ps,pe)){
                            path.emplace_back(ps);
                            path.emplace_back(pe);
                        }
                    }
                }
            }else if(dir_long == 0){
                if(clean_path->clean_rightObtuse_mark(boxinfo->curbox , p_tmp , ps , pe , 1)){
                    p_tmp = ps;
                    if(clean_path->clean_up_mark(boxinfo->curbox,p_tmp,ps,pe)){
                        path.emplace_back(ps);
                        path.emplace_back(pe);
                    }
                }else if(clean_path->clean_rightAcute_mark(boxinfo->curbox , p_tmp , ps , pe , 1)){
                    if(ps.point.x > path.front().point.x){
                        p_tmp = ps;
                        if(clean_path->clean_up_mark(boxinfo->curbox,p_tmp,ps,pe)){
                            path.emplace_back(ps);
                            path.emplace_back(pe);
                        }
                    }
                }
            }
        }else if(dir_shory == 0){
            if(dir_long == 1){
                if(clean_path->clean_rightObtuse_mark(boxinfo->curbox , p_tmp , ps , pe , 0)){
                    p_tmp = ps;
                    if(clean_path->clean_down_mark(boxinfo->curbox,p_tmp,ps,pe)){
                        path.emplace_back(ps);
                        path.emplace_back(pe);
                    }
                }else if(clean_path->clean_rightAcute_mark(boxinfo->curbox , p_tmp , ps , pe , 0)){
                    if(ps.point.x < path.front().point.x){
                        p_tmp = ps;
                        if(clean_path->clean_down_mark(boxinfo->curbox,p_tmp,ps,pe)){
                            path.emplace_back(ps);
                            path.emplace_back(pe);
                        }
                    }
                }
            }else if(dir_long == 0){
                if(clean_path->clean_leftObtuse_mark(boxinfo->curbox , p_tmp , ps , pe , 0)){
                    p_tmp = ps;
                    if(clean_path->clean_up_mark(boxinfo->curbox,p_tmp,ps,pe)){
                        path.emplace_back(ps);
                        path.emplace_back(pe);
                    }
                }else if(clean_path->clean_leftAcute_mark(boxinfo->curbox , p_tmp , ps , pe , 0)){
                    if(ps.point.x > path.front().point.x){
                        p_tmp = ps;
                        if(clean_path->clean_up_mark(boxinfo->curbox,p_tmp,ps,pe)){
                            path.emplace_back(ps);
                            path.emplace_back(pe);
                        }
                    }
                }
            }
        }
    }
    if(flg){
        if(path.size() == 4){
            if(path[2].pose.x > path[3].pose.x){
                path[3].pose.x = path[2].pose.x - 0.1f;
                path[3].point = s_map.WorldToMap(path[3].pose);
            }else {
                path[3].pose.x = path[2].pose.x + 0.1f;
                path[3].point = s_map.WorldToMap(path[3].pose);
            }
        }else if(path.size() == 2){
            if((abs(path[0].point.x - path[1].point.x) + abs(path[0].point.y - path[1].point.y)) < 3){
                LOGPLANNING(INFO) << "clean path is too short";
                flg = false;
            }
        }
    }
    if(!flg){
        map_manage->fill_cover_delimit_uncover(sensorinfo->Posture.point,s_map.RegionSegmentationMap,boxinfo->curbox,3);
        datadown->cmd.vl = 0.0f;
        datadown->cmd.va = 0.0f;
    }
    if(flg){
        LOGPLANNING(INFO) << "dir_long:" <<dir_long << ",dir_shory:" << dir_shory;
        vector<PoseStamped>::iterator iter;
        int i = 0;
        for(iter = path.begin() ; iter != path.end() ; iter++){
            LOGPLANNING(INFO) << "clean path:" << i << "-(" << iter->pose.x << "," << iter->pose.y << ")-(" << iter->point.x << "," << iter->point.y <<")";
            i++;
        }
        start = path.back();
    }
    return flg;
}
bool PlanningSdk::Slampath(BoxInfo BoxInfo_, PoseStamped& slamPose_s , PoseStamped& slamPose_e){
    LOGPLANNING(INFO) << "Posture.point(" << sensorinfo->Posture.point.x << "," << sensorinfo->Posture.point.y<< "),BoxInfo(" << BoxInfo_.point_min.x << "," << BoxInfo_.point_min.y << ")-(" << BoxInfo_.point_max.x<< "," << BoxInfo_.point_max.y << ")";

    if(sensorinfo->Posture.point.x < BoxInfo_.point_min.x || sensorinfo->Posture.point.y < BoxInfo_.point_min.y ||
            sensorinfo->Posture.point.x > BoxInfo_.point_max.x || sensorinfo->Posture.point.y > BoxInfo_.point_max.y){
        LOGPLANNING(INFO) << "Posture.point is out box !!!";
        return false;
    }

    PlanningMapManager * map_manage = s_planner.getmap_manage();
    Search * search = s_planner.getsearch();
    if(map_manage == nullptr || search == nullptr){
        LOGPLANNING(INFO) << "* map_manage || * searchnullptr !!!";
        return false;
    }
    Mat map = Mat(s_map.height,s_map.width,CV_8UC1,Scalar(UnknowCell));
    for(int i = BoxInfo_.point_min.y ; i <=  BoxInfo_.point_max.y ; i++)
        for(int j = BoxInfo_.point_min.x ; j <=  BoxInfo_.point_max.x ; j++){
            if(s_map.NavMap.at<uint8_t >(i,j) == NavFreeCell &&
                s_map.ForbidenMap.at<uint8_t >(i,j) == ForbiddenUnknowCell &&
                    s_map.CoverMap.at<uint8_t >(i,j) == CoverCell){
                map.at<uint8_t>(i - BoxInfo_.point_min.y , j - BoxInfo_.point_min.x) = FreeCell;
            }else{
                map.at<uint8_t>(i - BoxInfo_.point_min.y , j - BoxInfo_.point_min.x) = ObsCell;
            }
        }
    list<Point> p_list;
    Point p_start = Point(sensorinfo->Posture.point.x - BoxInfo_.point_min.x,sensorinfo->Posture.point.y - BoxInfo_.point_min.y);
    search->count_set_type(map , p_start , Point(0,0) , Point(s_map.width - 1 , s_map.height - 1),p_list);
    map.release();

    Mat map_free = Mat(s_map.height,s_map.width,CV_8UC1,Scalar(UnknowCell));
    int side_min = (p_start.y - 20) <= 0 ? 0:(p_start.y - 20);
    int side_max = (p_start.y + 20) >= (s_map.width - 1) ? (s_map.width - 1):(p_start.y + 20);
    int boxside_min = 10;
    int boxside_max = s_map.height - 10;
    list<Point>::iterator iter;
    for(iter = p_list.begin() ; iter != p_list.end() ; iter ++){
        if(iter->y > boxside_min && iter->y < boxside_max){
            if(iter->y < side_min || iter->y > side_max){
                map_free.at<uint8_t >(*iter) = FreeCell;
            }
        }
    }
    int length = 0;
    int length_max = 0;
    int side = 0;
    Point p_min = Point(0,0);
    Point p_max = Point(0,0);
    for(int i = 0 ; i < s_map.height ; i++) {
        for (int j = 0; j < s_map.width; j++) {
            if (map_free.at<uint8_t>(i, j) == FreeCell) {
                length++;
            } else {
                if (length > length_max) {
                    side = i;
                    length_max = length;
                }
                length = 0;
            }
        }
        length = 0;
    }
    vector<Point> vtmp;
    vector<Point> vc;
    for(int i = 0 ; i < s_map.width ; i++){
        if(map_free.at<uint8_t>(side, i) == FreeCell){
            vtmp.emplace_back(Point(i,side));
        }else{
            if(vtmp.size() > vc.size()){
                vc.clear();
                vc.swap(vtmp);
            }else{
                vtmp.clear();
            }
        }
    }
    map_free.release();

    if(vtmp.size() > vc.size()){
        vc.clear();
        vc.swap(vtmp);
    }
    if(vc.size() > 2){
        vc.pop_back();
        vc.pop_back();
    }
    if(vc.size() > 2){
        vc.erase(vc.begin(),vc.begin() + 2);
    }
    if(!vc.empty()){
        slamPose_s.point = vc.front() + BoxInfo_.point_min;
        slamPose_e.point = vc.back() + BoxInfo_.point_min;
    }else{
        slamPose_s = PoseStamped();
        slamPose_e = PoseStamped();
    }

    vtmp.clear();
    vc.clear();
    p_list.clear();
    vector<Point>().swap(vtmp);
    vector<Point>().swap(vc);
    list<Point>().swap(p_list);

    if(slamPose_s.point == slamPose_e.point){
        return false;
    }else{
        slamPose_s.pose = s_map.MapToWorld(slamPose_s.point);
        slamPose_e.pose = s_map.MapToWorld(slamPose_e.point);
        LOGPLANNING(INFO) << "slamPose_s:(" <<slamPose_s.point.x << "," << slamPose_s.point.y << ")-(" << slamPose_s.pose.x << "," << slamPose_s.pose.y << "),slamPose_e:(" <<slamPose_e.point.x << "," << slamPose_e.point.y << ")-(" << slamPose_e.pose.x << "," << slamPose_e.pose.y <<")";
        return true;
    }
}

bool PlanningSdk::GetNearPoint(PoseStamped start , PoseStamped pose_1 , PoseStamped pose_2 , PoseStamped& target){
    Mat src = s_map.NavMap.clone();

    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            if(src.at<uint8_t >(i,j) == NavSensorObsCell || src.at<uint8_t >(i,j) == NavObsExpandCell || src.at<uint8_t >(i,j) == NavObsCell){
                src.at<uint8_t >(i,j) = NavObsCell;
            }
            if(s_map.ForbidenMap.at<uint8_t >(i,j) == ForbiddenCell || s_map.DeepLearningMap.at<uint8_t >(i,j) == ObsCell ){
                src.at<uint8_t >(i,j) = NavObsCell;
            }
        }

    Search * search = s_planner.getsearch();
    if(search == nullptr){
        LOGPLANNING(INFO) << "* searchnullptr !!!";
        return false;
    }
    Point p;
    src.at<uint8_t >(pose_1.point) = 1;
    src.at<uint8_t >(pose_2.point) = 1;
    search->Search_point_set(src , start.point , 1 ,p);
    src.release();

    if(p == pose_1.point){
        target = pose_1;
        LOGPLANNING(INFO) << "GetNearPoint find near point: (" << pose_1.point.x << "," <<pose_1.point.y << ")-("<<pose_1.pose.x << " , " <<pose_1.pose.y << ")";
        return true;
    }else if(p == pose_2.point){
        target = pose_2;
        LOGPLANNING(INFO) << "GetNearPoint find near point: (" << pose_2.point.x << "," <<pose_2.point.y << ")-("<<pose_2.pose.x << " , " <<pose_2.pose.y << ")";
        return true;
    }else{
        LOGPLANNING(INFO) << "GetNearPoint not find near point";
        return false;
    }
}

int PlanningSdk::OutObs(Point2f start){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    Search * search = s_planner.getsearch();
    if(map_manage == nullptr || search == nullptr){
        LOGPLANNING(INFO) << "* map_manage || * searchnullptr !!!";
        return 0;
    }
    Point start_ = s_map.WorldToMap(start);
    vector<vector<Point>> * StructuringElement = s_map.GetExpandElement();

//    s_map.ForbidenMapMutex.readLock();
    for(int n = 0 ; n < 4 ; n ++)
        for(int i = 0 ; i < (StructuringElement->begin() + n)->size() ; i++){
            Point p = *((StructuringElement->begin() + n)->begin()+i) + start_;
            if(s_map.ForbidenMap.at<uint8_t >(p) == ForbiddenUnknowCell){
//                s_map.ForbidenMapMutex.readUnlock();
                LOGPLANNING(INFO) << "outof trouble !!!";
                return 1;
            }
        }
//    s_map.ForbidenMapMutex.readUnlock();
    return 0;
}

int  PlanningSdk::UploadSensor(SensorInfo & dataup , int MotionControlMode){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    CleanBox * boxinfo = s_planner.getcleanbox();
    if(map_manage == nullptr || boxinfo == nullptr){
        LOGPLANNING(INFO) << "* map_manage || boxinfo nullptr !!!";
        return false;
    }
    static Point p_last_robot = Point(0,0);
    static uint64_t stamp_tof = 0;
    static uint64_t stamp_ir = 0;
    static uint64_t stamp_cliff = 0;

    if(dataup.PerceptionDelimitInfo.header.status){
        if(!dataup.PerceptionDelimitInfo.delimit.empty()){
            s_map.perceptiondata = dataup.PerceptionDelimitInfo;
        }
        dataup.PerceptionDelimitInfo.header.status = false;
    }
    if(dataup.appinfo.forbiden.header.status){
        map_manage->fill_forbiden(dataup.appinfo.forbiden.wall,dataup.appinfo.forbiden.closed);
        dataup.appinfo.forbiden.header.status = false;
    }
    if(MotionControlMode != motionState::RELOCATION && MotionControlMode != motionState::LEAVEPILE && MotionControlMode != motionState::INIT){
        if(dataup.perceptionInfo.status){
            map_manage->fill_perception_obs(dataup.perceptionInfo , dataup.Posture.point);
            dataup.perceptionInfo.status = perception::NO_UPDATE;
        }
#if 0
        if(stamp_tof != dataup.tofdata.header.stamp){
//            std::chrono::steady_clock::time_point tof_time_start = std::chrono::steady_clock::now();

            map_manage->fill_appmap_pose(dataup);

//            std::chrono::steady_clock::time_point tof_time_end = std::chrono::steady_clock::now();
//            std::chrono::duration<double, std::milli> elapsed = tof_time_end - tof_time_start;
//
//            LOGPLANNING(INFO) << "time cost filltof:" <<elapsed.count();

            stamp_tof = dataup.tofdata.header.stamp;
        }
#endif
        if(dataup.tofdata.header.status){
//            std::chrono::steady_clock::time_point tof_time_start = std::chrono::steady_clock::now();

//            map_manage->fill_appmap_pose(dataup);
//            map_manage->fill_appmap_cutpose(dataup);
            map_manage->fill_appmap_source(dataup , datadown);
//            std::chrono::steady_clock::time_point tof_time_end = std::chrono::steady_clock::now();
//            std::chrono::duration<double, std::milli> elapsed = tof_time_end - tof_time_start;

//            LOGPLANNING(INFO) << "time cost filltof:" <<elapsed.count();
            dataup.tofdata.header.status = false;
        }
        if(stamp_cliff != dataup.Cliff.header.stamp){
            if(dataup.Cliff.CliffTriggered){
                if(dataup.Cliff.Cliff_Left){
                    map_manage->fill_cliff(dataup.Posture.pose,(float)dataup.Posture.euler.yaw,0);
                }
                if(dataup.Cliff.Cliff_LeftF){
                    map_manage->fill_cliff(dataup.Posture.pose,(float)dataup.Posture.euler.yaw,1);
                }
                if(dataup.Cliff.Cliff_Right){
                    map_manage->fill_cliff(dataup.Posture.pose,(float)dataup.Posture.euler.yaw,2);
                }
                if(dataup.Cliff.Cliff_RightF){
                    map_manage->fill_cliff(dataup.Posture.pose,(float)dataup.Posture.euler.yaw,3);
                }
            }
            stamp_cliff = dataup.Cliff.header.stamp;
        }

        if(stamp_ir != dataup.Followir.header.stamp){
            if(dataup.Followir.Ir_Left > OFFSETDEPTH_FOLLOW){
                if(dataup.Followir.Ir_Left > MAXVALID_FOLLOW){
                    dataup.Followir.Ir_Left = -1.0f;
                }
                map_manage->fill_ir(dataup.Posture,(float)dataup.Posture.euler.yaw,0,dataup.Followir.Ir_Left);
            }
            if(dataup.Followir.Ir_Right > OFFSETDEPTH_FOLLOW){
                if(dataup.Followir.Ir_Right > MAXVALID_FOLLOW){
                    dataup.Followir.Ir_Right = -1.0f;
                }
                map_manage->fill_ir(dataup.Posture,(float)dataup.Posture.euler.yaw,1,dataup.Followir.Ir_Right);
            }

            stamp_ir = dataup.Followir.header.stamp;
        }

        if(p_last_robot == Point(0,0)){
            p_last_robot = dataup.Posture.point;
        }else if(p_last_robot != dataup.Posture.point){
            map_manage->fill_pathmap(p_last_robot,dataup.Posture.point);
            p_last_robot = dataup.Posture.point;
        }
    }

    return true;
}
int  PlanningSdk::UploadApp(const APPInfo & data){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    if(map_manage == nullptr){
        LOGPLANNING(INFO) << "* map_manage nullptr !!!";
        return false;
    }

    if(data.header.stamp != s_map.appdata.header.stamp){
        if(data.delimit.header.stamp != s_map.appdata.delimit.header.stamp){
//            s_map.RegionSegmentationMapMutex.writeLock();
            if(data.delimit.limit.header.stamp != s_map.appdata.delimit.limit.header.stamp){
                s_map.appdata.delimit.limit.lines.clear();
                s_map.appdata.delimit.limit = data.delimit.limit;
                map_manage->fill_limit(s_map.appdata.delimit.limit);
            }
            if(data.delimit.opened.header.stamp != s_map.appdata.delimit.opened.header.stamp){
                s_map.appdata.delimit.opened.rectangles.clear();
                s_map.appdata.delimit.opened = data.delimit.opened;
//                map_manage->fill_opened(s_map.appdata.delimit.opened);
            }
//            s_map.RegionSegmentationMapMutex.writeUnlock();
        }
        if(data.forbiden.header.stamp != s_map.appdata.forbiden.header.stamp){
//            s_map.ForbidenMapMutex.writeLock();
            if(data.forbiden.wall.header.stamp != s_map.appdata.forbiden.wall.header.stamp){
                s_map.appdata.forbiden.wall.lines.clear();
                s_map.appdata.forbiden.wall = data.forbiden.wall;
            }
            if(data.forbiden.closed.header.stamp != s_map.appdata.forbiden.closed.header.stamp){
                s_map.appdata.forbiden.closed.rectangles.clear();
                s_map.appdata.forbiden.closed = data.forbiden.closed;
            }
//            map_manage->fill_forbiden(s_map.appdata.forbiden.wall,s_map.appdata.forbiden.closed);
//            s_map.ForbidenMapMutex.writeUnlock();
        }
    }

    return true;
}
int  PlanningSdk::DownloadData(){
    PlanningMapManager * map_manage = s_planner.getmap_manage();
    if(map_manage == nullptr){
        LOGPLANNING(INFO) << "* map_manage nullptr !!!";
        return false;
    }

    return true;
}
bool PlanningSdk::SetCleanTask(int WorkingTask){
    workingtask = WorkingTask;
    LOGPLANNING(INFO) << "workingtask:" << workingtask;
    return true;
}
void PlanningSdk::savemap(string name , const Mat &map){
    time_t tt;
    tm* t= gmtime( &tt );

//    printf("%d-%02d-%02d %02d:%02d:%02d\n",
//           t->tm_year + 1900,
//           t->tm_mon + 1,
//           t->tm_mday,
//           t->tm_hour,
//           t->tm_min,
//           t->tm_sec);
    string nowtime = to_string(t->tm_year + 1900);
    nowtime = nowtime + to_string(t->tm_mon + 1);
    nowtime = nowtime + to_string(t->tm_mday);
//    string nowday = logpath + "/" + nowtime;
    nowtime = nowtime + to_string(t->tm_hour);
    nowtime = nowtime + to_string(t->tm_min);
    nowtime = nowtime + to_string(t->tm_sec);

//    if (access(nowday.c_str(), 0) == -1)
//    {
//        cout << nowday << " is not existing" << endl;
//        cout << "now make it" << endl;
//        int flag = mkdir(nowday.c_str(), 0777);
//        if (flag == 0)
//        {
//            cout << "make successfully" << endl;
//        } else {
//            cout << "make errorly" << endl;
//        }
//    }
//
//    imwrite(nowday + "/" + nowtime + "_" + name,map);
}

#ifdef ROSDEBUG

#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <nav_msgs/OccupancyGrid.h>
#include <visualization_msgs/MarkerArray.h>


int PlanningSdk::creat_thread_planning(){
    if(T_planning != nullptr) {
        LOGPLANNING(INFO) << "T_motion is nullptr !!!";
        return 0;
    }

    T_planning = new thread([this](){
        //          int argc;
        //          char **argv;
        //          ros::init (argc, argv, "motionpublish");
        LOGPLANNING(INFO) << "Enter T_planning , pid:" << getpid() << ", tid: " << syscall(SYS_gettid);
        LOGPLANNING(INFO) << "this thread id is: " << std::this_thread::get_id();
        ros::NodeHandle nh;
        #ifndef SLAMTEST
        ros::Publisher cleanboxmarker_publisher_ = nh.advertise<visualization_msgs::MarkerArray>("cleanbox_markerarray", 25);
        #endif

        nav_msgs::Path mypath;
        mypath.header.frame_id = "odom";
        mypath.header.stamp = ros::Time(0);

        geometry_msgs::Point p_marker;
        visualization_msgs::MarkerArray marker_array_msg;
        visualization_msgs::Marker marker_msg;

        int num_count = 0;
        CleanBox * clean_box = s_planner.getcleanbox();
        flg_thread_planning = true;
        while(flg_thread_planning) {
            num_count ++;
            if(num_count % 50 == 0){
                if(clean_box != nullptr){
                    if(!clean_box->CleanBox_history.empty()) {
                        for (int i = 0; i < clean_box->CleanBox_history.size(); i++) {

//                            printf("box %d :(%f,%f)-(%f,%f) - [%d,%d]-[%d,%d]\n",i,
//                                    clean_box->CleanBox_history[i].pose_min.x,clean_box->CleanBox_history[i].pose_min.y,
//                                   clean_box->CleanBox_history[i].pose_max.x,clean_box->CleanBox_history[i].pose_max.y,
//                                   clean_box->CleanBox_history[i].point_min.x,clean_box->CleanBox_history[i].point_min.y,
//                                   clean_box->CleanBox_history[i].point_max.x,clean_box->CleanBox_history[i].point_max.y);

                            marker_msg.header.frame_id = "odom";
                            marker_msg.header.stamp = ros::Time(0);
                            marker_msg.ns = "m";
                            marker_msg.id = i;
                            marker_msg.type = visualization_msgs::Marker::LINE_STRIP;
                            marker_msg.action = visualization_msgs::Marker::ADD;
                            marker_msg.scale.x = 0.05;
                            marker_msg.scale.y = 0.1;
                            marker_msg.scale.z = 0.1;
                            marker_msg.color.a = 1.0;
                            marker_msg.color.r = 0.0;
                            marker_msg.color.g = 0.0;
                            marker_msg.color.b = 1.0;
                            marker_msg.lifetime = ros::Duration();

                            p_marker.x = clean_box->CleanBox_history[i].pose_min.x;
                            p_marker.y = clean_box->CleanBox_history[i].pose_min.y;
                            marker_msg.points.push_back(p_marker);

                            p_marker.x = clean_box->CleanBox_history[i].pose_max.x;
                            p_marker.y = clean_box->CleanBox_history[i].pose_min.y;
                            marker_msg.points.push_back(p_marker);

                            p_marker.x = clean_box->CleanBox_history[i].pose_max.x;
                            p_marker.y = clean_box->CleanBox_history[i].pose_max.y;
                            marker_msg.points.push_back(p_marker);

                            p_marker.x = clean_box->CleanBox_history[i].pose_min.x;
                            p_marker.y = clean_box->CleanBox_history[i].pose_max.y;
                            marker_msg.points.push_back(p_marker);

                            p_marker.x = clean_box->CleanBox_history[i].pose_min.x;
                            p_marker.y = clean_box->CleanBox_history[i].pose_min.y;
                            marker_msg.points.push_back(p_marker);
                            marker_msg.pose.orientation.x = 0.0;
                            marker_msg.pose.orientation.y = 0.0;
                            marker_msg.pose.orientation.z = 0.0;
                            marker_msg.pose.orientation.w = 1;
                            marker_array_msg.markers.push_back(marker_msg);
                            marker_msg.points.clear();
                        }
                        #ifndef SLAMTEST
                        cleanboxmarker_publisher_.publish(marker_array_msg);
                        #endif
                        marker_array_msg.markers.clear();
                    }
                }
            }
            this_thread::sleep_for(chrono::microseconds(20000));//50HZ
        }
        #ifndef SLAMTEST
        cleanboxmarker_publisher_.shutdown();
        #endif
        if(!marker_msg.points.empty()){
            marker_msg.points.clear();
        }
        if(!marker_array_msg.markers.empty()){
            marker_array_msg.markers.clear();
        }
    });
    return 0;
}

int PlanningSdk::kill_thread_planning(){
    flg_thread_planning = false;
    if(T_planning != nullptr){
        T_planning->join();
        delete T_planning;
        T_planning = nullptr;
    }
    return 0;
}
#endif

int PlanningSdk::creat_thread_buildmap(){
    if(T_buildmap != nullptr) {
        LOGPLANNING(INFO) << "T_motion is nullptr !!!";
        return 0;
    }
    T_buildmap = new thread([this](){
        LOGPLANNING(INFO) << "Enter T_buildmap , pid:" << getpid() << ", tid: " << syscall(SYS_gettid);
        LOGPLANNING(INFO) << "this thread id is: " << std::this_thread::get_id();
        int timecount = 0;
        flg_thread_buildmap = true;
        uint8_t v_tmp = 0;
        while(flg_thread_buildmap){
            timecount ++;
            if(sensorinfo != nullptr){
                UploadSensor(*sensorinfo , datadown->MotionState);
//                findChargingpile::searchCharging::PointClassification(sensorinfo);
            }

            if((timecount % 50) == 0){
                if(!s_map.APPMap.empty()){
                    Point p_min = sensorinfo->Posture.point - Point(s_map.width2slam/2,s_map.height2slam/2);
                    if(p_min.x < 0){
                        p_min.x = 0;
                    }
                    if(p_min.y < 0){
                        p_min.y = 0;
                    }
                    if((p_min.x + s_map.width2slam) > s_map.APPMap.cols){
                        p_min.x = datadown->appmap.width - s_map.width2slam;
                    }
                    if((p_min.y + s_map.height2slam) > s_map.APPMap.rows){
                        p_min.y = datadown->appmap.height - s_map.height2slam;
                    }

                    datadown->appmap.height = s_map.height2slam;
                    datadown->appmap.width = s_map.width2slam;
                    datadown->appmap.resolving = s_map.resolving;
                    datadown->appmap.origin = s_map.MapToWorld(p_min) - Point2f(0.025f,0.025f);
                    datadown->appmap.origin_real = s_map.originPoseOccupy;
                    datadown->appmap.height_real = s_map.heightOccupy;
                    datadown->appmap.width_real = s_map.widthOccupy;

                    datadown->appmap.map = s_map.APPMap(Rect(p_min.x,p_min.y,datadown->appmap.width,datadown->appmap.height)).clone();

                    for(int i = 0 ; i < datadown->appmap.map.rows ; i++)
                        for(int j = 0 ; j < datadown->appmap.map.cols ; j++){
                            v_tmp = datadown->appmap.map.at<uint8_t >(i,j);
                            if(v_tmp > FreeCell && v_tmp <= ObsCell){
                                datadown->appmap.map.at<uint8_t >(i,j) = ObsCell;
                            }
                        }

                    auto nowTime = std::chrono::system_clock::now();
                    auto microSeconds = std::chrono::duration_cast<std::chrono::microseconds>(nowTime.time_since_epoch()).count();
                    datadown->appmap.header.stamp = (uint64_t)microSeconds;
                }
            }
            if((timecount % 100) == 0){
                int num = 0;
                for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
                    for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
                        if(s_map.CoverMap.at<uint8_t >(i,j) == CoverCell){
                            num ++;
                        }
                    }
                datadown->area = (float)(pow(s_map.resolving,2) * num);
//                LOGPLANNING(INFO) << "clean area:" <<datadown->area << " (M^2) , " << num;

                clean_time_end = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::milli> elapsed = clean_time_end - clean_time_start; // std::micro 表示以微秒为时间单位
                datadown->clean_du = (int)(elapsed.count()/1000);
//                LOGPLANNING(INFO) << "clean time:" <<datadown->clean_du << " seconds";
            }

            this_thread::sleep_for(chrono::microseconds(20000));//50HZ
        }
    });
}

int PlanningSdk::kill_thread_buildmap(){
    flg_thread_buildmap = false;
    if(T_buildmap != nullptr){
        T_buildmap->join();
        delete T_buildmap;
        T_buildmap = nullptr;
    }
    return 0;
}
