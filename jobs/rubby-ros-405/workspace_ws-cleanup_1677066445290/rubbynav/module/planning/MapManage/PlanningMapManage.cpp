//
// Created by lcy on 21-6-10.
//

#include <opencv2/imgproc.hpp>
#include <cmath>
#include <glog/logging.h>
#include "PlanningMapManage.h"
#include "Map.h"


#if 1
#if 0
void MapManager::init(){

    NavMapMutex.writeLock();
    NavMap.release();
    NavMap = cv::Mat::ones(navmap_rows,navmap_cols,CV_8UC1)*255;
    NavMapMutex.writeUnlock();

    PathMapMutex.writeLock();
    PathMap.release();
    PathMap = cv::Mat::ones(navmap_rows,navmap_cols,CV_8UC1)*255;
    PathMapMutex.writeUnlock();

    CoverMapMutex.writeLock();
    CoverMap.release();
    CoverMap = cv::Mat::ones(cleanmap_rows,cleanmap_cols,CV_8UC1)*255;
    CoverMapMutex.writeUnlock();

    FollowMapMutex.writeLock();
    FollowMap.release();
    FollowMap = cv::Mat::ones(navmap_rows,navmap_cols,CV_8UC1)*255;
    FollowMapMutex.writeUnlock();

    ForbidenMapMutex.writeLock();
    ForbidenMap.release();
    ForbidenMap = cv::Mat::ones(navmap_rows,navmap_cols,CV_8UC1)*255;
    ForbidenMapMutex.writeUnlock();

    RegionSegmentationMapMutex.writeLock();
    RegionSegmentationMap.release();
    RegionSegmentationMap = cv::Mat::ones(cleanmap_rows,cleanmap_cols,CV_8UC1)*255;
    RegionSegmentationMapMutex.writeUnlock();

    CleanBoxMapMutex.writeLock();
    CleanBoxMap.release();
    CleanBoxMap.create(navmap_rows,navmap_cols,CV_8UC3);
    CleanBoxMapMutex.writeUnlock();

    DeepLearningMapMutex.writeLock();
    DeepLearningMap.release();
    DeepLearningMap.create(navmap_rows,navmap_cols,CV_8UC3);
    DeepLearningMapMutex.writeUnlock();

//    namedWindow("CleanBoxMap", CV_WINDOW_NORMAL);
//    imshow("CleanBoxMap", CleanBoxMap);
}
void MapManager::datarelease(){
    NavMapMutex.writeLock();
    NavMap.release();
    NavMapMutex.writeUnlock();

    PathMapMutex.writeLock();
    PathMap.release();
    PathMapMutex.writeUnlock();

    CoverMapMutex.writeLock();
    CoverMap.release();
    CoverMapMutex.writeUnlock();

    FollowMapMutex.writeLock();
    FollowMap.release();
    FollowMapMutex.writeUnlock();

    CleanBoxMapMutex.writeLock();
    CleanBoxMap.release();
    CleanBoxMapMutex.writeUnlock();

    ForbidenMapMutex.writeLock();
    ForbidenMap.release();
    ForbidenMapMutex.writeUnlock();

    RegionSegmentationMapMutex.writeLock();
    RegionSegmentationMap.release();
    RegionSegmentationMapMutex.writeUnlock();

    CleanBoxMapMutex.writeLock();
    CleanBoxMap.release();
    CleanBoxMapMutex.writeUnlock();

    DeepLearningMapMutex.writeLock();
    DeepLearningMap.release();
    DeepLearningMapMutex.writeUnlock();
}
#endif
void PlanningMapManager::cleanagain(){
//    s_map.CoverMapMutex.writeLock();
    s_map.CoverMap.release();
    s_map.CoverMap = cv::Mat::ones(s_map.height,s_map.width,CV_8UC1)*255;
//    s_map.CoverMapMutex.writeUnlock();

//    s_map.NavMapMutex.writeLock();
    s_map.NavMap.release();
    for(int i = 0 ; i < s_map.height ; i++)
        for(int j = 0 ; j < s_map.width ; j++){
            s_map.NavMap.at<uint8_t >(i,j) = s_map.NavMap.at<uint8_t >(i,j) == NavSensorObsCell ? NavObsCell:s_map.NavMap.at<uint8_t >(i,j);
    }
//    s_map.NavMapMutex.writeUnlock();

//    s_map.FollowMapMutex.writeLock();
    s_map.FollowMap.release();
    s_map.FollowMap = cv::Mat::ones(s_map.height,s_map.width,CV_8UC1)*255;
//    s_map.FollowMapMutex.writeUnlock();
}

int PlanningMapManager::setexplormap(Mat &src , Mat & dst){
    if(src.empty()){
        return 0;
    }
    dst = src.clone();

    for(int i = 0 ; i < src.rows ; i++)
        for(int j = 0 ; j < src.cols ; j++){
            if(src.at<uint8_t >(i,j) == NavFreeCell){
                for(int m = 0 ; m < s_map.GetExpandSingleElement(1)->size() ; m++){
                    if(src.at<uint8_t >(s_map.GetExpandSingleElement(1,m)->y + i , s_map.GetExpandSingleElement(1,m)->x + j) == NavUnknowCell){
                        for(int n = 0 ; n < 5 ; n++)
                            for(int k = 0 ; k < s_map.GetExpandSingleElement(n)->size() ; k++){
                                if(src.at<uint8_t >(i + s_map.GetExpandSingleElement(n,k)->y,j + s_map.GetExpandSingleElement(n,k)->x) == NavUnknowCell) {
                                    dst.at<uint8_t >(i + s_map.GetExpandSingleElement(n,k)->y,j + s_map.GetExpandSingleElement(n,k)->x) = NavFreeCell;
                                }
                            }
                        break;
                    }
                }

            }
        }
    Mat src_ = dst.clone();

    src = src_.clone();
    for(int i = 10 ; i < src_.rows - 10 ; i++)
        for(int j = 10 ; j < src_.cols - 10 ; j++){
            if(src_.at<uint8_t >(i,j) == NavUnknowCell){
                for(int m = 0 ; m < s_map.GetExpandSingleElement(1)->size() ; m++){
                    if(src_.at<uint8_t >(s_map.GetExpandSingleElement(1,m)->y + i , s_map.GetExpandSingleElement(1,m)->x + j) == NavFreeCell){
                        for(int n = 0 ; n < 5 ; n++)
                            for(int k = 0 ; k < s_map.GetExpandSingleElement(n)->size() ; k++){
                                if(src_.at<uint8_t >(i + s_map.GetExpandSingleElement(n,k)->y,j + s_map.GetExpandSingleElement(n,k)->x) == NavFreeCell) {
                                    src.at<uint8_t >(i + s_map.GetExpandSingleElement(n,k)->y,j + s_map.GetExpandSingleElement(n,k)->x) = NavUnknowCell;
                                }
                            }
                        break;
                    }
                }

            }
        }
    for(int i = 10 ; i < src_.rows - 10 ; i++)
        for(int j = 10 ; j < src_.cols - 10 ; j++){
            if(src_.at<uint8_t >(i,j) == NavUnknowCell){
                for(int m = 0 ; m < s_map.GetExpandSingleElement(1)->size() ; m++){
                    if(src_.at<uint8_t >(s_map.GetExpandSingleElement(1,m)->y + i , s_map.GetExpandSingleElement(1,m)->x + j) == NavFreeCell){
                        for(int n = 0 ; n < 9 ; n++)
                            for(int k = 0 ; k < s_map.GetExpandSingleElement(n)->size() ; k++){
                                if(src_.at<uint8_t >(i + s_map.GetExpandSingleElement(n,k)->y,j + s_map.GetExpandSingleElement(n,k)->x) == NavFreeCell) {
                                    dst.at<uint8_t >(i + s_map.GetExpandSingleElement(n,k)->y,j + s_map.GetExpandSingleElement(n,k)->x) = NavUnknowCell;
                                }
                            }
                        break;
                    }
                }

            }
        }
    src_.release();

    return 1;
}
void PlanningMapManager::map_dilate(Mat & src , uint8_t type , int num){
    Mat dst = src.clone();
    for(int i = 10 ; i < src.rows - 10 ; i++)
        for(int j = 10 ; j < src.cols - 10 ; j++){
            if(dst.at<uint8_t >(i,j) == type){
                for(int n = 0 ; n < num ; n++)
                for(int k = 0 ; k < s_map.GetExpandSingleElement(n)->size() ; k++){
                    if(src.at<uint8_t >(i + s_map.GetExpandSingleElement(n,k)->y,j + s_map.GetExpandSingleElement(n,k)->x) != type) {
                        src.at<uint8_t>(i,j) = type;
                        break;
                    }
                }

            }
        }
    dst.release();
}
void PlanningMapManager::map_erode(Mat & src , uint8_t type , int num){
    Mat dst = src.clone();
    uint8_t tmp = 0;
    for(int i = 0 ; i < src.rows ; i++)
        for(int j = 0 ; j < src.cols ; j++){
            if(dst.at<uint8_t >(i,j) == type){
                for(int n = 0 ; n < num ; n++)
                for(int k = 0 ; k < s_map.GetExpandSingleElement(n)->size() ; k++){
                    tmp = dst.at<uint8_t >(i + s_map.GetExpandSingleElement(n,k)->y,j + s_map.GetExpandSingleElement(n,k)->x);
                    if(tmp != type) {
                        src.at<uint8_t>(i,j) = tmp;
                        break;
                    }
                }
            }
        }
    dst.release();
}
void PlanningMapManager::set_FollowMap(Point2f p){

}

