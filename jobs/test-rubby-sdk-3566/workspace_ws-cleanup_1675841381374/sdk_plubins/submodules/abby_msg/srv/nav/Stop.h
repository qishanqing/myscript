#pragma once
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

