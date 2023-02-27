#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

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

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::Init::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Init::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const nav_srvs::Init::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Init::Response& data)
    {
        deserialize(buffer, data.result);
    }
}
