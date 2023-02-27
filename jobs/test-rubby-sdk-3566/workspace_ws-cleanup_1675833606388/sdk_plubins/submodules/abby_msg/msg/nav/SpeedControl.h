#pragma once

#include <cstdint>

namespace nav_msgs {

struct SpeedControl
    {
        int Id;/*帧数*/
        float Speed_Linear;/*线速度*/
        float Speed_Angle;/*角速度*/
        std::uint64_t TimeStamp;
    };

}
