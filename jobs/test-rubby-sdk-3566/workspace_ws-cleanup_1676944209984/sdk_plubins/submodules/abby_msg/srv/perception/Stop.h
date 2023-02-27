//////////////////////////////////////////////////////////////////////
///  @file     stop.h
///  @brief    perception stop service
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
#include <mind_os/serialization/serialization.h>

namespace perception_srvs 
{
struct Stop
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

namespace mind_os
{
    template <>
    inline void serialize(const perception_srvs::Stop::Request& data, BytesBuffer &buffer)
    {
        serialize(data.time, buffer);
    } /* serialization of perception_srvs::Stop Request */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Stop::Request& data)
    {
        deserialize(buffer, data.time);
    } /* de-serialization of perception_srvs::Stop Request */

    template <>
    inline void serialize(const perception_srvs::Stop::Response& data, BytesBuffer &buffer)
    {
        serialize(data.status, buffer);
    } /* serialization of perception_srvs::Stop Response */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Stop::Response& data)
    {
        deserialize(buffer, data.status);
    } /* de-serialization of perception_srvs::Stop Response */

    template <>
    inline void serialize(const perception_srvs::Stop& data, BytesBuffer &buffer)
    {
        serialize(data.request, buffer);
        serialize(data.response, buffer);
    } /* serialization of perception_srvs::Stop */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Stop& data)
    {
        deserialize(buffer, data.request);
        deserialize(buffer, data.response);
    } /* de-serialization of perception_srvs::Stop */
};