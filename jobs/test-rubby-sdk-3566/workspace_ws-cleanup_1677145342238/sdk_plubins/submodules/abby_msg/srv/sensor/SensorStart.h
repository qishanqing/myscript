#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
#include "msg/common/error/ErrorMessage.h"
namespace sensor_srvs {

struct SensorStart {
  SensorStart() {}

  struct Request {
  } request;

  struct Response {
    bool result;  // true 成功， false 失败
    std::uint64_t timeStamp;
    error_msgs::ErrorMessage errMsg;
  } response;
};

}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::SensorStart::Request& data, BytesBuffer& buffer) {


}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SensorStart::Request& data) {

}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::SensorStart::Response& data, BytesBuffer& buffer) {
  serialize(data.result, buffer);
  serialize(data.timeStamp, buffer);
  serialize(data.errMsg, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SensorStart::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.errMsg);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */



