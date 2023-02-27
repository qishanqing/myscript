#pragma once

#include <iostream>
#include <map>
#include <mind_os/serialization/serialization.h>
namespace sensor_srvs {
enum SelfTestResult {
  ALLSUCCESS = 0,
  IMUFAILED,
  IMGFAILED,
  IMUAUXIFAILED,
  CHARGEPOSITIONFAILED,
  MOTORCURRENTFAILED,
  CLIFFFAILED,
  EMERGENCYFAILED,
  BATTERYFAILED,
  LIGHTFLOWFAILED,
  ENCODERFAILED,
  VELOCITYFAILED,
  WALLSENSORFAILED,
  ALL

};
struct SelfTest {
  struct Request {
    std::uint64_t timeStamp;
  } request;
  struct Response {
    uint8_t selfTest[ALL];
    std::uint64_t timeStamp;  // ms
  };
};

}  // namespace sensor_srvs

namespace mind_os {
template <>
inline void serialize(const sensor_srvs::SelfTest::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);

}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SelfTest::Request& data) {
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::SelfTest::Response& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
  serialize(data.selfTest,sensor_srvs::SelfTestResult::ALL, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SelfTest::Response& data) {
  deserialize(buffer, data.selfTest,sensor_srvs::SelfTestResult::ALL);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */




