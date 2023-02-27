/*
 * StartNavSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once

#include "../../msg/slam/SlamResult.h"

namespace slam_srvs {

struct GetPoseSrv
{
    GetPoseSrv() {}

    struct Request
    {
        std::uint64_t timestamp;

    } request;

    struct Response
    {
        slam_msgs::SlamResult pose;
    } response;

};

} //namespace slam_srvs