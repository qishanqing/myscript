#pragma once

namespace nav_srvs {
    struct Resume
    {
        struct Request
        {
            std::uint64_t timestamp;
        } request;

        struct Response
        {
            bool result = false;
        } response;
    };
}



