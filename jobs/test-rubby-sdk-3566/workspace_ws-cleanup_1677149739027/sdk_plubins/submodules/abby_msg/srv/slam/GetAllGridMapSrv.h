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
#include <mind_os/serialization/serialization.h>

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

namespace mind_os
{
    template<>
    inline void serialize(const slam_srvs::GetAllGridMapSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::GetAllGridMapSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const slam_srvs::GetAllGridMapSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.allgridmap, buffer);
        serialize(data.success, buffer);
    }
    
    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::GetAllGridMapSrv::Response& data)
    {
        deserialize(buffer, data.allgridmap);
        deserialize(buffer, data.success);
    }
}
