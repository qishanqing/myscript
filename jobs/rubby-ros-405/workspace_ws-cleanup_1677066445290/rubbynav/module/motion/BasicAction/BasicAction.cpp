//
// Created by lcy on 22-11-28.
//

#include "BasicAction.h"

bool BasicAction::RunParking(SpeedControlInfo &cmd)
{
    cmd.vl = 0.0;
    cmd.va = 0.0;
    return true;
}


void BasicAction::RunTurnInit(double dlt, double yaw)
{
    dlt_target = dlt;
    dlt_tmp = yaw;
    dlt_sum = 0.0;
    return;
}

bool BasicAction::RunTurnLeft(double yaw, SpeedControlInfo &cmd)
{
    dlt_sum += ternaryoperator(((dlt_tmp > 0 && yaw > 0) || (dlt_tmp < 0 && yaw < 0) || (dlt_tmp == 0 && yaw == 0)), fabs(dlt_tmp - yaw), fabs(dlt_tmp + yaw));
    dlt_tmp = yaw;
    if (dlt_sum < dlt_target)
    {
        cmd.va = 0.8;
        cmd.vl = 0.0;
        return false;
    }
    else
    {
        cmd.va = 0.0;
        cmd.vl = 0.0;
        return true;
    }
}

bool BasicAction::RunTurnLeft(double yaw, SpeedControlInfo &cmd, float va)
{
    dlt_sum += ternaryoperator(((dlt_tmp > 0 && yaw > 0) || (dlt_tmp < 0 && yaw < 0) || (dlt_tmp == 0 && yaw == 0)), fabs(dlt_tmp - yaw), fabs(dlt_tmp + yaw));
    dlt_tmp = yaw;
    if (dlt_sum < dlt_target)
    {
        cmd.va = va;
        cmd.vl = 0.0;
        return false;
    }
    else
    {
        cmd.va = 0.0;
        cmd.vl = 0.0;
        return true;
    }
}

bool BasicAction::RunTurnRight(double yaw, SpeedControlInfo &cmd)
{
    dlt_sum += ternaryoperator(((dlt_tmp > 0 && yaw > 0) || (dlt_tmp < 0 && yaw < 0) || (dlt_tmp == 0 && yaw == 0)), fabs(dlt_tmp - yaw), fabs(dlt_tmp + yaw));
    dlt_tmp = yaw;
    if (dlt_sum < dlt_target)
    {
        cmd.va = -0.8f;
        cmd.vl = 0.0;
        return false;
    }
    else
    {
        cmd.va = 0.0;
        cmd.vl = 0.0;
        return true;
    }
}

bool BasicAction::RunTurnRight(double yaw, SpeedControlInfo &cmd, float va)
{
    dlt_sum += ternaryoperator(((dlt_tmp > 0 && yaw > 0) || (dlt_tmp < 0 && yaw < 0) || (dlt_tmp == 0 && yaw == 0)), fabs(dlt_tmp - yaw), fabs(dlt_tmp + yaw));
    dlt_tmp = yaw;
    if (dlt_sum < dlt_target)
    {
        cmd.va = va;
        cmd.vl = 0.0;
        return false;
    }
    else
    {
        cmd.va = 0.0;
        cmd.vl = 0.0;
        return true;
    }
}

void BasicAction::RunDriveInit(float dlt, const PoseStamped &p)
{
    turning_point = p;
    dlt_tmp = pow(dlt, 2);
    return;
}

bool BasicAction::RunDrive(const PoseStamped &p, SpeedControlInfo &cmd, float vl)
{
    if (SquareDis_p2p(turning_point.pose, p.pose) < dlt_tmp)
    {
        cmd.va = 0.0;
        cmd.vl = vl;
        return false;
    }
    else
    {
        cmd.va = 0.0;
        cmd.vl = 0.0;
        return true;
    }
}


void BasicAction::RunReverseInit(float dlt, const PoseStamped &p)
{
    turning_point = p;
    dlt_tmp = pow(dlt, 2);
    time_start = std::chrono::steady_clock::now();
    return;
}

bool BasicAction::RunReverse(const PoseStamped &p, SpeedControlInfo &cmd)
{
    time_end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = time_end - time_start; // std::micro 表示以微秒为时间单位

    if ((SquareDis_p2p(turning_point.pose, p.pose) > dlt_tmp) || elapsed.count() > time_reverse)
    {
        cmd.va = 0.0;
        cmd.vl = 0.0;
        return true;
    }
    else
    {
        cmd.va = 0.0;
        cmd.vl = -0.1f;
        return false;
    }
}


bool BasicAction::RunGoTurnLeft(SpeedControlInfo &cmd)
{
    cmd.va = 1.25f;
    cmd.vl = 0.2f;
    return true;
}

bool BasicAction::RunGoTurnRight(SpeedControlInfo &cmd)
{
    cmd.va = -1.25f;
    cmd.vl = 0.2f;
    return true;
}