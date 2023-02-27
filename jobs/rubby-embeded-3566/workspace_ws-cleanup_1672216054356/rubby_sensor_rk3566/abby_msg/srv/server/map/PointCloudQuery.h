#pragma once

#include "PointCloud.h"

namespace map_srvs {

struct PointCloudQuery
{
    PointCloudQuery() = default;

    struct Request
    {
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        std::vector<PointCloud> maps;
        bool success;
    } response;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::PointCloudQuery::Request& data, BytesBuffer& buffer) {
//     buffer << data.sceneId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudQuery::Request& data) {
//     buffer >> data.sceneId;
// };

// template<>
// inline void serialize(const map_srvs::PointCloudQuery::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
//     serializeVector(data.maps, buffer);
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudQuery::Response& data) {
//     buffer >> data.success;
//     deserializeVector(buffer, data.maps);
// };

// } /* namespace mind_os */
