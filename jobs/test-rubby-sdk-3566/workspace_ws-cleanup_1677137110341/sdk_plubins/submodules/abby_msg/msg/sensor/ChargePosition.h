#pragma once

#include <mind_os/serialization/serialization.h>

#include <cstdint>
namespace sensor_msgs {

struct ChargePosition {
  enum LedCode {
    OUTSIDE_REGION = 0,  //区域之外；
    A_REGION = 1,        //区域A；
    B_REGION = 2,        //区域B；
    C_REGION = 4,        //区域C；
    E_REGION = 6,        //区域E；
    D_REGION = 8,        //区域D；
  };

  uint32_t frameId;         // 帧号
  uint8_t chargeSignal[4];  // buf[0] 后方左侧编码值；buf[1] 后方右侧编码值；buf[2] 左侧编码值；buf[3] 右侧编码值；
  std::uint64_t timeStamp;
};

}  // namespace sensor_msgs

namespace mind_os {
template <>
inline void serialize(const sensor_msgs::ChargePosition& data, BytesBuffer& buffer) {
  serialize(data.frameId, buffer);
  // serialize(data.chargeSignal, buffer);
  /************************序列化数组************************/
  serialize(data.chargeSignal, 4, buffer);
  /*********************************************************/
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::ChargePosition& data) {
  deserialize(buffer, data.frameId);
  // deserialize(buffer, data.chargeSignal);

  /*****************反序列化数组************************************/
  deserialize(buffer, data.chargeSignal, 4);
  /*************************************************************/

  deserialize(buffer, data.timeStamp);
};
}  // namespace mind_os
