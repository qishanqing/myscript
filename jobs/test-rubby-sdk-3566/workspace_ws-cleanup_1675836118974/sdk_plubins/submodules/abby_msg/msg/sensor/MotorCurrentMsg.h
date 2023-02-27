#pragma once

#include <cstdint>
#include <string>

namespace sensor_msgs {

struct MotorCurrentMsg {
  struct MotorCurrent {
    float mainBrushMotorCurrent;        //主刷电机
    float leftBrushMotorsCurrent;       //左边刷电机
    float rightBrushMotorsCurrent;      //右边刷电机
    float dustSuctionFanMotorsCurrent;  //吸尘风机电机
    float leftWheelMotorsCurrent;       //左轮电机
    float rightWheelMotorsCurrent;      //右轮电机
  };
  uint32_t frameId;  // 帧号
  MotorCurrent MotorCurrent;
  uint64_t timeStamp;
};

}  // namespace sensor_msgs