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
#include <mind_os/serialization/serialization.h>

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

namespace mind_os
{
    template <>
    inline void serialize(const perception_srvs::Restart::Request& data, BytesBuffer &buffer)
    {
        serialize(data.time, buffer);
    } /* serialization of perception_srvs::Restart Request */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Restart::Request& data)
    {
        deserialize(buffer, data.time);
    } /* de-serialization of perception_srvs::Restart Request */

    template <>
    inline void serialize(const perception_srvs::Restart::Response& data, BytesBuffer &buffer)
    {
        serialize(data.status, buffer);
    } /* serialization of perception_srvs::Restart Response */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Restart::Response& data)
    {
        deserialize(buffer, data.status);
    } /* de-serialization of perception_srvs::Restart Response */

    template <>
    inline void serialize(const perception_srvs::Restart& data, BytesBuffer &buffer)
    {
        serialize(data.request, buffer);
        serialize(data.response, buffer);
    } /* serialization of perception_srvs::Restart */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::Restart& data)
    {
        deserialize(buffer, data.request);
        deserialize(buffer, data.response);
    } /* de-serialization of perception_srvs::Restart */
};