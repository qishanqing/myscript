#pragma once

#include <cstdint>
#include <vector>
#include <mind_os/serialization/serialization.h>

namespace slam_msgs {

struct SlamResult {
    std::uint64_t s_time;
    float s_rotation[4];  // w, x, y, z
    float s_position[3];
    int s_state;
    bool relocal_success;
    bool closure_success;
    bool normal;
};

}  // namespace slam_msgs

namespace mind_os
{
    template<>
    inline void serialize(const slam_msgs::SlamResult& data, BytesBuffer& buffer)
    {
        serialize(data.s_time, buffer);
        serialize(data.s_rotation, 4, buffer);
        serialize(data.s_position, 3, buffer);
        serialize(data.s_state, buffer);
        serialize(data.relocal_success, buffer);
        serialize(data.closure_success, buffer);
        serialize(data.normal, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_msgs::SlamResult& data)
    {
        deserialize(buffer, data.s_time);
        deserialize(buffer, data.s_rotation, 4);
        deserialize(buffer, data.s_position, 3);
        deserialize(buffer, data.s_state);
        deserialize(buffer, data.relocal_success);
        deserialize(buffer, data.closure_success);
        deserialize(buffer, data.normal);
    }
}
