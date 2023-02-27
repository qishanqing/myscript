#pragma once

namespace nav_srvs {
    struct RegionLimited
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


