//
// Created by lcy on 22-4-26.
//

#ifndef RUBBYDECISION_MAP_H
#define RUBBYDECISION_MAP_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include "RWlock.h"
#include "Common.h"
#include "Singleton.h"
#include  "config.h"

class Map{
public:
    Map(){
        // init();
    };
    ~Map(){
#ifdef ROSDEBUG
        kill_thread_map();
#endif
        datarelease();
        if(T_map != nullptr){
            T_map = nullptr;
        }
    };

    void init();
    void datarelease();

    int width = 1200;//cols
    int height = 1200;//rows
    float resolving = 0.05;//导航世界地图分辨率
    cv::Point2f origin = cv::Point2f(-30.0f,-30.0f);

#ifdef SLAMTEST
    int widthOccupy = width;//cols
    int heightOccupy = height;//rows
    cv::Point originPointOccupyMin = cv::Point(0,0);
    cv::Point originPointOccupyMax = cv::Point(width - 1,height - 1);
#else
    int widthOccupy = 0;//cols
    int heightOccupy = 0;//rows
    cv::Point originPointOccupyMin = cv::Point(width - 1,height - 1);
    cv::Point originPointOccupyMax = cv::Point(0,0);
#endif

    cv::Point2f originPoseOccupy = cv::Point2f(-30.0f,-30.0f);

    int width2slam = 40;//cols 发送给slam的增量地图 ,用于美化地图
    int height2slam = 40;//rows　发送给slam的增量地图 ,用于美化地图

    cv::Mat NavMap;//导航地图

    cv::Mat CoverMap;//清扫覆盖图

    cv::Mat PathMap;//路径图

    cv::Mat ForbidenMap;//禁区地图

    cv::Mat  RegionSegmentationMap;//区域分割图

    APPInfo appdata;
    PerceptionDelimit perceptiondata;

    cv::Mat APPMap;//app显示地图地图（由slam美化后）

    cv::Mat FollowMap;//区域沿边,障碍物沿边
//    std::vector<uint8_t > Followobsidarray;
    uint8_t FollowObSidId = 0;

    cv::Mat CleanBoxMap;//清扫框地图

    cv::Mat DeepLearningMap;//语义地图

    cv::Point2f MapToWorld(cv::Point m) {
        return cv::Point2f((origin.x + (m.x + 0.5f) * resolving),(origin.y + (m.y + 0.5f) * resolving));
    }

    cv::Point WorldToMap(cv::Point2f w) {
        return cv::Point((int)((w.x - origin.x) / resolving),(int)((w.y - origin.y) / resolving));
    }

    vector<vector<cv::Point>> * GetExpandElement();
    vector<cv::Point> * GetExpandSingleElement(int id);
    const cv::Point * GetExpandSingleElement(int id , int num);
#ifdef ROSDEBUG
    int creat_thread_map();
    int kill_thread_map();

#endif
private:
    std::thread *T_map = nullptr;
    bool flg_thread_map = true;

#ifdef ROSDEBUG
    bool flg_showNavMap = false;
    bool flg_showForbidenMap = false;
    bool flg_showRegionSegmentationMap = false;
    bool flg_showCoverMap = false;
    bool flg_showFollowMap = false;
    bool flg_showPathMap = false;
    bool flg_showAPPMap = false;
    bool flg_showPerceptionMap = false;
#endif
    int flg_onebox = 0;
};
#define s_map Singleton<Map>::GetInstance()

#endif //RUBBYDECISION_MAP_H
