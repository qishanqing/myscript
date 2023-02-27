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

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::PointCloudSave::Request& data, BytesBuffer& buffer) {
//     buffer << data.sceneId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudSave::Request& data) {
//     buffer >> data.sceneId;
// };

// template<>
// inline void serialize(const map_srvs::PointCloudSave::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
//     serializeVector(data.maps, buffer);
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudSave::Response& data) {
//     buffer >> data.success;
//     deserializeVector(buffer, data.maps);
// };

// } /* namespace mind_os */
