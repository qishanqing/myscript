#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {

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
  RELEASED = 0u,
  TRIGGER = 1u,
};

enum {
  LEFT = BIT0,         //左
  RIGHT = BIT1,        //右
  FRONT_LEFT = BIT2,   //前左
  FRONT_RIGHT = BIT3,  //前右
};

struct CliffEvent {
  uint32_t frameId;     // 帧号
  uint8_t cliffStatus;  // 哪个cliff触发
  std::uint64_t timeStamp;
};
}  // namespace sensor_msgs


namespace mind_os {
template <>
inline void serialize(const sensor_msgs::CliffEvent& data, BytesBuffer& buffer) {
   serialize(data.frameId, buffer);
   serialize(data.cliffStatus, buffer);
   serialize(data.timeStamp, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::CliffEvent& data) {
   deserialize(buffer, data.frameId);
   deserialize(buffer, data.cliffStatus);
   deserialize(buffer, data.timeStamp);
};
}  // namespace mind_os


