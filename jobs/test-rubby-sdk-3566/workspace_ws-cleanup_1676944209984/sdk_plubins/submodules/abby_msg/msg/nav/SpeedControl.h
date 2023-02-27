#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace nav_msgs {

struct SpeedControl
    {
        int Id;/*帧数*/
        float Speed_Linear;/*线速度*/
        float Speed_Angle;/*角速度*/
        std::uint64_t TimeStamp;
    };

}

namespace mind_os
{
    template <>
    inline void serialize(const nav_msgs::SpeedControl& data, BytesBuffer& buffer){
        serialize(data.Id, buffer);
        serialize(data.Speed_Linear, buffer);
        serialize(data.Speed_Angle, buffer);
        serialize(data.TimeStamp, buffer);
    }

    template <>
    inline void deserialize(BytesBuffer& buffer, nav_msgs::SpeedControl& data)
    {
        deserialize(buffer, data.Id);
        deserialize(buffer, data.Speed_Linear);
        deserialize(buffer, data.Speed_Angle);
        deserialize(buffer, data.TimeStamp);
    }
}