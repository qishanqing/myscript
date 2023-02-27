#pragma once

namespace nav_srvs {
    struct Init
    {
        struct Request
        {
            std::uint64_t timestamp;
        } request;

        struct Response
        {
            bool result;  // true成功, false失败
        } response;
    };
}

