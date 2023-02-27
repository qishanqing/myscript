//
// Created by lcy on 22-11-29.
//

#ifndef HSMCONTROLCORE_RELOCATION_H
#define HSMCONTROLCORE_RELOCATION_H

#include <iostream>
#include "Common.h"
#include "BasicAction.h"

class Relocation: virtual public BasicAction{
public:
    bool RelocationInit(float dis);
    int RelocationRun(SensorInfo * sensor , SpeedControlInfo & cmd);

    void FirstBuildInit(double yaw);
    bool FirstBuildRun(double yaw , SpeedControlInfo & cmd);
private:
    int reLocation_step_cur = 0;
    float Relocation_go = 0.5;                  //重定位前进距离
    float Relocation_turn = (float)Pi_*2/3;//重定位转向角度

    bool flg_reverse = false;
    int reverse_step_cur = 0;
    int reverserun(const PoseStamped & p , SpeedControlInfo & cmd);


};
#endif //HSMCONTROLCORE_RELOCATION_H
