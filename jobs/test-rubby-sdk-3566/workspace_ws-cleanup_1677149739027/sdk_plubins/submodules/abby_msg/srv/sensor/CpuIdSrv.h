#pragma once

#include <mind_os/serialization/serialization.h>

#include <cstdint>
#include <string>
namespace sensor_srvs {
struct CpuIdSrv {
  CpuIdSrv() {}

  struct Request {
    std::uint64_t timeStamp;
  } request;

  struct Response {
    bool result;  // true 成功， false 失败
    std::uint64_t timeStamp;
    std::string cpuId;
  } response;
};
}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::CpuIdSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::CpuIdSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::CpuIdSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.result, buffer);
  serialize(data.timeStamp, buffer);
  serialize(data.cpuId, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::CpuIdSrv::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.cpuId);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */
