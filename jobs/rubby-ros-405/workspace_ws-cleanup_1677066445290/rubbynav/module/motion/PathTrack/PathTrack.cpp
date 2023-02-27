//
// Created by lcy on 22-4-22.
//

#include "PathTrack.h"
#include "Map.h"

bool PathTrack::pathtrackInit(vector<PoseStamped> & path, PoseStamped robotpose, vector<PoseStamped> & insertPath_, int flag_)
{
    if(flag_ == 1)
    {
        PathTrackNav::pathTrackNav::clear();
        PathTrackNav::pathTrackNav::setPathNav(path);
        insertPath_ = PathTrackNav::pathTrackNav::getPathNav();
        PathTrackNav::pathTrackNav::isEndReached = false;
        return true;
    }
    else
    {
        PathTrackBow::pathTrackBow::clear();
        PathTrackBow::pathTrackBow::setPathBow(path);
        insertPath_ = PathTrackBow::pathTrackBow::getPathBow();
        PathTrackBow::pathTrackBow::isEndReached = false;
        return true;
    }
}

int PathTrack::pathtrackRun(PoseStamped robotpose, SpeedControlInfo & speed, int flag_)
{
    if(flag_ == 1)
    {
        PathTrackNav::pathTrackNav::setRobotPose(robotpose);
        PathTrackNav::pathTrackNav::update(speed.vl, speed.va);

        if(!PathTrackNav::pathTrackNav::isEndReached)
        {
            return Clean_Follow_Run;
        }
        else
        {
            return Clean_Follow_Finish;
        }
    }
    else
    {
        PathTrackBow::pathTrackBow::setRobotPose(robotpose);
        PathTrackBow::pathTrackBow::update(speed.vl, speed.va);

        if(!PathTrackBow::pathTrackBow::isEndReached)
        {
            return Clean_Follow_Run;
        }
        else
        {
            return Clean_Follow_Finish;
        }
    }
}