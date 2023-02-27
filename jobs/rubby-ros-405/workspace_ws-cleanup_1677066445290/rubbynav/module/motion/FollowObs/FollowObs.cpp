//
// Created by lcy on 22-10-27.
//

#include "FollowObs.h"

bool FollowObs::FollowObsInit(PoseStamped p_nav, PoseStamped p_obs){
    followObs_nav = std::move(p_nav);
    followObs_obs = std::move(p_obs);
}

bool FollowObs::FollowObsBumperRightInit(){
    followobs_step_cur = 1;
}
int FollowObs::FollowObsBumperRight(SensorInfo *sensor, SpeedControlInfo &cmd){
    switch (followobs_step_cur)
    {
    case 1:
        followObs_obsDir = Dir_Offset(sensor->Posture, followObs_obs);
        LOGMOTION(INFO) << "follow obs r, pose:("<< sensor->Posture.pose.x << " , " << sensor->Posture.pose.y << ") yaw:" << sensor->Posture.euler.yaw <<" obs:("<< followObs_obs.pose.x << " , " << followObs_obs.pose.y << ") followObs_obsDir:" << followObs_obsDir;
        RunTurnInit(fabs(followObs_obsDir), sensor->Posture.euler.yaw);
        followobs_step_cur = 2;
        break;
    case 2:
        if (followObs_obsDir >= 0)
        {
            if (RunTurnLeft(sensor->Posture.euler.yaw, cmd))
            {
                followobs_step_cur = 3;
            }
        }
        else
        {
            if (RunTurnRight(sensor->Posture.euler.yaw, cmd))
            {
                followobs_step_cur = 3;
            }
        }
        break;
    case 3:
        RunDriveInit(ROBOTRADIUS * 2, sensor->Posture);
        followobs_step_cur = 4;
        break;
    case 4:
        if (RunDrive(sensor->Posture, cmd) || sensor->Bumper.BumperTriggered)
        {
            followobs_step_cur = 5;
        }
        break;
    case 5: // 3
        RunReverseInit(0.02, sensor->Posture);
        followobs_step_cur = 6;
        break;
    case 6:
        if (RunReverse(sensor->Posture, cmd))
        {
            followobs_step_cur = 7;
        }
        break;
    case 7:
        RunTurnInit(Pi_ / 4, sensor->Posture.euler.yaw);
        followobs_step_cur = 8;
        break;
    case 8:
        if (RunTurnLeft(sensor->Posture.euler.yaw, cmd))
        {
            followobs_step_cur = 9;
        }
        break;
    case 9:
        if (RunGoTurnRight(cmd) && sensor->Bumper.BumperTriggered)
        {
            followobs_step_cur = 5;
        }
        break;

    default:
        RunParking(cmd);
        break;
    }
    return followobs_step_cur;
}

bool FollowObs::FollowObsBumperLeftInit(){
    followobs_step_cur = 1;
}
int FollowObs::FollowObsBumperLeft(SensorInfo *sensor, SpeedControlInfo &cmd){
    switch (followobs_step_cur)
    {
    case 1:
        followObs_obsDir = Dir_Offset(sensor->Posture, followObs_obs);
        LOGMOTION(INFO) << "follow obs l, pose:("<< sensor->Posture.pose.x << " , " << sensor->Posture.pose.y << ") yaw:" << sensor->Posture.euler.yaw <<" obs:("<< followObs_obs.pose.x << " , " << followObs_obs.pose.y << ") followObs_obsDir:" << followObs_obsDir;
        RunTurnInit(fabs(followObs_obsDir), sensor->Posture.euler.yaw);
        followobs_step_cur = 2;
        break;
    case 2:
        if (followObs_obsDir >= 0)
        {
            if (RunTurnLeft(sensor->Posture.euler.yaw, cmd))
            {
                followobs_step_cur = 3;
            }
        }
        else
        {
            if (RunTurnRight(sensor->Posture.euler.yaw, cmd))
            {
                followobs_step_cur = 3;
            }
        }
        break;
    case 3:
        RunDriveInit(ROBOTRADIUS * 2, sensor->Posture);
        followobs_step_cur = 4;
        break;
    case 4:
        if (RunDrive(sensor->Posture, cmd) || sensor->Bumper.BumperTriggered)
        {
            followobs_step_cur = 5;
        }
        break;
    case 5: // 3
        RunReverseInit(0.02, sensor->Posture);
        followobs_step_cur = 6;
        break;
    case 6:
        if (RunReverse(sensor->Posture, cmd))
        {
            followobs_step_cur = 7;
        }
        break;
    case 7:
        RunTurnInit(Pi_ / 4, sensor->Posture.euler.yaw);
        followobs_step_cur = 8;
        break;
    case 8:
        if (RunTurnRight(sensor->Posture.euler.yaw, cmd))
        {
            followobs_step_cur = 9;
        }
        break;
    case 9:
        if (RunGoTurnLeft(cmd) && sensor->Bumper.BumperTriggered)
        {
            followobs_step_cur = 5;
        }
        break;

    default:
        RunParking(cmd);
        break;
    }
    return followobs_step_cur;
}

