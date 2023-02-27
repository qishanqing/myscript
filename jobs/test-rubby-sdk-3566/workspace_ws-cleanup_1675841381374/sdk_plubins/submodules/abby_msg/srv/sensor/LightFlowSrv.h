#pragma once

#include <cstdint>
#include <string>

namespace sensor_srvs {

struct LightFlowSrv {
  LightFlowSrv() {}
  struct Request {
    std::uint64_t timeStamp;
    int workMode; //光流工作模式,值为1 LD模式,值为2 LED模式
  } request;

  struct Response {
    std::uint64_t timeStamp;
    bool result; // true 连接成功， false 连接失败
  } response;
};
} // namespace sensor_srvs