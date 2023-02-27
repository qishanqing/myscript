#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>
#include "../../msg/common/base/CvPoint2f.h"
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

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::NavLoadMap::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.loadMapPath, strlen(data.loadMapPath), buffer);
        serialize(buffer, data.areaLimit);
        serialize(buffer, data.wallLimit);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::NavLoadMap::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.loadMapPath, strlen(data.loadMapPath));
        deserialize(buffer, data.areaLimit);
        deserialize(buffer, data.wallLimit);
    }

    template<>
    inline void serialize(const nav_srvs::NavLoadMap::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::NavLoadMap::Response& data)
    {
        deserialize(buffer, data.result);
    }
}
