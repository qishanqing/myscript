/*
 * WakeupSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace slam_srvs {

struct WakeupSlamSrv
{
    struct Request
    {
       std::uint64_t timestamp;
    } request;

    struct Response
    {
        bool success;
    } response;


};

} //namespace slam_srvs 

namespace mind_os
{
    inline void serialize(const slam_srvs::WakeupSlamSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    inline void deserialize(BytesBuffer& buffer, slam_srvs::WakeupSlamSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    inline void serialize(const slam_srvs::WakeupSlamSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.success, buffer);
    }

    inline void deserialize(BytesBuffer& buffer, slam_srvs::WakeupSlamSrv::Response& data)
    {
        deserialize(buffer, data.success);
    }
}
