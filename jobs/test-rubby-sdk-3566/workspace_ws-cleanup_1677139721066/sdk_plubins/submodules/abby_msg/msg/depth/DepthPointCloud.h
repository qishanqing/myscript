#pragma once

#include <vector>
#include <cstdint>
#include <mind_os/serialization/serialization.h>
#include <msg/common/base/CvMat.h>

namespace depth_msgs {

struct Point32 {
    float x;
    float y;
    float z;
};


struct DepthPointCloud {
    using Point = Point32;

    std::uint64_t frame_id;
    std::uint64_t timestamp;

    enum {
        NONE,
    } flag;

    float rotation[4];  // (w, x, y, z)
    float position[3];  // (x, y, z)

    float angle_min;
    float angle_max;
    float angle_increment;

    struct label_point{
    std::vector<Point> points;
    std::vector<char> label;
    };

    Point center;
    std::vector<label_point> data;
};
}

namespace mind_os
{
    template<>
    inline void serialize(const depth_msgs::Point32& data, BytesBuffer& buffer)
    {
        serialize(data.x, buffer);
        serialize(data.y, buffer);
        serialize(data.z, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_msgs::Point32& data)
    {
        deserialize(buffer, data.x);
        deserialize(buffer, data.y);
        deserialize(buffer, data.z);
    }

    template<>
    inline void serialize(const depth_msgs::DepthPointCloud::label_point& data, BytesBuffer& buffer)
    {
        serialize(data.points, buffer);
        serialize(data.label, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_msgs::DepthPointCloud::label_point& data)
    {
        deserialize(buffer, data.points);
        deserialize(buffer, data.label);
    }

    template<>
    inline void serialize(const depth_msgs::DepthPointCloud& data, BytesBuffer& buffer)
    {
        serialize(data.frame_id, buffer);
        serialize(data.timestamp, buffer);
        serialize(data.flag,  buffer);
        serialize(data.rotation, 4, buffer);
        serialize(data.position, 3, buffer);
        serialize(data.angle_min,  buffer);
        serialize(data.angle_max,  buffer);
        serialize(data.angle_increment,  buffer);
        serialize(data.center, buffer);
        serialize(data.data, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_msgs::DepthPointCloud& data)
    {
        deserialize(buffer, data.frame_id);
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.flag);
        deserialize(buffer, data.rotation, 4);
        deserialize(buffer, data.position, 3);
        deserialize(buffer, data.angle_min);
        deserialize(buffer, data.angle_max);
        deserialize(buffer, data.angle_increment);
        deserialize(buffer, data.center);
        deserialize(buffer, data.data);
    }
}



