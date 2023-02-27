#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>
#include <string>
namespace nav_srvs {
    struct NavSaveMap
    {
        struct Request
        {
            std::uint64_t timestamp;
			std::string saveMapPath;    // 保存历史地图的地址
            float rotation[4];   // 地图旋转角度四元数（w, x, y, z）

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
    inline void serialize(const nav_srvs::NavSaveMap::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.saveMapPath, buffer);
        serialize(data.rotation, 4, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::NavSaveMap::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.saveMapPath);
        deserialize(buffer, data.rotation, 4);
    }

    template<>
    inline void serialize(const nav_srvs::NavSaveMap::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::NavSaveMap::Response& data)
    {
        deserialize(buffer, data.result);
    }
}

