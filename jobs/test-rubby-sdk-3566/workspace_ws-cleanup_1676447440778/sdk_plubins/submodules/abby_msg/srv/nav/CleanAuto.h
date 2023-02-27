//
// Created by lcy on 23-2-13.
//

#ifndef RUBBYNAV_CLEANAUTO_H
#define RUBBYNAV_CLEANAUTO_H

#include <cstdint>
#include "../../msg/nav/SegmentInfoMsg.h"

#include <mind_os/serialization/serialization.h>

namespace nav_srvs
{
    struct CleanAuto
    {
        struct Request
        {
            std::uint64_t timestamp;
            nav_msgs::SegmentInfoMsg info;
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
    inline void serialize(const nav_srvs::CleanAuto::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.info, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::CleanAuto::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.info);
    }

    template<>
    inline void serialize(const nav_srvs::CleanAuto::Response& data, BytesBuffer& buffer)
    {
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::CleanAuto::Response& data)
    {
        deserialize(buffer, data.result);
    }
}

#endif //RUBBYNAV_CLEANAUTO_H
