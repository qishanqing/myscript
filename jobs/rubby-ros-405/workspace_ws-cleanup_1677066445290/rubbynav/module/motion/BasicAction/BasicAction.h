//
// Created by lcy on 22-11-28.
//

#ifndef HSMCONTROLCORE_BASICACTION_H
#define HSMCONTROLCORE_BASICACTION_H

#include <iostream>
#include <chrono>
#include "Common.h"
#include "Operation.h"

class BasicAction : virtual public Operation{
public:
    bool RunParking(SpeedControlInfo & cmd);

    void RunTurnInit(double dlt , double yaw);
    bool RunTurnLeft(double yaw , SpeedControlInfo & cmd);
    bool RunTurnLeft(double yaw , SpeedControlInfo & cmd, float va);
    bool RunTurnRight(double yaw , SpeedControlInfo & cmd);
    bool RunTurnRight(double yaw , SpeedControlInfo & cmd, float va);

    void RunDriveInit(float dlt , const PoseStamped & p);
    bool RunDrive(const PoseStamped & p , SpeedControlInfo & cmd, float vl = 0.2f);

    void RunReverseInit(float dlt , const PoseStamped & p);
    bool RunReverse(const PoseStamped & p , SpeedControlInfo & cmd);

    bool RunGoTurnLeft(SpeedControlInfo & cmd);
    bool RunGoTurnRight(SpeedControlInfo & cmd);

private:
    double dlt_tmp = 0.0;
    double dlt_sum = 0.0;
    double dlt_target = 0.0;
    PoseStamped turning_point;
    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    int time_reverse = 3000;//后退时间,毫秒
};

#endif //HSMCONTROLCORE_BASICACTION_H
