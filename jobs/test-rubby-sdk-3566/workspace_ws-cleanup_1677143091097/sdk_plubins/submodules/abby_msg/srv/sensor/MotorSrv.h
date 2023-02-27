#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
namespace sensor_srvs {
/*
外设通用接口，目前主要是电机的控制
motorSetting 掩码定义清洁方式，置0表示无效，置1表示有效位。定义参考wiki：
http://192.168.50.191:8090/pages/viewpage.action?pageId=50758988
*/
struct MotorSrv {
  MotorSrv() {}

  struct Request {
    std::uint64_t timeStamp;
    uint16_t motorSetting;
  } request;

  struct Response {
    std::uint64_t timeStamp;
    bool result;  // true 连接成功， false 连接失败
  } response;
};
}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::MotorSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.motorSetting, buffer);

}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::MotorSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.motorSetting);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::MotorSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.result, buffer);
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::MotorSrv::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */

