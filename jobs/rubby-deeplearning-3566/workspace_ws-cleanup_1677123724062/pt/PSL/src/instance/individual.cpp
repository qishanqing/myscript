#include "individual.h"

#include <cmath>

#include "src/utils/define.h"
#include "src/utils/lidar.h"
#include "src/utils/slam.h"
#include "src/utils/utils.h"
#include "src/utils/map_define.h"

const std::vector<float> SOFT_BOUNDARY = {5, 5};
const std::vector<float> mClipRange = {0.17f, 10.0f};
const float AMEND_THRESHOLD = 0.2;
const float MIN_SCALE = 0.05;
const float MAX_SCALE = 0.5;
const float ESCALATOR_SHAPE_DEPTH = 1.0; // 1m
const float PERSON_SHAPE_DEPTH = 0.5;  // 30cm
const size_t RESERVE_NUM_DEPTH = 1;
const size_t RESERVE_NUM_EDGE = 5;
const float MAX_ANGLE_DIFF = 0.12; // 7
const psl::Location DEFAULT_LOCATION;
const int EXTEND_PERSON = 15; // 10 pixel
const int EXTEND_ESCALATOR = 25; // 10 pixel
const int EXTEND_HANDRAILS = 30; // 10 pixel
const int EXTEND_STEP = 20; // 10 pixel

#define RANGE_EXTEND(height) (((height) < 100) ? 0 : MIN((height), 200) / 10)

Individual::Individual()
{

}

//only lidar and angle
void Individual::GetDistance(const Lidar &lidar, DetectResult &boxes)
{
    std::vector<Eigen::Vector3d> points;

    Polar2Cartesian(lidar.data, points); // --> x,y,z
//    ShowLidar(points);

    int size = boxes.boxes.size();

    LOG_CHECK_DEBUG(INFO) << "image rotation: " << (boxes.time == boxes.pose.s_time)
                          << " " << boxes.time
                          << " " << boxes.pose.s_time
                          << " " << Join(boxes.pose.s_rotation, 4, ", ");
    LOG_CHECK_DEBUG(INFO) << "lidar rotation: "
                          << (lidar.data.header.stamp == lidar.pose.s_time)
                          << " " << lidar.data.header.stamp
                          << " " << lidar.pose.s_time
                          << " " << Join(lidar.pose.s_rotation, 4, ", ");

    double radianDelta = ::GetAngle(boxes.pose, lidar.pose);
    LOG_CHECK_DEBUG(INFO) << "lidar first: " << (lidar.data.header.stamp < boxes.time)
                          << " " << ABS_DIFF(lidar.data.header.stamp, boxes.time) / 1000
                          << " delta: " << radianDelta * RADIAN_2_ANGLE;

    if (abs(radianDelta) > MAX_ANGLE_DIFF)
    {
        boxes.boxes.clear();
        LOG_CHECK_STATUS(INFO)
                << "angle between image and lidar is too large (" << radianDelta
                << ")";
        return;
    }

    for (int i = 0; i < size; ++i)
    {
        auto &box = boxes.boxes.at(i);
        LOG_CHECK_DEBUG(INFO) << box.className << " " << box.cx << ", " << box.cy;
        if (not GetLocationByRegion(points, radianDelta, box))
        {
            boxes.boxes.erase(boxes.boxes.begin() + i);
            i--;
            size--;
        }
    }
}

void Individual::GetLidarFromImageRegion(const double radianDelta, float left, float right
                                         , const std::vector<Eigen::Vector3d> &points
                                         , std::vector<Eigen::Vector3d> &pointsSelected
                                         , float &radianLeft, float &radianRight) const
{
// angle to left of camera
    ::GetAngle(P, left, radianLeft);
    ::GetAngle(P, right, radianRight);

//    LOG_CHECK_DEBUG(INFO) << "left: " << radianLeft * RADIAN_2_ANGLE;
//    LOG_CHECK_DEBUG(INFO) << "right: " << radianRight * RADIAN_2_ANGLE;

    // fix angle
    radianLeft += radianDelta;
    radianRight += radianDelta;

//    LOG_CHECK_DEBUG(INFO) << "left fix: " << radianLeft * RADIAN_2_ANGLE;
//    LOG_CHECK_DEBUG(INFO) << "right fix: " << radianRight * RADIAN_2_ANGLE;

// get roi lidar data by angle check
    CutSortEigen3DAngle(configParam, points, radianLeft + radianDelta, radianRight + radianDelta
                        , pointsSelected);
}

void Individual::ShowLidar(const std::vector<Eigen::Vector3d> &points) const
{
    float angleLeft, angleRight;
    ::GetAngle(P, 0, angleLeft);
    ::GetAngle(P, 640, angleRight);

    // TODO :: add angle delta

    std::vector<Eigen::Vector3d> pointsSelected;

    CutSortEigen3DAngle(configParam, points, angleLeft, angleRight, pointsSelected);

    std::vector<Eigen::Vector3d> pointSort = pointsSelected;

    const int w = 1080, h = 960;
    cv::Mat image = cv::Mat::zeros(h, w, CV_8UC3);
    const size_t size = pointsSelected.size();

    if (pointsSelected.empty())
    {
        cv::imshow("lidar", image);
        cv::waitKey(20);
        return;
    }

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

    // draw cols
    for (int i = 0; i <= stepIDX; ++i)
    {
        int x = i * scaleIDX;
        const int value = configParam.structure.lidarDirection ? leftID - i : i + leftID;

        cv::line(image, cv::Point(x, 0), cv::Point(x, h), colorBack, 1);
        cv::putText(image, std::to_string(value), cv::Point(MAX(10, x - 25), h - 10)
                    , cv::FONT_HERSHEY_PLAIN, 1, colorBack);
    }

    // draw rows
    for (int i = minValue; i <= maxValue; ++i)
    {
        int x = (stepIDY - i) * scaleIDY;
        cv::line(image, cv::Point(0, x), cv::Point(w, x), colorBack, 1);

        cv::putText(image, std::to_string(i)
                    , cv::Point(10, MAX(20, x - 10))
                    , cv::FONT_HERSHEY_PLAIN, 1, colorBack);
    }

    // draw lidar data
    for (size_t i = 0; i < size; ++i)
    {
        const auto &p = pointsSelected.at(i);
        const unsigned int value = (unsigned int) ((stepIDY - p.x()) * scaleValue);
        const unsigned int id = (unsigned int) (
                (configParam.structure.lidarDirection ? leftID - p.y() : p.y() - leftID) *
                scaleIDX);
        image.at<cv::Vec3b>(value, id)[0] = value;
        image.at<cv::Vec3b>(value, id)[1] = value;
        image.at<cv::Vec3b>(value, id)[2] = value;

        cv::Scalar color(255, 255, 255);

        if (p.x() < 3)
            color = cv::Scalar(value, 255, 255);
        else if (p.x() < 5)
            color = cv::Scalar(255, 255, value);
        else
            color = cv::Scalar(255, value, 255);
        cv::circle(image, cv::Point(id, value), 1, color, -1);
    }

    cv::imshow("lidar", image);
    cv::waitKey(20);
}

