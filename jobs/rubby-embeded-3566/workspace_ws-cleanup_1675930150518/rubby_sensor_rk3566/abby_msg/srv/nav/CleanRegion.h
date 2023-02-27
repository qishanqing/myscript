#pragma once

#include <vector>
#include <opencv2/highgui/highgui.hpp>

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
