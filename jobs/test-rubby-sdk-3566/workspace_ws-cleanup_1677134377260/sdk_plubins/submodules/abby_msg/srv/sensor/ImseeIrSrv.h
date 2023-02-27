#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
//补光亮度控制
namespace sensor_srvs {
struct ImseeIrSrv {
  ImseeIrSrv() {}
  struct Request {
    int value;  //补光亮度值
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
inline void serialize(const sensor_srvs::ImseeIrSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.value, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ImseeIrSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.value);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::ImseeIrSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.result, buffer);
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ImseeIrSrv::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */



