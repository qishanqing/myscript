#pragma once
#include <mind_os/serialization/serialization.h>
#include <cstdint>
namespace sensor_msgs {
  
enum USB_HOTPLUG_STATE{
  USB_INSERT = 0,
  USB_LEFT = 1,
};
class CImseeMsgHotplug {
 public:
  int imseeid;
  uint8_t bus;
  uint8_t addr;
  USB_HOTPLUG_STATE state;  // 1 拔出， 0 插入

 public:
  CImseeMsgHotplug() {}
  ~CImseeMsgHotplug() {}
};
}  // namespace sensor_msgs

namespace mind_os {
template <>
inline void serialize(const sensor_msgs::CImseeMsgHotplug& data, BytesBuffer& buffer) {
   serialize(data.imseeid, buffer);
   serialize(data.bus, buffer);
   serialize(data.addr, buffer);
   serialize(data.state, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::CImseeMsgHotplug& data) {
   deserialize(buffer, data.imseeid);
   deserialize(buffer, data.bus);
   deserialize(buffer, data.addr);
   deserialize(buffer, data.state);
};
}  // namespace mind_os


