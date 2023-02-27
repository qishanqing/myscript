//
// Created by lcy on 23-2-10.
//

#ifndef RUBBYNAV_SEGMENTGETINFO_H
#define RUBBYNAV_SEGMENTGETINFO_H

#include <cstdint>
#include "../../msg/nav/SegmentInfoMsg.h"

#include <mind_os/serialization/serialization.h>

namespace nav_srvs {
    struct SegmentGetInfo
    {
        struct Request
        {
            std::uint64_t timestamp;
            char* MapPath;    //用于自动分区地图的地址
            Request()
            {
                MapPath = nullptr;
            }
            ~Request()
            {
                if(MapPath != nullptr)
                {
                    delete[] MapPath;
                }
            }
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
    inline void serialize(const nav_srvs::SegmentGetInfo::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(strlen(data.MapPath), buffer);
        serialize(data.MapPath, strlen(data.MapPath) + 1, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::SegmentGetInfo::Request& data)
    {
        deserialize(buffer, data.timestamp);
        size_t size;
        deserialize(buffer, size);
        data.MapPath = new char[size];
        deserialize(buffer, data.MapPath, size);
    } 

    template<>
    inline void serialize(const nav_srvs::SegmentGetInfo::Response& data, BytesBuffer& buffer)
    {
        serialize(data.info, buffer);
        serialize(data.result, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_srvs::SegmentGetInfo::Response& data)
    {
        deserialize(buffer, data.info);        
        deserialize(buffer, data.result);
    }
}
#endif //RUBBYNAV_SEGMENTGETINFO_H
