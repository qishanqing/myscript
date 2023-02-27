#pragma once

#include <cstdint>

namespace sensor_msgs {
struct LightFlowMsg {
  uint32_t frameId; // 帧号
  int16_t flowX;    // x轴方向数据
  int16_t flowY;    // y轴方向数据
  uint16_t quality; //光流质量
  int workMode;     //光流工作模式 值为1 LD模式,值为2 LED模式
  std::uint64_t timeStamp;
};

} // namespace sensor_msgs
