#include "instance.h"
#include <vector>
#include <chrono>
#include <numeric>
#include "src/utils/define.h"
#include "src/utils/lidar.h"

const size_t RESERVE_NUM = 3;
const size_t SERIALS_NUM = 5;

using namespace std;

Instance::Instance()
{
    K = Eigen::Matrix<double, 3, 3>::Identity();
    D = Eigen::Matrix<double, 3, 1>::Zero();
    R = Eigen::Matrix<double, 3, 3>::Identity();
    P = Eigen::Matrix<double, 3, 4>::Zero();
}

Instance::~Instance()
{

}

void Instance::InitCamera(const psl::CameraMoudleParam &cameraParam)
{
    const double *k = cameraParam._left_camera.at(RESOLUTION)._K;
    const double *p = cameraParam._left_camera.at(RESOLUTION)._P;
    const double *r = cameraParam._left_camera.at(RESOLUTION)._R;
    const double *d = cameraParam._left_camera.at(RESOLUTION)._D;

    K << k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8];
    P << p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11];
    R << r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8];
    D << d[0], d[1], d[2];
    K_T << p[0], p[1], p[2], p[4], p[5], p[6], p[8], p[9], p[10];
}

float Instance::Mean(const vector<Eigen::Vector3d> &points, const int flag) const
{
    float sum = 0;
    for (auto &point : points)
    {
        sum += point[flag];
    }
    return sum / points.size();
}

float Instance::Min(const vector<Eigen::Vector3d> &points, const int flag) const
{
    float min = INT16_MAX;
    for (auto &point : points)
    {
        min = MIN(point[flag], min);
    }
    return min;
}

float Instance::Max(const vector<Eigen::Vector3d> &points, const int flag) const
{
    float max = INT16_MAX;
    for (auto &point : points)
    {
        max = MAX(point[flag], max);
    }
    return max;
}

psl::Location
Instance::GetLocationFromLidar(std::vector<Eigen::Vector3d> &points, const int num) const
{
    sort(points.begin(), points.end(), CmpLidarCloser);
    std::size_t size = MIN(points.size(), num);
    points.resize(size);

    psl::Location loc;
    // TODO : return the value , do not directly set th machine
    loc.y = Mean(points, 0);
    loc.x = Mean(points, 1);
    loc.z = Mean(points, 2);



    return loc;
}

void Instance::SetParam(const ConfigParam &param)
{
    this->configParam = param;
}
