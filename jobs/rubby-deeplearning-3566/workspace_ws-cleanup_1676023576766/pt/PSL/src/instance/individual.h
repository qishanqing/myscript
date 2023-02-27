//////////////////////////////////////////////////////////////////////
///  @file     individual.h
///  @brief    get the distance for an individual body
///  @author   sunhao
///  @date     2021.07.27
//////////////////////////////////////////////////////////////////////
#ifndef DETECTOR_SAMPLE_INDIVIDUAL_H
#define DETECTOR_SAMPLE_INDIVIDUAL_H

#include "src/instance/instance.h"

class Individual : public Instance
{
public:
    Individual();

    void GetDistance(const Lidar &lidar, DetectResult &boxes);
    void GetDistance(DetectResult &boxes);

    void GetLidarFromImageRegion(const double radianDelta, float left, float right
                                 , const std::vector<Eigen::Vector3d> &points
                                 , std::vector<Eigen::Vector3d> &pointsSelected
                                 , float &radianLeft, float &radianRight) const;

private:
    void ShowLidar(const std::vector<Eigen::Vector3d> &points) const;

    bool GetLocationByRegion(const std::vector<Eigen::Vector3d> &points
                             , const double radianDelta, BoxInfo &box);

    bool GetLocationByRegion(BoxInfo &box);

    psl::Location TriangleEstimate(BoxInfo &box, const int baseExtend = 0);


private:
    float cameraHeight;
};


#endif //DETECTOR_SAMPLE_INDIVIDUAL_H
