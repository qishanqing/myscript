#pragma once

#include <cstdint>

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