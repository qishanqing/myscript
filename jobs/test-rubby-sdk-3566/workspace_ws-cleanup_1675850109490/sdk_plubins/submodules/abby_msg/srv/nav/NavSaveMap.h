#pragma once

namespace nav_srvs {
    struct NavSaveMap
    {
        struct Request
        {
            std::uint64_t timestamp;
			char* saveMapPath;    // 保存历史地图的地址
            float rotation[4];   // 地图旋转角度四元数（w, x, y, z）
        } request;

        struct Response
        {
            bool result;  // true成功, false失败
        } response;
    };
}

