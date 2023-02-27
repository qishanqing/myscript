//////////////////////////////////////////////////////////////////////
///  @file     slam.h
///  @brief    some slam operator about matrix
///  @author   sunhao
///  @date     2021.11.15
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_SLAM_H
#define DETECTOR_SAMPLE_SLAM_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "slam/SlamResult.h"


template<class T>
Eigen::Matrix<T, 3, 1> Quaternion2Angle(const Eigen::Quaternion<T> &quaternion);

Eigen::Quaternion<double> Pose2Quaternion(const psl::SlamResult &pose);

template<class T>
Eigen::Matrix<T, 3, 1> Quaternion2Angle(const Eigen::Quaternion<T> &quaternion)
{
    Eigen::Quaternion<T> normalized_quaternion = quaternion.normalized();

    if (normalized_quaternion.w() < 0.)
    {
        // Multiply by -1. http://eigen.tuxfamily.org/bz/show_bug.cgi?id=560
        normalized_quaternion.w() = -1. * normalized_quaternion.w();
        normalized_quaternion.x() = -1. * normalized_quaternion.x();
        normalized_quaternion.y() = -1. * normalized_quaternion.y();
        normalized_quaternion.z() = -1. * normalized_quaternion.z();
    }

    const T angle = 2. * atan2(normalized_quaternion.vec().norm()
                               , normalized_quaternion.w());
    constexpr double kCutoffAngle = 1e-7;
    const T scale = angle < kCutoffAngle ? T(2.) : angle / sin(angle / 2.);
    return Eigen::Matrix<T, 3, 1>(scale * normalized_quaternion.x(),
            scale * normalized_quaternion.y(), scale * normalized_quaternion.z());
}


#endif //DETECTOR_SAMPLE_SLAM_H
