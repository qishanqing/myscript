#ifndef __INSTANCE_MANAGER_H__
#define __INSTANCE_MANAGER_H__

#include <vector>
#include <cmath>
#include <iostream>
#include <map>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "src/utils/data_type.h"
#include "src/utils/config.h"
#include "src/utils/detector_param.h"
#include "src/instance/motion_map.h"
#include "src/instance/individual.h"
#include "src/instance/composed.h"


class InstanceManager
{

public:
    InstanceManager();

    virtual ~InstanceManager();

    void InitCamera(const psl::CameraMoudleParam& cameraParam);

    void UnloadLidar();

    /*only lidar and angle*/
    bool GetInstancesInfo(DetectResult &boxes
                          , const Lidar &lidar
                          , std::vector<psl::Instance> &instances
                          , psl::Obstacle &obstacle
                          , DetectResult &boxesClean);

    void SetParam(const ConfigParam param);

    bool ErrorDomain() const;

    void UpdateObstacle(Obstacle &obstacle);

private:
    void GetMap(const std::vector<BoxInfo> &boxes
                , const std::uint64_t time
                , std::vector<psl::Instance> &instances
                , psl::Obstacle &obstacleOut);

private:
    Individual individual;
    Composed composed;
    float distSafe;
    float distAngle;
    bool errorDomain;
    MotionMap motionMap;
    ConfigParam configParam;
    bool withLidar;
};

#endif
