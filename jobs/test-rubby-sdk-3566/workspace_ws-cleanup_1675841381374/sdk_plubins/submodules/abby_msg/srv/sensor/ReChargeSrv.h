#pragma once

#include <cstdint>
#include <string>

namespace sensor_srvs {

struct ReChargeSrv {
  ReChargeSrv() {}

  enum {
    START_SEND_DATA = 0u,  //开始
    STOP_SEND_DATA = 1u,   //停止
  };

  struct Request {
    std::uint64_t timeStamp;
    uint8_t rechargeCtrl;
  } request;

  struct Response {
    std::uint64_t timeStamp;
    bool result;  // true 连接成功， false 连接失败
  } response;
};
}  // namespace sensor_srvs