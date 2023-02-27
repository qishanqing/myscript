//
// Created by donglijian on 1/10/22.
//

#include <iostream>
#include <map>
#include "capture_image.h"

//#define SHOW_IMG
#define PARAMS_READ_TEST

using namespace std;
using namespace imsee_module;
const psl::Resolution RESOLUTION = psl::Resolution::RES_640X400;

#ifndef RobotDEBUG
#define RobotDEBUG(format, ...) {fprintf(stdout, format, ##__VA_ARGS__);}
#endif
#ifndef RobotINFO
#define RobotINFO(format, ...) {fprintf(stdout, format, ##__VA_ARGS__);}
#endif
#ifndef RobotWARN
#define RobotWARN(format, ...) {fprintf(stdout, format, ##__VA_ARGS__);}
#endif
#ifndef RobotERR
#define RobotERR(format, ...) {fprintf(stderr, format, ##__VA_ARGS__);}
#endif

ImageModule CaptureImage::img;
std::mutex CaptureImage::leftImgMutex;

void CaptureImage::SaveConfigFile(const std::string &path)
{
    // TODO : reference to src/utils/utils.cpp: read
//    if (driver == nullptr) return;
//    psl::CameraMoudleParam param;
//    int type = 0;
//    bool getState = driver->GetParam(param);
//    if (getState)
//    {
//        std::string fullpath = path + "/config.yaml";
//        cv::FileStorage fs(fullpath, cv::FileStorage::WRITE);
//        int width = 640;
//        int height = 400;
//        CameraParameter leftCamera = param._left_camera[RESOLUTION];
//        CameraParameter rightCamera = param._right_camera[RESOLUTION];
//
//        cv::Mat m_mHeadKl = cv::Mat(3, 3, CV_64FC1, leftCamera._K);
//        cv::Mat m_mHeadKr = cv::Mat(3, 3, CV_64FC1, rightCamera._K);
//        cv::Mat m_mHeadDl = cv::Mat(4, 1, CV_64FC1, leftCamera._D);
//        cv::Mat m_mHeadDr = cv::Mat(4, 1, CV_64FC1, rightCamera._D);
//        cv::Mat m_mHeadRl = cv::Mat(3, 3, CV_64FC1, leftCamera._R);
//        cv::Mat m_mHeadRr = cv::Mat(3, 3, CV_64FC1, rightCamera._R);
//        cv::Mat m_mHeadPl = cv::Mat(3, 4, CV_64FC1, leftCamera._P);
//        cv::Mat m_mHeadPr = cv::Mat(3, 4, CV_64FC1, rightCamera._P);
//        cv::Mat leftTSC(4, 4, CV_64FC1, leftCamera._TSC);
//        cv::Mat rightTSC(4, 4, CV_64FC1, rightCamera._TSC);
//        //左相机参数
//        fs << "cameras" << "[";
//        fs << "{:" << "T_SC" << "[:"
//           << leftCamera._TSC[0] << leftCamera._TSC[1] << leftCamera._TSC[2]
//           << leftCamera._TSC[3]
//           << leftCamera._TSC[4] << leftCamera._TSC[5] << leftCamera._TSC[6]
//           << leftCamera._TSC[7]
//           << leftCamera._TSC[8] << leftCamera._TSC[9] << leftCamera._TSC[10]
//           << leftCamera._TSC[11]
//           << leftCamera._TSC[12] << leftCamera._TSC[13] << leftCamera._TSC[14]
//           << leftCamera._TSC[15]
//           << "]";
//        fs << "image_dimension" << "[:" << width << height << "]";
//        fs << "distortion_coefficients" << "[:" << leftCamera._D[0] << leftCamera._D[1]
//           << leftCamera._D[2] << leftCamera._D[3] << "]";
//        fs << "distortion_type" << "equidistant";
//        fs << "focal_length" << "[:" << leftCamera._focal_length[0]
//           << leftCamera._focal_length[1] << "]";
//        fs << "principal_point" << "[:" << leftCamera._principal_point[0]
//           << leftCamera._principal_point[1] << "]";
//        fs << "}";
//        //右相机参数
//        fs << "{:" << "T_SC" << "[:"
//           << rightCamera._TSC[0] << rightCamera._TSC[1] << rightCamera._TSC[2]
//           << rightCamera._TSC[3]
//           << rightCamera._TSC[4] << rightCamera._TSC[5] << rightCamera._TSC[6]
//           << rightCamera._TSC[7]
//           << rightCamera._TSC[8] << rightCamera._TSC[9] << rightCamera._TSC[10]
//           << rightCamera._TSC[11]
//           << rightCamera._TSC[12] << rightCamera._TSC[13] << rightCamera._TSC[14]
//           << rightCamera._TSC[15]
//           << "]";
//        fs << "image_dimension" << "[:" << width << height << "]";
//        fs << "distortion_coefficients" << "[:" << rightCamera._D[0]
//           << rightCamera._D[1] << rightCamera._D[2] << rightCamera._D[3] << "]";
//        fs << "distortion_type" << "equidistant";
//        fs << "focal_length" << "[:" << rightCamera._focal_length[0]
//           << rightCamera._focal_length[1] << "]";
//        fs << "principal_point" << "[:" << rightCamera._principal_point[0]
//           << rightCamera._principal_point[1] << "]";
//        fs << "}";
//        fs << "]";
//
//        int cameraRate = 25;
//        double sigmaAbsoluteTranslation = 0.0;
//        double sigmaAbsoluteOrientation = 0.0;
//        double sigmaCRrelativeTranslation = 0.0;
//        double sigmaCRrelativeOrientation = 0.0;
//        double timestampTolerance = 0.005;
//        fs << "camera_params" << "{"
//           << "camera_rate" << cameraRate
//           << "sigma_absolute_translation" << sigmaAbsoluteTranslation
//           << "sigma_absolute_orientation" << sigmaAbsoluteOrientation
//           << "sigma_c_relative_translation" << sigmaCRrelativeTranslation
//           << "sigma_c_relative_orientation" << sigmaCRrelativeOrientation
//           << "timestampTolerance" << timestampTolerance
//           << "}";
//        //3.4 输出外参
//        fs << "Rl" << m_mHeadRl << "Rr" << m_mHeadRr << "Pl" << m_mHeadPl << "Pr"
//           << m_mHeadPr;
//        fs << "Kl" << m_mHeadKl << "Kr" << m_mHeadKr << "Dl" << m_mHeadDl << "Dr"
//           << m_mHeadDr;
//        //3.5 输出imu_parameters
//        double aMax = 176.0;
//        double gMax = 30.0;
//        double sigmaGC = 0.12000000000000000e+00;
//        double sigmaAC = 9.0000000000000002e-03;
//        double sigmaBg = 1.000000000000000e-01;
//        double sigmaBa = 1.000000000000000e-03;
//        double sigmaGwC = 4.0000000000000002e-05;
//        double sigmaAwC = 4.0000000000000003e-05;
//        double tau = 3600.;
//        double g = 9.8019967;
//        cv::Vec3d a0(0, 0, 0);
//        int imuRate = 200;
//
//        fs << "imu_params" << "{"
//           << "a_max" << aMax
//           << "g_max" << gMax
//           << "sigma_g_c" << sigmaGC
//           << "sigma_a_c" << sigmaAC
//           << "sigma_bg" << sigmaBg
//           << "sigma_ba" << sigmaBa
//           << "sigma_gw_c" << sigmaGwC
//           << "sigma_aw_c" << sigmaAwC
//           << "tau" << tau
//           << "g" << g
//           << "a0" << a0
//           << "imu_rate" << imuRate
//           << "T_BS" << "[:"
//           << 1. << 0. << 0. << 0.
//           << 0. << 1. << 0. << 0.
//           << 0. << 0. << 1. << 0.
//           << 0. << 0. << 0. << 1.
//           << "]"
//           << "}";
//
//        ////3.9 输出delay, display, use direct driver...
//        ////string strDisplayImages = "true";
//        //string strDisplayImages = "false";
//        string strUseDriver = "true";
//        //fs << "imageDelay" << 0.0
//        //    << "displayImages" << strDisplayImages;
//        fs << "useDriver" << strUseDriver;
//
//        //3.10 输出publishing_options
//        string strPublishLandmarks = "true";
//        string strPublishImuPropagatedState = "true";
//        string strTrackedBodyFrame = "B";
//        string strVelocitiesFrame = "Wc";
//        fs << "publishing_options" << "{"
//           << "publish_rate" << 200
//           << "publishLandmarks" << strPublishLandmarks
//           << "landmarkQualityThreshold" << 1.0e-2
//           << "maximumLandmarkQuality" << 0.05
//           << "maxPathLength" << 20
//           << "publishImuPropagatedState" << strPublishImuPropagatedState
//           << "T_Wc_W" << "[:"
//           << 1. << 0. << 0. << 0.
//           << 0. << 1. << 0. << 0.
//           << 0. << 0. << 1. << 0.
//           << 0. << 0. << 0. << 1.
//           << "]"
//           << "trackedBodyFrame" << strTrackedBodyFrame
//           << "velocitiesFrame" << strVelocitiesFrame
//           << "}";
//
//        cv::Mat acc(3, 4, CV_64FC1, param._parent._imu._Acc);
//        cv::Mat gyr(3, 4, CV_64FC1, param._parent._imu._Gyr);
//        fs << "Acc" << acc;
//        fs << "Gyr" << gyr;
//        fs.release();
//    }
//    else
//    {
//        ERROR_PRINT("driver->GetModuleParams has matter!");
//    }
}

