//
// Created by gby on 22-11-29.
//

#ifndef OBSTACLEAVOID_H_
#define OBSTACLEAVOID_H_

#include "BasicAction.h"
#include "Operation.h"
#include "MotionMapManage.h"

class ObstacleAvoid: virtual public MotionMapManage, virtual public BasicAction, virtual public Operation
{
public:
    int ObstacleAvoidanceInit(const PoseStamped & p, int flg_navfollowobs_, uint8_t & motionState);
    bool ObstacleAvoidance(const SensorInfo *sensor_, SpeedControlInfo & cmd);

private:
    int navfollowobs_step_cur = 0;
    int flg_navfollowobs = 0;
    unsigned long long Dir_360 = 0;
    PoseStamped Obs_start;
    int dir_tmp = 0;

    int LeftObstacleAvoidance(const SensorInfo *sensor_, SpeedControlInfo &cmd);
    int RightObstacleAvoidance(const SensorInfo *sensor_, SpeedControlInfo &cmd);

};

#endif