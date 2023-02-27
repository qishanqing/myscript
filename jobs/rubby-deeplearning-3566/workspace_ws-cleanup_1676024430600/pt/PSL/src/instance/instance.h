#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include <vector>
#include <cmath>
#include <iostream>
#include <map>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "psl/slam/SlamResult.h"
#include "psl/sensor/LaserScan.h"
#include "src/utils/data_type.h"
#include "src/utils/config.h"
#include "src/utils/detector_param.h"


class Instance
{
public:
    Instance();

    virtual ~Instance();

    void InitCamera(const psl::CameraMoudleParam &cameraParam);

    void SetParam(const ConfigParam &param);

protected:
    float Mean(const std::vector<Eigen::Vector3d> &points, const int flag) const;

    float Min(const std::vector<Eigen::Vector3d> &points, const int flag) const;

    float Max(const std::vector<Eigen::Vector3d> &points, const int flag) const;

    psl::Location
    GetLocationFromLidar(std::vector<Eigen::Vector3d> &points, const int num) const;

protected:
    // camera
    Eigen::Matrix<double, 3, 3> K;
    Eigen::Matrix<double, 3, 4> P;
    Eigen::Matrix<double, 3, 3> R;
    Eigen::Matrix<double, 3, 1> D;
    Eigen::Matrix<double, 3, 3> K_T;

    ConfigParam configParam;
};

#endif
