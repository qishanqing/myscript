#pragma once

#include <cstdint>
#include <string>

namespace sensor_srvs {

struct MotorSrv {
  MotorSrv() {}

  enum {
    BIT0 = 0u,
    BIT1 = 1u,
    BIT2 = 2u,
    BIT3 = 3u,
    BIT4 = 4u,
    BIT5 = 5u,
    BIT6 = 6u,
    BIT7 = 7u,
  };

  enum {

    MAIN_BRUSH = 0u,   //主刷
    LEFT_BRUSH,        //左侧边刷
    RIGHT_BRUSH,       //右侧边刷
    DUST_SUCTION_FAN,  //吸尘风机
  };

  enum {
    RUN = 1u,   //电机运行
    STOP = 0u,  //电机停止
  };
  
  struct Request {
    std::uint64_t timeStamp;
    uint8_t motorSetting;
  } request;

  struct Response {
    std::uint64_t timeStamp;
    bool result;  // true 连接成功， false 连接失败
  } response;
};
}  // namespace sensor_srvs