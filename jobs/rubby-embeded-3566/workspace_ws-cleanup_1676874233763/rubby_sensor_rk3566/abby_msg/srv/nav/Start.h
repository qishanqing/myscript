#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace nav_srvs
{
    struct Start
    {
        struct Request
        {
            std::uint64_t timestamp;
            char *logAddress; /*log地址*/
        } request;

        struct Response
        {
            bool result; // true成功, false失败
        } response;
    };
}

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::Start::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.logAddress, strlen(data.logAddress), buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Start::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.logAddress, strlen(data.logAddress));
    }

    template<>
    inline void serialize(const nav_srvs::Start::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Start::Response& data)
    {
        deserialize(buffer, data.result);
    }
}