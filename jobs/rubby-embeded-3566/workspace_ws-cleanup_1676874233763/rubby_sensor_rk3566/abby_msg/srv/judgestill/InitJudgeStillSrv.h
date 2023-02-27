/*
 * StartSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <vector>
#include <mind_os/serialization/serialization.h>
namespace still_srvs {

struct InitJudgeStillSrv
{
    struct Request
    {
        std::uint64_t timestamp;
    } request;

    struct Response
    {
        bool success;
    } response;
};

} //namespace still_srvs 

namespace mind_os
{
   template<>
   inline void serialize(const still_srvs::InitJudgeStillSrv::Request& data, BytesBuffer& buffer)
   {
       serialize(data.timestamp, buffer);
   }

   template<>
   inline void deserialize(BytesBuffer& buffer, still_srvs::InitJudgeStillSrv::Request& data)
   {
       deserialize(buffer, data.timestamp);
   }

   template<>
   inline void serialize(const still_srvs::InitJudgeStillSrv::Response& data, BytesBuffer& buffer)
   {
       serialize(data.success, buffer);
   }

   template<>
   inline void deserialize(BytesBuffer& buffer, still_srvs::InitJudgeStillSrv::Response& data)
   {
       deserialize(buffer, data.success);
   }
}
