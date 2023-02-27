#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace nav_srvs {
    struct Stop
    {
        struct Request
        {
            std::uint64_t timestamp;
        } request;

        struct Response
        {
            bool result = false;  //true成功, false失败
        } response;
    };
}

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::Stop::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Stop::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const nav_srvs::Stop::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Stop::Response& data)
    {
        deserialize(buffer, data.result);
    }
}
