#pragma once

#include <cstdint>
#include <vector>

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
