/*
 * StartDepthSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace depth_srvs {

struct StartDepthSrv
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

} //namespace depth_srvs 

namespace  mind_os
{
    template<>
    inline void serialize(const depth_srvs::StartDepthSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_srvs::StartDepthSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const depth_srvs::StartDepthSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.success, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_srvs::StartDepthSrv::Response& data)
    {
        deserialize(buffer, data.success);
    }
}