void PlanningMapManager::fill_pathmap(Point p1 , Point p2){
//    s_map.PathMapMutex.writeLock();
    line(s_map.PathMap,p1,p2,Scalar(PathFreeCell),1,LINE_4);
//    s_map.PathMapMutex.writeUnlock();
}

void PlanningMapManager::fill_tof(SensorInfo & dataup){
    if(dataup.tofdata.data.empty()){
        return;
    }
    Point p_1,p_2,p_robot;
    Point ptmp;
    Point2f po_1,po_2;
    float laserlength = 0.75;
    double angle_min= -0.785390019417;
    double angle_max= 0.785390019417;
    double angle_increment= 0.0158664640039;

    po_1 = Point2f((float)(dataup.Posture.pose.x + 0.175*cos(dataup.Posture.euler.yaw)),(float)(dataup.Posture.pose.y + 0.175*sin(dataup.Posture.euler.yaw)));
    p_1 = s_map.WorldToMap(po_1);
    p_robot = dataup.Posture.pose;
    vector<Point> rasterlist;
    vector<Point> rasterlist_tof;
//    s_map.NavMapMutex.writeLock();
    for(int i = 0 ; i < 100 ; i++){
        if(isinf(dataup.tofdata.data[i])){
            po_2 = Point2f((float)(po_1.x + laserlength*cos(dataup.Posture.euler.yaw + angle_min + angle_increment*i)),(float)(po_1.y + laserlength*sin(dataup.Posture.euler.yaw + angle_min + angle_increment*i)));
            p_2 = s_map.WorldToMap(po_2);
            if(RasterLine_0(p_1 , p_2 , rasterlist_tof)){
                if(!rasterlist_tof.empty()){
                    rasterlist.insert(rasterlist.end(),rasterlist_tof.begin(),rasterlist_tof.begin() + rasterlist_tof.size());
                }
            }
        }else if(dataup.tofdata.data[i] > 0){
            po_2 = Point2f((float)(po_1.x + dataup.tofdata.data[i]*cos(dataup.Posture.euler.yaw + angle_min + angle_increment*i)),(float)(po_1.y + dataup.tofdata.data[i]*sin(dataup.Posture.euler.yaw + angle_min + angle_increment*i)));
            p_2 = s_map.WorldToMap(po_2);
            if(abs(p_1.x - p_2.x) + abs(p_1.y - p_2.y) <= 2){
            }else{
                if(RasterLine_0(p_1 , p_2 , rasterlist_tof)){
                    if(!rasterlist_tof.empty()){
                        rasterlist.insert(rasterlist.end(),rasterlist_tof.begin(),rasterlist_tof.begin() + rasterlist_tof.size());
                    }
                }
            }
        }
    }
    for(const auto &i:rasterlist){
        if(s_map.NavMap.at<uint8_t >(i) == NavUnknowCell || s_map.NavMap.at<uint8_t >(i) == NavObsCell){
            s_map.NavMap.at<uint8_t >(i) = NavFreeCell;
        }
    }

    for(int i = 0 ; i < 100 ; i++){
        if(!isinf(dataup.tofdata.data[i]) && dataup.tofdata.data[i] > 0){
            po_2 = Point2f((float)(po_1.x + dataup.tofdata.data[i]*cos(dataup.Posture.euler.yaw + angle_min + angle_increment*i)),(float)(po_1.y + dataup.tofdata.data[i]*sin(dataup.Posture.euler.yaw + angle_min + angle_increment*i)));
            p_2 = s_map.WorldToMap(po_2);
            for(int n = 1 ; n < 5 ; n++)
                for(int m = 0 ; m < s_map.GetExpandSingleElement(n)->size() ; m++){
                    ptmp = Point(s_map.GetExpandSingleElement(n,m)->x + p_2.x,s_map.GetExpandSingleElement(n,m)->y + p_2.y);
                    if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                        continue;
                    }
                    uint8_t cell = s_map.NavMap.at<uint8_t >(ptmp);
                    if(cell == NavUnknowCell || cell == NavFreeCell){
                        s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
                    }
                }
            for(int m = 0 ; m < s_map.GetExpandSingleElement(1)->size() ; m++){
                ptmp = Point(s_map.GetExpandSingleElement(1,m)->x + p_2.x,s_map.GetExpandSingleElement(1,m)->y + p_2.y);
                if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                    continue;
                }
                if(s_map.NavMap.at<uint8_t >(ptmp) != NavSensorObsCell){
                    s_map.NavMap.at<uint8_t >(ptmp) = NavObsCell;
                }
            }
            if(s_map.NavMap.at<uint8_t >(p_2) != NavSensorObsCell){
                s_map.NavMap.at<uint8_t >(p_2) = NavObsCell;
            }
        }
    }
#if 1
    int flg_ = 0;
    Point p_min = Point(p_robot.x - 50,p_robot.y - 50);
    Point p_max = Point(p_robot.x + 50,p_robot.y + 50);
    for(int i = p_min.x ; i < p_max.x ; i++)
        for(int j = p_min.y ; j < p_max.y ; j++){
            if(s_map.NavMap.at<uint8_t>(j,i) == NavObsExpandCell) {
                flg_ = 0;
                for (int n = 0; n < 5; n++)
                    for (int k = 0; k < s_map.GetExpandSingleElement(n)->size(); k++) {
                        ptmp = Point(s_map.GetExpandSingleElement(n,k)->x + i, s_map.GetExpandSingleElement(n,k)->y + j);
                        if (ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height) {
                            continue;
                        }
                        if(s_map.NavMap.at<uint8_t>(ptmp) == NavObsCell){
                            n = 5;
                            flg_ = 1;
                            break;
                        }
                    }
                if(flg_ == 0){
                    s_map.NavMap.at<uint8_t>(j,i) = NavFreeCell;
                }
            }
        }
#endif

//    s_map.NavMapMutex.writeUnlock();
    rasterlist.clear();
    rasterlist_tof.clear();
    vector<Point>().swap(rasterlist);
    vector<Point>().swap(rasterlist_tof);
}

void PlanningMapManager::fill_appmap(SensorInfo & dataup){
#if 0
    if(dataup.tofdata.data.empty()){
        return;
    }
    if(flg_firstfill){
        circle(s_map.NavMap, dataup.Posture.point, 7, Scalar(NavFreeCell), -1);
        circle(s_map.APPMap, dataup.Posture.point, 7, Scalar(FreeCell), -1);
        flg_firstfill = false;
    }
    Point p_1,p_2,p_robot;
    Point ptmp;
    Point2f po_1,po_2;
    float laserlength = 0.75;
    double angle_min= -0.785390019417;
    double angle_max= 0.785390019417;
    double angle_followir_r = (-1) * Pi_/3;
    double angle_increment= 0.0158664640039;

    po_1 = Point2f((float)(dataup.Posture.pose.x + 0.175*cos(dataup.Posture.euler.yaw)),(float)(dataup.Posture.pose.y + 0.175*sin(dataup.Posture.euler.yaw)));
    p_1 = s_map.WorldToMap(po_1);
    p_robot = dataup.Posture.point;
    vector<Point> rasterlist;
    vector<Point> rasterlist_tof;
    for(int i = 0 ; i < 100 ; i++){
        if(isinf(dataup.tofdata.data[i])){
            po_2 = Point2f((float)(po_1.x + laserlength*cos(dataup.Posture.euler.yaw + angle_min + angle_increment*i)),(float)(po_1.y + laserlength*sin(dataup.Posture.euler.yaw + angle_min + angle_increment*i)));
            p_2 = s_map.WorldToMap(po_2);
            if(RasterLine_0(p_1 , p_2 , rasterlist_tof)){
                if(!rasterlist_tof.empty()){
                    rasterlist.insert(rasterlist.end(),rasterlist_tof.begin(),rasterlist_tof.begin() + rasterlist_tof.size());
                }
            }
        }else if(dataup.tofdata.data[i] > 0){
            po_2 = Point2f((float)(po_1.x + dataup.tofdata.data[i]*cos(dataup.Posture.euler.yaw + angle_min + angle_increment*i)),(float)(po_1.y + dataup.tofdata.data[i]*sin(dataup.Posture.euler.yaw + angle_min + angle_increment*i)));
            p_2 = s_map.WorldToMap(po_2);
            if(abs(p_1.x - p_2.x) + abs(p_1.y - p_2.y) <= 2){
            }else{
                if(RasterLine_0(p_1 , p_2 , rasterlist_tof)){
                    if(!rasterlist_tof.empty()){
                        rasterlist.insert(rasterlist.end(),rasterlist_tof.begin(),rasterlist_tof.begin() + rasterlist_tof.size());
                    }
                }
            }
        }
    }

    for(const auto &i:rasterlist){
        if(s_map.NavMap.at<uint8_t >(i) == NavUnknowCell || s_map.NavMap.at<uint8_t >(i) == NavObsCell){
            s_map.NavMap.at<uint8_t >(i) = NavFreeCell;
        }
        s_map.APPMap.at<uint8_t >(i) = FreeCell;
    }

    for(int i = 0 ; i < 100 ; i++){
        if(!isinf(dataup.tofdata.data[i]) && dataup.tofdata.data[i] > 0){
            po_2 = Point2f((float)(po_1.x + dataup.tofdata.data[i]*cos(dataup.Posture.euler.yaw + angle_min + angle_increment*i)),(float)(po_1.y + dataup.tofdata.data[i]*sin(dataup.Posture.euler.yaw + angle_min + angle_increment*i)));
            p_2 = s_map.WorldToMap(po_2);
            for(int n = 1 ; n < radius_obs + 1 ; n++)
                for(int m = 0 ; m < s_map.GetExpandSingleElement(n)->size() ; m++){
                    ptmp = Point(s_map.GetExpandSingleElement(n,m)->x + p_2.x,s_map.GetExpandSingleElement(n,m)->y + p_2.y);
                    if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                        continue;
                    }
                    uint8_t cell = s_map.NavMap.at<uint8_t >(ptmp);
                    if(cell == NavUnknowCell || cell == NavFreeCell){
                        s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
                    }
                }
            for(int m = 0 ; m < s_map.GetExpandSingleElement(1)->size() ; m++){
                ptmp = Point(s_map.GetExpandSingleElement(1,m)->x + p_2.x,s_map.GetExpandSingleElement(1,m)->y + p_2.y);
                if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                    continue;
                }
                if(s_map.NavMap.at<uint8_t >(ptmp) != NavSensorObsCell){
                    s_map.NavMap.at<uint8_t >(ptmp) = NavObsCell;
                }
            }
            if(s_map.NavMap.at<uint8_t >(p_2) != NavSensorObsCell){
                s_map.NavMap.at<uint8_t >(p_2) = NavObsCell;
            }
            s_map.APPMap.at<uint8_t >(p_2) = ObsCell;
        }
    }

