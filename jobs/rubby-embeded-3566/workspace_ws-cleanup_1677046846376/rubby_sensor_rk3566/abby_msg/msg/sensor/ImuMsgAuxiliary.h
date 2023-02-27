#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {

struct ImuMsgAuxiliary {
  /** IMU frame sensorId */
  uint32_t frameId;
  /** IMU accelerometer data for 3-axis: X, Y, Z. */
  float accel[3];
  /** IMU gyroscope data for 3-axis: X, Y, Z. */
  float gyro[3];
  /** IMU timeStamp */
  std::uint64_t timeStamp;
};

}  // namespace sensor_msgs
namespace mind_os {
template <>
inline void serialize(const sensor_msgs::ImuMsgAuxiliary& data, BytesBuffer& buffer) {
   serialize(data.frameId, buffer);
   serialize(data.timeStamp, buffer);
   serialize(data.accel,3, buffer);
   serialize(data.gyro, 3,buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::ImuMsgAuxiliary& data) {
   deserialize(buffer,data.frameId);
   deserialize(buffer,data.timeStamp);
   deserialize(buffer,data.accel,3 );
   deserialize(buffer,data.gyro, 3);
};
}  // namespace mind_os