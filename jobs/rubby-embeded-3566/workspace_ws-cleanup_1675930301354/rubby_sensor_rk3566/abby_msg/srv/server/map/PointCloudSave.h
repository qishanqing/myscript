#pragma once

#include "PointCloud.h"

namespace map_srvs {

struct PointCloudSave
{
    PointCloudSave() = default;

    struct Request
    {
        PointCloud map;
    } request;

    struct Response
    {
        bool success;
    } response;
};


}

namespace mind_os {
template<>
inline void serialize(const map_srvs::PointCloudSave::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.map);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudSave::Request& data) {
    deserialize(buffer,data.map);
};

template<>
inline void serialize(const map_srvs::PointCloudSave::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.success);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudSave::Response& data) {
    deserialize(buffer,data.success);
};

} /* namespace mind_os */