#if 1
    int flg_ = 0;
    Point p_min = Point(p_robot.x - 50,p_robot.y - 50);
    Point p_max = Point(p_robot.x + 50,p_robot.y + 50);
    for(int i = p_min.x ; i < p_max.x ; i++)
        for(int j = p_min.y ; j < p_max.y ; j++){
            if(s_map.NavMap.at<uint8_t>(j,i) == NavObsExpandCell) {
                flg_ = 0;
                for (int n = 1; n < radius_obs + 1; n++)
                    for (int k = 0; k < s_map.GetExpandSingleElement(n)->size(); k++) {
                        ptmp = Point(s_map.GetExpandSingleElement(n,k)->x + i, s_map.GetExpandSingleElement(n,k)->y + j);
                        if (ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height) {
                            continue;
                        }
                        if(s_map.NavMap.at<uint8_t>(ptmp) == NavObsCell){
                            n = radius_obs + 1;
                            flg_ = 1;
                            break;
                        }
                    }
                if(flg_ == 0){
                    s_map.NavMap.at<uint8_t>(j,i) = NavFreeCell;
                }
            }
        }
#endif

    rasterlist.clear();
    rasterlist_tof.clear();
    vector<Point>().swap(rasterlist);
    vector<Point>().swap(rasterlist_tof);
#endif
    if(dataup.tofdata.flg.empty()){
        return;
    }
    if(flg_firstfill){
        LOGPLANNING(INFO) << "flg_firstfill !!!";
        circle(s_map.NavMap, dataup.Posture.point, 7, Scalar(NavFreeCell), -1);
        circle(s_map.APPMap, dataup.Posture.point, 7, Scalar(FreeCell), -1);
        flg_firstfill = false;
    }

    for(int i = 0 ; i < dataup.tofdata.flg.size() ; i++){
        line(s_map.APPMap,dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,Scalar(FreeCell),1);
    }
    for (auto &&i : dataup.tofdata.flg) {
        if(i){
            s_map.APPMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = ObsCell;
        }
    }

    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }else if(s_map.NavMap.at<uint8_t >(j,i) == NavUnknowCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }
        }

    bool flg = false;
    int sidelength = radius_obs * 2;
    bool flg_side = false;
    if(dataup.tofdata.posemin.point.x < sidelength ||
    dataup.tofdata.posemin.point.y < sidelength ||
    dataup.tofdata.posemax.point.x > (s_map.NavMap.cols - sidelength) ||
    dataup.tofdata.posemax.point.y > (s_map.NavMap.rows - sidelength)){
        flg_side = true;
    }
    Point ptmp;
    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsExpandCell){
                flg = false;
                for (int n = 1; n < radius_obs + 1; n++)
                    for (int k = 0; k < s_map.GetExpandSingleElement(n)->size(); k++) {
                        ptmp = Point(s_map.GetExpandSingleElement(n,k)->x + i, s_map.GetExpandSingleElement(n,k)->y + j);
                        if(flg_side){
                            if (ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height) {
                                continue;
                            }
                        }
                        if(s_map.NavMap.at<uint8_t>(ptmp) == NavObsCell){
                            n = radius_obs + 1;
                            flg = true;
                            break;
                        }
                    }
                if(!flg){
                    s_map.NavMap.at<uint8_t>(j,i) = NavFreeCell;
                }
            }
        }
}
void PlanningMapManager::fill_appmap_pose(SensorInfo & dataup){
#if 1
    if(dataup.tofdata.flg.empty()){
        return;
    }
#if 0
    if(flg_firstfill){
        LOGPLANNING(INFO) << "flg_firstfill !!!";
        circle(s_map.NavMap, dataup.Posture.point, 7, Scalar(NavFreeCell), -1);
        circle(s_map.APPMap, dataup.Posture.point, 7, Scalar(FreeCell), -1);
        flg_firstfill = false;
    }
#endif

    vector<Point> vc;
	bool flg_obs_through = true;
    for(int i = 0 ; i < dataup.tofdata.flg.size() ; i++){
        if(dataup.tofdata.flg[i]){
            line(s_map.APPMap,dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,Scalar(FreeCell),1);
        }else{
            RasterLine_0(dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,vc);
			flg_obs_through = true;
            while(!vc.empty()){
                /*if(s_map.APPMap.at<uint8_t >(vc.back()) == UnknowCell){
                    s_map.APPMap.at<uint8_t >(vc.back()) = FreeCell;
                }*/
				if(s_map.APPMap.at<uint8_t >(vc.back()) != ObsCell &&
					s_map.APPMap.at<uint8_t >(vc.back() + Point(-1,0)) != ObsCell &&
					s_map.APPMap.at<uint8_t >(vc.back() + Point(0,-1)) != ObsCell &&
					s_map.APPMap.at<uint8_t >(vc.back() + Point(1,0)) != ObsCell &&
					s_map.APPMap.at<uint8_t >(vc.back() + Point(0,1)) != ObsCell){

                }else{
					flg_obs_through = false;
					break;
				}
                vc.pop_back();
            }
			vc.clear();
			if(flg_obs_through){
				line(s_map.APPMap,dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,Scalar(FreeCell),1);
			}
        }
    }
    vc.clear();
    vector<Point>().swap(vc);

    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            s_map.APPMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = ObsCell;
        }
    }

    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }else if(s_map.NavMap.at<uint8_t >(j,i) == NavUnknowCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }
        }

    int sidelength = radius_obs * 2;
    bool flg_side = false;
    if(dataup.tofdata.posemin.point.x < sidelength ||
    dataup.tofdata.posemin.point.y < sidelength ||
    dataup.tofdata.posemax.point.x > (s_map.NavMap.cols - sidelength) ||
    dataup.tofdata.posemax.point.y > (s_map.NavMap.rows - sidelength)){
        flg_side = true;
    }
    Point ptmp;
    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            for(int n = 1 ; n < radius_obs + 1 ; n++)
                for(int m = 0 ; m < s_map.GetExpandSingleElement(n)->size() ; m++){
                    ptmp = Point(s_map.GetExpandSingleElement(n,m)->x + dataup.tofdata.pose2[i].point.x,s_map.GetExpandSingleElement(n,m)->y + dataup.tofdata.pose2[i].point.y);
                    if(flg_side){
                        if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                            continue;
                        }
                    }
                    uint8_t cell = s_map.NavMap.at<uint8_t >(ptmp);
                    if(cell == NavUnknowCell || cell == NavFreeCell){
                        s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
                    }
                }
            s_map.NavMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = NavObsCell;
        }
    }

    bool flg = false;
    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsExpandCell){
                flg = false;
                for (int n = 1; n < radius_obs + 1; n++)
                    for (int k = 0; k < s_map.GetExpandSingleElement(n)->size(); k++) {
                        ptmp = Point(s_map.GetExpandSingleElement(n,k)->x + i, s_map.GetExpandSingleElement(n,k)->y + j);
                        if(flg_side){
                            if (ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height) {
                                continue;
                            }
                        }
                        if(s_map.NavMap.at<uint8_t>(ptmp) == NavObsCell){
                            n = radius_obs + 1;
                            flg = true;
                            break;
                        }
                    }
                if(!flg){
                    s_map.NavMap.at<uint8_t>(j,i) = NavFreeCell;
                }
            }
        }
#endif
}
void PlanningMapManager::fill_appmap_cutpose(SensorInfo & dataup){
#if 1
    if(dataup.tofdata.flg.empty()){
        return;
    }
#if 0
    if(flg_firstfill){
        LOGPLANNING(INFO) << "flg_firstfill !!!";
        circle(s_map.NavMap, dataup.Posture.point, 7, Scalar(NavFreeCell), -1);
        circle(s_map.APPMap, dataup.Posture.point, 7, Scalar(FreeCell), -1);
        flg_firstfill = false;
    }
#endif
	for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
			if(pow((dataup.tofdata.pose2[i].pose.x - dataup.Posture.pose.x) , 2) + pow((dataup.tofdata.pose2[i].pose.y - dataup.Posture.pose.y) , 2) < pow(ROBOTRADIUS * 2 , 2)){
				continue;
			}
			line(s_map.APPMap,dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,Scalar(FreeCell),1);            
        }
    }
    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            s_map.APPMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = ObsCell;
        }
    }
	
    vector<Point> vc;
    bool flg_obs_through = true;
    for(int i = 0 ; i < dataup.tofdata.flg.size() ; i++){
        if(dataup.tofdata.flg[i]){
            /*line(s_map.APPMap,dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,Scalar(FreeCell),1);
            s_map.APPMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = ObsCell;*/
        }else{

            RasterLine_0(dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,vc);
            flg_obs_through = true;
            while(!vc.empty()){
                if(flg_obs_through){
                    if(s_map.APPMap.at<uint8_t >(vc.back()) > FreeCell && s_map.APPMap.at<uint8_t >(vc.back()) < UnknowCell){
                        flg_obs_through = false;
                    }else if(s_map.APPMap.at<uint8_t >(vc.back() + Point(-1,0)) > FreeCell && s_map.APPMap.at<uint8_t >(vc.back() + Point(-1,0)) < UnknowCell){
                        flg_obs_through = false;
                    }else if(s_map.APPMap.at<uint8_t >(vc.back() + Point(0,-1)) > FreeCell && s_map.APPMap.at<uint8_t >(vc.back() + Point(0,-1)) < UnknowCell){
                        flg_obs_through = false;
                    }else if(s_map.APPMap.at<uint8_t >(vc.back() + Point(1,0)) > FreeCell && s_map.APPMap.at<uint8_t >(vc.back() + Point(1,0)) < UnknowCell){
                        flg_obs_through = false;
                    }else if(s_map.APPMap.at<uint8_t >(vc.back() + Point(0,1)) > FreeCell && s_map.APPMap.at<uint8_t >(vc.back() + Point(0,1)) < UnknowCell){
                        flg_obs_through = false;
                    }
                }
                if(s_map.APPMap.at<uint8_t >(vc.back()) >= 5 && s_map.APPMap.at<uint8_t >(vc.back()) != UnknowCell){
					/*if(s_map.APPMap.at<uint8_t >(vc.back() + Point(-1,0)) != UnknowCell && 
						s_map.APPMap.at<uint8_t >(vc.back() + Point(0,-1)) != UnknowCell && 
						s_map.APPMap.at<uint8_t >(vc.back() + Point(1,0)) != UnknowCell && 
						s_map.APPMap.at<uint8_t >(vc.back() + Point(0,1)) != UnknowCell &&
						
						s_map.APPMap.at<uint8_t >(vc.back() + Point(-1,1)) != UnknowCell && 
						s_map.APPMap.at<uint8_t >(vc.back() + Point(1,-1)) != UnknowCell && 
						s_map.APPMap.at<uint8_t >(vc.back() + Point(1,1)) != UnknowCell && 
						s_map.APPMap.at<uint8_t >(vc.back() + Point(-1,-1)) != UnknowCell){
							
						s_map.APPMap.at<uint8_t >(vc.back()) = (uint8_t)(s_map.APPMap.at<uint8_t >(vc.back()) - 5);	
					}*/
					if(pow(vc.back().x - dataup.Posture.point.x , 2) + pow(vc.back().y - dataup.Posture.point.y , 2) <= pow(7 , 2)){
						
					}else{
						s_map.APPMap.at<uint8_t >(vc.back()) = (uint8_t)(s_map.APPMap.at<uint8_t >(vc.back()) - 5);		
					}                    
                }
                vc.pop_back();
            }
            vc.clear();
            if(flg_obs_through){
                line(s_map.APPMap,dataup.tofdata.pose1[i].point,(dataup.tofdata.pose1[i].point + dataup.tofdata.pose2[i].point)/2,Scalar(FreeCell),1);
            }
        }
    }

    vc.clear();
    vector<Point>().swap(vc);

    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            s_map.APPMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = ObsCell;
        }
    }

    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }else if(s_map.NavMap.at<uint8_t >(j,i) == NavUnknowCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }
        }

    int sidelength = radius_obs * 2;
    bool flg_side = false;
    if(dataup.tofdata.posemin.point.x < sidelength ||
        dataup.tofdata.posemin.point.y < sidelength ||
        dataup.tofdata.posemax.point.x > (s_map.NavMap.cols - sidelength) ||
        dataup.tofdata.posemax.point.y > (s_map.NavMap.rows - sidelength)){
        flg_side = true;
    }
    Point ptmp;
    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            for(int n = 1 ; n < radius_obs + 1 ; n++)
                for(int m = 0 ; m < s_map.GetExpandSingleElement(n)->size() ; m++){
                    ptmp = Point(s_map.GetExpandSingleElement(n,m)->x + dataup.tofdata.pose2[i].point.x,s_map.GetExpandSingleElement(n,m)->y + dataup.tofdata.pose2[i].point.y);
                    if(flg_side){
                        if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                            continue;
                        }
                    }
                    uint8_t cell = s_map.NavMap.at<uint8_t >(ptmp);
                    if(cell == NavUnknowCell || cell == NavFreeCell){
                        s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
                    }
                }
            s_map.NavMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = NavObsCell;
        }
    }

    bool flg = false;
    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsExpandCell){
                flg = false;
                for (int n = 1; n < radius_obs + 1; n++)
                    for (int k = 0; k < s_map.GetExpandSingleElement(n)->size(); k++) {
                        ptmp = Point(s_map.GetExpandSingleElement(n,k)->x + i, s_map.GetExpandSingleElement(n,k)->y + j);
                        if(flg_side){
                            if (ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height) {
                                continue;
                            }
                        }
                        if(s_map.NavMap.at<uint8_t>(ptmp) == NavObsCell){
                            n = radius_obs + 1;
                            flg = true;
                            break;
                        }
                    }
                if(!flg){
                    s_map.NavMap.at<uint8_t>(j,i) = NavFreeCell;
                }
            }
        }
#endif
}
void PlanningMapManager::fill_appmap_weight(SensorInfo & dataup){
    if(dataup.tofdata.flg.empty()){
        return;
    }
#if 1//视野范围内看不到障碍物就把该障碍物消掉
    vector<Point> vc_;
    vector<int> vc_obs;
    vector<int> vc_free;
    for(int i = 0 ; i < dataup.tofdata.flg.size() ; i++){
        if(dataup.tofdata.flg[i]){
            RasterLine_0(dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,vc_);
            for(int j = 0 ; j < vc_.size() ; j++){
                vc_obs.emplace_back(vc_[j].y * s_map.width + vc_[j].x);
            }
        }else{
            RasterLine_0(dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,vc_);
            for(int j = 0 ; j < vc_.size() ; j++){
                vc_free.emplace_back(vc_[j].y * s_map.width + vc_[j].x);
            }
        }
    }
    vc_.clear();
    vector<Point>().swap(vc_);

    sort(vc_free.begin(), vc_free.end());
    sort(vc_obs.begin(), vc_obs.end());

    vc_free.erase(unique(vc_free.begin() , vc_free.end()) , vc_free.end());
    vc_obs.erase(unique(vc_obs.begin() , vc_obs.end()) , vc_obs.end());

    vector<int> target_free;
    target_free.resize(max(vc_obs.size(), vc_free.size()));
    vector<int>::iterator itend_target_free = set_difference(vc_free.begin(), vc_free.end(), vc_obs.begin(), vc_obs.end(), target_free.begin());

    /*vector<int> target_union;
    target_union.resize(vc_obs.size() + vc_free.size());
    vector<int>::iterator itend_target_union = set_union(vc_free.begin(), vc_free.end(), vc_obs.begin(), vc_obs.end(), target_union.begin());

    for(vector<int>::iterator iter = target_union.begin() ; iter != itend_target_union ; iter++){
        s_map.APPMap.at<uint8_t >((*iter)/s_map.width , (*iter)%(s_map.width)) = UnknowCell;
    }*/

    Point p_tmp;
    for(vector<int>::iterator iter = target_free.begin() ; iter != itend_target_free ; iter++){
        p_tmp = Point((*iter)%(s_map.width) , (*iter)/s_map.width);
        if(s_map.APPMap.at<uint8_t >(p_tmp) == UnknowCell){
            s_map.APPMap.at<uint8_t >(p_tmp) = FreeCell;
        }else if(s_map.APPMap.at<uint8_t >(p_tmp) != FreeCell){
            s_map.APPMap.at<uint8_t >(p_tmp) = (uint8_t)(s_map.APPMap.at<uint8_t >(p_tmp) - 20);
        }
    }
    target_free.clear();
    vector<int>().swap(target_free);

    for(vector<int>::iterator iter = vc_obs.begin() ; iter != vc_obs.end() ; iter++){
        s_map.APPMap.at<uint8_t >((*iter)/s_map.width , (*iter)%(s_map.width)) = FreeCell;
    }
    vc_obs.clear();
    vector<int>().swap(vc_obs);

    for(int i = 0 ; i < dataup.tofdata.flg.size() ; i++){
        if(dataup.tofdata.flg[i]){
            s_map.APPMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = ObsCell;
        }
    }
#endif
#if 0 //只消机器人到障碍物之间的障碍物
    vector<Point> vc;
    bool flg_obs_through = true;
    for(int i = 0 ; i < dataup.tofdata.flg.size() ; i++){
        if(dataup.tofdata.flg[i]){
            line(s_map.APPMap,dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,Scalar(FreeCell),1);
        }else{
            RasterLine_0(dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,vc);
            flg_obs_through = true;
            while(!vc.empty()){
                if(s_map.APPMap.at<uint8_t >(vc.back()) != ObsCell &&
                   s_map.APPMap.at<uint8_t >(vc.back() + Point(-1,0)) != ObsCell &&
                   s_map.APPMap.at<uint8_t >(vc.back() + Point(0,-1)) != ObsCell &&
                   s_map.APPMap.at<uint8_t >(vc.back() + Point(1,0)) != ObsCell &&
                   s_map.APPMap.at<uint8_t >(vc.back() + Point(0,1)) != ObsCell){

                }else{
                    flg_obs_through = false;
                    break;
                }
                vc.pop_back();
            }
            vc.clear();
            if(flg_obs_through){
                line(s_map.APPMap,dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,Scalar(FreeCell),1);
            }
        }
    }
    vc.clear();
    vector<Point>().swap(vc);

    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            s_map.APPMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = ObsCell;
        }
    }
#endif

    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }else if(s_map.NavMap.at<uint8_t >(j,i) == NavUnknowCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }
        }

    int sidelength = radius_obs * 2;
    bool flg_side = false;
    if(dataup.tofdata.posemin.point.x < sidelength ||
                                        dataup.tofdata.posemin.point.y < sidelength ||
                                                                         dataup.tofdata.posemax.point.x > (s_map.NavMap.cols - sidelength) ||
                                        dataup.tofdata.posemax.point.y > (s_map.NavMap.rows - sidelength)){
        flg_side = true;
    }
    Point ptmp;
    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            for(int n = 1 ; n < radius_obs + 1 ; n++)
                for(int m = 0 ; m < s_map.GetExpandSingleElement(n)->size() ; m++){
                    ptmp = Point(s_map.GetExpandSingleElement(n,m)->x + dataup.tofdata.pose2[i].point.x,s_map.GetExpandSingleElement(n,m)->y + dataup.tofdata.pose2[i].point.y);
                    if(flg_side){
                        if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                            continue;
                        }
                    }
                    uint8_t cell = s_map.NavMap.at<uint8_t >(ptmp);
                    if(cell == NavUnknowCell || cell == NavFreeCell){
                        s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
                    }
                }
            s_map.NavMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = NavObsCell;
        }
    }

    bool flg = false;
    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsExpandCell){
                flg = false;
                for (int n = 1; n < radius_obs + 1; n++)
                    for (int k = 0; k < s_map.GetExpandSingleElement(n)->size(); k++) {
                        ptmp = Point(s_map.GetExpandSingleElement(n,k)->x + i, s_map.GetExpandSingleElement(n,k)->y + j);
                        if(flg_side){
                            if (ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height) {
                                continue;
                            }
                        }
                        if(s_map.NavMap.at<uint8_t>(ptmp) == NavObsCell){
                            n = radius_obs + 1;
                            flg = true;
                            break;
                        }
                    }
                if(!flg){
                    s_map.NavMap.at<uint8_t>(j,i) = NavFreeCell;
                }
            }
        }
}
void PlanningMapManager::set_range_removedepth(int range){
	LOGPLANNING(INFO) << "set_range_removedepth range:"<< range;
	if(range < 0){
		range_removedepth = 0;
	}else if(range > 7){
		range_removedepth = 7;
	}else{
		range_removedepth = range;
	}
	LOGPLANNING(INFO) << "set_range_removedepth range_removedepth:"<< range_removedepth;
}
void PlanningMapManager::fill_appmap_source(SensorInfo & dataup , DataDown * datadown){
#if 1
    if(dataup.tofdata.flg.empty()){
        return;
    }
	//if(datadown->MotionState != FIRSTBUILDMAP && (/*fabs(datadown->cmd.vl) < 0.1f || */fabs(datadown->cmd.va) > 0.4f)){
	//	return;
	//}

    vector<Point> vc;
	vector<Point> vc_rm;
    unsigned long num_half = 0;
	bool flg = true;
	double dlt = 0;
    for(int i = 0 ; i < dataup.tofdata.flg.size() ; i++){
        if(dataup.tofdata.flg[i]){

        }else{
            RasterLine_0(dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,vc);
			for(int j = 0 ; j < vc.size() ; j ++){
				dlt = pow(vc[j].x - dataup.tofdata.pose1[i].point.x , 2) + pow(vc[j].y - dataup.tofdata.pose1[i].point.y , 2);
				if(dlt <= pow(8 , 2)){
				    if(s_map.APPMap.at<uint8_t >(vc[j]) == UnknowCell){
                        if(s_map.APPMap.at<uint8_t >(vc[j] + Point(-1,0)) != ObsCell &&
                           s_map.APPMap.at<uint8_t >(vc[j] + Point(0,-1)) != ObsCell &&
                           s_map.APPMap.at<uint8_t >(vc[j] + Point(1,0)) != ObsCell &&
                           s_map.APPMap.at<uint8_t >(vc[j] + Point(0,1)) != ObsCell){

                            s_map.APPMap.at<uint8_t >(vc[j]) = FreeCell;

                        }else{
                            break;
                        }
				    }
                }else if(s_map.APPMap.at<uint8_t >(vc[j]) == ObsCell && (i >= range_removedepth && i <= dataup.tofdata.flg.size() - range_removedepth)){
					/*if(j > 10 && j < 13){//>11
						vc_rm.emplace_back(vc[j]);
					}*/
					if(dlt > pow(10 , 2) && dlt < pow(13 , 2)){
						vc_rm.emplace_back(vc[j]);
					}
                    break;
                }else if(dlt >= pow(13 , 2)){
                    break;
				}
			}
        }
    }

    vc.clear();
    vector<Point>().swap(vc);
	for(int i = 0 ; i < vc_rm.size() ; i ++){
		s_map.APPMap.at<uint8_t >(vc_rm[i]) = FreeCell;
	}
	vc_rm.clear();
    vector<Point>().swap(vc_rm);

	for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            //if(pow(dataup.tofdata.pose2[i].point.x - dataup.tofdata.posecamera.point.x , 2) + pow(dataup.tofdata.pose2[i].point.y - dataup.tofdata.posecamera.point.y , 2) <= pow(11 , 2)){
            //}else{
                line(s_map.APPMap,dataup.tofdata.pose1[i].point,dataup.tofdata.pose2[i].point,Scalar(FreeCell),1);
            //}
        }
    }

    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
			s_map.APPMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = ObsCell;
        }
    }

    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }else if(s_map.NavMap.at<uint8_t >(j,i) == NavUnknowCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }
        }

    int sidelength = radius_obs * 2;
    bool flg_side = false;
    if(dataup.tofdata.posemin.point.x < sidelength ||
    dataup.tofdata.posemin.point.y < sidelength ||
    dataup.tofdata.posemax.point.x > (s_map.NavMap.cols - sidelength) ||
    dataup.tofdata.posemax.point.y > (s_map.NavMap.rows - sidelength)){
        flg_side = true;
    }
    Point ptmp;
    for (int i = 0 ; i < dataup.tofdata.flg.size() ; i++) {
        if(dataup.tofdata.flg[i]){
            for(int n = 1 ; n < radius_obs + 1 ; n++)
                for(int m = 0 ; m < s_map.GetExpandSingleElement(n)->size() ; m++){
                    ptmp = Point(s_map.GetExpandSingleElement(n,m)->x + dataup.tofdata.pose2[i].point.x,s_map.GetExpandSingleElement(n,m)->y + dataup.tofdata.pose2[i].point.y);
                    if(flg_side){
                        if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                            continue;
                        }
                    }
                    uint8_t cell = s_map.NavMap.at<uint8_t >(ptmp);
                    if(cell == NavUnknowCell || cell == NavFreeCell){
                        s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
                    }
                }
            s_map.NavMap.at<uint8_t >(dataup.tofdata.pose2[i].point) = NavObsCell;
        }
    }

    flg = false;
    for(int i = dataup.tofdata.posemin.point.x ; i <= dataup.tofdata.posemax.point.x ; i++)
        for(int j = dataup.tofdata.posemin.point.y ; j <= dataup.tofdata.posemax.point.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsExpandCell){
                flg = false;
                for (int n = 1; n < radius_obs + 1; n++)
                    for (int k = 0; k < s_map.GetExpandSingleElement(n)->size(); k++) {
                        ptmp = Point(s_map.GetExpandSingleElement(n,k)->x + i, s_map.GetExpandSingleElement(n,k)->y + j);
                        if(flg_side){
                            if (ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height) {
                                continue;
                            }
                        }
                        if(s_map.NavMap.at<uint8_t>(ptmp) == NavObsCell || s_map.NavMap.at<uint8_t>(ptmp) == NavSensorObsCell){
                            n = radius_obs + 1;
                            flg = true;
                            break;
                        }
                    }
                if(!flg){
                    s_map.NavMap.at<uint8_t>(j,i) = NavFreeCell;
                }
            }
        }
#endif
}
bool PlanningMapManager::fill_perception_delimit(const PerceptionDelimit & info){
#if 0
    cv::Mat img = cv::Mat::ones(100, 100, CV_8UC1)*255;
    vector<Point> pts;
    vector<vector<Point>> ppts;
    pts.emplace_back(cv::Point(50, 50));
    pts.emplace_back(cv::Point(80, 50));
    pts.emplace_back(cv::Point(80, 80));
    pts.emplace_back(cv::Point(50, 80));
    ppts.emplace_back(pts);
    fillPoly(img,ppts,cv::Scalar(0));
    ppts.clear();
    pts.clear();
    pts.emplace_back(cv::Point(5, 5));
    pts.emplace_back(cv::Point(8, 5));
    pts.emplace_back(cv::Point(8, 8));
    pts.emplace_back(cv::Point(5, 8));
    ppts.emplace_back(pts);

    fillPoly(img,ppts,cv::Scalar(100));

#endif
    vector<Point> pts;
    vector<vector<Point>> ppts;
    for(int i = 0 ; i < info.delimit.size() ; i++){
        for(int j = 0 ; j < info.delimit[i].size() ; j++){
            pts.emplace_back(info.delimit[i][j].point);
        }
        ppts.emplace_back(pts);
        fillPoly(s_map.RegionSegmentationMap,ppts,cv::Scalar(i));
		pts.clear();
        ppts.clear();
    }
    pts.clear();
    vector<Point>().swap(pts);
    ppts.clear();
    vector<vector<Point>>().swap(ppts);
    return true;
}
bool PlanningMapManager::fill_perception_obs(const perception & info , Point p_cur){
#if 0
    vector<Point> pts;
    vector<vector<Point>> ppts;
    for(int i = 0 ; i < s_map.perceptiondata.Obs.size() ; i++){
        for(int j = 0 ; j < s_map.perceptiondata.Obs[i].size() ; j++){
            pts.emplace_back(s_map.perceptiondata.Obs[i][j].point);
        }
        ppts.emplace_back(pts);
        fillPoly(s_map.RegionSegmentationMap,ppts,cv::Scalar(254));
        ppts.clear();
    }
    pts.clear();
    vector<Point>().swap(pts);
    ppts.clear();
    vector<vector<Point>>().swap(ppts);
#endif
    for(int i = s_map.originPointOccupyMin.y ; i < s_map.originPointOccupyMax.y ; i++)
        for(int j = s_map.originPointOccupyMin.x ; j < s_map.originPointOccupyMax.x ; j++){
            s_map.DeepLearningMap.at<uint8_t >(i,j) = UnknowCell;
        }
    vector<Point> pts;
    vector<vector<Point>> ppts;
//    int expand;
    LOGPLANNING(INFO) << "info.objects.size:" << info.objects.size() << " p_cur:(" << p_cur.x << " , " << p_cur.y << ")";
    for(int i = 0 ; i < info.objects.size() ; i++){
        if(info.objects[i].locationWorld.size() < 3){
            continue;
        }
        // expand = (int)((info.objects[i].detectExtend + ROBOTRADIUS)/s_map.resolving) + 1;
#if 0//膨胀在外部进行
        expand = (int)((ROBOTRADIUS)/s_map.resolving) + 1;
        LOGPLANNING(INFO) << "info.objects expand :" << expand;
        LOGPLANNING(INFO) << "info.objects " << i << ":(" << info.objects[i].locationWorld[0].point.x<< " , " <<info.objects[i].locationWorld[0].point.y <<")";
        LOGPLANNING(INFO) << "info.objects " << i << ":(" << info.objects[i].locationWorld[1].point.x<< " , " <<info.objects[i].locationWorld[1].point.y <<")";
        LOGPLANNING(INFO) << "info.objects " << i << ":(" << info.objects[i].locationWorld[2].point.x<< " , " <<info.objects[i].locationWorld[2].point.y <<")";
        LOGPLANNING(INFO) << "info.objects " << i << ":(" << info.objects[i].locationWorld[3].point.x<< " , " <<info.objects[i].locationWorld[3].point.y <<")";

        line(s_map.DeepLearningMap,info.objects[i].locationWorld[0].point,info.objects[i].locationWorld[1].point,Scalar(ObsCell),expand);
        line(s_map.DeepLearningMap,info.objects[i].locationWorld[1].point,info.objects[i].locationWorld[2].point,Scalar(ObsCell),expand);
        line(s_map.DeepLearningMap,info.objects[i].locationWorld[2].point,info.objects[i].locationWorld[3].point,Scalar(ObsCell),expand);
        line(s_map.DeepLearningMap,info.objects[i].locationWorld[3].point,info.objects[i].locationWorld[0].point,Scalar(ObsCell),expand);
#endif
        for(int j = 0 ; j < info.objects[i].locationWorld.size() ; j++){
            pts.emplace_back(info.objects[i].locationWorld[j].point);
        }
        if(CheckPoint2Area(pts , p_cur) != 3){
            LOGPLANNING(INFO) << "fill_perception_obs p_cur - (" << p_cur.x << " , " << p_cur.y << ")  is not out area";
            for(int k = 0 ; k < pts.size() ; k++){
                LOGPLANNING(INFO) << "area point:" << k << " -("<< pts[k].x << " , " << pts[k].y << ")";
            }
            pts.clear();
            continue;
        }
        ppts.emplace_back(pts);
        pts.clear();

        fillPoly(s_map.DeepLearningMap,ppts,cv::Scalar(ObsCell));
        ppts.clear();
    }

    pts.clear();
    vector<Point>().swap(pts);
    ppts.clear();
    vector<vector<Point>>().swap(ppts);

    return true;
}
void PlanningMapManager::fill_limit(Lines & lines){
//    s_map.NavMapMutex.readLock();
    Mat map_tmp = s_map.NavMap.clone();
//    s_map.NavMapMutex.readUnlock();

    for(int i = 0 ; i < s_map.RegionSegmentationMap.rows ; i++)
        for(int j = 0 ; j < s_map.RegionSegmentationMap.cols ; j++){
            map_tmp.at<uint8_t >(i,j) = map_tmp.at<uint8_t >(i,j) != NavFreeCell ? RegionUnknowCell:RegionareaCell;
        }

    for(auto &i:lines.lines){
        i.point1 = s_map.WorldToMap(i.pose1);
        i.point2 = s_map.WorldToMap(i.pose2);

        line(map_tmp, i.point1, i.point2, Scalar(RegionUnknowCell), 1);
    }

    MarkingPartition(map_tmp,s_map.RegionSegmentationMap);
    map_tmp.release();
}
void PlanningMapManager::fill_opened(Rectangles & rectangles){
#if 0
    for(int i = 0 ; i < s_map.RegionSegmentationMap.rows ; i++)
        for(int j = 0 ; j < s_map.RegionSegmentationMap.cols ; j++){
            s_map.RegionSegmentationMap.at<uint8_t >(i,j) = RegionUnknowCell;
        }

    for(int i = 0 ; i < rectangles.rectangles.size() ; i++){
        rectangles.rectangles[i].point1 = s_map.WorldToMap(rectangles.rectangles[i].pose1);
        rectangles.rectangles[i].point2 = s_map.WorldToMap(rectangles.rectangles[i].pose2);

        rectangle(s_map.RegionSegmentationMap, rectangles.rectangles[i].point1, rectangles.rectangles[i].point2, cv::Scalar(i + 1), -1, LINE_8, 0);
    }
#endif
}
void PlanningMapManager::fill_forbiden(Lines & lines , Rectangles & rectangles){
#if 1
    for(int i = 0 ; i < s_map.ForbidenMap.rows ; i++)
        for(int j = 0 ; j < s_map.ForbidenMap.cols ; j++){
            s_map.ForbidenMap.at<uint8_t >(i,j) = ForbiddenUnknowCell;
        }

    for (auto &i : lines.lines) {
        i.point1 = s_map.WorldToMap(i.pose1);
        i.point2 = s_map.WorldToMap(i.pose2);

        line(s_map.ForbidenMap, i.point1, i.point2, Scalar(ForbiddenCell), 3);
    }

    vector<Point> pts;
    vector<vector<Point>> ppts;
    for(int i = 0 ; i < rectangles.rectangles.size() ; i++){
        if(rectangles.rectangles[i].size() < 3){
            continue;
        }
        for(int j = 0 ; j < rectangles.rectangles[i].size() ; j++){
            pts.emplace_back(rectangles.rectangles[i][j].point);
        }
        ppts.emplace_back(pts);
        pts.clear();
        line(s_map.ForbidenMap, rectangles.rectangles[i][0].point, rectangles.rectangles[i][1].point, Scalar(ForbiddenCell), 3);
        line(s_map.ForbidenMap, rectangles.rectangles[i][1].point, rectangles.rectangles[i][2].point, Scalar(ForbiddenCell), 3);
        line(s_map.ForbidenMap, rectangles.rectangles[i][2].point, rectangles.rectangles[i][3].point, Scalar(ForbiddenCell), 3);
        line(s_map.ForbidenMap, rectangles.rectangles[i][3].point, rectangles.rectangles[i][0].point, Scalar(ForbiddenCell), 3);

        fillPoly(s_map.ForbidenMap,ppts,cv::Scalar(ForbiddenCell));
        ppts.clear();
    }

    pts.clear();
    vector<Point>().swap(pts);
    ppts.clear();
    vector<vector<Point>>().swap(ppts);
#endif
}

