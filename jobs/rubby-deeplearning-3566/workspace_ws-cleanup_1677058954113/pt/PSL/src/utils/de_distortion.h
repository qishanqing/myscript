//////////////////////////////////////////////////////////////////////
///  @file     de_distortion.h
///  @brief    image remap function
///  Details.
///
///  @author   donglijian
///  @version  1.7.14
///  @date     2021.11.03
//////////////////////////////////////////////////////////////////////

#ifndef SINGLE_TEST_SAMPLE_REMAP_H
#define SINGLE_TEST_SAMPLE_REMAP_H

#include "psl/sensor/CameraMoudleParam.h"
#include <opencv2/opencv.hpp>

const int WIDTH = 640;
const int HEIGHT = 400;

enum CameraType
{
    LEFT = 0, RIGHT
};

class DeDistortion
{
public:
    static bool ReadParamLeft(const psl::CameraMoudleParam &moduleParam);

    static bool ReadParamRight(const psl::CameraMoudleParam &moduleParam);

    static bool Remap(const CameraType type, cv::Mat &image);

private:
    static bool ReadPara(const psl::CameraParam &cameraParam
                         , cv::Mat& remapX, cv::Mat& remapY);

private:
    static psl::CameraMoudleParam moduleParam;
    static cv::Mat remapXLeftFisheye, remapYLeftFisheye;
    static cv::Mat remapXRightFisheye, remapYRightFisheye;
    static bool flag;
};


#endif //SINGLE_TEST_SAMPLE_REMAP_H
