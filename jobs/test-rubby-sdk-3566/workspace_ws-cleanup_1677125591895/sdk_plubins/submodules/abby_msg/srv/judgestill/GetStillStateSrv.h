/*
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once

#include <cstdint>

namespace still_srvs {

struct GetStillStateSrv
{
    GetStillStateSrv() {}

    struct Request
    {
        std::uint64_t timestamp;

    } request;

    struct Response
    {
        std::uint64_t finalStateTime;
        bool isStill;
    } response;

};

} //namespace still_srvs

namespace mind_os
{
    template<>
    inline void serialize(const still_srvs::GetStillStateSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, still_srvs::GetStillStateSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
    }

    template<>
    inline void serialize(const still_srvs::GetStillStateSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.finalStateTime, buffer);
        serialize(data.isStill, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, still_srvs::GetStillStateSrv::Response& data)
    {
        deserialize(buffer, data.finalStateTime);
        deserialize(buffer, data.isStill);
    }
}
