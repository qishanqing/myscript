#pragma once

#include <mind_os/serialization/serialization.h>

#include <cstdint>
#include <string>
// 纹理亮度控制
namespace sensor_srvs {
struct ImseeTxSrv {
  ImseeTxSrv() {}
  struct Request {
    int value;  //纹理亮度值
    std::uint64_t timeStamp;
  } request;

  struct Response {
    bool result;  // true 连接成功， false 连接失败
    std::uint64_t timeStamp;
  } response;
};
}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::ImseeTxSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.value, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ImseeTxSrv::Request& data) {
  deserialize(buffer, data.value);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::ImseeTxSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.result, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ImseeTxSrv::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */
