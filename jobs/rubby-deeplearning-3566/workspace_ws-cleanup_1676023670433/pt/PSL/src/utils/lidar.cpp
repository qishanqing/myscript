//
// Created by hao on 2021/9/11.
//

#include "lidar.h"
#include <Eigen/Dense>
#include "src/utils/define.h"
#include "src/utils/file.h"
#include "src/utils/slam.h"
#include "src/utils/utils.h"
#include <cmath>


int CmpLidarCloser(Eigen::Vector3d &point1, Eigen::Vector3d &point2)
{
    return point1[0] < point2[0];
}

int CmpLeft2Right(Eigen::Vector3d &point1, Eigen::Vector3d &point2)
{
    // TODO : take care of lidar direction
    return point1[1] > point2[1];
}


void GetAngle(const Eigen::Matrix<double, 3, 4> &pMatrix, const float point
              , float &angle)
{
    auto fx = pMatrix(0, 0);
    auto cx = pMatrix(0, 2);
    angle = atanf32((point - cx) / fx);
}


void Polar2Cartesian(const swr::sensor_types::LaserScan &lidarOrigin
                     , std::vector<Eigen::Vector3d> &lidarPoints)
{
    // TODO : check
    size_t pointCount = lidarOrigin.ranges.size();
    Eigen::ArrayXXd rangeWhole(pointCount, 2);//pt_num行，2列
    Eigen::ArrayXXd locationCartesian(pointCount, 2);
    Eigen::ArrayXXd angleWhole(pointCount, 2);
    for (size_t i = 0; i < pointCount; i++)
    {
        auto angle =
                lidarOrigin.angle_min + (double) i * lidarOrigin.angle_increment;
        if (abs(angle) > 1.3) // outof camera
        {
            continue;
        }
        rangeWhole(i, 0) = (double) lidarOrigin.ranges[i];
        rangeWhole(i, 1) = (double) lidarOrigin.ranges[i];
        angleWhole(i, 0) = cos(
                lidarOrigin.angle_min + (double) i * lidarOrigin.angle_increment);
        angleWhole(i, 1) = sin(
                lidarOrigin.angle_min + (double) i * lidarOrigin.angle_increment);
    }
    locationCartesian = rangeWhole * angleWhole; // x, y
    for (size_t i = 0; i < pointCount; i++)
    {
        auto temp_angle =
                lidarOrigin.angle_min + (double) i * lidarOrigin.angle_increment;
        if (abs(temp_angle) > 1.3)
        {
            continue;
        }
        double range_cutoff = 10;//假设有效距离10m
        const float range = lidarOrigin.ranges[i];
        if (range < range_cutoff && range > lidarOrigin.range_min)
            lidarPoints.push_back(
                    Eigen::Vector3d(locationCartesian(i, 0), locationCartesian(i, 1)
                                    , 0.27));
    }
}

void CutSortEigen3DAngle(const ConfigParam &configParam
                         , const std::vector<Eigen::Vector3d> &points
                         , float minAngle, float maxAngle
                         , std::vector<Eigen::Vector3d> &pointROI)
{
    pointROI.clear();

    for (auto &lidar : points)
    {
        // cos(M_PI * 2 / 180) : 2 is the angle of lidar
        // 0.0778 : the distance between lidar and camera
        // 0.060 : the distance between left camera and the center of camera
        // - : is from left to right of lidar; +: lidar revers

        double horizontal = -lidar[1];
        if (not configParam.structure.lidarDirection)
        {
            horizontal = lidar[1];
        }

        auto temp_angle = atan(
                (horizontal + configParam.structure.leftcamera2lidar.at(1)) /
                (lidar[0] * cos(M_PI * configParam.structure.lidarAngle / 180) -
                 configParam.structure.leftcamera2lidar.at(0)));
        // convert lidar data coordinate to left camera coordinate for angle check

        // angle check
        if (minAngle <= temp_angle && temp_angle < maxAngle)
        {
            if (lidar[0] >= 0)
            {
                pointROI.push_back(lidar);
            }
            else
            {
                LOG_CHECK_DEBUG(INFO) << lidar;
            }
        }
    }
}

cv::Mat ShowLidar(const Eigen::Matrix<double, 3, 4> &pMatrix
                  , const ConfigParam &configParam
          , const DataGroup &item)
{
    cv::Mat image;
    if (configParam.NOT_SHOW == configParam.display) return image;

    if (item.image.data.empty())
    {
        LOG_CHECK_STATUS(ERROR) << "image is empty";
        return image;
    }

    std::vector<Eigen::Vector3d> points;
    double radianDelta = GetAngle(item.image.pose, item.lidar.pose);
    float radianLeft, radianRight;
    GetAngle(pMatrix, 0, radianLeft);
    GetAngle(pMatrix, 640, radianRight);
    radianLeft += radianDelta;
    radianRight += radianDelta;

    std::vector<Eigen::Vector3d> pointsSelected;

    Polar2Cartesian(item.lidar.data, points); // --> x,y,z
//    CutSortEigen3DAngle(configParam, points, angleLeft, angleRight, pointsSelected);

    std::vector<Eigen::Vector3d> pointSort = pointsSelected;

    image = item.image.data.clone();
    const int w = image.cols, h = image.rows;

    std::sort(pointsSelected.begin(), pointsSelected.end(), CmpLeft2Right);
    double rightID = 10, leftID = -10;

    if (configParam.structure.lidarDirection)
    {
        leftID = 10;
        rightID = -10;
    }

    const double scaleIDX = w / abs(rightID - leftID);
    std::sort(pointSort.begin(), pointSort.end(), CmpLidarCloser);
    const double minValue = 0, maxValue = 10;
    const double scaleIDY = h / abs(maxValue - minValue);
    const double scaleValue = 255 / maxValue;
    const double stepIDX = abs(rightID - leftID);
    const double stepIDY = abs(maxValue - minValue);
    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorForward(255, 255, 255);

    // draw cols
    for (int i = 0; i <= stepIDX; ++i)
    {
        int x = i * scaleIDX;
        const int value = configParam.structure.lidarDirection ? leftID - i : i + leftID;

        cv::line(image, cv::Point(x, 0), cv::Point(x, h), colorBack, 1);
        cv::putText(image, std::to_string(value), cv::Point(MAX(10, x - 25), h - 10)
                    , cv::FONT_HERSHEY_PLAIN, 1, colorForward);
    }

    // draw rows
    for (int i = minValue; i <= maxValue; ++i)
    {
        int x = (stepIDY - i) * scaleIDY;
        cv::line(image, cv::Point(0, x), cv::Point(w, x), colorBack, 1);

        cv::putText(image, std::to_string(i)
                    , cv::Point(10, MAX(20, x - 10))
                    , cv::FONT_HERSHEY_PLAIN, 1, colorForward);
    }

    // draw lidar data
    for (size_t i = 0; i < w; ++i)
    {
        GetAngle(pMatrix, i, radianLeft);
        GetAngle(pMatrix, i + 1, radianRight);

        radianLeft += radianDelta;
        radianRight += radianDelta;

        CutSortEigen3DAngle(configParam, points, radianLeft, radianRight, pointsSelected);

        if (pointsSelected.empty()) continue;

        auto &p = pointsSelected.at(0);

        int x = i;

        const unsigned int y = (unsigned int) ((stepIDY - p.x()) * scaleIDY);
        const unsigned int value = (unsigned int) ((stepIDY - p.x()) * scaleValue);
        image.at<cv::Vec3b>(y, x)[0] = value;
        image.at<cv::Vec3b>(y, x)[1] = value;
        image.at<cv::Vec3b>(y, x)[2] = value;

        cv::Scalar color(255, 255, 255);

        const int defaultOther = 100;
        color = cv::Scalar(255, defaultOther, defaultOther);

        cv::circle(image, cv::Point(x, y), 2, color, -1);
    }

    return image;
}

double GetAngle(const psl::SlamResult &poseLast
                            , const psl::SlamResult &poseCurrent)
{
    // last -> current

    Eigen::Quaternion<double> last = Pose2Quaternion(poseLast);
    Eigen::Quaternion<double> current = Pose2Quaternion(poseCurrent);
    Eigen::Quaternion<double> quaternion = last.inverse() * current;
    Eigen::Matrix<double, 3, 1> radian = Quaternion2Angle(quaternion);

//    auto angle = radian * RADIAN_2_ANGLE;
    double heading = radian[2];

    if (abs(heading) < 0.0001) heading = 0.0001;

    return heading;
}
