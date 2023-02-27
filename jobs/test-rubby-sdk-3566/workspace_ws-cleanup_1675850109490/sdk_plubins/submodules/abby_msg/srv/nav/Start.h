#pragma once

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
