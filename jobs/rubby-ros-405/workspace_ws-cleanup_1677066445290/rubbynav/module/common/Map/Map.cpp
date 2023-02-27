//
// Created by lcy on 22-4-26.
//

#include "Map.h"

void Map::init(){

    flg_onebox = s_config.readParam<int>("boxNumber");

    if(flg_onebox == 1){
        width = 600;//cols
        height = 600;//rows
        origin = cv::Point2f(-15.0f,-15.0f);
    }

    width2slam = s_config.readParam<int>("width2slam");
    height2slam = s_config.readParam<int>("height2slam");

    if(!NavMap.empty()){
        NavMap.release();
    }
    NavMap = cv::Mat::ones(height,width,CV_8UC1)*255;

    if(!APPMap.empty()){
        APPMap.release();
    }
    APPMap = cv::Mat::ones(height,width,CV_8UC1)*255;

    if(!PathMap.empty()){
        PathMap.release();
    }
    PathMap = cv::Mat::ones(height,width,CV_8UC1)*255;

    if(!CoverMap.empty()){
        CoverMap.release();
    }
    CoverMap = cv::Mat::ones(height,width,CV_8UC1)*255;

    if(!FollowMap.empty()){
        FollowMap.release();
    }
    FollowMap = cv::Mat::ones(height,width,CV_8UC1)*255;
    FollowObSidId = 0;

    if(!ForbidenMap.empty()){
        ForbidenMap.release();
    }
    ForbidenMap = cv::Mat::ones(height,width,CV_8UC1)*255;

    if(!RegionSegmentationMap.empty()){
        RegionSegmentationMap.release();
    }
    RegionSegmentationMap = cv::Mat::ones(height,width,CV_8UC1)*255;

    if(!CleanBoxMap.empty()){
        CleanBoxMap.release();
    }
    CleanBoxMap.create(height,width,CV_8UC3);

    if(!DeepLearningMap.empty()){
        DeepLearningMap.release();
    }
    DeepLearningMap = cv::Mat::ones(height,width,CV_8UC1)*255;

    #ifdef ROSDEBUG
        flg_showNavMap = s_config.readParam<int>("showNavMap") > 0;
        flg_showForbidenMap = s_config.readParam<int>("showForbidenMap") > 0;
        flg_showRegionSegmentationMap = s_config.readParam<int>("showRegionSegmentationMap") > 0;
        flg_showCoverMap = s_config.readParam<int>("showCoverMap") > 0;
        flg_showFollowMap = s_config.readParam<int>("showFollowMap") > 0;
        flg_showPathMap = s_config.readParam<int>("showPathMap") > 0;
        flg_showAPPMap = s_config.readParam<int>("showAPPMap") > 0;
        flg_showPerceptionMap = s_config.readParam<int>("showPerceptionMap") > 0;
    #endif

    return;
}
void Map::datarelease(){
    if(!NavMap.empty()){
        NavMap.release();
    }

    if(!APPMap.empty()){
        APPMap.release();
    }

    if(!PathMap.empty()){
        PathMap.release();
    }

    if(!CoverMap.empty()){
        CoverMap.release();
    }

    if(!FollowMap.empty()){
        FollowMap.release();
    }

    if(!ForbidenMap.empty()){
        ForbidenMap.release();
    }

    if(!RegionSegmentationMap.empty()){
        RegionSegmentationMap.release();
    }

    if(!CleanBoxMap.empty()){
        CleanBoxMap.release();
    }

    if(!DeepLearningMap.empty()){
        DeepLearningMap.release();
    }

//    if(!Followobsidarray.empty()){
//        Followobsidarray.clear();
//        std::vector<uint8_t >().swap(Followobsidarray);
//    }
    FollowObSidId = 0;

    appdata.release();
    perceptiondata.release();
    return;
}

#if 1
vector<cv::Point> expand_1 = {
        cv::Point(0,-1),cv::Point(-1,0),cv::Point(0,1),cv::Point(1,0)
};
vector<cv::Point> expand_2 = {
        cv::Point(0,-2),cv::Point(-1,-1),cv::Point(1,-1),cv::Point(-2,0),cv::Point(2,0),cv::Point(-1,1),cv::Point(1,1),cv::Point(0,2)
};
vector<cv::Point> expand_3 = {
        cv::Point(0,-3),cv::Point(-2,-2),cv::Point(-1,-2),cv::Point(1,-2),cv::Point(2,-2),cv::Point(-2,-1),cv::Point(2,-1),cv::Point(-3,0),cv::Point(3,0),cv::Point(-2,1),cv::Point(2,1),cv::Point(-2,2),cv::Point(-1,2),cv::Point(1,2),cv::Point(2,2),cv::Point(0,3)
};
vector<cv::Point> expand_4 = {
        cv::Point(0,-4),cv::Point(-2,-3),cv::Point(-1,-3),cv::Point(1,-3),cv::Point(2,-3),cv::Point(-3,-2),cv::Point(3,-2),cv::Point(-3,-1),cv::Point(3,-1),cv::Point(-4,0),cv::Point(4,0),cv::Point(-3,1),cv::Point(3,1),cv::Point(-3,2),cv::Point(3,2),cv::Point(-2,3),
        cv::Point(-1,3),cv::Point(1,3),cv::Point(2,3),cv::Point(0,4)
};
vector<cv::Point> expand_5 = {
        cv::Point(0,-5),cv::Point(-3,-4),cv::Point(-2,-4),cv::Point(-1,-4),cv::Point(1,-4),cv::Point(2,-4),cv::Point(3,-4),cv::Point(-4,-3),cv::Point(-3,-3),cv::Point(3,-3),cv::Point(4,-3),cv::Point(-4,-2),cv::Point(4,-2),cv::Point(-4,-1),cv::Point(4,-1),cv::Point(-5,0),
        cv::Point(5,0),cv::Point(-4,1),cv::Point(4,1),cv::Point(-4,2),cv::Point(4,2),cv::Point(-4,3),cv::Point(-3,3),cv::Point(3,3),cv::Point(4,3),cv::Point(-3,4),cv::Point(-2,4),cv::Point(-1,4),cv::Point(1,4),cv::Point(2,4),cv::Point(3,4),cv::Point(0,5)
};
vector<cv::Point> expand_6 = {
        cv::Point(0,-6),cv::Point(-3,-5),cv::Point(-2,-5),cv::Point(-1,-5),cv::Point(1,-5),cv::Point(2,-5),cv::Point(3,-5),cv::Point(-4,-4),cv::Point(4,-4),cv::Point(-5,-3),cv::Point(5,-3),cv::Point(-5,-2),cv::Point(5,-2),cv::Point(-5,-1),cv::Point(5,-1),cv::Point(-6,0),
        cv::Point(6,0),cv::Point(-5,1),cv::Point(5,1),cv::Point(-5,2),cv::Point(5,2),cv::Point(-5,3),cv::Point(5,3),cv::Point(-4,4),cv::Point(4,4),cv::Point(-3,5),cv::Point(-2,5),cv::Point(-1,5),cv::Point(1,5),cv::Point(2,5),cv::Point(3,5),cv::Point(0,6)
};
vector<cv::Point> expand_7 = {
        cv::Point(0,-7),cv::Point(-3,-6),cv::Point(-2,-6),cv::Point(-1,-6),cv::Point(1,-6),cv::Point(2,-6),cv::Point(3,-6),cv::Point(-4,-5),cv::Point(4,-5),cv::Point(-5,-4),cv::Point(5,-4),cv::Point(-6,-3),cv::Point(6,-3),cv::Point(-6,-2),cv::Point(6,-2),cv::Point(-6,-1),
        cv::Point(6,-1),cv::Point(-7,0),cv::Point(7,0),cv::Point(-6,1),cv::Point(6,1),cv::Point(-6,2),cv::Point(6,2),cv::Point(-6,3),cv::Point(6,3),cv::Point(-5,4),cv::Point(5,4),cv::Point(-4,5),cv::Point(4,5),cv::Point(-3,6),cv::Point(-2,6),cv::Point(-1,6),cv::Point(1,6),
        cv::Point(2,6),cv::Point(3,6),cv::Point(0,7)
};
vector<cv::Point> expand_8 = {
        cv::Point(0,-8),cv::Point(-3,-7),cv::Point(-2,-7),cv::Point(-1,-7),cv::Point(1,-7),cv::Point(2,-7),cv::Point(3,-7),cv::Point(-5,-6),cv::Point(-4,-6),cv::Point(4,-6),cv::Point(5,-6),cv::Point(-6,-5),cv::Point(-5,-5),cv::Point(5,-5),cv::Point(6,-5),cv::Point(-6,-4),
        cv::Point(6,-4),cv::Point(-7,-3),cv::Point(7,-3),cv::Point(-7,-2),cv::Point(7,-2),cv::Point(-7,-1),cv::Point(7,-1),cv::Point(-8,0),cv::Point(8,0),cv::Point(-7,1),cv::Point(7,1),cv::Point(-7,2),cv::Point(7,2),cv::Point(-7,3),cv::Point(7,3),cv::Point(-6,4),cv::Point(6,4),
        cv::Point(-6,5),cv::Point(-5,5),cv::Point(5,5),cv::Point(6,5),cv::Point(-5,6),cv::Point(-4,6),cv::Point(4,6),cv::Point(5,6),cv::Point(-3,7),cv::Point(-2,7),cv::Point(-1,7),cv::Point(1,7),cv::Point(2,7),cv::Point(3,7),cv::Point(0,8)
};
vector<cv::Point> expand_9 = {
        cv::Point(0,-9),cv::Point(-4,-8),cv::Point(-3,-8),cv::Point(-2,-8),cv::Point(-1,-8),cv::Point(1,-8),cv::Point(2,-8),cv::Point(3,-8),cv::Point(4,-8),cv::Point(-5,-7),cv::Point(-4,-7),cv::Point(4,-7),cv::Point(5,-7),cv::Point(-6,-6),cv::Point(6,-6),cv::Point(-7,-5),
        cv::Point(7,-5),cv::Point(-8,-4),cv::Point(-7,-4),cv::Point(7,-4),cv::Point(8,-4),cv::Point(-8,-3),cv::Point(8,-3),cv::Point(-8,-2),cv::Point(8,-2),cv::Point(-8,-1),cv::Point(8,-1),cv::Point(-9,0),cv::Point(9,0),cv::Point(-8,1),cv::Point(8,1),cv::Point(-8,2),cv::Point(8,2),
        cv::Point(-8,3),cv::Point(8,3),cv::Point(-8,4),cv::Point(-7,4),cv::Point(7,4),cv::Point(8,4),cv::Point(-7,5),cv::Point(7,5),cv::Point(-6,6),cv::Point(6,6),cv::Point(-5,7),cv::Point(-4,7),cv::Point(4,7),cv::Point(5,7),cv::Point(-4,8),cv::Point(-3,8),cv::Point(-2,8),cv::Point(-1,8),
        cv::Point(1,8),cv::Point(2,8),cv::Point(3,8),cv::Point(4,8),cv::Point(0,9)
};
vector<cv::Point> expand_10 = {
        cv::Point(0,-10),cv::Point(-4,-9),cv::Point(-3,-9),cv::Point(-2,-9),cv::Point(-1,-9),cv::Point(1,-9),cv::Point(2,-9),cv::Point(3,-9),cv::Point(4,-9),cv::Point(-6,-8),cv::Point(-5,-8),cv::Point(5,-8),cv::Point(6,-8),cv::Point(-7,-7),cv::Point(-6,-7),cv::Point(6,-7),
        cv::Point(7,-7),cv::Point(-8,-6),cv::Point(-7,-6),cv::Point(7,-6),cv::Point(8,-6),cv::Point(-8,-5),cv::Point(8,-5),cv::Point(-9,-4),cv::Point(9,-4),cv::Point(-9,-3),cv::Point(9,-3),cv::Point(-9,-2),cv::Point(9,-2),cv::Point(-9,-1),cv::Point(9,-1),cv::Point(-10,0),
        cv::Point(10,0),cv::Point(-9,1),cv::Point(9,1),cv::Point(-9,2),cv::Point(9,2),cv::Point(-9,3),cv::Point(9,3),cv::Point(-9,4),cv::Point(9,4),cv::Point(-8,5),cv::Point(8,5),cv::Point(-8,6),cv::Point(-7,6),cv::Point(7,6),cv::Point(8,6),cv::Point(-7,7),cv::Point(-6,7),cv::Point(6,7),
        cv::Point(7,7),cv::Point(-6,8),cv::Point(-5,8),cv::Point(5,8),cv::Point(6,8),cv::Point(-4,9),cv::Point(-3,9),cv::Point(-2,9),cv::Point(-1,9),cv::Point(1,9),cv::Point(2,9),cv::Point(3,9),cv::Point(4,9),cv::Point(0,10)
};

vector<vector<cv::Point>> mapexpand = {expand_1,expand_2,expand_3,expand_4,expand_5,expand_6,expand_7,expand_8,expand_9,expand_10};
vector<vector<cv::Point>> * Map::GetExpandElement(){
    return &mapexpand;
}
vector<cv::Point> * Map::GetExpandSingleElement(int id){
    vector<cv::Point> * tmp = nullptr;
    if(id < 1 || id > 10){
        return tmp;
    }
    switch (id){
        case 1:tmp = &expand_1;
            break;
        case 2:tmp = &expand_2;
            break;
        case 3:tmp = &expand_3;
            break;
        case 4:tmp = &expand_4;
            break;
        case 5:tmp = &expand_5;
            break;
        case 6:tmp = &expand_6;
            break;
        case 7:tmp = &expand_7;
            break;
        case 8:tmp = &expand_8;
            break;
        case 9:tmp = &expand_9;
            break;
        case 10:tmp = &expand_10;
            break;
        default:tmp = nullptr;
    }
    return tmp;
}
const cv::Point * Map::GetExpandSingleElement(int id , int num){
    cv::Point * tmp = nullptr;
    if(id < 1 || id > 10){
        return tmp;
    }
    switch (id){
        case 1:tmp = (num < 0 || num >= expand_1.size()) ? nullptr:&expand_1[num];
            break;
        case 2:tmp = (num < 0 || num >= expand_2.size()) ? nullptr:&expand_2[num];
            break;
        case 3:tmp = (num < 0 || num >= expand_3.size()) ? nullptr:&expand_3[num];
            break;
        case 4:tmp = (num < 0 || num >= expand_4.size()) ? nullptr:&expand_4[num];
            break;
        case 5:tmp = (num < 0 || num >= expand_5.size()) ? nullptr:&expand_5[num];
            break;
        case 6:tmp = (num < 0 || num >= expand_6.size()) ? nullptr:&expand_6[num];
            break;
        case 7:tmp = (num < 0 || num >= expand_7.size()) ? nullptr:&expand_7[num];
            break;
        case 8:tmp = (num < 0 || num >= expand_8.size()) ? nullptr:&expand_8[num];
            break;
        case 9:tmp = (num < 0 || num >= expand_9.size()) ? nullptr:&expand_9[num];
            break;
        case 10:tmp = (num < 0 || num >= expand_10.size()) ? nullptr:&expand_10[num];
            break;
        default:tmp = nullptr;
    }
    return tmp;
}

#ifdef ROSDEBUG

#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <sys/syscall.h>

int Map::creat_thread_map(){
    if(T_map != nullptr) {
        LOGCOMMON(INFO) << "T_planning is nullptr !!!";
        return 0;
    }

    T_map = new thread([this](){
//        int argc;
//        char **argv;
//        ros::init (argc, argv, "mappublish");
        LOGCOMMON(INFO) << "Enter T_map , pid: " << getpid() << ", tid: " << syscall(SYS_gettid);
        LOGCOMMON(INFO) << "this thread id is: " << std::this_thread::get_id();
        ros::NodeHandle nh;
        ros::Publisher pub_nav = nh.advertise<nav_msgs::OccupancyGrid> ("nav_map", 1);
        nav_msgs::OccupancyGrid navMap;
        navMap.info.resolution = resolving;
        navMap.info.width = (unsigned int)width;
        navMap.info.height = (unsigned int)height;
        navMap.info.origin.position.x = origin.x;
        navMap.info.origin.position.y = origin.y;
        navMap.info.origin.position.z = 0;
        navMap.info.origin.orientation.x = 0;
        navMap.info.origin.orientation.y = 0;
        navMap.info.origin.orientation.z = 0;
        navMap.header.frame_id = "odom";

        ros::Publisher pub_forbiden = nh.advertise<nav_msgs::OccupancyGrid> ("forbiden_map", 1);
        nav_msgs::OccupancyGrid forbidenMap;
        forbidenMap.info.resolution = resolving;
        forbidenMap.info.width = (unsigned int)width;
        forbidenMap.info.height = (unsigned int)height;
        forbidenMap.info.origin.position.x = origin.x;
        forbidenMap.info.origin.position.y = origin.y;
        forbidenMap.info.origin.position.z = 0;
        forbidenMap.info.origin.orientation.x = 0;
        forbidenMap.info.origin.orientation.y = 0;
        forbidenMap.info.origin.orientation.z = 0;
        forbidenMap.header.frame_id = "odom";

        ros::Publisher pub_region = nh.advertise<nav_msgs::OccupancyGrid> ("region_map", 1);
        nav_msgs::OccupancyGrid regionMap;
        regionMap.info.resolution = resolving;
        regionMap.info.width = (unsigned int)width;
        regionMap.info.height = (unsigned int)height;
        regionMap.info.origin.position.x = origin.x;
        regionMap.info.origin.position.y = origin.y;
        regionMap.info.origin.position.z = 0;
        regionMap.info.origin.orientation.x = 0;
        regionMap.info.origin.orientation.y = 0;
        regionMap.info.origin.orientation.z = 0;
        regionMap.header.frame_id = "odom";

        ros::Publisher pub_cov = nh.advertise<nav_msgs::OccupancyGrid> ("cov_map", 1);
        nav_msgs::OccupancyGrid covMap;
        covMap.info.resolution = resolving;
        covMap.info.width = (unsigned int)width;
        covMap.info.height = (unsigned int)height;
        covMap.info.origin.position.x = origin.x;
        covMap.info.origin.position.y = origin.y;
        covMap.info.origin.position.z = 0;
        covMap.info.origin.orientation.x = 0;
        covMap.info.origin.orientation.y = 0;
        covMap.info.origin.orientation.z = 0;
        covMap.header.frame_id = "odom";

        ros::Publisher pub_follow = nh.advertise<nav_msgs::OccupancyGrid> ("follow_map", 1);
        nav_msgs::OccupancyGrid followMap;
        followMap.info.resolution = resolving;
        followMap.info.width = (unsigned int)width;
        followMap.info.height = (unsigned int)height;
        followMap.info.origin.position.x = origin.x;
        followMap.info.origin.position.y = origin.y;
        followMap.info.origin.position.z = 0;
        followMap.info.origin.orientation.x = 0;
        followMap.info.origin.orientation.y = 0;
        followMap.info.origin.orientation.z = 0;
        followMap.header.frame_id = "odom";

        ros::Publisher pub_path = nh.advertise<nav_msgs::OccupancyGrid> ("path_map", 1);
        nav_msgs::OccupancyGrid pathMap;
        pathMap.info.resolution = resolving;
        pathMap.info.width = (unsigned int)width;
        pathMap.info.height = (unsigned int)height;
        pathMap.info.origin.position.x = origin.x;
        pathMap.info.origin.position.y = origin.y;
        pathMap.info.origin.position.z = 0;
        pathMap.info.origin.orientation.x = 0;
        pathMap.info.origin.orientation.y = 0;
        pathMap.info.origin.orientation.z = 0;
        pathMap.header.frame_id = "odom";

        ros::Publisher pub_app = nh.advertise<nav_msgs::OccupancyGrid> ("app_map", 1);
        nav_msgs::OccupancyGrid appMap;
        appMap.info.resolution = resolving;
        appMap.info.width = (unsigned int)width;
        appMap.info.height = (unsigned int)height;
        appMap.info.origin.position.x = origin.x;
        appMap.info.origin.position.y = origin.y;
        appMap.info.origin.position.z = 0;
        appMap.info.origin.orientation.x = 0;
        appMap.info.origin.orientation.y = 0;
        appMap.info.origin.orientation.z = 0;
        appMap.header.frame_id = "odom";

        ros::Publisher pub_perception = nh.advertise<nav_msgs::OccupancyGrid> ("perception_map", 1);
        nav_msgs::OccupancyGrid perceptionMap;
        perceptionMap.info.resolution = resolving;
        perceptionMap.info.width = (unsigned int)width;
        perceptionMap.info.height = (unsigned int)height;
        perceptionMap.info.origin.position.x = origin.x;
        perceptionMap.info.origin.position.y = origin.y;
        perceptionMap.info.origin.position.z = 0;
        perceptionMap.info.origin.orientation.x = 0;
        perceptionMap.info.origin.orientation.y = 0;
        perceptionMap.info.origin.orientation.z = 0;
        perceptionMap.header.frame_id = "odom";

        cv::Mat map_tmp;
        int num_count = 0;
        flg_thread_map = true;
        while(flg_thread_map) {
            num_count ++;
            if(num_count % 50 == 0) {
                if (widthOccupy != 0 && heightOccupy != 0) {
//                    LOGCOMMON(INFO) << "thread_planning is runing ...";

                    if(flg_showNavMap){
                        map_tmp = NavMap(cv::Rect(originPointOccupyMin.x, originPointOccupyMin.y, widthOccupy,heightOccupy)).clone();
                        if (navMap.data.size() != map_tmp.rows * map_tmp.cols) {
                            navMap.data.clear();
                            navMap.data.resize((unsigned int) (map_tmp.rows * map_tmp.cols));
                        }
                        std::memcpy(&navMap.data[0], &map_tmp.data[0], widthOccupy * heightOccupy * sizeof(uint8_t));

                        navMap.info.width = (unsigned int) widthOccupy;
                        navMap.info.height = (unsigned int) heightOccupy;
                        navMap.info.origin.position.x = originPoseOccupy.x;
                        navMap.info.origin.position.y = originPoseOccupy.y;
                        navMap.info.map_load_time = ros::Time(0);
                        pub_nav.publish(navMap);
                    }

                    /*************************************/
                    if(flg_showForbidenMap){
                        map_tmp = ForbidenMap(cv::Rect(originPointOccupyMin.x, originPointOccupyMin.y, widthOccupy,heightOccupy)).clone();
                        if (forbidenMap.data.size() != map_tmp.rows * map_tmp.cols) {
                            forbidenMap.data.clear();
                            forbidenMap.data.resize((unsigned int) (map_tmp.rows * map_tmp.cols));
                        }
                        std::memcpy(&forbidenMap.data[0], &map_tmp.data[0], widthOccupy * heightOccupy * sizeof(uint8_t));

                        forbidenMap.info.width = (unsigned int) widthOccupy;
                        forbidenMap.info.height = (unsigned int) heightOccupy;
                        forbidenMap.info.origin.position.x = originPoseOccupy.x;
                        forbidenMap.info.origin.position.y = originPoseOccupy.y;
                        forbidenMap.info.map_load_time = ros::Time(0);
                        pub_forbiden.publish(forbidenMap);
                    }

                    /*************************************/
                    if(flg_showRegionSegmentationMap){
                        map_tmp = RegionSegmentationMap(cv::Rect(originPointOccupyMin.x, originPointOccupyMin.y, widthOccupy,heightOccupy)).clone();

                        if (regionMap.data.size() != map_tmp.rows * map_tmp.cols) {
                            regionMap.data.clear();
                            regionMap.data.resize((unsigned int) (map_tmp.rows * map_tmp.cols));
                        }
                        std::memcpy(&regionMap.data[0], &map_tmp.data[0], widthOccupy * heightOccupy * sizeof(uint8_t));
//                        for(int i = 0 ; i < regionMap.data.size() ; i++){
//                            regionMap.data[i] = RegionSegmentationMap.data[i] == RegionUnknowCell ? RegionUnknowCell:(RegionSegmentationMap.data[i] + 1)*10;
//                        }
                        regionMap.info.width = (unsigned int) widthOccupy;
                        regionMap.info.height = (unsigned int) heightOccupy;
                        regionMap.info.origin.position.x = originPoseOccupy.x;
                        regionMap.info.origin.position.y = originPoseOccupy.y;
                        regionMap.info.map_load_time = ros::Time(0);
                        pub_region.publish(regionMap);
                    }

                    /*************************************/
                    if(flg_showCoverMap){
                        map_tmp = CoverMap(cv::Rect(originPointOccupyMin.x, originPointOccupyMin.y, widthOccupy,heightOccupy)).clone();
                        if (covMap.data.size() != map_tmp.rows * map_tmp.cols) {
                            covMap.data.clear();
                            covMap.data.resize((unsigned int) (map_tmp.rows * map_tmp.cols));
                        }
                        std::memcpy(&covMap.data[0], &map_tmp.data[0], widthOccupy * heightOccupy * sizeof(uint8_t));

                        covMap.info.width = (unsigned int) widthOccupy;
                        covMap.info.height = (unsigned int) heightOccupy;
                        covMap.info.origin.position.x = originPoseOccupy.x;
                        covMap.info.origin.position.y = originPoseOccupy.y;
                        covMap.info.map_load_time = ros::Time(0);
                        pub_cov.publish(covMap);
                    }

                    /*************************************/

                    if(flg_showFollowMap){
                        map_tmp = FollowMap(cv::Rect(originPointOccupyMin.x, originPointOccupyMin.y, widthOccupy,heightOccupy)).clone();
                        if (followMap.data.size() != map_tmp.rows * map_tmp.cols) {
                            followMap.data.clear();
                            followMap.data.resize((unsigned int) (map_tmp.rows * map_tmp.cols));
                        }
                        std::memcpy(&followMap.data[0], &map_tmp.data[0], widthOccupy * heightOccupy * sizeof(uint8_t));

                        followMap.info.width = (unsigned int) widthOccupy;
                        followMap.info.height = (unsigned int) heightOccupy;
                        followMap.info.origin.position.x = originPoseOccupy.x;
                        followMap.info.origin.position.y = originPoseOccupy.y;
                        followMap.info.map_load_time = ros::Time(0);
                        pub_follow.publish(followMap);
                    }

                    /*************************************/
                    if(flg_showPathMap){
                        map_tmp = PathMap(cv::Rect(originPointOccupyMin.x, originPointOccupyMin.y, widthOccupy,heightOccupy)).clone();
                        if (pathMap.data.size() != map_tmp.rows * map_tmp.cols) {
                            pathMap.data.clear();
                            pathMap.data.resize((unsigned int) (map_tmp.rows * map_tmp.cols));
                        }
                        std::memcpy(&pathMap.data[0], &map_tmp.data[0], widthOccupy * heightOccupy * sizeof(uint8_t));

                        pathMap.info.width = (unsigned int) widthOccupy;
                        pathMap.info.height = (unsigned int) heightOccupy;
                        pathMap.info.origin.position.x = originPoseOccupy.x;
                        pathMap.info.origin.position.y = originPoseOccupy.y;
                        pathMap.info.map_load_time = ros::Time(0);
                        pub_path.publish(pathMap);
                    }

                    /*************************************/
                    if(flg_showAPPMap){
                        map_tmp = APPMap(cv::Rect(originPointOccupyMin.x, originPointOccupyMin.y, widthOccupy,heightOccupy)).clone();
                        if (appMap.data.size() != map_tmp.rows * map_tmp.cols) {
                            appMap.data.clear();
                            appMap.data.resize((unsigned int) (map_tmp.rows * map_tmp.cols));
                        }
                        std::memcpy(&appMap.data[0], &map_tmp.data[0], widthOccupy * heightOccupy * sizeof(uint8_t));

                        appMap.info.width = (unsigned int) widthOccupy;
                        appMap.info.height = (unsigned int) heightOccupy;
                        appMap.info.origin.position.x = originPoseOccupy.x;
                        appMap.info.origin.position.y = originPoseOccupy.y;
                        appMap.info.map_load_time = ros::Time(0);
                        pub_app.publish(appMap);
                    }
                    /*************************************/
                    if(flg_showPerceptionMap){
                        map_tmp = DeepLearningMap(cv::Rect(originPointOccupyMin.x, originPointOccupyMin.y, widthOccupy,heightOccupy)).clone();
                        if (perceptionMap.data.size() != map_tmp.rows * map_tmp.cols) {
                            perceptionMap.data.clear();
                            perceptionMap.data.resize((unsigned int) (map_tmp.rows * map_tmp.cols));
                        }
                        std::memcpy(&perceptionMap.data[0], &map_tmp.data[0], widthOccupy * heightOccupy * sizeof(uint8_t));

                        perceptionMap.info.width = (unsigned int) widthOccupy;
                        perceptionMap.info.height = (unsigned int) heightOccupy;
                        perceptionMap.info.origin.position.x = originPoseOccupy.x;
                        perceptionMap.info.origin.position.y = originPoseOccupy.y;
                        perceptionMap.info.map_load_time = ros::Time(0);
                        pub_perception.publish(perceptionMap);
                    }
                    /*************************************/
                }
            }

            this_thread::sleep_for(chrono::microseconds(20000));//50HZ
        }
        pub_nav.shutdown();
        pub_forbiden.shutdown();
        pub_region.shutdown();
        pub_cov.shutdown();
        pub_follow.shutdown();
        pub_path.shutdown();
        pub_app.shutdown();
        pub_perception.shutdown();
        if(!map_tmp.empty()){
            map_tmp.release();
        }
    });
    return 0;
}

int Map::kill_thread_map(){
    flg_thread_map = false;
    if(T_map != nullptr){
        T_map->join();
        delete T_map;
        T_map = nullptr;
    }
    return 0;
}

#endif

#endif