//
// Created by lcy on 23-2-10.
//

#ifndef RUBBYNAV_SEGMENTOPERATION_H
#define RUBBYNAV_SEGMENTOPERATION_H

#include <cstdint>
#include "../../msg/nav/SegmentInfoMsg.h"
#include "../../msg/nav/SegmentOperationMsg.h"

#include <mind_os/serialization/serialization.h>

namespace nav_srvs {
    struct SegmentOperation
    {
        struct Request
        {
            std::uint64_t timestamp;
            nav_msgs::SegmentOperationMsg info;
        } request;

        struct Response
        {
            nav_msgs::SegmentInfoMsg info;
            bool result;  // true成功, false失败
        } response;
    };
}

namespace mind_os
{
    template<>
    inline void serialize(const nav_srvs::SegmentOperation::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.info, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::SegmentOperation::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.info);
    }

    template<>
    inline void serialize(const nav_srvs::SegmentOperation::Response& data, BytesBuffer& buffer)
    {
        serialize(data.info, buffer);
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::SegmentOperation::Response& data)
    {
        deserialize(buffer, data.info);
        deserialize(buffer, data.result);
    }
}
#endif //RUBBYNAV_SEGMENTOPERATION_H
