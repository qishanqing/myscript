#pragma once

#include <memory>
#include "msg/slam/SlamResult.h"
#include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct RotateIdSet
{
    RotateIdSet() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
        std::uint64_t mapId;
    } request;

    struct Response
    {
        bool success;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::RotateIdSet::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.timestamp);
    serialize(buffer,data.sceneId);
    serialize(buffer,data.mapId);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::RotateIdSet::Request& data) {
    deserialize(buffer,data.timestamp);
    deserialize(buffer,data.sceneId);
    deserialize(buffer,data.mapId);
};

template<>
inline void serialize(const map_srvs::RotateIdSet::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.success);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::RotateIdSet::Response& data) {
    deserialize(buffer,data.success);
};

} /* namespace mind_os */
