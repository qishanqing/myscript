#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
namespace sensor_srvs {

/**
 * @brief 使用方法
 *      AP 热点模式
 *              a. 需要指定 ssid  psk
 *      STA 模式：
 *              a. 如果 ssid 为空则默认链接已保存网络
 *              b. 如果 psk 为空则默认链接已保存的对应 ssid 的网络
 *              c. 如果 ssid 和 psk 都不未空默认先排查是否已有对应网络配置，如果有配置则直接连接，如果没有配置或链接失败则重新按照新的ssid psk 进行链接
 */

struct WifiConfigSrv {
  WifiConfigSrv() {}

  struct Request {
    enum WIFI_STATE {
      OFF = 0,
      ON = 1,
    };
    enum WIFI_MODE {
      STA = 1,  //设备可以连接热点上网
      AP = 2,   //设备打开设点，可以被连接
    };

    WIFI_MODE wifiMode;  // wifi工作模式
    WIFI_STATE cmd;      // on;  off;
    std::string ssid;    // wifi名
    std::string psk;     // wifi密码
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
inline void serialize(const sensor_srvs::WifiConfigSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.wifiMode, buffer);
  serialize(data.cmd, buffer);
  serialize(data.ssid, buffer);
  serialize(data.psk, buffer);
  serialize(data.timeStamp, buffer);

}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::WifiConfigSrv::Request& data) {
  deserialize(buffer, data.wifiMode);
  deserialize(buffer, data.cmd);
  deserialize(buffer, data.ssid);
  deserialize(buffer, data.psk);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::WifiConfigSrv::Response& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.result, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::WifiConfigSrv::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */








