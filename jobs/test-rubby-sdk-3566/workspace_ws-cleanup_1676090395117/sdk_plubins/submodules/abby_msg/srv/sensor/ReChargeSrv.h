#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
namespace sensor_srvs {

struct ReChargeSrv {
  ReChargeSrv() {}

  enum {
    START_SEND_DATA = 0u,  //开始
    STOP_SEND_DATA = 1u,   //停止
  };

  struct Request {
    std::uint64_t timeStamp;
    uint8_t rechargeCtrl;
  } request;

  struct Response {
    std::uint64_t timeStamp;
    bool result;  // true 连接成功， false 连接失败
  } response;
};
}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::ReChargeSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.rechargeCtrl, buffer);

}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ReChargeSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.rechargeCtrl);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::ReChargeSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.result, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ReChargeSrv::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */

