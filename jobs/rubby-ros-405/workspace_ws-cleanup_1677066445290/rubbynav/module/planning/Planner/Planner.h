//
// Created by lcy on 21-6-10.
//

#ifndef ROBOT_MATLAB_PLANNER_H
#define ROBOT_MATLAB_PLANNER_H

#include <iostream>
#include <thread>
#include "RegionSegmentation.h"
#include "Singleton.h"
#include "PlanningMapManage.h"
#include "Nav.h"
#include "CleanBox.h"
#include "Search.h"
#include "CleanPath.h"
#include "Delimit.h"
#include "segment.h"

class Planner{
public:
    Planner():image_p(nullptr),map_manage(nullptr),nav(nullptr),cleanbox_(nullptr),search_(nullptr),cleanpath_(nullptr),delimit_(nullptr),roomsegment_(
            nullptr){};
    ~Planner() {
        destroy();
    };

    void init(){
        if(image_p == nullptr){
            image_p = new RegionSegmentation();
        }
        if(map_manage == nullptr){
            map_manage = new PlanningMapManager();
        }
        if(nav == nullptr){
            nav = new global_planner::NavPath();
        }
        if(cleanbox_ == nullptr){
            cleanbox_ = new CleanBox();
        }
        if(search_ == nullptr){
            search_ = new Search();
        }
        if(cleanpath_ == nullptr){
            cleanpath_ = new CleanPath();
        }
        if(delimit_ == nullptr){
            delimit_ = new Delimit();
        }
        if(roomsegment_ == nullptr){
            roomsegment_ = new RoomSegment();
        }
    }
    void destroy(){
        if(image_p != nullptr){
            delete image_p;
            image_p = nullptr;
        }
        if(map_manage != nullptr){
            delete map_manage;
            map_manage = nullptr;
        }
        if(nav != nullptr){
            delete nav;
            nav = nullptr;
        }
        if(cleanbox_ != nullptr){
            delete cleanbox_;
            cleanbox_ = nullptr;
        }
        if(search_ != nullptr){
            delete search_;
            search_ = nullptr;
        }
        if(cleanpath_ != nullptr){
            delete cleanpath_;
            cleanpath_ = nullptr;
        }
        if(delimit_ != nullptr){
            delete delimit_;
            delimit_ = nullptr;
        }
        if(roomsegment_ != nullptr){
            delete roomsegment_;
            roomsegment_ = nullptr;
        }
    }

    PlanningMapManager * getmap_manage(){return map_manage;}
    RegionSegmentation *getimage_p(){return image_p;}
    global_planner::NavPath *getnav(){return nav;}
    CleanBox * getcleanbox(){return cleanbox_;}
    Search * getsearch(){return search_;}
    CleanPath * getcleanpath(){return cleanpath_;}
    Delimit * getdelimit(){return delimit_;}
    RoomSegment * getroomsegment(){return roomsegment_;}

private:
    std::thread *T_planning = nullptr;

    PlanningMapManager * map_manage = nullptr;
    RegionSegmentation * image_p = nullptr;
    global_planner::NavPath * nav = nullptr;
    CleanBox * cleanbox_ = nullptr;
    Search * search_ = nullptr;
    CleanPath * cleanpath_ = nullptr;
    Delimit * delimit_ = nullptr;
    RoomSegment * roomsegment_ = nullptr;
};

#define s_planner Singleton<Planner>::GetInstance()

#endif