bool FollowObs::FollowObsIrRightInit(){
    followobs_step_cur = 1;
    allongWallInit();
    return true;
}
int FollowObs::FollowObsIrRight(SensorInfo * sensor , SpeedControlInfo & cmd){
    switch (followobs_step_cur)
    {
        case 1:
            followObs_obsDir = Dir_Offset(sensor->Posture, followObs_obs);
            LOGMOTION(INFO) << "follow obs r, pose:("<< sensor->Posture.pose.x << " , " << sensor->Posture.pose.y << ") yaw:" << sensor->Posture.euler.yaw <<" obs:("<< followObs_obs.pose.x << " , " << followObs_obs.pose.y << ") followObs_obsDir:" << followObs_obsDir;
            RunTurnInit(fabs(followObs_obsDir), sensor->Posture.euler.yaw);
            followobs_step_cur = 2;
            break;
        case 2:
            if (followObs_obsDir >= 0)
            {
                if (RunTurnLeft(sensor->Posture.euler.yaw, cmd))
                {
                    followobs_step_cur = 3;
                }
            }
            else
            {
                if (RunTurnRight(sensor->Posture.euler.yaw, cmd))
                {
                    followobs_step_cur = 3;
                }
            }
            break;
        case 3:
            RunDriveInit(ROBOTRADIUS * 2, sensor->Posture);
            followobs_step_cur = 4;
            break;
        case 4:
            if (RunDrive(sensor->Posture, cmd , 0.1f))
            {
                followobs_step_cur = 9;
            }
            if(sensor->Bumper.BumperTriggered){
                followobs_step_cur = 9;
            }
            break;
//        case 5: // 3
//            RunReverseInit(0.02, sensor->Posture);
//            followobs_step_cur = 6;
//            break;
//        case 6:
//            if (RunReverse(sensor->Posture, cmd))
//            {
//                followobs_step_cur = 7;
//            }
//            break;
//        case 7:
//            RunTurnInit(Pi_ / 4, sensor->Posture.euler.yaw);
//            followobs_step_cur = 8;
//            break;
//        case 8:
//            if (RunTurnLeft(sensor->Posture.euler.yaw, cmd))
//            {
//                followobs_step_cur = 9;
//            }
//            break;
        case 9:
            allongWallRight(sensor , cmd);
            /*if (RunGoTurnRight(cmd) && sensor->Bumper.BumperTriggered)
            {
                followobs_step_cur = 5;
            }*/
            break;

        default:
            RunParking(cmd);
            break;
    }
	NAVLOG(INFO) << "right followobs_step_cur: " << followobs_step_cur;
    return followobs_step_cur;
}

bool FollowObs::FollowObsIrLeftInit(){
    followobs_step_cur = 1;
    allongWallInit();
    return true;
}
int FollowObs::FollowObsIrLeft(SensorInfo * sensor , SpeedControlInfo & cmd){
    switch (followobs_step_cur)
    {
        case 1:
            followObs_obsDir = Dir_Offset(sensor->Posture, followObs_obs);
            LOGMOTION(INFO) << "follow obs l, pose:("<< sensor->Posture.pose.x << " , " << sensor->Posture.pose.y << ") yaw:" << sensor->Posture.euler.yaw <<" obs:("<< followObs_obs.pose.x << " , " << followObs_obs.pose.y << ") followObs_obsDir:" << followObs_obsDir;
            RunTurnInit(fabs(followObs_obsDir), sensor->Posture.euler.yaw);
            followobs_step_cur = 2;
            break;
        case 2:
            if (followObs_obsDir >= 0)
            {
                if (RunTurnLeft(sensor->Posture.euler.yaw, cmd))
                {
                    followobs_step_cur = 3;
                }
            }
            else
            {
                if (RunTurnRight(sensor->Posture.euler.yaw, cmd))
                {
                    followobs_step_cur = 3;
                }
            }
            break;
        case 3:
            RunDriveInit(ROBOTRADIUS * 2, sensor->Posture);
            followobs_step_cur = 4;
            break;
        case 4:
            if (RunDrive(sensor->Posture, cmd , 0.1f))
            {
                followobs_step_cur = 9;
            }
            if(sensor->Bumper.BumperTriggered){
                followobs_step_cur = 9;
            }
            break;
//        case 5: // 3
//            RunReverseInit(0.02, sensor->Posture);
//            followobs_step_cur = 6;
//            break;
//        case 6:
//            if (RunReverse(sensor->Posture, cmd))
//            {
//                followobs_step_cur = 7;
//            }
//            break;
//        case 7:
//            RunTurnInit(Pi_ / 4, sensor->Posture.euler.yaw);
//            followobs_step_cur = 8;
//            break;
//        case 8:
//            if (RunTurnRight(sensor->Posture.euler.yaw, cmd))
//            {
//                followobs_step_cur = 9;
//            }
//            break;
        case 9:
            allongWallLeft(sensor , cmd);
            /*if (RunGoTurnLeft(cmd) && sensor->Bumper.BumperTriggered)
            {
                followobs_step_cur = 5;
            }*/
            break;

        default:
            RunParking(cmd);
            break;
    }
	NAVLOG(INFO) << "left followobs_step_cur: " << followobs_step_cur;
    return followobs_step_cur;
}