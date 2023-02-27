#pragma once

#include <cstdint>
#include <vector>
#include <mind_os/serialization/serialization.h>

namespace slam_msgs {

struct RelocationMapID
{
    std::uint64_t timestamp;
    std::uint64_t mapID;
 };

}  // namespace slam_msgs

namespace mind_os
{
    template<>
    inline void serialize(const slam_msgs::RelocationMapID& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.mapID, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_msgs::RelocationMapID& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.mapID);
    }
}
