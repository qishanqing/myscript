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

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::PointCloudNew::Request& data, BytesBuffer& buffer) {
//     buffer << data.timestamp;
//     buffer << data.sceneId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudNew::Request& data) {
//     buffer >> data.timestamp;
//     buffer >> data.sceneId;
// };

// template<>
// inline void serialize(const map_srvs::PointCloudNew::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
//     serialize(data.map, buffer);
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudNew::Response& data) {
//     buffer >> data.success;
//     deserialize(buffer, data.map);
// };

// } /* namespace mind_os */
