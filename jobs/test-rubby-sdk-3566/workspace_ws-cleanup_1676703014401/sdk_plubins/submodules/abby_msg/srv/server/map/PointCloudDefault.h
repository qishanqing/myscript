#pragma once

#include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct PointCloudDefault
{
    enum MapEnum{
        NewMap,
        Default,
    };

    PointCloudDefault() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        MapEnum mapState;
        std::uint64_t mapId;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::PointCloudDefault::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.timestamp);
    serialize(buffer,data.sceneId);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudDefault::Request& data) {
    deserialize(buffer,data.timestamp);
    deserialize(buffer,data.sceneId);
};

template<>
inline void serialize(const map_srvs::PointCloudDefault::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.mapState);
    serialize(buffer,data.mapId);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudDefault::Response& data) {
    deserialize(buffer,data.mapState);
    deserialize(buffer,data.mapId);
};

} /* namespace mind_os */
