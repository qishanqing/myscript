/*
 * StartNavSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once

#include "../../msg/slam/SlamResult.h"
#include <mind_os/serialization/serialization.h>

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

namespace mind_os
{
    template<>
    inline void serialize(const slam_srvs::RotateSlamMapSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::RotateSlamMapSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const slam_srvs::RotateSlamMapSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.needRotateMap, buffer);
        serialize(data.pose, buffer);
        serialize(data.worldYaw, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::RotateSlamMapSrv::Response& data)
    {
        deserialize(buffer, data.needRotateMap);
        deserialize(buffer, data.pose);
        deserialize(buffer, data.worldYaw);
    }
    
}
