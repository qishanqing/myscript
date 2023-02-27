#pragma once

#include "PointCloud.h"

namespace map_srvs {

struct PointCloudNew
{
    PointCloudNew() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        bool success;
        PointCloud map;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::PointCloudNew::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.timestamp);
    serialize(buffer,data.sceneId);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudNew::Request& data) {
    deserialize(buffer, data.timestamp);
    deserialize(buffer, data.sceneId);
};

template<>
inline void serialize(const map_srvs::PointCloudNew::Response& data, BytesBuffer& buffer) {
    serialize(buffer, data.success);
    serialize(buffer, data.map);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudNew::Response& data) {
    deserialize(buffer, data.success);
    deserialize(buffer, data.map);
};

} /* namespace mind_os */
