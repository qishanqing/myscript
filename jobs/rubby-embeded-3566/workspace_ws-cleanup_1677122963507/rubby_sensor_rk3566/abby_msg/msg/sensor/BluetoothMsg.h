#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {

struct BluetoothMsg
{
  std::uint8_t connectStatus; //连接状态
  std::uint8_t ledStatus;     // 指示灯状态
  std::uint64_t timeStamp;
};

}

namespace mind_os {
template <>
inline void serialize(const sensor_msgs::BluetoothMsg& data, BytesBuffer& buffer) {
   serialize(data.connectStatus, buffer);
   serialize(data.ledStatus, buffer);
   serialize(data.timeStamp, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::BluetoothMsg& data) {
   deserialize(buffer, data.connectStatus);
   deserialize(buffer, data.ledStatus);
   deserialize(buffer, data.timeStamp);
};
}  // namespace mind_os