bool Individual::GetLocationByRegion(const std::vector<Eigen::Vector3d> &points
                                     , const double radianDelta, BoxInfo &box)
{
    std::vector<Eigen::Vector3d> pointsSelected;

    // get 3D box
    GetLocationByRegion(box);
    psl::Location locationFromTriangleEstimate = box.location.machine;

    float frontDistance = locationFromTriangleEstimate.y, backDistance = 0;
    float radianLeft, radianRight;

    // crop the lidar position
    if (box.IsEscalatorHandrails())
    {
        backDistance = frontDistance + ESCALATOR_SHAPE_DEPTH;
        const int expandWidth = 0;
        GetLidarFromImageRegion(radianDelta, box.box.x - expandWidth,
                box.box.x + box.width + (expandWidth << 1), points, pointsSelected
                                , radianLeft, radianRight);
    }
    else
    {
        backDistance = frontDistance + PERSON_SHAPE_DEPTH;
        GetLidarFromImageRegion(radianDelta, box.box.x, box.box.x + box.width, points
                                , pointsSelected, radianLeft, radianRight);
    }
    if (pointsSelected.empty())
    {
        box.location.machine = DEFAULT_LOCATION;
        LOG_CHECK_DEBUG(WARNING) << "lidar data is empty";
        return false;
    }

    std::sort(pointsSelected.begin(), pointsSelected.end(), CmpLeft2Right);
    auto pointsTemp = pointsSelected;
    pointsTemp.resize(MIN(pointsTemp.size(), RESERVE_NUM_EDGE));
    box.edgeLeft.machine.y =
            MAX_DISTANCE_4_DETECT_REGION * cos(radianLeft); // Mean(pointsTemp, 0);
    box.edgeLeft.machine.x =
            MAX_DISTANCE_4_DETECT_REGION * sin(radianLeft); // Mean(pointsTemp, 1);
    box.edgeLeft.machine.z = 0; //pointsSelected.begin()->z();
    pointsTemp = pointsSelected;
    std::reverse(pointsTemp.begin(), pointsTemp.end());
    pointsTemp.resize(MIN(pointsTemp.size(), RESERVE_NUM_EDGE));
    box.edgeRight.machine.y =
            MAX_DISTANCE_4_DETECT_REGION * cos(radianRight); //Mean(pointsTemp, 0);
    box.edgeRight.machine.x =
            MAX_DISTANCE_4_DETECT_REGION * sin(radianRight);; //Mean(pointsTemp, 1);
    box.edgeRight.machine.z = 0; // pointsSelected.rbegin()->z();
    box.location.machine = GetLocationFromLidar(pointsSelected, RESERVE_NUM_DEPTH);

    LOG_CHECK_DEBUG(INFO) << "location(machine): " << box.location.machine.Print();
    LOG_CHECK_DEBUG(INFO) << "left(machine): " << box.edgeLeft.machine.Print();
    LOG_CHECK_DEBUG(INFO) << "right(machine): " << box.edgeRight.machine.Print();

    if (box.location.machine.y < frontDistance)
    {
        locationFromTriangleEstimate = TriangleEstimate(box, EXTEND_STEP);
        frontDistance = locationFromTriangleEstimate.y;
        LOG_CHECK_DEBUG(INFO) << "triangle location 2: "
                              << locationFromTriangleEstimate.Print();
    }

    if (box.IsEscalator())
    {
        box.location.machine = locationFromTriangleEstimate;
    }
    else if (box.location.machine.y < frontDistance)
    {
        if (box.IsEscalatorHandrails())
        {
            box.lidarInValid = true;
            LOG_CHECK_DEBUG(INFO) << "drop location.";
        }
        else
        {
            box.location.machine = DEFAULT_LOCATION;
            LOG_CHECK_DEBUG(INFO) << "drop location.";
        }
    }

    return true;

    // get distance from lidar and triangle estimate
    bool frontDistanceInvalid = locationFromTriangleEstimate.IsDefault();


    // TODO : check x axis and y axis
    if (box.IsEscalator() or box.IsEscalatorHandrails())
    {
        if ((not frontDistanceInvalid) and (box.location.machine.y > frontDistance and
                                            box.location.machine.y < backDistance))
        {
            LOG_CHECK_DEBUG(INFO) << "depth normal: " << box.location.machine.Print()
                                  << " [" << locationFromTriangleEstimate.Print()
                                  << "]";
            return true;
        }
        else
        {

            // estimate fail
            if (frontDistanceInvalid)
            {
                LOG_CHECK_DEBUG(INFO) << "depth estimate error: "
                                      << " [" << box.location.machine.Print()
                                      << "]";
                box.location.machine = DEFAULT_LOCATION;
                return true;
            }

            LOG_CHECK_DEBUG(INFO) << "depth block: "
                                  << locationFromTriangleEstimate.Print()
                                  << " [" << box.location.machine.Print() << "]";
            // object is blocked
            box.location.machine = DEFAULT_LOCATION;

            return true;
            box.location.machine.x = locationFromTriangleEstimate.x;
            box.location.machine.y = locationFromTriangleEstimate.y;

            if (box.IsEscalator())
            {
                BoxInfo left = box;
                left.width = MAX(25, MIN(35, int(box.box.width * 0.3)));
                left.box.width = left.width;
                left.cx = box.box.x + (left.width * 0.5);
                psl::Location leftLocationFromTriangleEstimate = TriangleEstimate(
                        left);
                box.edgeLeft.machine = leftLocationFromTriangleEstimate;

                BoxInfo right = box;
                right.width = MAX(25, MIN(35, int(box.box.width * 0.3)));
                right.box.x = box.box.x + box.box.width - right.width;
                right.box.width = right.width;
                right.cx = right.box.x + (right.width * 0.5);
                psl::Location rightLocationFromTriangleEstimate = TriangleEstimate(
                        right);
                box.edgeRight.machine = rightLocationFromTriangleEstimate;
            }
        }
    }

    return true;
}

