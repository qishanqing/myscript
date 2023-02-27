/*
 * StartSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <vector>
#include "../../msg/slam/SlamResult.h"
#include <mind_os/serialization/serialization.h>

namespace slam_srvs {

struct StartSlamSrv
{
    enum Mode {
        NoExistMap,
        ExistMap
    };

    struct Request
    {
        Mode mode;
        std::string map_path;
        std::uint64_t sceneId;
        std::uint64_t mapId;
        std::uint64_t timestamp;
        bool useLastMap;
        slam_msgs::SlamResult initPose;
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
    inline void serialize(const slam_srvs::StartSlamSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.mode, buffer);
        serialize(data.map_path, buffer);
        serialize(data.sceneId, buffer);
        serialize(data.mapId, buffer);
        serialize(data.timestamp, buffer);
        serialize(data.useLastMap, buffer);
        serialize(data.initPose, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::StartSlamSrv::Request& data)
    {
        deserialize(buffer, data.mode);
        deserialize(buffer, data.map_path);
        deserialize(buffer, data.sceneId);
        deserialize(buffer, data.mapId);
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.useLastMap);
        deserialize(buffer, data.initPose);
    }

    template<>
    inline void serialize(const slam_srvs::StartSlamSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.success, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::StartSlamSrv::Response& data)
    {
        deserialize(buffer, data.success);
    }
}