void MessageCallBack(psl::CImseeMsgHotplug &data)
{
    if (data.state)
    {
        std::cout << "bus: "
                  << static_cast<int>(data.bus) << ", "
                  << "addr: "
                  << static_cast<int>(data.addr) << ", "
                  << "    left" << std::endl;
    }
    else
    {
        std::cout << "bus: "
                  << static_cast<int>(data.bus) << ", "
                  << "addr: "
                  << static_cast<int>(data.addr) << ", "
                  << "    arrived" << std::endl;
    }
}

//void ErrorCallBack(psl::ErrorMessage &msg)
//{
//    if (msg.state == psl::ON)
//    {
//        RobotINFO("error msg %u: 0x%.8x ON"
//                  , msg.stamp, msg.code);
//    }
//    else
//    {
//        RobotINFO("error msg %u: 0x%.8x OFF"
//                  , msg.stamp, msg.code);
//    }
//}

void CaptureImage::GetImage(psl::CImseeMsgImage &data)
//void GetImage(psl::CImseeMsgImage &data)
{

#if CALLBACK_LIGHT_CONTROL
    sensor->AutoCtrlIRAndTxLight(false);
                                if (sensor->SerIrLight(0) < 0) {
                                  RobotERR("set ir ligtht faild");
                                }
                                if (sensor->SerTxLight(0) < 0) {
                                  RobotERR("set tx ligtht faild");
                                }
#endif

    {
        if (not(data.ir_tx_state & 1))
        {
            const int W = data.camera_width;
            const int H = data.camera_height;
            std::unique_lock<std::mutex> lock(leftImgMutex);
            img.left = cv::Mat(H, W, CV_8UC1, data.img_data_left).clone();
            img.right = cv::Mat(H, W, CV_8UC1, data.img_data_right).clone();
        }
    }

#ifdef SHOW_IMG

    if (data.ir_tx_state & 1)
    {
        cv::imshow(
                std::to_string(data.imseeid) + "_TX_" +
                std::string(" Left"), imgl);
        cv::imshow(
                std::to_string(data.imseeid) + "_TX_" +
                std::string(" Right"), imgr);
    }
    else
    {
        cv::imshow(
                std::to_string(data.imseeid) + "_IR_" +
                std::string(" Left"), imgl);
        cv::imshow(
                std::to_string(data.imseeid) + "_IR_" +
                std::string(" Right"), imgr);
    }

    cv::waitKey(1);
    imgl.release();
    imgr.release();
#endif
}

void GetIMU(psl::CImseeMsgImu &data)
{
    // std::cout << GetSystimeMs()<<" "<<data.timeStamp<<std::endl;
#if SHOW_IMU_DATA
    std::cout << data.imu_index << "  " << data.timeStamp << "  "
                                          << data.acc[0] << "  " << data.acc[1] << "  " << data.acc[2]
                                          << "  " << data.gyr[0] << "  " << data.gyr[1] << "  "
                                          << data.gyr[2] << "  " << std::endl;
#endif
}

void CaptureImage::InitDriver()
{
    int width = 640;
    int height = 400;
    int fps = 25;

    std::map<int, ImseeDevice> *devsMap = nullptr;
    ImseeModule::HotPlugDetachStart();
    devsMap = ImseeModule::GetDeviceList();
    while (devsMap == nullptr)
    {
        devsMap = ImseeModule::GetDeviceList();
        RobotWARN("No device found !!!");
        usleep(300000);
    }
    RobotINFO("Now get %d devices", devsMap->size());
    ImseeModule::RegisterCallBack([&](psl::CImseeMsgHotplug &data)
                                  {
                                      std::cout << "+++++++++++++++++++++++++ "
                                                << (int) data.state << std::endl;
                                  });
    for (auto &iter : *devsMap)
    {
        sensor = ImseeModule::Create(iter.second, "./config/");
        RobotDEBUG("width %d, height %d, fps %d", width, height, fps);
        int ret = sensor->Init(width, height, fps, 500);
        if (ret < 0)
        {
            std::cout << "================ init error, exit now !!!!" << std::endl;
            delete sensor;
            continue;
        }

        RobotINFO("================= sensor start =================");
        sensor->Start();

// 读 参数测试
#ifdef PARAMS_READ_TEST

#endif

        sensor->RegisterCallBack(MessageCallBack);
//        sensor->SetErrorMessageCallback(ErrorCallBack);
        sensor->RegisterCallBack(GetImage);
        sensor->RegisterCallBack(GetIMU);
        sensor->RegisterCallBack([&](psl::CImseeMsgParam &data)
                                {});
    }
}

CaptureImage::~CaptureImage()
{
    if (sensor != nullptr)
    {
        sensor->Stop();
    }
}

bool CaptureImage::GetParam(psl::CameraMoudleParam &moduleParam)
{
    psl::CImseeMsgParam param;
    if (sensor->GetParams(param) < 0) return false;

    moduleParam = psl::CameraMoudleParam(param.param);

    return true;
}

ImageModule CaptureImage::GetImg()
{
    ImageModule imgLast;

    {
        std::unique_lock<std::mutex> lock(leftImgMutex);
        imgLast = img;
        img.Reset();
    }

    return imgLast;
}
