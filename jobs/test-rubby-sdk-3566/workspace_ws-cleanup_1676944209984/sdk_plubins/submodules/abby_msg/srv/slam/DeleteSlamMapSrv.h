/*
 * PauseSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace slam_srvs {

struct DeleteSlamMapSrv
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

} // namespace slam_srvs

namespace mind_os
{
    template<>
    inline void serialize(const slam_srvs::DeleteSlamMapSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::DeleteSlamMapSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const slam_srvs::DeleteSlamMapSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.success, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::DeleteSlamMapSrv::Response& data)
    {
        deserialize(buffer, data.success);
    }
}
