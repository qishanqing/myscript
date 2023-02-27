#pragma once

#include <cstdint>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <mind_os/serialization/serialization.h>
#include "../common/base/CvMat.h"
namespace sensor_msgs {
//双目数据与纹理灯控制
struct ImgMsg {
	//双目标定参数
  struct CImseeMsgParam {
    int imseeid;
    int len = 0;
    uint8_t param[8 * 1024];
  };
  enum TX_IR_STATE {
    TX_IR_OFF = 0u,  // 0：纹理和补光都不亮
    TX_ON = 1u,      // 1：纹理亮
    IR_ON = 2u,      // 2：补光亮
    TX_IR_ON = 3u    // 3：纹理补光同时亮
  };

  //纹理灯控制
  struct TxIrStatus {
    int txIrState;      // 1、3纹理亮， 0、2非纹理亮
    int irLightValue;   //补光灯亮度值 0～100
    int txLightValue;   //纹理亮度值 0～100
    int envLightValue;  //环境光亮度 单位lux
  };

  int imseeId;       // 模组ID号，非模组内部ID，类似于handle
  int cameraWidth;   // 图像宽度（width x 2）
  int cameraHeight;  // 图像高度
  int cameraFps;     // 图像帧率
  int cameraIndex;   // 图像当前帧号//原始值
  int imgSize;       // 图像大小
  int channel;       // 颜色通道，常数1
  float timeExpose;  // 曝光时间(ms)

  cv::Mat imageL;  // 左目图像数据 数据克隆
  cv::Mat imageR;  // 右目图像数据
  bool fillLight;
  std::uint64_t sysTimeStamp;
  std::uint64_t timeStamp;     // 时间us
  struct TxIrStatus txIrStatus;  // 纹理灯实时数据
};
}

namespace mind_os {
template <>
inline void serialize(const sensor_msgs::ImgMsg::CImseeMsgParam& data, BytesBuffer& buffer) {
   serialize(data.imseeid, buffer);
   serialize(data.len, buffer);
   serialize(data.param, 8*1024,buffer);

}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::ImgMsg::CImseeMsgParam& data) {
   deserialize(buffer, data.imseeid);
   deserialize(buffer, data.len);
   deserialize(buffer, data.param);
};

template <>
inline void serialize(const sensor_msgs::ImgMsg::TxIrStatus& data, BytesBuffer& buffer) {
   serialize(data.txIrState, buffer);
   serialize(data.irLightValue, buffer);
   serialize(data.txLightValue, buffer);
   serialize(data.envLightValue, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::ImgMsg::TxIrStatus& data) {
   deserialize(buffer, data.txIrState);
   deserialize(buffer, data.irLightValue);
   deserialize(buffer, data.txLightValue);
   deserialize(buffer, data.envLightValue);

};

template <>
inline void serialize(const sensor_msgs::ImgMsg& data, BytesBuffer& buffer) {
   serialize(data.fillLight, buffer);
   serialize(data.sysTimeStamp, buffer);
   serialize(data.txIrStatus, buffer);
   serialize(data.timeStamp, buffer);
   serialize(data.imageL, buffer);
   serialize(data.imageR, buffer);

   serialize(data.imseeId, buffer);
   serialize(data.cameraWidth, buffer);
   serialize(data.cameraHeight, buffer);
   serialize(data.cameraFps, buffer);
   serialize(data.cameraIndex, buffer);
   serialize(data.imgSize, buffer);
   serialize(data.channel, buffer);
   serialize(data.timeExpose, buffer);

}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::ImgMsg& data) {
   deserialize(buffer, data.fillLight);
   deserialize(buffer, data.sysTimeStamp);
   deserialize(buffer, data.txIrStatus);
   deserialize(buffer, data.timeStamp);
   deserialize(buffer, data.imageL);
   deserialize(buffer, data.imageR);

   deserialize(buffer, data.imseeId);
   deserialize(buffer, data.cameraWidth);
   deserialize(buffer, data.cameraHeight);
   deserialize(buffer, data.cameraFps);
   deserialize(buffer, data.cameraIndex);
   deserialize(buffer, data.imgSize);
   deserialize(buffer, data.channel);
   deserialize(buffer, data.timeExpose);

};






}  // namespace mind_os




