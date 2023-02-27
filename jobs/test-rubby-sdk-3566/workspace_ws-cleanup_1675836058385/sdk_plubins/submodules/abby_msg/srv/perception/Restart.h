//////////////////////////////////////////////////////////////////////
///  @file     restart.h
///  @brief    perception restart service
///  Details.
///
///  @author   liujiaxing
///  @version  1.0.0
///  @date     2022.10.20
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#pragma once
#include <cstdint>

namespace perception_srvs 
{
struct Restart
{
    struct Request
    {
        std::uint64_t time;
    } request;

    struct Response
    {
        bool status;
    } response;
};

}// namespace perception 
