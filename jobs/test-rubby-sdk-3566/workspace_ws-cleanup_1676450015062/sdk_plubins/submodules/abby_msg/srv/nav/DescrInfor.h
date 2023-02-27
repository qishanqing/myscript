#pragma once

#include "../../msg/nav/DescrInfoMsg.h"
#include <mind_os/serialization/serialization.h>

namespace nav_srvs {
    struct DescrInfor
    {
        struct Request
        {
            std::uint64_t timestamp;
        } request;

        struct Response
        {
            nav_msgs::DescrInfo descrInfo;  // 描述信息
        } response;
    };
}

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::DescrInfor::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::DescrInfor::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const nav_srvs::DescrInfor::Response& data, BytesBuffer& buffer)
    {
        serialize(data.descrInfo, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::DescrInfor::Response& data)
    {
        deserialize(buffer, data.descrInfo);
    }
}