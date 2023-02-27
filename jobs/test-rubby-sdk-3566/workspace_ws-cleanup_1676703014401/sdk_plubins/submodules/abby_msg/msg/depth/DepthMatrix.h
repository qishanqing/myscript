#pragma once

#include <opencv2/core.hpp>
#include <mind_os/serialization/serialization.h>
#include <msg/common/base/CvMat.h>

namespace depth_msgs {
    struct DepthMatrix {
        std::uint64_t frame_id;
        std::uint64_t timestamp;

        enum {
            NONE,} flag;

        cv::Mat data;
        cv::Mat gray;
        float rotation[4];  // (w, x, y, z)
        float position[3];  // (x, y, z)
        cv::Mat trans;
    };
}

namespace mind_os
{
    template<>
    inline void serialize(const depth_msgs::DepthMatrix& data, BytesBuffer& buffer)
    {
        serialize(data.frame_id, buffer);
        serialize(data.timestamp, buffer);
        serialize(data.flag,  buffer);
        serialize(data.data, buffer);
        serialize(data.gray, buffer);
        serialize(data.rotation, 4, buffer);
        serialize(data.position, 3, buffer);
        serialize(data.trans, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_msgs::DepthMatrix& data)
    {
        deserialize(buffer, data.frame_id);
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.flag);
        deserialize(buffer, data.data);
        deserialize(buffer, data.gray);
        deserialize(buffer, data.rotation, 4);
        deserialize(buffer, data.position, 3);
        deserialize(buffer, data.trans);
    }
}


