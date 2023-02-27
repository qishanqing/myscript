#pragma once

#include "../../msg/nav/DescrInfoMsg.h"

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