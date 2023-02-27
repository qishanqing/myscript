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
#include "slam/SlamResult.h"
#include "sensor/LaserScan.h"
#include "sensor/CameraMoudleParam.h"
#include "perception/instance.h"
#include "src/utils/de_distortion.h"

const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 400;
const float ANGLE_2_RADIAN = M_PI / 180;
const float MIN_DISTANCE_EDGE_2_VERTEX = 0.25;
const int MIN_VALID_ANGLE_4_VIEW = 10;
const double MIN_VALID_RADAIN_4_VIEW = MIN_VALID_ANGLE_4_VIEW * ANGLE_2_RADIAN;

using Point = psl::Location;

bool Empty(const swr::sensor_types::LaserScan &data);

struct Lidar
{
    swr::sensor_types::LaserScan data;
    psl::SlamResult pose;

    Lidar();

    Lidar(const swr::sensor_types::LaserScan data
          , const psl::SlamResult pose);
};

struct Image
{
    psl::Time time;
    cv::Mat data;
    psl::SlamResult pose;

    Image();

    Image(psl::Time time, cv::Mat data);

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

class BoxInfo;

struct Sector
{
    Point vertex;
    Point left, right;
    Point leftOrg, rightOrg;

    Point leftLeft, rightRight;
    Point vertexOrigin;
    Point leftRight, rightLeft;

    bool leftUpdate, rightUpdate;
    bool isTarget;
    int id, boxID;
    int partCount;
    bool leftClose2Edge = false, rightCLose2Edge = false;
    bool invalid = false;

    Sector();

    Sector(const BoxInfo &box);

    Sector(const Point& vertex, const Point& left, const Point& right);

    bool UpdateEdge(const BoxInfo &box, const BoxInfo &view);

    bool UpdateLeft(const BoxInfo &box, const BoxInfo &view);

    bool UpdateRight(const BoxInfo &box, const BoxInfo &view);

    void UpdateVertex(const BoxInfo &box, const Point &view
                      , const bool leftUpdate = true, const bool rightUpdate = true);

    void Reset();

    void SetVertex(const Point& point);

    std::vector<Point> GetPoint() const;

    bool Read(std::ifstream &stream);

    bool Write(std::ofstream &stream) const;
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
