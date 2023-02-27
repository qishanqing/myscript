//
// Created by gby on 22-11-29.
//

#include "ObstacleAvoid.h"

int ObstacleAvoid::ObstacleAvoidanceInit(const PoseStamped & p, int flg_navfollowobs_, uint8_t & motionState)
{
    if (flg_navfollowobs_ == 0)
    {
        flg_navfollowobs = 0;
        BasicAction::RunReverseInit(0.05, p);
    }
    else
    {
        if(flg_navfollowobs_ == 1){
            motionState = motionState::FENAVLEFT;
            flg_navfollowobs = 1;
        }else if(flg_navfollowobs_ == 2){
            motionState = motionState::FENAVRIGHT;
            flg_navfollowobs = 2;
        }
        navfollowobs_step_cur = 1;
    }
    LOGMOTION(INFO) << "Obs Avoidance Init , flg_navfollowobs_:" << flg_navfollowobs_;
    return true;
}

bool ObstacleAvoid::ObstacleAvoidance(const SensorInfo *sensor_, SpeedControlInfo & cmd)
{
    if (flg_navfollowobs == 0)
    {
        return BasicAction::RunReverse(sensor_->Posture, cmd);
    }
    else if (flg_navfollowobs == 1)
    {
        int flg = LeftObstacleAvoidance(sensor_, cmd);
        if(flg == 8){
            return true;
        }
    }
    else if (flg_navfollowobs == 2)
    {
        int flg = RightObstacleAvoidance(sensor_, cmd);
        if(flg == 8){
            return true;
        }
    }
    return false;
}

int ObstacleAvoid::LeftObstacleAvoidance(const SensorInfo *sensor_, SpeedControlInfo &cmd){
    switch (navfollowobs_step_cur){
        case 1:
            RunReverseInit(0.02 , sensor_->Posture);
            fill_bumperobs_init(sensor_);
            navfollowobs_step_cur = 2;
            break;
        case 2:
            if(RunReverse(sensor_->Posture , cmd)){
                navfollowobs_step_cur = 3;
            }
            break;
        case 3:
            RunTurnInit(Pi_/4 , sensor_->Posture.euler.yaw);
            navfollowobs_step_cur = 4;
            break;
        case 4:
            if(RunTurnRight(sensor_->Posture.euler.yaw , cmd)){
                navfollowobs_step_cur = 5;
                Dir_360 = 0;
                Obs_start = sensor_->Posture;
            }
            break;
        case 5:
            dir_tmp = (int)floor((sensor_->Posture.euler.yaw + Pi_)/0.1047);//0.104719755 == 6/180*Pi_
            if(Dir_360 == 0xFFFFFFFFFFFFFFF){
                if(SquareDis_p2p(Obs_start.pose,sensor_->Posture.pose) <= 0.1*0.1){
                    Dir_360 = 0;
                    navfollowobs_step_cur = 8;
                }
            }else{
                if(dir_tmp < 60){
                    unsigned long long  d = 0x01;
                    Dir_360 |= (d << dir_tmp);
                }
            }
            if(sensor_->Bumper.BumperTriggered){
                fill_bumperobs(sensor_, 1);
                navfollowobs_step_cur = 6;
            }else {
                RunGoTurnLeft(cmd);
            }
            break;
        case 6:
            RunReverseInit(0.05 , sensor_->Posture);
            navfollowobs_step_cur = 7;
            break;
        case 7:
            if(RunReverse(sensor_->Posture , cmd)){
                navfollowobs_step_cur = 8;
            }
            break;
        default:
            RunParking(cmd);
            break;
    }
    return navfollowobs_step_cur;
}
int ObstacleAvoid::RightObstacleAvoidance(const SensorInfo *sensor_, SpeedControlInfo &cmd){
    switch (navfollowobs_step_cur){
        case 1:
            RunReverseInit(0.02 , sensor_->Posture);
            fill_bumperobs_init(sensor_);
            navfollowobs_step_cur = 2;
            break;
        case 2:
            if(RunReverse(sensor_->Posture , cmd)){
                navfollowobs_step_cur = 3;
            }
            break;
        case 3:
            RunTurnInit(Pi_/4 , sensor_->Posture.euler.yaw);
            navfollowobs_step_cur = 4;
            break;
        case 4:
            if(RunTurnLeft(sensor_->Posture.euler.yaw , cmd)){
                navfollowobs_step_cur = 5;
                Dir_360 = 0;
                Obs_start = sensor_->Posture;
            }
            break;
        case 5:
            dir_tmp = (int)floor((sensor_->Posture.euler.yaw + Pi_)/0.1047);//0.104719755 == 6/180*Pi_
            if(Dir_360 == 0xFFFFFFFFFFFFFFF){
                if(SquareDis_p2p(Obs_start.pose,sensor_->Posture.pose) <= 0.1*0.1){
                    Dir_360 = 0;
                    navfollowobs_step_cur = 8;
                }
            }else{
                if(dir_tmp < 60){
                    unsigned long long  d = 0x01;
                    Dir_360 |= (d << dir_tmp);
                }
            }
            if(sensor_->Bumper.BumperTriggered){
                fill_bumperobs(sensor_, -1);
                navfollowobs_step_cur = 6;
            }else {
                RunGoTurnRight(cmd);
            }
            break;
        case 6:
            RunReverseInit(0.05 , sensor_->Posture);
            navfollowobs_step_cur = 7;
            break;
        case 7:
            if(RunReverse(sensor_->Posture , cmd)){
                navfollowobs_step_cur = 8;
            }
            break;
        default:
            RunParking(cmd);
            break;
    }
    return navfollowobs_step_cur;
}
