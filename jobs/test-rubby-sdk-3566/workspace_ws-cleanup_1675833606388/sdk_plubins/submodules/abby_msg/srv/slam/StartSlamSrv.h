/*
 * StartSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <vector>
#include "../../msg/slam/SlamResult.h"

namespace slam_srvs {

struct StartSlamSrv
{
    enum Mode {
        NoExistMap,
        ExistMap
    };

    struct Request
    {
        Mode mode;
        std::string map_path;
        std::uint64_t sceneId;
        std::uint64_t mapId;
        std::uint64_t timestamp;
        bool useLastMap;
        slam_msgs::SlamResult initPose;
    } request;

    struct Response
    {
        bool success;
    } response;


};

} //namespace slam_srvs 