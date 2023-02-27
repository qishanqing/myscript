#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace nav_srvs {
    struct Pause
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

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::Pause::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Pause::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const nav_srvs::Pause::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Pause::Response& data)
    {
        deserialize(buffer, data.result);
    }
}


