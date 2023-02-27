#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace nav_srvs {
    struct RegionLimited
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
    inline void serialize(const nav_srvs::RegionLimited::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::RegionLimited::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const nav_srvs::RegionLimited::Response& data, BytesBuffer& buffer)
    {

    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::RegionLimited::Response& data)
    {

    }
}

