#pragma once

#include <mind_os/serialization/serialization.h>

#include <cstdint>
namespace sensor_srvs {

struct MemorySrv {
  MemorySrv() {}

  struct Request {
    std::uint64_t timeStamp;
  } request;

  struct Response {
    float freeMem;   //剩余磁盘 单位： Gbyte
    float freeRam;   //剩余内存 单位： Gbyte
    float totalRam;  //总共内存 单位： Gbyte
    std::uint64_t timeStamp;
  } response;
};

}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::MemorySrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::MemorySrv::Request& data) {
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::MemorySrv::Response& data, BytesBuffer& buffer) {
  serialize(data.freeMem, buffer);
  serialize(data.timeStamp, buffer);
  serialize(data.freeRam, buffer);
  serialize(data.totalRam, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::MemorySrv::Response& data) {
  deserialize(buffer, data.freeMem);
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.freeRam);
  deserialize(buffer, data.totalRam);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */
