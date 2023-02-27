#pragma once

#include <opencv2/core.hpp>
#include <mind_os/serialization/serialization.h>
#include <msg/common/base/CvMat.h>

namespace depth_msgs {

struct CalibrationImage {
    std::uint64_t timestamp;
    cv::Mat left;
    cv::Mat right;
    float fillRate;
};
}

namespace mind_os
{
    template<>
    inline void serialize(const depth_msgs::CalibrationImage& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.left,  buffer);
        serialize(data.right, buffer);
        serialize(data.fillRate, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, depth_msgs::CalibrationImage& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.left);
        deserialize(buffer, data.right);
        deserialize(buffer, data.fillRate);
    }
}



