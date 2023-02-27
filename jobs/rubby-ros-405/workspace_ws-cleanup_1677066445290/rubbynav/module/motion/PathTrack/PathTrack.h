//
// Created by lcy on 22-4-22.
//

#ifndef RUBBYDECISION_MOTIONPLANNING_H
#define RUBBYDECISION_MOTIONPLANNING_H

#include <iostream>
#include <vector>
#include "Log.h"
#include "PathTrackNav.h"
#include "PathTrackBow.h"
#include "config.h"

#include <fstream>

using namespace std;

class PathTrack:public PathTrackNav::pathTrackNav, public PathTrackBow::pathTrackBow{
public:
    bool pathtrackInit(vector<PoseStamped> & path, PoseStamped robotpose, vector<PoseStamped> & insertPath_, int flag_ = 0);
    int pathtrackRun(PoseStamped robotpose, SpeedControlInfo & speed, int flag_ = 0);

private:
    void insertPoint(const PoseStamped& point1, const PoseStamped& point2, std::vector<PoseStamped>& vec_);

};

#endif //RUBBYDECISION_MOTIONPLANNING_H
