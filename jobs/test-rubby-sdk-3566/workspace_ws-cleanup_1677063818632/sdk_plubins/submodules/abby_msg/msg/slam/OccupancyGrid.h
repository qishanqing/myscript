#pragma once

#include <cstdint>
#include <vector>

#include <mind_os/serialization/serialization.h>

namespace slam_msgs {

struct grid
{
    int x;
    int y;
    int8_t value;
};

struct OccupancyGrid
{
    std::uint64_t time;
    int width;
    int height;
    float resolution;
    std::vector<grid> grid_data;
};

}  // namespace slam_msgs

namespace mind_os
{
    template<>
    inline void serialize(const slam_msgs::grid& data, BytesBuffer& buffer)
    {
        serialize(data.x, buffer);
        serialize(data.y, buffer);
        serialize(data.value, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_msgs::grid& data)
    {
        deserialize(buffer, data.x);
        deserialize(buffer, data.y);
        deserialize(buffer, data.value);
    }

    template<>
    inline void serialize(const slam_msgs::OccupancyGrid& data, BytesBuffer& buffer)
    {
        serialize(data.time, buffer);
        serialize(data.width, buffer);
        serialize(data.height, buffer);
        serialize(data.resolution, buffer);
        serialize(data.grid_data, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_msgs::OccupancyGrid& data)
    {
        deserialize(buffer, data.time);
        deserialize(buffer, data.width);
        deserialize(buffer, data.height);
        deserialize(buffer, data.resolution);
        deserialize(buffer, data.grid_data);
    }
}
