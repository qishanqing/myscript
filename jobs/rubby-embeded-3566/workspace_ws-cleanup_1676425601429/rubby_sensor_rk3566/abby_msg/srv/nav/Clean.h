#pragma once

#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <mind_os/serialization/serialization.h>
#include "../../msg/common/base/CvPoint2f.h"
namespace nav_srvs
{
    struct Clean
    {
        struct Request
        {
            std::uint64_t timestamp;
            std::vector<std::vector<cv::Point2f>> areaLimit; // 禁区点 2
            std::vector<std::vector<cv::Point2f>> wallLimit; // 虚拟墙点 2
        } request;

        struct Response
        {
            bool result = false;
        } response;
    };
}

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::Clean::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.areaLimit, buffer);
        serialize(data.wallLimit, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Clean::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.areaLimit);
        deserialize(buffer, data.wallLimit);
    }

    template<>
    inline void serialize(const nav_srvs::Clean::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Clean::Response& data)
    {
        deserialize(buffer, data.result);
    }
}