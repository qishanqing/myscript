#pragma once

#include "PointCloud.h"

namespace map_srvs {

struct PointCloudQuery
{
    PointCloudQuery() = default;

    struct Request
    {
        std::uint64_t time;
    } request;

    struct Response
    {
        std::vector<PointCloud> maps;
        bool success;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::PointCloudQuery::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.time);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudQuery::Request& data) {
    deserialize(buffer,data.time);
};

template<>
inline void serialize(const map_srvs::PointCloudQuery::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.maps);
    serialize(buffer,data.success);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudQuery::Response& data) {
    deserialize(buffer,data.maps);
    deserialize(buffer,data.success);
};

} /* namespace mind_os */
