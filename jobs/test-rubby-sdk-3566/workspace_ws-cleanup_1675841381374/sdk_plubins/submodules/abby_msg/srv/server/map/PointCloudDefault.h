#pragma once

// #include <mind_os/serialization/serialization.h>

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

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::PointCloudDefault::Request& data, BytesBuffer& buffer) {
//     buffer << data.timestamp;
//     buffer << data.sceneId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudDefault::Request& data) {
//     buffer >> data.timestamp;
//     buffer >> data.sceneId;
// };

// template<>
// inline void serialize(const map_srvs::PointCloudDefault::Response& data, BytesBuffer& buffer) {
//     buffer << data.mapState;
//     buffer << data.mapId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloudDefault::Response& data) {
//     buffer >> data.mapState;
//     buffer >> data.mapId;
// };

// } /* namespace mind_os */