void PlanningMapManager::fill_cover(cv::Point p , BoxInfo box , int num){
    for(int n = 1 ; n < num + 1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
            cv::Point ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < box.point_min.x || ptmp.y < box.point_min.y || ptmp.x > box.point_max.x || ptmp.y > box.point_max.y){
                continue;
            }
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverIgnoreCell;
        }
    s_map.CoverMap.at<uint8_t >(p) = CoverIgnoreCell;
}
void PlanningMapManager::fill_cover_uncover(Point p , BoxInfo box , int num){
    for(int n = 1 ; n < num + 1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
            cv::Point ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < box.point_min.x || ptmp.y < box.point_min.y || ptmp.x > box.point_max.x || ptmp.y > box.point_max.y || s_map.CoverMap.at<uint8_t >(ptmp) == CoverCell){
                continue;
            }
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverCell;
        }
    if(s_map.CoverMap.at<uint8_t >(p) != CoverCell){
        s_map.CoverMap.at<uint8_t >(p) = CoverCell;
    }
}
void PlanningMapManager::fill_cover_delimit(Point p , const Mat & mark , BoxInfo box , int num){
    for(int n = 1 ; n < num + 1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
            cv::Point ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(mark.at<uint8_t >(ptmp) != box.id){
                continue;
            }
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverIgnoreCell;
        }
    s_map.CoverMap.at<uint8_t >(p) = CoverIgnoreCell;
}
void PlanningMapManager::fill_cover_delimit_uncover(Point p , const Mat & mark , BoxInfo box , int num){
    for(int n = 1 ; n < num + 1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
            cv::Point ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(mark.at<uint8_t >(ptmp) != box.id || s_map.CoverMap.at<uint8_t >(ptmp) == CoverCell){
                continue;
            }
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverCell;
        }
    if(s_map.CoverMap.at<uint8_t >(p) != CoverCell){
        s_map.CoverMap.at<uint8_t >(p) = CoverCell;
    }
}
void PlanningMapManager::fill_cover_rectangle(Point p , BoxInfo box , int type){
    Point ptmp;
    if(type == 3){
        for(int i = -1 ; i < 2 ; i++)
            for(int j = -1 ; j < 2 ; j++){
                ptmp = p + Point(i,j);
                if(ptmp.x < box.point_min.x || ptmp.y < box.point_min.y || ptmp.x > box.point_max.x || ptmp.y > box.point_max.y){
                    continue;
                }
                if(s_map.CoverMap.at<uint8_t >(ptmp) == CoverUnknowCell){
                    s_map.CoverMap.at<uint8_t >(ptmp) = CoverIgnoreCell;
                }

            }
    }
}
void PlanningMapManager::fill_cover_delimit_rectangle(Point p , const Mat & mark , BoxInfo box , int type){
    Point ptmp;
    if(type == 3){
        for(int i = -1 ; i < 2 ; i++)
            for(int j = -1 ; j < 2 ; j++){
                ptmp = p + Point(i,j);
                if(mark.at<uint8_t >(ptmp) != box.id){
                    continue;
                }
                s_map.CoverMap.at<uint8_t >(ptmp) = CoverIgnoreCell;
            }
    }
}

void PlanningMapManager::fill_sensorobs(Point2f p){
    Point p_2 = s_map.WorldToMap(p);
//    if(s_map.NavMap.at<uint8_t >(p_2) != NavObsCell){
        for(int n = 1 ; n < radius_obs + 1 ; n++)
            for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
                Point ptmp = Point(s_map.GetExpandSingleElement(n,i)->x + p_2.x,s_map.GetExpandSingleElement(n,i)->y + p_2.y);
                if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                    continue;
                }
                s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
            }
        s_map.NavMap.at<uint8_t >(p_2) = NavSensorObsCell;
//    }
}

void PlanningMapManager::fill_cliff(Point2f p , float dir , int flg){
    Point2f ptmp;
    if(flg == 0){
        ptmp.x = OFFSETRADIUS_CLIFF_L * cosf(dir + OFFSETANGLE_CLIFF_L);
        ptmp.y = OFFSETRADIUS_CLIFF_L * sinf(dir + OFFSETANGLE_CLIFF_L);
    }else if(flg == 1){
        ptmp.x = OFFSETRADIUS_CLIFF_LF * cosf(dir + OFFSETANGLE_CLIFF_LF);
        ptmp.y = OFFSETRADIUS_CLIFF_LF * sinf(dir + OFFSETANGLE_CLIFF_LF);
    }else if(flg == 2){
        ptmp.x = OFFSETRADIUS_CLIFF_R * cosf(dir + OFFSETANGLE_CLIFF_R);
        ptmp.y = OFFSETRADIUS_CLIFF_R * sinf(dir + OFFSETANGLE_CLIFF_R);
    }else if(flg == 3){
        ptmp.x = OFFSETRADIUS_CLIFF_RF * cosf(dir + OFFSETANGLE_CLIFF_RF);
        ptmp.y = OFFSETRADIUS_CLIFF_RF * sinf(dir + OFFSETANGLE_CLIFF_RF);
    }
    ptmp = p + ptmp;
//    fill_navobs(ptmp);
    fill_sensorobs(ptmp);
}
void PlanningMapManager::fill_ir(PoseStamped p , float dir , int flg , float l){
    PoseStamped p_e;
    float dis = l < 0 ? (ROBOTRADIUS + MAXVALID_FOLLOW - OFFSETDEPTH_FOLLOW):(ROBOTRADIUS + l - OFFSETDEPTH_FOLLOW);
    if(flg == 0){
        p_e.pose.x = dis * cosf(dir + OFFSETANGLE_FOLLOW_L);
        p_e.pose.y = dis * sinf(dir + OFFSETANGLE_FOLLOW_L);
    }else if(flg == 1){
        p_e.pose.x = dis * cosf(dir + OFFSETANGLE_FOLLOW_R);
        p_e.pose.y = dis * sinf(dir + OFFSETANGLE_FOLLOW_R);
    }
    p_e.pose = p.pose + p_e.pose;
    p_e.point = s_map.WorldToMap(p_e.pose);

    //line(s_map.APPMap,p.point,p_e.point,Scalar(FreeCell),1);
	vector<Point> vc;
	RasterLine_0(p.point,p_e.point,vc);
	while(!vc.empty()){
		if(s_map.APPMap.at<uint8_t >(vc.back()) == UnknowCell){
			s_map.APPMap.at<uint8_t >(vc.back()) = FreeCell;
		}
		vc.pop_back();
	}
	vector<Point>().swap(vc);
    /*if(l > 0.0f){
        s_map.APPMap.at<uint8_t >(p_e.point) = ObsCell;
    }*/

    Point p_min,p_max;
    p_min.x = min(p.point.x,p_e.point.x);
    p_min.y = min(p.point.y,p_e.point.y);
    p_min = p_min - Point(4,4);
    p_max.x = max(p.point.x,p_e.point.x);
    p_max.y = max(p.point.y,p_e.point.y);
    p_max = p_max + Point(4,4);

    if(p_min.x < 0){
        p_min.x = 0;
    }
    if(p_min.y < 0){
        p_min.y = 0;
    }
    if(p_max.x >= s_map.width){
        p_max.x = s_map.width - 1;
    }
    if(p_max.y >= s_map.height){
        p_max.y = s_map.height - 1;
    }

    for(int i = p_min.x ; i <= p_max.x ; i++)
        for(int j = p_min.y ; j <= p_max.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }else if(s_map.NavMap.at<uint8_t >(j,i) == NavUnknowCell){
                if(s_map.APPMap.at<uint8_t >(j,i) == FreeCell){
                    s_map.NavMap.at<uint8_t >(j,i) = NavFreeCell;
                }
            }
        }

    int sidelength = radius_obs * 2;
    bool flg_side = false;
    if(p_min.x < sidelength || p_min.y < sidelength ||
        p_max.x > (s_map.NavMap.cols - sidelength) || p_max.y > (s_map.NavMap.rows - sidelength)){

        flg_side = true;
    }

    Point ptmp;
