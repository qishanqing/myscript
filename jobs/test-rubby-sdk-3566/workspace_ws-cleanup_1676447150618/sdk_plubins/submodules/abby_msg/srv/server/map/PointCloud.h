#pragma once

#include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct PointCloud
{
    std::uint64_t   id;
    std::uint64_t   sceneId;
    std::uint32_t   useCount;
    std::string     image;
    std::string     thumbnail;
    std::uint64_t   date;
    std::uint64_t   lastDate;
    bool valid;
};

}

namespace mind_os {
template<>
inline void serialize(BytesBuffer& buffer,const map_srvs::PointCloud& data) {
    serialize(buffer,data.id);
    serialize(buffer,data.sceneId);
    serialize(buffer,data.useCount);
    serialize(buffer,data.image);
    serialize(buffer,data.thumbnail);
    serialize(buffer,data.date);
    serialize(buffer,data.lastDate);
    serialize(buffer,data.valid);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::PointCloud& data) {
    deserialize(buffer, data.id);
    deserialize(buffer, data.sceneId);
    deserialize(buffer, data.useCount);
    deserialize(buffer, data.image);
    deserialize(buffer, data.thumbnail);
    deserialize(buffer, data.date);
    deserialize(buffer, data.lastDate);
    deserialize(buffer, data.valid);
};

} /* namespace mind_os */
