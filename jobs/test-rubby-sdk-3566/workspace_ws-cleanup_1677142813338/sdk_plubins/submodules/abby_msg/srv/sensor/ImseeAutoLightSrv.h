#pragma once

#include <mind_os/serialization/serialization.h>

#include <cstdint>
#include <string>

// 纹理亮度控制
namespace sensor_srvs {
struct ImseeAutoLightSrv {
  ImseeAutoLightSrv() {}
  struct Request {
    bool autoLight;  //自动调亮度, true 使能， false 禁用
    std::uint64_t timeStamp;
  } request;

  struct Response {
    bool result;  // true 成功 ， false 失败
    std::uint64_t timeStamp;
  } response;
};
}  // namespace sensor_srvs
namespace mind_os {
template <>
inline void serialize(const sensor_srvs::ImseeAutoLightSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.autoLight, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ImseeAutoLightSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.autoLight);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::ImseeAutoLightSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.result, buffer);
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ImseeAutoLightSrv::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */
