//////////////////////////////////////////////////////////////////////
///  @file     lidar.h
///  @brief    lidar data process
///  @author   sunhao
///  @date     2021.09.11
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_LIDAR_H
#define DETECTOR_SAMPLE_LIDAR_H

#include "psl/sensor/LaserScan.h"
#include <vector>
#include <Eigen/Core>
#include "src/utils/config.h"

const float RADIAN_2_ANGLE = 180 / M_PI;


int CmpLidarCloser(Eigen::Vector3d &point1, Eigen::Vector3d &point2);

int CmpLeft2Right(Eigen::Vector3d &point1, Eigen::Vector3d &point2);

void GetAngle(const Eigen::Matrix<double, 3, 4> &pMatrix, const float point
              , float &angle);

void Polar2Cartesian(const swr::psl_sensor_types::LaserScan &lidarOrigin
                     , std::vector<Eigen::Vector3d> &lidarPoints);

void CutSortEigen3DAngle(const ConfigParam &configParam
                         , const std::vector<Eigen::Vector3d> &points
                         , float minAngle, float maxAngle
                         , std::vector<Eigen::Vector3d> &pointROI);

cv::Mat ShowLidar(const Eigen::Matrix<double, 3, 4> &pMatrix, const ConfigParam &configParam
               , const DataGroup &item);

double GetAngle(const psl::SlamResult &poseLast
                , const psl::SlamResult &poseCurrent);

#endif //DETECTOR_SAMPLE_LIDAR_H
