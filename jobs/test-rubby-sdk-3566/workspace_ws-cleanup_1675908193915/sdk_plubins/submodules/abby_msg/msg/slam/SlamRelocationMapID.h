#pragma once

#include <cstdint>
#include <vector>

namespace slam_msgs {

struct RelocationMapID
{
    std::uint64_t timestamp;
    std::uint64_t mapID;
 };

}  // namespace slam_msgs
