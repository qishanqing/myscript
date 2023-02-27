//////////////////////////////////////////////////////////////////////
///  @file     load_map.h
///  @brief    perception LoadMap service
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
struct LoadMap
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
    inline void serialize(const perception_srvs::LoadMap::Request& data, BytesBuffer &buffer)
    {
        serialize(data.file, buffer);
    } /* serialization of perception_srvs::LoadMap Request */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::LoadMap::Request& data)
    {
        deserialize(buffer, data.file);
    } /* de-serialization of perception_srvs::LoadMap Request */

    template <>
    inline void serialize(const perception_srvs::LoadMap::Response& data, BytesBuffer &buffer)
    {
        serialize(data.status, buffer);
    } /* serialization of perception_srvs::LoadMap Response */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::LoadMap::Response& data)
    {
        deserialize(buffer, data.status);
    } /* de-serialization of perception_srvs::LoadMap Response */

    template <>
    inline void serialize(const perception_srvs::LoadMap& data, BytesBuffer &buffer)
    {
        serialize(data.request, buffer);
        serialize(data.response, buffer);
    } /* serialization of perception_srvs::LoadMap */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_srvs::LoadMap& data)
    {
        deserialize(buffer, data.request);
        deserialize(buffer, data.response);
    } /* de-serialization of perception_srvs::LoadMap */
};