#pragma once

#include <cstdint>

struct TestSrv
{
    struct Request
    {
        std::uint64_t stamp;
        int param;
    } request;

    struct Response
    {
        bool success;
    } response;
};
