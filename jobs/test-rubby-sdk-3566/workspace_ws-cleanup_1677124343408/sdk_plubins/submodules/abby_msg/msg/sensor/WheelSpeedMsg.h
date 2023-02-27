#pragma once

#include <cstddef>
#include <cstdint>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {

struct WheelSpeedMsg {
  uint32_t frameId;  // 帧号
  float leftSpeed;   // 左轮速度
  float rightSpeed;  // 右轮速度
  std::uint64_t timeStamp;
};
}  // namespace sensor_msgs

namespace mind_os {
template <>
inline void serialize(const sensor_msgs::WheelSpeedMsg& data, BytesBuffer& buffer) {
   serialize(data.frameId, buffer);
   serialize(data.timeStamp, buffer);
   serialize(data.leftSpeed, buffer);
   serialize(data.rightSpeed, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::WheelSpeedMsg& data) {
   deserialize(buffer,data.frameId);
   deserialize(buffer,data.timeStamp);
   deserialize(buffer,data.leftSpeed);
   deserialize(buffer,data.rightSpeed);
};
}  // namespace mind_os