psl::Location Individual::TriangleEstimate(BoxInfo &box, const int baseExtend)
{
    psl::Location location;
    double s = 1.0;
    double height = box.height;

    if (baseExtend > 0)
    {
        double extentHeight = baseExtend - RANGE_EXTEND(HEIGHT - (box.box.y + box.height));

        LOG_CHECK_DEBUG(INFO) << "extend: " << extentHeight;
        if (box.IsEscalator()) height += EXTEND_ESCALATOR + extentHeight;
        else if (box.IsPerson()) height += EXTEND_PERSON + extentHeight;
        else if (box.IsEscalatorHandrails()) height += EXTEND_HANDRAILS + extentHeight;
        LOG_CHECK_DEBUG(INFO) << "before y: " << box.box.y + box.height
                              << " last y: " << box.box.y + height;
    }
    height = MIN(HEIGHT, box.box.y + height) - box.box.y;

    return TriangleEstimate(box.cx, box.box.y + height);

    double bottom = box.box.y + height;
    double bottomCorrect = bottom;
    float cx = K_T(0, 2);
    float cy = K_T(1, 2);
    const float f = K_T(0, 0);

    const float angle = this->configParam.structure.cameraAngle;
    float cameraHeight = this->configParam.structure.cameraHeight;

    if ((angle - 0) > 0.001)
    {
        bottomCorrect = f * tan(atan((bottom - cy) / f) - angle) + cy;
//        bottomCorrect = cos(angle) * (bottom - cy) * f / (sin(angle) * (bottom - cy) + f) + cy;
    }


    double ground = s * (bottomCorrect); // - 4;
    const double z = 1.0;

    // ignore object above the primary point
    if (ground < cy) // ignore the point above the image half
    {
        return location;
    }

//    ground -= cy;

    // calculate location in world coords system
    Eigen::MatrixXd points(3, 3);
    Eigen::Vector3d leftGround((box.box.x) * s, ground, z);
    Eigen::Vector3d centerGround(box.cx * s, ground, z);
    Eigen::Vector3d rightGround((box.box.x + box.width) * s, ground, z);
    points << leftGround, centerGround, rightGround;

    auto tmp = K_T.inverse() * points;
    double scale = cameraHeight / tmp(1);
    auto locationTmp = tmp * scale;
    location.x = locationTmp(0, 1);
    location.y = locationTmp(2, 1);

//    // check depth
//    float distanceEuclidean = locationTmp(2, 1);
//    if (distanceEuclidean > mClipRange[1] || distanceEuclidean < mClipRange[0])
//    {
//        return location;
//    }
//
//    Eigen::Matrix<double, 3, 3> rotate = Eigen::Matrix<double, 3, 3>::Identity();
//    rotate << 1, 0, 0, 0, 0, 1, 0, -1, 0;
//
//    auto newLocation = (R * rotate * locationTmp).colwise() + D;
//
//    float targetScale = (newLocation.col(0) - newLocation.col(2)).norm();
//    float scaleTmp = std::min(std::max(targetScale, MIN_SCALE), MAX_SCALE);
//
//    // add rear location
//    Eigen::Matrix3d rearLocation = locationTmp;
//    for (int i = 0; i < 3; i++)
//    {
//        rearLocation(2, i) += scaleTmp;
//    }
//
//    rearLocation = (R * rotate * rearLocation).colwise() + D;
//
//    location.x = (newLocation(0, 1) + rearLocation(0, 1)) / 2.0;
//    location.y = (newLocation(1, 1) + rearLocation(1, 1)) / 2.0;
//
////  location.x = newLocation(0, 1);
////  location.y = newLocation(1, 1);
//    double theta = atan((abs(location.x) / abs(location.y))); // TODO : why abs
//    theta = theta * 180.0 / M_PI;
//


    return location;
}

void Individual::ConvertCamera2Lidar(psl::Location &location)
{
    // left camera to lidar
    location.x -= configParam.structure.leftcamera2lidar[1];
    location.y += configParam.structure.leftcamera2lidar[0];

    if (configParam.structure.lidarDirection)
    {
        location.x = -location.x;
    }
}

psl::Location Individual::TriangleEstimate(float x, float y)
{
    psl::Location location;
    double s = 1.0;

    double bottom = y;
    double bottomCorrect = bottom;
    auto currentK = K_T;
//    currentK(0, 2) = K(0, 2);
//    currentK(1, 2) = K(1, 2);
    float cy = currentK(1, 2);
    float delta_cy = currentK(1, 2) - cy;
    const float f = currentK(0, 0);

    const float angle = this->configParam.structure.cameraAngle;
    float cameraHeight = this->configParam.structure.cameraHeight;

    if ((angle - 0) > 0.001)
    {
        bottomCorrect = f * tan(atan((bottom - cy) / f) - angle);
//        LOG_CHECK_STATUS(INFO) << " cy P, f K: " << K(0, 0) * tan(atan((bottom - K_T(1, 2)) / K(0, 0)) - angle);
//        LOG_CHECK_STATUS(INFO) << " cy K, f P: " << f * tan(atan((bottom - K(1, 2)) / f) - angle);
//        LOG_CHECK_STATUS(INFO) << " cy K, f K: " << K(0, 0) * tan(atan((bottom - K(1, 2)) / K(0, 0)) - angle) << "\n\n";

        bottomCorrect += cy;
        LOG_CHECK_DEBUG(INFO) << " height: " << bottomCorrect - cy << " bottom: " << bottom << ", " << bottomCorrect;

//        bottomCorrect = cos(angle) * (bottom - cy) * f / (sin(angle) * (bottom - cy) + f) + cy;
    }

    double ground = s * (bottomCorrect); // - 4;
    const double z = 1.0;

    // ignore object above the primary point
    if (ground < cy) // ignore the point above the image half
    {
        return location;
    }

    Eigen::MatrixXd points(3, 3);
    Eigen::Vector3d leftGround(x * s, ground, z);
    points << leftGround, leftGround, leftGround;

    auto tmp = currentK.inverse() * points;
    double scale = cameraHeight / tmp(1);
    auto locationTmp = tmp * scale;
    location.x = locationTmp(0, 1);
    location.y = locationTmp(2, 1);
    // TODO : 2022-08-24 10:59:20 [hao]  set default z as 0, not INT16_MAX
//    location.z = locationTmp(1, 1);
    location.z = 0;

    LOG_CHECK_DEBUG(INFO) << "triangle estimate: " << location.y;
    ConvertCamera2Lidar(location);

    return location;
//    // check depth
//    float distanceEuclidean = locationTmp(2, 1);
//    if (distanceEuclidean > mClipRange[1] || distanceEuclidean < mClipRange[0])
//    {
//        return location;
//    }
//
//    Eigen::Matrix<double, 3, 3> rotate = Eigen::Matrix<double, 3, 3>::Identity();
//    rotate << 1, 0, 0, 0, 0, 1, 0, -1, 0;
//
//    auto newLocation = (R * rotate * locationTmp).colwise() + D;
//
//    float targetScale = (newLocation.col(0) - newLocation.col(2)).norm();
//    float scaleTmp = std::min(std::max(targetScale, MIN_SCALE), MAX_SCALE);
//
//    // add rear location
//    Eigen::Matrix3d rearLocation = locationTmp;
//    for (int i = 0; i < 3; i++)
//    {
//        rearLocation(2, i) += scaleTmp;
//    }
//
//    rearLocation = (R * rotate * rearLocation).colwise() + D;
//
//    location.x = (newLocation(0, 1) + rearLocation(0, 1)) / 2.0;
//    location.y = (newLocation(1, 1) + rearLocation(1, 1)) / 2.0;
//
////  location.x = newLocation(0, 1);
////  location.y = newLocation(1, 1);
//    double theta = atan((abs(location.x) / abs(location.y))); // TODO : why abs
//    theta = theta * 180.0 / M_PI;
}

void Individual::GetDistance(DetectResult &boxes)
{
    int size = boxes.boxes.size();

    for (int i = 0; i < size; ++i)
    {
        auto &box = boxes.boxes.at(i);
        if (not GetLocationByRegion(box))
        {
            boxes.boxes.erase(boxes.boxes.begin() + i);
            i--;
            size--;
        }
    }
}

bool Individual::GetLocationByRegion(BoxInfo &box)
{
    if (box.className == "desk_rect" || box.className == "desk_circle")
    {
        for (int j = 0; j < 4; ++j)
        {
            psl::Location locationFromTriangleEstimate
                    = TriangleEstimate(box.keypoint.point[j * 2]
                                       , box.keypoint.point[j * 2 + 1]);
            box.locationSerials.push_back(Location(locationFromTriangleEstimate));
        }
    }
    psl::Location locationFromTriangleEstimate = TriangleEstimate(box);

    LOG_CHECK_DEBUG(INFO) << "triangle location: "
                          << locationFromTriangleEstimate.Print();
    box.location.machine = locationFromTriangleEstimate;

    box.edgeLeft.machine = TriangleEstimate(box.box.x, box.box.y + box.box.height);
    box.edgeRight.machine = TriangleEstimate(box.box.x + box.box.width, box.box.y + box.box.height);

    return true;
}

