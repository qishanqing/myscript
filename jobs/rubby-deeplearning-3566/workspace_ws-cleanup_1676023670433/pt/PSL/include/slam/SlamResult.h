#pragma once

#include "base/Time.h"
#include <vector>

namespace psl
{
struct SlamResult
{
    psl::Time s_time;
    float s_rotation[4];  // w, x, y, z
    float s_position[3];
    float delt_rotation[4];  // w, x, y, z
    float delt_position[3];
    float home_rotation[4];  // w, x, y, z
    float home_position[3];
    float s_state;
    bool relocal_success;
    bool closure_success;
    bool update_home;
    bool normal;

};
struct grid
{
    float x;
    float y;
    int8_t value;
};

struct OccupancyGrid
{
    psl::Time time;
    int width;
    int height;
    double origin_x;
    double origin_y;
    float resolution;
    std::vector<int8_t> data;
    std::vector<grid> grid_data;
};
}  // namespace psl
