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

struct PauseSlamSrv
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
    inline void serialize(const slam_srvs::PauseSlamSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::PauseSlamSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const slam_srvs::PauseSlamSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.success, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::PauseSlamSrv::Response& data)
    {
        deserialize(buffer, data.success);
    }

}
