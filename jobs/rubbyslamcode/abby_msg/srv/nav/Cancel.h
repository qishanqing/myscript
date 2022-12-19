#pragma once

namespace nav_srvs {
    struct Cancel
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



