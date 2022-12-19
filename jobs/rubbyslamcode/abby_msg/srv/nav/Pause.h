#pragma once

namespace nav_srvs {
    struct Pause
    {
        struct Request
        {
            std::uint64_t timestamp;
        } request;

        struct Response
        {
        } response;
    };
}



