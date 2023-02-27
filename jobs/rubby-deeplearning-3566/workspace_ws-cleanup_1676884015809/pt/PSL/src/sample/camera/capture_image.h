//
// Created by donglijian on 1/10/22.
//

#ifndef DETECTOR_SAMPLE_CAPTURE_IMAGE_H
#define DETECTOR_SAMPLE_CAPTURE_IMAGE_H

#include <string>
#include<cstring>
#include <opencv2/highgui.hpp>
#include <mutex>
#include "ImseeModule.hpp"
#include "src/utils/define.h"
#include "psl/sensor/LaserScan.h"
#include "psl/sensor/CameraMoudleParam.h"


struct ImageModule
{
    cv::Mat left = cv::Mat();
    cv::Mat right = cv::Mat();

    void Reset()
    {
        left = cv::Mat();
        right = cv::Mat();
    }
};

class CaptureImage
{
public:
    ~CaptureImage();

    void SaveConfigFile(const std::string &path);

    void InitDriver();

    bool GetParam(psl::CameraMoudleParam& moduleParam);

    ImageModule GetImg();

private:
    imsee_module::ImseeModule *sensor = nullptr;

    static void GetImage(psl::CImseeMsgImage &data);

    static ImageModule img;
    static std::mutex leftImgMutex;
};

#endif //DETECTOR_SAMPLE_CAPTURE_IMAGE_H
