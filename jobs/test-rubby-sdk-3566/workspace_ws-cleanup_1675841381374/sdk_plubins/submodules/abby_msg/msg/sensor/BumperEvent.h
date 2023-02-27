#pragma once

#include <cstdint>

namespace sensor_msgs {

struct BumperEvent {
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
    LEFT = BIT0,        //左侧bumper状态
    RIGHT = BIT1,       //右侧bumper状态
    LEFT_RIGHT = BIT2,  //左右两侧bumper状态
  };

  uint32_t frameId;  //帧号
  uint8_t status;    // 1:接触 0:弹开
  uint64_t timeStamp;
};

}  // namespace sensor_msgs