/*
 * StartNavSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once

#include "../../msg/slam/OccupancyGrid.h"
#include <cstdint>
#include <vector>
namespace slam_srvs {

struct GetAllGridMapSrv
{
    GetAllGridMapSrv() {}

    struct Request
    {
        std::uint64_t timestamp;

    } request;

    struct Response
    {
        slam_msgs::OccupancyGrid allgridmap;
        bool success;
    } response;

};

} //namespace slam_srvs