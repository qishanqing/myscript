#pragma once

#include "../../msg/nav/OptimizeMapMsg.h"
#include <mind_os/serialization/serialization.h>

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

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::OptimizeMap::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::OptimizeMap::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const nav_srvs::OptimizeMap::Response& data, BytesBuffer& buffer)
    {
        serialize(data.map, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::OptimizeMap::Response& data)
    {
        deserialize(buffer, data.map);
    }
}