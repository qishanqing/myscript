#pragma once

#include <memory>
#include "msg/slam/SlamResult.h"
#include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct RotateParamSet
{
    RotateParamSet() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
        std::uint64_t mapId;
    } request;

    struct Response
    {
        slam_msgs::SlamResult pose;
        float worldYaw;
        bool needRotateMap;
        bool success;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::RotateParamSet::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.timestamp);
    serialize(buffer,data.sceneId);
    serialize(buffer,data.mapId);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::RotateParamSet::Request& data) {
    deserialize(buffer,data.timestamp);
    deserialize(buffer,data.sceneId);
    deserialize(buffer,data.mapId);
};

template<>
inline void serialize(const map_srvs::RotateParamSet::Response& data, BytesBuffer& buffer) {
    //serialize(buffer,data.pose);
    serialize(data.pose,buffer);
    serialize(buffer,data.worldYaw);
    serialize(buffer,data.needRotateMap);
    serialize(buffer,data.success);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::RotateParamSet::Response& data) {
    deserialize(buffer,data.pose);
    deserialize(buffer,data.worldYaw);
    deserialize(buffer,data.needRotateMap);
    deserialize(buffer,data.success);
};

} /* namespace mind_os */
