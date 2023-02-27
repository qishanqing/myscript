#pragma once

#include <cstdint>
#include <vector>

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
