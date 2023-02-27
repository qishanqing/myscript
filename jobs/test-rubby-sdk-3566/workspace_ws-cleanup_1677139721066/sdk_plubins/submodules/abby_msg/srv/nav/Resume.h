#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

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

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::Resume::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Resume::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const nav_srvs::Resume::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Resume::Response& data)
    {
        deserialize(buffer, data.result);
    }
}


