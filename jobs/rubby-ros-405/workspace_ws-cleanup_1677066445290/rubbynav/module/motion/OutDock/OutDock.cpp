//
// Created by gby on 22-11-29.
//

#include "OutDock.h"

bool OutDock::outDockInit(SensorInfo* sensor_, DataDown* datadown_)
{
    datadown_->MotionState = motionState::LEAVEPILE;
    BasicAction::RunDriveInit(0.5, sensor_->Posture);
    return true;
}

bool OutDock::outDockRun(SensorInfo* sensor_, DataDown* datadown_, float vl_)
{
    datadown_->MotionState = motionState::LEAVEPILE;
#ifdef RUBBYSIMULATION
    return true;
#endif
    return BasicAction::RunDrive(sensor_->Posture, datadown_->cmd, vl_);
}