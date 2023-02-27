#pragma once

#include <cstdint>

namespace sensor_msgs {

struct BatteryEvent {
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
    NOT_CHARGING = BIT0,     //未充电
    BATTERY_LOW = BIT1,      //电池电量低
    CHARGING = BIT2,         //充电中
    CHARGE_COMPLETE = BIT3,  //充满电
  };

  uint32_t frameId;       //帧号
  uint8_t batteryStatus;  //电池状态
  uint8_t batteryLevel;  //电池电量 0-100
  uint64_t timeStamp;
};

}  // namespace sensor_msgs