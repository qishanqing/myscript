#pragma once

#include "../../msg/nav/OptimizeMapMsg.h"

namespace nav_srvs {
    struct OptimizeMap
    {
        struct Request
        {
            std::uint64_t timestamp;
        } request;

        struct Response
        {
			nav_msgs::OptimizeMapMsg map;
        } response;
    };
}
