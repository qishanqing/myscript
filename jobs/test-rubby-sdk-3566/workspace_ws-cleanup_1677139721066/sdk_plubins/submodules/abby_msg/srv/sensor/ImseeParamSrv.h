#pragma once

#include <iostream>
#include <map>
#include <mind_os/serialization/serialization.h>
#include "math.h"
namespace sensor_srvs {

struct IMUParameter {
  double _a_max;
  double _g_max;
  double _sigma_g_c;
  double _sigma_a_c;
  double _sigma_bg;
  double _sigma_ba;
  double _sigma_gw_c;
  double _sigma_aw_c;
  double _tau;  // time relationship param
  double _g;
  double _a0[4];
  double _T_BS[16];
  double _Acc[12];  //加速度计的补偿参数
  double _Gyr[12];  //陀螺仪的补偿参数
};
struct ModuleInfo {
  char _id[32];
  char _designer[32];
  char _fireware_version[32];
  char _hardware_version[32];
  char _lens[32];
  char _imu[32];
  char _viewing_angle[32];
  char _baseline[32];
};
struct SDebugSet {
  bool s_d_receive;
  bool s_d_result;
  bool s_d_track;
  bool s_d_lba;
  bool s_d_map;
  bool s_d_lpc;
  bool s_d_opt;
};
struct SlamParameter {
  int _numKeyframes;             //关键帧数量 =5
  int _numImuFrames;             //相邻imu帧数量 =3
  int _ceres_minIterations;      //最小优化次数 =3
  int _ceres_maxIterations;      //最大优化次数 =4
  double _ceres_timeLimit;       //=3.5000000000000003e-02
  int detection_threshold;       //角点阈值 =30
  int detection_octaves;         //=0
  int detection_maxNoKeypoints;  //最大角点数量    = 100
  bool displayImages;            //是否显示slam界面  = true
};

enum Resolution : uint8_t { RES_640X400, RES_1280X800, LAST };

struct CameraParameterRaw {
  double _TSC[16];  //
  /** 4X4 matrix from camera to imu */
  int _width;
  /** width */
  int _height;
  /** height */
  double _focal_length[2];
  /** fx,fy */
  double _principal_point[2];
  /** cx,cy */
  double _R[9];
  /** Rectification matrix (stereo cameras only)
     A rotation matrix aligning the camera coordinate system to the ideal
    stereo image plane so that epipolar lines in both stereo images are
    parallel. */

  double _P[12];
  /** Projection/camera matrix
         [fx'  0  cx' Tx]
    P = [ 0  fy' cy' Ty]
        [ 0   0   1   0] */
  double _K[9];
  /** Intrinsic camera matrix for the raw (distorted) images.
         [fx  0 cx]
    K = [ 0 fy cy]
        [ 0  0  1] */
  double _D[4];
  /** The distortion parameters, size depending on the distortion model.
     For us, the 4 parameters are: (k1, k2, t1, t2).*/
};

struct CameraParameter {
  double _TSC[16];  //
  /** 4X4 matrix from camera to imu */
  int _width;
  /** width */
  int _height;
  /** height */
  double _focal_length[2];
  /** fx,fy */
  double _principal_point[2];
  /** cx,cy */
  double _R[9];
  /** Rectification matrix (stereo cameras only)
     A rotation matrix aligning the camera coordinate system to the ideal
    stereo image plane so that epipolar lines in both stereo images are
    parallel. */

  double _P[12];
  /** Projection/camera matrix
         [fx'  0  cx' Tx]
    P = [ 0  fy' cy' Ty]
        [ 0   0   1   0] */
  double _K[9];
  /** Intrinsic camera matrix for the raw (distorted) images.
         [fx  0 cx]
    K = [ 0 fy cy]
        [ 0  0  1] */
  double _D[4];
  /** The distortion parameters, size depending on the distortion model.
     For us, the 4 parameters are: (k1, k2, t1, t2).*/

  float b;
  float bf;
};

struct ImseeParamSrv {
  ImseeParamSrv() {}

  struct Request {
    std::uint64_t timeStamp;
  } request;

  struct Response {
    std::map<Resolution, CameraParameter> _left_camera;
    std::map<Resolution, CameraParameter> _right_camera;
    int _camera_channel = 1;
    double _baseline = 0.12;  // 基线 单位 m
    IMUParameter _imu;
    ModuleInfo _device;
    SlamParameter _slam;
    sensor_srvs::SDebugSet _s_debug;
    std::uint64_t timeStamp;
  } response;
};

}  // namespace sensor_srvs

namespace mind_os {

template <>
inline void serialize(const sensor_srvs::SDebugSet& data, BytesBuffer& buffer) {
  serialize(data.s_d_receive,buffer);
  serialize(data.s_d_result, buffer);
  serialize(data.s_d_track, buffer);
  serialize(data.s_d_map, buffer);
  serialize(data.s_d_lpc, buffer);
  serialize(data.s_d_opt, buffer);
  serialize(data.s_d_lba, buffer);
  
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SDebugSet& data) {
  deserialize(buffer, data.s_d_receive);
  deserialize(buffer, data.s_d_result);
  deserialize(buffer, data.s_d_track);
  deserialize(buffer, data.s_d_map);
  deserialize(buffer, data.s_d_lpc);
  deserialize(buffer, data.s_d_opt);
  deserialize(buffer, data.s_d_lba);

}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::SlamParameter& data, BytesBuffer& buffer) {
  serialize(data._numKeyframes,buffer);
  serialize(data._numImuFrames, buffer);
  serialize(data._ceres_minIterations, buffer);
  serialize(data._ceres_maxIterations, buffer);
  serialize(data._ceres_timeLimit, buffer);
  serialize(data.detection_threshold, buffer);
  serialize(data.detection_octaves, buffer);
  serialize(data.detection_maxNoKeypoints, buffer);
  serialize(data.displayImages, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::SlamParameter& data) {
  deserialize(buffer, data._numKeyframes);
  deserialize(buffer, data._numImuFrames);
  deserialize(buffer, data._ceres_minIterations);
  deserialize(buffer, data._ceres_maxIterations);
  deserialize(buffer, data._ceres_timeLimit);
  deserialize(buffer, data.detection_threshold);
  deserialize(buffer, data.detection_octaves);
  deserialize(buffer, data.detection_maxNoKeypoints);
  deserialize(buffer, data.displayImages);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::ModuleInfo& data, BytesBuffer& buffer) {
  serialize(data._id, 32,buffer);
  serialize(data._designer, 32,buffer);
  serialize(data._fireware_version, 32,buffer);
  serialize(data._hardware_version, 32,buffer);
  serialize(data._lens, 32,buffer);
  serialize(data._imu, 32,buffer);
  serialize(data._viewing_angle, 32,buffer);
  serialize(data._baseline, 32,buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ModuleInfo& data) {
  deserialize(buffer, data._id,32);
  deserialize(buffer, data._designer,32);
  deserialize(buffer, data._fireware_version,32);
  deserialize(buffer, data._hardware_version,32);
  deserialize(buffer, data._lens,32);
  deserialize(buffer, data._imu,32);
  deserialize(buffer, data._viewing_angle,32);
  deserialize(buffer, data._baseline,32);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */


template <>
inline void serialize(const sensor_srvs::IMUParameter& data, BytesBuffer& buffer) {
  serialize(data._a_max, buffer);
  serialize(data._g_max, buffer);
  serialize(data._sigma_g_c, buffer);
  serialize(data._sigma_a_c, buffer);
  serialize(data._sigma_bg, buffer);
  serialize(data._sigma_ba, buffer);
  serialize(data._sigma_gw_c, buffer);
  serialize(data._sigma_aw_c, buffer);
  serialize(data._tau, buffer);
  serialize(data._g, buffer);
  serialize(data._a0, 4,buffer);
  serialize(data._T_BS, 16,buffer);
  serialize(data._Acc, 12,buffer);
  serialize(data._Gyr, 12,buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::IMUParameter& data) {
  deserialize(buffer, data._a_max);
  deserialize(buffer, data._g_max);
  deserialize(buffer, data._sigma_g_c);
  deserialize(buffer, data._sigma_a_c);
  deserialize(buffer, data._sigma_bg);
  deserialize(buffer, data._sigma_ba);
  deserialize(buffer, data._sigma_gw_c);
  deserialize(buffer, data._sigma_aw_c);
  deserialize(buffer, data._tau);
  deserialize(buffer, data._g);
  deserialize(buffer, data._a0,4);
  deserialize(buffer, data._T_BS,16);
  deserialize(buffer, data._Acc,12);
  deserialize(buffer, data._Gyr,12);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */


template <>
inline void serialize(const sensor_srvs::ImseeParamSrv::Request& data, BytesBuffer& buffer) {
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ImseeParamSrv::Request& data) {
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Request */

template <>
inline void serialize(const sensor_srvs::ImseeParamSrv::Response& data, BytesBuffer& buffer) {
  serialize(data._left_camera, buffer);
  serialize(data._right_camera, buffer);
  serialize(data._camera_channel, buffer);
  serialize(data._baseline, buffer);
  serialize(data._imu, buffer);
  serialize(data._device, buffer);
  serialize(data._slam, buffer);
  serialize(data._s_debug, buffer);
  serialize(data.timeStamp, buffer);
}; /* serialization of sensor_srvs::BluetoothSrv Response */
template <>
inline void deserialize(BytesBuffer& buffer, sensor_srvs::ImseeParamSrv::Response& data) {
  deserialize(buffer, data._left_camera);
  deserialize(buffer, data._right_camera);
  deserialize(buffer, data._camera_channel);
  deserialize(buffer, data._baseline);
  deserialize(buffer, data._imu);
  deserialize(buffer, data._device);
  deserialize(buffer, data._slam);
  deserialize(buffer, data._s_debug);
  deserialize(buffer, data.timeStamp);
}; /* de-serialization of sensor_srvs::BluetoothSrv Response */

} /* namespace mind_os */

