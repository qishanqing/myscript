#pragma once

#include <cstdint>

namespace sensor_msgs {

struct KeyEvent {
  uint32_t frameId;   //帧号
  //http://192.168.50.191:8090/pages/resumedraft.action?draftId=39357565&draftShareId=5f17ab3a-8842-4a21-96e0-9018a03f10a2&
  uint8_t keyStatus;  //位操作
  uint64_t timeStamp;
};

}  // namespace sensor_msgs