#pragma once

#include <cstdint>

namespace sensor_msgs {
//具体内容待定
struct WifiStatus {
  std::uint8_t connectStatus;  // wifi连接状态
  std::uint8_t wifiStatus;     // wifi指示灯状态
  std::uint64_t timeStamp;
};

}  // namespace sensor_msgs