#pragma once

// #include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct PointCloudDel
{
    PointCloudDel() = default;

    struct Request
    {
        std::uint64_t mapId;
    } request;

    struct Response
    {
        bool success;
    } response;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::PointCloudDel::Request& data, BytesBuffer& buffer) {
//     buffer << data.mapId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudDel::Request& data) {
//     buffer >> data.mapId;
// };

// template<>
// inline void serialize(const map_srvs::PointCloudDel::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudDel::Response& data) {
//     buffer >> data.success;
// };

// } /* namespace mind_os */
