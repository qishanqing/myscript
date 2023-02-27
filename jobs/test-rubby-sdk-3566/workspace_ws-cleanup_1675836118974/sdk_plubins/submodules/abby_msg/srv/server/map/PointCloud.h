#pragma once

// #include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct PointCloud
{
    std::uint64_t   id;
    std::uint64_t   sceneId;
    std::uint64_t   mapId;
    std::uint32_t   useCount;
    std::string     image;
    std::string     thumbnail;
    std::uint64_t   date;
    std::uint64_t   lastDate;
    bool valid;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::PointCloud& data, BytesBuffer& buffer) {
//     buffer << data.id;
//     buffer << data.sceneId;
//     buffer << data.useCount;
//     buffer << data.image;
//     buffer << data.thumbnail;
//     buffer << data.date;
//     buffer << data.lastDate;
//     buffer << data.valid;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloud& data) {
//     buffer >> data.id;
//     buffer >> data.sceneId;
//     buffer >> data.useCount;
//     buffer >> data.image;
//     buffer >> data.thumbnail;
//     buffer >> data.date;
//     buffer >> data.lastDate;
//     buffer >> data.valid;
// };

// } /* namespace mind_os */
