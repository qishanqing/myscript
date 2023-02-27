#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>
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
namespace mind_os {
template <>
inline void serialize(const sensor_msgs::LightFlowMsg& data, BytesBuffer& buffer) {
   serialize(data.frameId, buffer);
   serialize(data.flowX, buffer);
   serialize(data.flowY, buffer);
   serialize(data.quality, buffer);
   serialize(data.workMode, buffer);
   serialize(data.timeStamp, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::LightFlowMsg& data) {
   deserialize(buffer,data.frameId);
   deserialize(buffer,data.flowX);
   deserialize(buffer,data.flowY);
   deserialize(buffer,data.quality);
   deserialize(buffer,data.workMode);
   deserialize(buffer,data.timeStamp);
};
}  // namespace mind_os