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
            Request()
            {
                logAddress = nullptr;
            }
            ~Request()
            {
                if(logAddress != nullptr)
                {
                    delete[] logAddress;
                }
            }
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
        serialize(strlen(data.logAddress), buffer);
        serialize(data.logAddress, strlen(data.logAddress) + 1, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::Start::Request& data)
    {
        deserialize(buffer, data.timestamp);
        size_t size;
        deserialize(buffer, size);
        data.logAddress = new char[size];
        deserialize(buffer, data.logAddress, size);
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