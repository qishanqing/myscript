#pragma once

#include <mind_os/serialization/serialization.h>

#include <cstdint>
namespace sensor_msgs {
//具体内容待定
struct WifiStatus {
  std::uint8_t connectStatus;  // wifi连接状态
  std::uint8_t wifiStatus;     // wifi指示灯状态
  std::uint64_t timeStamp;
};

}  // namespace sensor_msgs
namespace mind_os {
template <>
inline void serialize(const sensor_msgs::WifiStatus& data, BytesBuffer& buffer) {
  serialize(data.connectStatus, buffer);
  serialize(data.timeStamp, buffer);
  serialize(data.wifiStatus, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::WifiStatus& data) {
  deserialize(buffer, data.connectStatus);
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.wifiStatus);
};
}  // namespace mind_os
