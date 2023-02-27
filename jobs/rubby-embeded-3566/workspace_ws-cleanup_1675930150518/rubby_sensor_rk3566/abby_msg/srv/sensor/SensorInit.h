#pragma once

#include <cstdint>
#include <string>
#include "msg/common/error/ErrorMessage.h"
namespace sensor_srvs
{

  struct SensorInit
  {
    SensorInit() {}
    struct ImseeConfig{
      int imgWitdth = 640, imgHeight = 400,imgFreq = 25,imuFreq = 200;
    };
    struct Request
    {
      std::string sensorConfigPath; //sensor config path
      ImseeConfig imseeConfig;  //配置双目图像大小 img频率 imu频率 
    } request;

    struct Response
    {
      bool result; // true 成功， false 失败
      std::uint64_t timeStamp;
      error_msgs::ErrorMessage errMsg;
    } response;
  };

} // namespace sensor_srvs