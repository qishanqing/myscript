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

namespace mind_os
{
    template<>
    inline void serialize(const slam_srvs::GetPoseSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::GetPoseSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const slam_srvs::GetPoseSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.pose, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::GetPoseSrv::Response& data)
    {
        deserialize(buffer, data.pose);
    }
}
