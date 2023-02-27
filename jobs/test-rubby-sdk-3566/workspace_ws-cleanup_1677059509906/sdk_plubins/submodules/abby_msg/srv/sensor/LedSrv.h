#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
namespace sensor_srvs {

struct LedSrv {
  LedSrv() {}
  enum {
    BIT0 = 0u,
    BIT1 = 1u,
    BIT2 = 2u,
    BIT3 = 3u,
    BIT4 = 4u,
    BIT5 = 5u,
    BIT6 = 6u,
    BIT7 = 7u,
  };

  enum {
    LED_OFF = BIT0,          //熄灯
    POWER_ON = BIT1,         //上电过程结束后熄
    NETWORK_OFFLINE = BIT2,  //未联网
    NETWORK_ONLINE = BIT3,   //联网成功
    BATTERY_LOW = BIT4,      //电压低
    CHARGING = BIT5,         //充电中
    CHARGE_COMPLETE = BIT6,  //充满电
    FAULT = BIT7,            //故障
  };

  enum {
    RELEASED = 0u,
    TRIGGER = 1u,
  };

  struct Request {
    std::uint64_t timeStamp;
    uint8_t ledStatus;  //位操作
  } request;

  struct Response {
    std::uint64_t timeStamp;
    bool result;  // true 连接成功， false 连接失败
  } response;
};
}  // namespace sensor_srvs
namespace mind_os {
template <>
inline void serialize(const sensor_srvs::LedSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.ledStatus, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::LedSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
  deserialize(buffer, data.ledStatus);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::LedSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.result, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::LedSrv::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */



