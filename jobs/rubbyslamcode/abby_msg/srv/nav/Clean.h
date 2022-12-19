#pragma once

#include <vector>
#include <opencv2/highgui/highgui.hpp>

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
