#include "slam.h"


Eigen::Quaternion<double> Pose2Quaternion(const psl::SlamResult& pose)
{
    Eigen::Quaternion<double> quaternion;

    quaternion.w() = pose.s_rotation[0];
    quaternion.x() = pose.s_rotation[1];
    quaternion.y() = pose.s_rotation[2];
    quaternion.z() = pose.s_rotation[3];

    return quaternion;
}