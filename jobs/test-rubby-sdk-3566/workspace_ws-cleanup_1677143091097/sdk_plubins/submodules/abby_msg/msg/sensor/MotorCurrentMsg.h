#pragma once

#include <cstdint>
#include <string>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {

struct MotorCurrentMsg {
  struct MotorCurrentVal {
    float mainBrushMotorCurrent;        //主刷电机
    float leftBrushMotorsCurrent;       //左边刷电机
    float rightBrushMotorsCurrent;      //右边刷电机
    float dustSuctionFanMotorsCurrent;  //吸尘风机电机
    float leftWheelMotorsCurrent;       //左轮电机
    float rightWheelMotorsCurrent;      //右轮电机
  };
  uint32_t frameId;  // 帧号
  MotorCurrentVal MotorCurrent;
  uint64_t timeStamp;
};

}  // namespace sensor_msgs
namespace mind_os {

template <>
inline void serialize(const sensor_msgs::MotorCurrentMsg::MotorCurrentVal& data, BytesBuffer& buffer) {
   serialize(data.mainBrushMotorCurrent, buffer);
   serialize(data.leftBrushMotorsCurrent, buffer);
   serialize(data.rightBrushMotorsCurrent, buffer);
   serialize(data.dustSuctionFanMotorsCurrent, buffer);
   serialize(data.leftWheelMotorsCurrent, buffer);
   serialize(data.rightWheelMotorsCurrent, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::MotorCurrentMsg::MotorCurrentVal& data) {
   deserialize(buffer,data.mainBrushMotorCurrent);
   deserialize(buffer,data.leftBrushMotorsCurrent);
   deserialize(buffer,data.rightBrushMotorsCurrent);
   deserialize(buffer,data.dustSuctionFanMotorsCurrent);
   deserialize(buffer,data.leftWheelMotorsCurrent);
   deserialize(buffer,data.rightWheelMotorsCurrent);
};


template <>
inline void serialize(const sensor_msgs::MotorCurrentMsg& data, BytesBuffer& buffer) {
   serialize(data.frameId, buffer);
   serialize(data.MotorCurrent, buffer);
   serialize(data.timeStamp, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::MotorCurrentMsg& data) {
   deserialize(buffer,data.frameId);
   deserialize(buffer,data.MotorCurrent);
   deserialize(buffer,data.timeStamp);
};
}  // namespace mind_os

