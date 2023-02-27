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
#include <mind_os/serialization/serialization.h>

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

namespace mind_os
{
    template <>
    inline void serialize(const perception_srvs::RotateMap::Request& data, BytesBuffer &buffer)
    {
        serialize(data.angle, buffer);
    } /* serialization of perception_srvs::RotateMap Request */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::RotateMap::Request& data)
    {
        deserialize(buffer, data.angle);
    } /* de-serialization of perception_srvs::RotateMap Request */

    template <>
    inline void serialize(const perception_srvs::RotateMap::Response& data, BytesBuffer &buffer)
    {
        serialize(data.status, buffer);
    } /* serialization of perception_srvs::RotateMap Response */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::RotateMap::Response& data)
    {
        deserialize(buffer, data.status);
    } /* de-serialization of perception_srvs::RotateMap Response */

    template <>
    inline void serialize(const perception_srvs::RotateMap& data, BytesBuffer &buffer)
    {
        serialize(data.request, buffer);
        serialize(data.response, buffer);
    } /* serialization of perception_srvs::RotateMap */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::RotateMap& data)
    {
        deserialize(buffer, data.request);
        deserialize(buffer, data.response);
    } /* de-serialization of perception_srvs::RotateMap */
};