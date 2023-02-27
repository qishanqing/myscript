#pragma once

#include <cstdint>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

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

