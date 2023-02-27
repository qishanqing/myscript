/*
 * StartSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <vector>
#include <mind_os/serialization/serialization.h>
namespace slam_srvs {

struct ReStartSlamSrv
{
    struct Request
    {
        std::string map_path;
        std::uint64_t sceneId;
        std::uint64_t mapId;
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
    template<>
    inline void serialize(const slam_srvs::ReStartSlamSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.map_path, buffer);
        serialize(data.sceneId, buffer);
        serialize(data.mapId, buffer);
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::ReStartSlamSrv::Request& data)
    {
        deserialize(buffer, data.map_path);
        deserialize(buffer, data.sceneId);
        deserialize(buffer, data.mapId);
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const slam_srvs::ReStartSlamSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.success, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::ReStartSlamSrv::Response& data)
    {
        deserialize(buffer, data.success);
    }
}
