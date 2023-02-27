#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {
//固定帧率上报
struct EncoderMsg {
  uint32_t frameId;    //帧号
  int EncodeR;         //右边轮子编码器值
  int EncodeL;         //左边轮子编码器值
  uint64_t timeStamp;  // utc时间 单位 us
};
}  // namespace sensor_msgs

namespace mind_os {
template <>
inline void serialize(const sensor_msgs::EncoderMsg& data, BytesBuffer& buffer) {
   serialize(data.frameId, buffer);
   serialize(data.EncodeR, buffer);
   serialize(data.EncodeL, buffer);
   serialize(data.timeStamp, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::EncoderMsg& data) {
   deserialize(buffer, data.frameId);
   deserialize(buffer, data.EncodeR);
   deserialize(buffer, data.EncodeL);
   deserialize(buffer, data.timeStamp);
};
}  // namespace mind_os

