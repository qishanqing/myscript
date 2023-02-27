#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace slam_msgs {

struct SlamErrorInfo {
    std::uint64_t timeStamp; 
};

}  // namespace slam_msgs

namespace mind_os
{
    template<>
    inline void serialize(const slam_msgs::SlamErrorInfo& data, BytesBuffer& buffer)
    {
        serialize(data.timeStamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_msgs::SlamErrorInfo& data)
    {
        deserialize(buffer, data.timeStamp);
    }
}

