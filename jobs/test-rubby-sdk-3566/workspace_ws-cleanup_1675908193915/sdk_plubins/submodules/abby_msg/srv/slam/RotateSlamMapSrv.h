/*
 * StartNavSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once

#include "../../msg/slam/SlamResult.h"

namespace slam_srvs {

struct RotateSlamMapSrv
{
    struct Request
    {
        std::uint64_t timestamp;
    } request;

    struct Response
    {
        bool needRotateMap;
        slam_msgs::SlamResult pose;
        float worldYaw;
    } response;

};

} //namespace slam_srvs