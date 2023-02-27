#pragma once

#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <mind_os/serialization/serialization.h>
#include "../../msg/common/base/CvPoint2f.h"
namespace nav_srvs
{
    struct CleanRegion
    {
        struct Request
        {
            std::uint64_t timestamp;
            std::vector<std::vector<cv::Point2f>> areaForbiden; // 禁区点(2个)
            std::vector<std::vector<cv::Point2f>> wallForbiden; // 虚拟墙点(2个)
            std::vector<std::vector<cv::Point2f>> regionLimitFromApp; // 划区边界点(来源于APP)
            std::vector<std::vector<cv::Point2f>> regionLimitFromPerception; // 划区边界点(来源于深度学习)
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
    inline void serialize(const nav_srvs::CleanRegion::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(buffer, data.areaForbiden);
        serialize(buffer, data.wallForbiden);
        serialize(buffer, data.regionLimitFromApp);
        serialize(buffer, data.regionLimitFromPerception);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::CleanRegion::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.areaForbiden);
        deserialize(buffer, data.wallForbiden);
        deserialize(buffer, data.regionLimitFromApp);
        deserialize(buffer, data.regionLimitFromPerception);
    }

    template<>
    inline void serialize(const nav_srvs::CleanRegion::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::CleanRegion::Response& data)
    {
        deserialize(buffer, data.result);
    }
}