#if 0
    if(l > 0.0f){
        for(int n = 1 ; n < radius_obs + 1 ; n++)
            for(int m = 0 ; m < s_map.GetExpandSingleElement(n)->size() ; m++){
                ptmp = Point(s_map.GetExpandSingleElement(n,m)->x + p_e.point.x,s_map.GetExpandSingleElement(n,m)->y + p_e.point.y);
                if(flg_side){
                    if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                        continue;
                    }
                }
                uint8_t cell = s_map.NavMap.at<uint8_t >(ptmp);
                if(cell == NavUnknowCell || cell == NavFreeCell){
                    s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
                }
            }
        s_map.NavMap.at<uint8_t >(p_e.point) = NavObsCell;
    }
#endif
    bool sig = false;
    for(int i = p_min.x ; i <= p_min.x ; i++)
        for(int j = p_max.y ; j <= p_max.y ; j++){
            if(s_map.NavMap.at<uint8_t >(j,i) == NavObsExpandCell){
                sig = false;
                for (int n = 1; n < radius_obs + 1; n++)
                    for (int k = 0; k < s_map.GetExpandSingleElement(n)->size(); k++) {
                        ptmp = Point(s_map.GetExpandSingleElement(n,k)->x + i, s_map.GetExpandSingleElement(n,k)->y + j);
                        if(flg_side){
                            if (ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height) {
                                continue;
                            }
                        }
                        if(s_map.NavMap.at<uint8_t>(ptmp) == NavObsCell){
                            n = radius_obs + 1;
                            sig = true;
                            break;
                        }
                    }
                if(!sig){
                    s_map.NavMap.at<uint8_t>(j,i) = NavFreeCell;
                }
            }
        }
}

void PlanningMapManager::fill_navobs(Point2f p){
    Point p_2 = s_map.WorldToMap(p);
    for(int n = 1 ; n < radius_obs + 1 ; n++)
    for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
        Point ptmp = Point(s_map.GetExpandSingleElement(n,i)->x + p_2.x,s_map.GetExpandSingleElement(n,i)->y + p_2.y);
        if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
            continue;
        }
        s_map.NavMap.at<uint8_t >(ptmp) = NavSensorObsCell;
    }
    s_map.NavMap.at<uint8_t >(p_2) = NavSensorObsCell;
}

void PlanningMapManager::fill_explor_map(Mat & src , Point p , int num , uint8_t setcell){
    uint8_t cell = 0;
    Point ptmp;
    for(int n = 0 ; n < num -1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
            ptmp = Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                continue;
            }
            cell = src.at<uint8_t >(ptmp);
            if(cell != setcell){
                src.at<uint8_t >(ptmp) = setcell;
            }
        }
    if(src.at<uint8_t >(p) != setcell){
        src.at<uint8_t >(p) = setcell;
    }
}
bool PlanningMapManager::raster_Line(Point p_s , Point p_e , vector<Point> &list){
    list.clear();
    Vector2d O(p_s.x, p_s.y);
    Vector2d E(p_e.x, p_e.y);
    std::pair<Vector2d, Vector2d> line(O, E);
    vector<Vector2d> linePointList;

    RasterLine(line, linePointList);
    if(linePointList.empty()){
    return false;
    }
    for (auto &i : linePointList) {
    list.emplace_back(Point((int) i.x,(int) i.y));
    }
    vector<Vector2d>().swap(linePointList);
    return true;
}
void PlanningMapManager::RasterLine(std::pair<Vector2d, Vector2d> line, std::vector<Vector2d>& linePointList){
    Vector2d vecLine = line.second - line.first;
    double lineLength = vecLine.Mod();
    double step = 1.0;

    //根据距离逐步取
    vector<Vector2d> tmpPointList;
    double curLength = 0;
    while (curLength < lineLength)
    {
        curLength = curLength + step;
        Vector2d P = line.first + vecLine.Scalar(curLength / lineLength);
        P.x = lround(P.x + 0.5);
        P.y = lround(P.y + 0.5);
        tmpPointList.push_back(P);
    }

    //与最后一个值比较，去重
    linePointList.push_back(line.first);
    for (auto &i : tmpPointList) {
        //与最后一个值比较，去重
        if (!i.Equel(linePointList[linePointList.size() - 1]))
        {
            linePointList.push_back(i);
        }
    }

    if (!linePointList[linePointList.size() - 1].Equel(line.second))
    {
        linePointList.push_back(line.second);
    }
}
bool PlanningMapManager::RasterLine_0(Point start , Point end , vector<Point>& linePointList){
    linePointList.clear();
    int dlt_x = start.x - end.x;
    int dlt_y = start.y - end.y;
//    printf("s(%d,%d) , e(%d,%d)\n",start.x,start.y,end.x,end.y);
//    printf("dlt_x:%d , dlt_y:%d\n",dlt_x,dlt_y);
    int i_min = 0;
    int i_max = 0;
    if(dlt_x == 0){
//        printf("1\n");
        i_min = min(start.y,end.y);
        i_max = max(start.y,end.y);
        for(int i = i_min ; i <= i_max ; i++){
            linePointList.emplace_back(Point(start.x,i));
        }
    }else if(dlt_y == 0){
//        printf("2\n");
        i_min = min(start.x,end.x);
        i_max = max(start.x,end.x);
        for(int i = i_min ; i <= i_max ; i++){
            linePointList.emplace_back(Point(i,start.y));
        }
    }else if(abs(dlt_y) == abs(dlt_x)){
        i_min = min(start.x,end.x);
        i_max = max(start.x,end.x);
        int a = ((dlt_y > 0 && dlt_x > 0) || (dlt_y < 0 && dlt_x < 0)) ? 1:-1;
        int b= (start.y - a*start.x);
//            printf("3\n");
        for (int i = i_min; i <= i_max; i++) {
            linePointList.emplace_back(Point(i, (i * a + b)));
        }
    }else if(abs(dlt_y) > abs(dlt_x)){
        i_min = start.y;
        i_max = end.y;
//        printf("i_min:%d , i_max:%d\n",i_min,i_max);
        float a = (float)dlt_y/dlt_x;
        float b= (start.y - a*start.x);
        if(dlt_y > 0){
//            printf("5\n");
            for(int i = i_min ; i >= i_max ; i--){
                linePointList.emplace_back(Point((int)ceil((i - 0.5 - b)/a),i));
//                printf("(%d,%d)\n",(int)ceil((i - 0.5 - b)/a),i);
            }
        }else{
//            printf("6\n");
            for(int i = i_min ; i <= i_max ; i++){
                linePointList.emplace_back(Point((int)floor((i + 0.5 - b)/a),i));
//                printf("(%d,%d)\n",(int)floor((i + 0.5 - b)/a),i);
            }
        }
    }else{
        i_min = start.x;
        i_max = end.x;
//        printf("i_min:%d , i_max:%d\n",i_min,i_max);
        float a = (float)dlt_y/dlt_x;
        float b= (start.y - a*start.x);
        if(dlt_x > 0){
//            printf("7\n");
            for(int i = i_min ; i >= i_max ; i--){
                linePointList.emplace_back(Point(i,(int)ceil(a*(i - 0.5) + b)));
//                printf("(%d,%d)\n",i,(int)ceil(a*(i - 0.5) + b));
            }
        }else{
//            printf("8\n");
            for(int i = i_min ; i <= i_max ; i++){
                linePointList.emplace_back(Point(i,(int)floor(a*(i + 0.5) + b)));
//                printf("(%d,%d)\n",i,(int)floor(a*(i + 0.5) + b));
            }
        }
    }
	if(linePointList.front() == end && linePointList.back() == start){
		reverse(linePointList.begin() , linePointList.end());
	}
    return true;
}
#endif