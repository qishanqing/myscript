//////////////////////////////////////////////////////////////////////
///  @file     data.h
///  @brief    base data types for perception
///  @author   sunhao
///  @date     2021.07.05
//////////////////////////////////////////////////////////////////////

#ifndef DATA_TYPE_SAMPLE_DATA_TYPE_H
#define DATA_TYPE_SAMPLE_DATA_TYPE_H

#include <vector>
#include <iostream>
#include <string>
#include "psl/slam/SlamResult.h"
#include "psl/sensor/LaserScan.h"
#include "psl/sensor/CameraMoudleParam.h"
#include "psl/perception/instance.h"
#include "src/utils/de_distortion.h"

const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 400;
const float ANGLE_2_RADIAN = M_PI / 180;
const float MIN_DISTANCE_EDGE_2_VERTEX = 0.25;
const int MIN_VALID_ANGLE_4_VIEW = 10;
const double MIN_VALID_RADAIN_4_VIEW = MIN_VALID_ANGLE_4_VIEW * ANGLE_2_RADIAN;


bool Empty(const swr::psl_sensor_types::LaserScan &data);

struct Lidar
{
    swr::psl_sensor_types::LaserScan data;
    psl::SlamResult pose;

    Lidar();

    Lidar(const swr::psl_sensor_types::LaserScan data
          , const psl::SlamResult pose);
};

struct Image
{
    psl::Time time;
    cv::Mat left;
    cv::Mat right;
    psl::SlamResult pose;

    Image();

    Image(psl::Time time, cv::Mat data);

    Image(psl::Time time,const cv::Mat &imageLeft,const cv::Mat &imageRight);

    Image &Clone();

    void Reset();

    Image &operator=(const Image &image);
};

struct DataGroup
{
    Lidar lidar;
// swr::depth_types::DepthMatrix depthes;
    Image image, orgImage;
    psl::Time time = 0;

    DataGroup();

    DataGroup(const Lidar &lidar, const Image &image, const Image &orgImage);

    bool Remap();

    bool Empty() const;

    psl::SlamResult GetPose() const;
};

struct Data4SaveInfo
{
    std::string path = "";
    bool save = false;
    bool checkObject = false;

    Data4SaveInfo()
    {}

    Data4SaveInfo(const std::string path, const bool checkObject);
};

#endif //DATA_TYPE_SAMPLE_DATA_TYPE_H
