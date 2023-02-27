//
// Created by lcy on 22-10-27.
//

#ifndef RUBBYNAV_FOLLOWOBS_H
#define RUBBYNAV_FOLLOWOBS_H

#include "stdio.h"
#include "Common.h"
#include "BasicAction.h"
#include "irAlongWall.h"

class FollowObs : virtual public BasicAction , public IrAlongWall{
public:
    bool FollowObsInit(PoseStamped p_nav , PoseStamped p_obs);

    bool FollowObsBumperRightInit();
    int FollowObsBumperRight(SensorInfo * sensor , SpeedControlInfo & cmd);
    bool FollowObsBumperLeftInit();
    int FollowObsBumperLeft(SensorInfo * sensor , SpeedControlInfo & cmd);

    bool FollowObsIrRightInit();
    int FollowObsIrRight(SensorInfo * sensor , SpeedControlInfo & cmd);
    bool FollowObsIrLeftInit();
    int FollowObsIrLeft(SensorInfo * sensor , SpeedControlInfo & cmd);

private:
    int followobs_step_cur = 0;
    double followObs_obsDir = 0.0f;
    PoseStamped followObs_nav;
    PoseStamped followObs_obs;
};

#endif //RUBBYNAV_FOLLOWOBS_H
