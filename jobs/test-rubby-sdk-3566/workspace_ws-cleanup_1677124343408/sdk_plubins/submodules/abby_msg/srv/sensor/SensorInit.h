#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
#include "msg/common/error/ErrorMessage.h"
namespace sensor_srvs {

struct SensorInit {
  SensorInit() {}
  struct ImseeConfig {
    int imgWitdth = 640, imgHeight = 400, imgFreq = 25, imuFreq = 200;
  };
  struct Request {
    std::string sensorConfigPath;  // sensor config path
    ImseeConfig imseeConfig;       //配置双目图像大小 img频率 imu频率
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
inline void serialize(const sensor_srvs::SensorInit::ImseeConfig& data, BytesBuffer& buffer) {
  serialize(data.imgWitdth, buffer);
  serialize(data.imgHeight, buffer);
  serialize(data.imgFreq, buffer);
  serialize(data.imuFreq, buffer);

}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SensorInit::ImseeConfig& data) {
  deserialize(buffer, data.imgWitdth);
  deserialize(buffer, data.imgHeight);
  deserialize(buffer, data.imgFreq);
  deserialize(buffer, data.imuFreq);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */


template <>
inline void serialize(const sensor_srvs::SensorInit::Request& data, BytesBuffer& buffer) {
  serialize(data.imseeConfig, buffer);
  serialize(data.sensorConfigPath, buffer);

}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SensorInit::Request& data) {
  deserialize(buffer, data.imseeConfig);
  deserialize(buffer, data.sensorConfigPath);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::SensorInit::Response& data, BytesBuffer& buffer) {
  serialize(data.result, buffer);
  serialize(data.timeStamp, buffer);
  serialize(data.errMsg, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SensorInit::Response& data) {
  deserialize(buffer, data.result);
  deserialize(buffer, data.timeStamp);
  deserialize(buffer,data.errMsg);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */


