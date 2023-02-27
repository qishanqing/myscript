//////////////////////////////////////////////////////////////////////
///  @file     init.h
///  @brief    perception init service
///  Details.
///
///  @author   sunhao
///  @version  1.0.0
///  @date     2022.03.31
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#pragma once
#include <cstdint>
#include "../../msg/perception/SenmaticMap.h"
#include <mind_os/serialization/serialization.h>

namespace perception_srvs 
{
struct Init
{
    struct Request
    {
        std::string configPath;
    } request;

    struct Response
    {
        bool status;
    } response;
};

}// namespace perception 

namespace mind_os
{
    template <>
    inline void serialize(const perception_srvs::Init::Request& data, BytesBuffer &buffer)
    {
        serialize(data.configPath, buffer);
    } /* serialization of perception_srvs::Init Request */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Init::Request& data)
    {
        deserialize(buffer, data.configPath);
    } /* de-serialization of perception_srvs::Init Request */

    template <>
    inline void serialize(const perception_srvs::Init::Response& data, BytesBuffer &buffer)
    {
        serialize(data.status, buffer);
    } /* serialization of perception_srvs::Init Response */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Init::Response& data)
    {
        deserialize(buffer, data.status);
    } /* de-serialization of perception_srvs::Init Response */

    template <>
    inline void serialize(const perception_srvs::Init& data, BytesBuffer &buffer)
    {
        serialize(data.request, buffer);
        serialize(data.response, buffer);
    } /* serialization of perception_srvs::Init */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Init& data)
    {
        deserialize(buffer, data.request);
        deserialize(buffer, data.response);
    } /* de-serialization of perception_srvs::Init */
};