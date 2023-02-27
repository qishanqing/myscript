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

struct InitDepthSrv
{

    struct Request
    {
        std::uint64_t timestamp;
        std::string cfgPath;
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
    inline void serialize(const depth_srvs::InitDepthSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.cfgPath, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_srvs::InitDepthSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.cfgPath);
    }

    template<>
    inline void serialize(const depth_srvs::InitDepthSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.success, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_srvs::InitDepthSrv::Response& data)
    {
        deserialize(buffer, data.success);
    }
}
