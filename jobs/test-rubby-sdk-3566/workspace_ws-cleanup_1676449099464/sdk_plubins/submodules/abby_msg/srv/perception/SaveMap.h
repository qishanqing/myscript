//////////////////////////////////////////////////////////////////////
///  @file     stop.h
///  @brief    perception SaveMap service
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
#include <string>
#include <mind_os/serialization/serialization.h>

namespace perception_srvs 
{
struct SaveMap
{
    struct Request
    {
        std::string file;
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
    inline void serialize(const perception_srvs::SaveMap::Request& data, BytesBuffer &buffer)
    {
        serialize(data.file, buffer);
    } /* serialization of perception_srvs::SaveMap Request */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::SaveMap::Request& data)
    {
        deserialize(buffer, data.file);
    } /* de-serialization of perception_srvs::SaveMap Request */

    template <>
    inline void serialize(const perception_srvs::SaveMap::Response& data, BytesBuffer &buffer)
    {
        serialize(data.status, buffer);
    } /* serialization of perception_srvs::SaveMap Response */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::SaveMap::Response& data)
    {
        deserialize(buffer, data.status);
    } /* de-serialization of perception_srvs::SaveMap Response */

    template <>
    inline void serialize(const perception_srvs::SaveMap& data, BytesBuffer &buffer)
    {
        serialize(data.request, buffer);
        serialize(data.response, buffer);
    } /* serialization of perception_srvs::SaveMap */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::SaveMap& data)
    {
        deserialize(buffer, data.request);
        deserialize(buffer, data.response);
    } /* de-serialization of perception_srvs::SaveMap */
};