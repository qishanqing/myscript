#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

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

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::Cancel::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Cancel::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const nav_srvs::Cancel::Response& data, BytesBuffer& buffer)
    {
        
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Cancel::Response& data)
    {
        
    }
}


