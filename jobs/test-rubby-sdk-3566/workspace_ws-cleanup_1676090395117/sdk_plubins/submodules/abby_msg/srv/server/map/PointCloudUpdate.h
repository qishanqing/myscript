#pragma once

#include "PointCloud.h"

namespace map_srvs {

struct PointCloudUpdate
{
    PointCloudUpdate() = default;

    struct Request
    {
        PointCloud map;
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
inline void serialize(const map_srvs::PointCloudUpdate::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.map);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudUpdate::Request& data) {
    deserialize(buffer,data.map);
};

template<>
inline void serialize(const map_srvs::PointCloudUpdate::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.success);
    serialize(buffer,data.map);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudUpdate::Response& data) {
    deserialize(buffer,data.success);
    deserialize(buffer,data.map);
};

} /* namespace mind_os */
