#pragma once

#include <memory>
#include "msg/slam/SlamResult.h"

namespace map_srvs {

struct RotateParamSet
{
    RotateParamSet() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
        std::uint64_t mapId;
    } request;

    struct Response
    {
        slam_msgs::SlamResult pose;
        float worldYaw;
        bool needRotateMap;
        bool success;
    } response;
};

}
