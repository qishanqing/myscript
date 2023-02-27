#pragma once

#include <cstdint>

namespace sensor_msgs {
//固定帧率上报
struct EncoderMsg {
  uint32_t frameId;    //帧号
  int EncodeR;         //右边轮子编码器值
  int EncodeL;         //左边轮子编码器值
  uint64_t timeStamp;  // utc时间 单位 us
};
}  // namespace sensor_msgs
