#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {

// 沿边传感器
struct WallSensorMsg {
  enum {
    LEFT = 0u,
    RIGHT = 1u,
  };

  uint32_t frameId;      // 帧号
  uint16_t distance[2];  // 距离，单位mm
  uint64_t timeStamp;
};

}  // namespace sensor_msgs
namespace mind_os {
template <>
inline void serialize(const sensor_msgs::WallSensorMsg& data, BytesBuffer& buffer) {
   serialize(data.distance, 2,buffer);
   serialize(data.frameId, buffer);
   serialize(data.timeStamp, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::WallSensorMsg& data) {
   deserialize(buffer,data.distance,2);
   deserialize(buffer,data.frameId);
   deserialize(buffer,data.timeStamp);
};
}  // namespace mind_os

