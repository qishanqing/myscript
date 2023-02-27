//////////////////////////////////////////////////////////////////////
///  @file     motion_map.h
///  @brief    an map about motion
///  Details.
///
///  @author   sunhao
///  @date     2021.07.27
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_MOTION_MAP_H
#define DETECTOR_SAMPLE_MOTION_MAP_H
#include "src/instance/types.h"
#include "src/utils/config.h"

class MotionMap
{
public:
    MotionMap();

    void SetParam(const ConfigParam param);

    void GetInstanceStatus(const psl::Instance &instance, instance::Obstacle &obstacle);

private:
    float nFrames;
    float distRidus;
};


#endif //DETECTOR_SAMPLE_MOTION_MAP_H
