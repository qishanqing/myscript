//////////////////////////////////////////////////////////////////////
///  @file     rotate_map.h
///  @brief    perception RotateMap service
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
#include <string>

namespace perception_srvs 
{
struct RotateMap
{
    struct Request
    {
        float angle;
    } request;

    struct Response
    {
        bool status;
    } response;
};

}// namespace perception 
