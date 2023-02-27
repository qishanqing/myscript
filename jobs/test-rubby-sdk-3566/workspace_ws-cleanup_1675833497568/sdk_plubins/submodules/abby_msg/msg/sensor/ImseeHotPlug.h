#pragma once

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
