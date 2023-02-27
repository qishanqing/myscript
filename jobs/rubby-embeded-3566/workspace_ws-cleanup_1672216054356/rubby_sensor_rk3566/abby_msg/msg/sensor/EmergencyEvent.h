#pragma once

#include <cstdint>

namespace sensor_msgs {

struct EmergencyEvent {
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
    STATUS = BIT0,
    LEFT_BUMPER = BIT1,
    RIGHT_BUMPER = BIT2,
    LEFT_CLIFF = BIT3,        //左
    RIGHT_CLIF = BIT4,        //右
    FRONT_LEFT_CLIF = BIT5,   //前左
    FRONT_RIGHT_CLIF = BIT6,  //前右,
  };

  enum {
    RELEASED = 0u,
    TRIGGER = 1u,
  };

  uint32_t frameId;        //帧号
  uint16_t brakeDistance;  //急停时行驶距离mm
  uint8_t brakeEvent;      //四个电机,位操作
  uint64_t timeStamp;
};

}  // namespace sensor_msgs