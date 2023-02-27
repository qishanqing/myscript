#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {

struct KeyEvent {
  uint32_t frameId;   //帧号
  uint8_t keyStatus;  //位操作
  uint64_t timeStamp;
};

}  // namespace sensor_msgs
namespace mind_os {
template <>
inline void serialize(const sensor_msgs::KeyEvent& data, BytesBuffer& buffer) {
   serialize(data.frameId, buffer);
   serialize(data.timeStamp, buffer);
   serialize(data.keyStatus, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::KeyEvent& data) {
   deserialize(buffer,data.frameId);
   deserialize(buffer,data.timeStamp);
   deserialize(buffer,data.keyStatus);
};
}  // namespace mind_os