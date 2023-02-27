#pragma once

#include <cstdint>
#include <string>

namespace sensor_srvs
{
struct CpuIdSrv {
  CpuIdSrv() {}

  struct Request {
    std::uint64_t timeStamp;
  } request;

  struct Response {
    bool result; // true 成功， false 失败
    std::uint64_t timeStamp;
    std::string cpuId;  
  } response;
};
}  // namespace sensor_srvs