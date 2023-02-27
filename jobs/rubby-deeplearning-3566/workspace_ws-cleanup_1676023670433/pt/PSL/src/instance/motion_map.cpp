//
// Created by hao on 2021/7/27.
//

#include "motion_map.h"


MotionMap::MotionMap()
{
    nFrames = 10;
    distRidus = 20;
}

void MotionMap::SetParam(const ConfigParam param)
{
    nFrames = param.nFrames;
    distRidus = param.radius;
}


void MotionMap::GetInstanceStatus(const psl::Instance &instance
                                        , Obstacle &obstacle)
{
    if (obstacle.maps.size() == 0)
    {
        obstacle.mapInfoPreSize = 0;
        Map mapInfo(instance.locationMachine, distRidus);
        obstacle.maps.push_back(mapInfo);
        obstacle.mapInfoSize = 1;
    }
    else
    {
        int index = -1;
        int domainIn = 0;
        for (auto &map_info: obstacle.maps)
        {
            index += 1;
            if (instance.locationMachine.Distance(map_info.centerPoint) <= distRidus)
            {
                domainIn += 1;
                map_info.havePerson += 1;
                map_info.noPersonCount = 0;
                if (map_info.staticCount >= nFrames)
                {
                    map_info.status = 0;
                    continue;
                }
            }
        }
        if (domainIn == 0)
        {
            Map mapInfo(instance.locationMachine, distRidus);
            obstacle.maps.push_back(mapInfo);
            obstacle.mapInfoSize += 1;
        }
    }
}
