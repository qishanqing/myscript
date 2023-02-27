#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace sensor_srvs {

struct CpuTemperatureSrv {
  CpuTemperatureSrv() {}

  struct Request {
    std::uint64_t timeStamp;
  } request;

  struct Response {
    float temperature;  //总共内存 单位： 摄氏度
    std::uint64_t timeStamp;
  } response;
};

}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::CpuTemperatureSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::CpuTemperatureSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::CpuTemperatureSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.temperature, buffer);
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::CpuTemperatureSrv::Response& data) {
  deserialize(buffer, data.temperature);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */

