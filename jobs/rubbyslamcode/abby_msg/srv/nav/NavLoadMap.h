#pragma once

namespace nav_srvs {
    struct NavLoadMap
    {
        struct Request
        {
            std::uint64_t timestamp;
			char* loadMapPath;    // 保存历史地图的地址
        } request;

        struct Response
        {
            bool result;  // true成功, false失败
        } response;
    };
}

