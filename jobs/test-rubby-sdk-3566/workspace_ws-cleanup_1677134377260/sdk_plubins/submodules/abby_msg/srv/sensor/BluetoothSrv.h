#pragma once

#include <mind_os/serialization/serialization.h>

#include <cstdint>
#include <string>
namespace sensor_srvs {

struct BluetoothSrv {
  BluetoothSrv() {}

  struct Request {
    std::uint64_t timeStamp;
  } request;

  struct Response {
    std::uint64_t timeStamp;
    bool result;  // true 连接成功， false 连接失败
  } response;
};
}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::BluetoothSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::BluetoothSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::BluetoothSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.result, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::BluetoothSrv::Response& data) {
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.result);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */
