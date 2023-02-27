#pragma once
#include <vector>
#include <opencv2/highgui/highgui.hpp>

namespace nav_srvs {
    struct NavLoadMap
    {
        struct Request
        {
            std::uint64_t timestamp;
			char* loadMapPath;    // 保存历史地图的地址
            std::vector<std::vector<cv::Point2f>> areaLimit; // 禁区点 2
            std::vector<std::vector<cv::Point2f>> wallLimit; // 虚拟墙点 2
        } request;

        struct Response
        {
            bool result;  // true成功, false失败
        } response;
    };
}

