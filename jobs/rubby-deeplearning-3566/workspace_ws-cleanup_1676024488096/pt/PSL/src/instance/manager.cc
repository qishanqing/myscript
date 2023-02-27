#include "manager.h"
#include <vector>
#include <chrono>
#include "src/utils/utils.h"
#include "src/utils/define.h"
#include "src/instance/types.h"
#include "src/utils/lidar.h"

using namespace std;

InstanceManager::InstanceManager()
        : errorDomain(false)
        , withLidar(true)
{
    distSafe = 3;
    distAngle = 30;
}

InstanceManager::~InstanceManager()
{

}

void InstanceManager::InitCamera(const psl::CameraMoudleParam& cameraParam)
{
    individual.InitCamera(cameraParam);
    if (withLidar)
    {
        composed.InitCamera(cameraParam);
    }
}

bool InstanceManager::GetInstancesInfo(DetectResult &boxes
                                       , const Lidar &lidar
                                       , std::vector<psl::Instance> &instances
                                       , psl::Obstacle &obstacle
                                       , DetectResult &boxesClean)
{
    if (withLidar and 0 == lidar.data.ranges.size())
    {
        CHECK_STATUS_WITH_TIME_END(false, ONE_SECOND, "lidar data empty!");
        return false;
    }

    DetectResult boxesTemp = boxes;

    if (withLidar)
    {
        individual.GetDistance(lidar, boxesTemp);
        boxes = boxesTemp;
        composed.GetDistance(lidar, boxesTemp);
    }
    else
    {
        individual.GetDistance(boxesTemp);
        boxes = boxesTemp;
    }
    GetMap(boxesTemp.boxes, boxes.pose.s_time, instances, obstacle);
    boxesClean.Swap(boxesTemp);

    return true;
}

void InstanceManager::SetParam(const ConfigParam param)
{
    configParam = param;
    individual.SetParam(param);
    motionMap.SetParam(param);
    distSafe = param.distance;
    distAngle = param.angle;
}

bool InstanceManager::ErrorDomain() const
{
    return this->errorDomain;
}

void InstanceManager::UpdateObstacle(instance::Obstacle &obstacle)
{
    if (obstacle.mapInfoPreSize == obstacle.mapInfoSize)
    {
        obstacle.status = 0;
    }
    int index = -1;
    for (auto &map_info: obstacle.maps)
    {
        index += 1;
        if (map_info.havePerson == 0)
        {
            map_info.noPersonCount += 1;
            if (map_info.noPersonCount >= 10)
            {
                obstacle.mapIndexes.push_back(index);
            }
        }
        else
        {
            map_info.staticCount += 1;
        }

        map_info.havePerson = 0;
    }
    if (obstacle.mapIndexes.size() > 0)
    {
        reverse(obstacle.mapIndexes.begin(), obstacle.mapIndexes.end());
        for (auto &map_index: obstacle.mapIndexes)
        {
            obstacle.maps.erase(obstacle.maps.begin() + map_index);
            obstacle.mapInfoSize -= 1;
        }
        obstacle.mapIndexes.clear();
    }
}

void InstanceManager::GetMap(const vector<BoxInfo> &boxes
                             , const std::uint64_t time
                             , vector<psl::Instance> &instances
                             , psl::Obstacle &obstacleOut)
{
    instance::Obstacle obstacle;

    errorDomain = false;
    obstacle.mapInfoPreSize = obstacle.mapInfoSize;
    obstacle.status = 1;


    for (auto &box: boxes)
    {
        psl::Instance object = box.ConvertInstace();
        if (psl::DEFAULT_ID == object.classID)
        {
            continue;
        }
        object.time = time;

        if (object.locationMachine.y<distSafe
                                     && tan(distAngle * 3.1415926 / 180.0f)>
                (abs(object.locationMachine.x) / abs(object.locationMachine.y)))
        {
            LOG_CHECK_DEBUG(ERROR) << "errorDomain";
            errorDomain = true;
            motionMap.GetInstanceStatus(object, obstacle);
        }

        instances.push_back(object);
    }

    UpdateObstacle(obstacle);

    obstacle.ToObstacle(obstacleOut);
}

void InstanceManager::UnloadLidar()
{
    withLidar = false;
}

void InstanceManager::EnableLidar(const bool withLidar)
{
    this->withLidar = withLidar;
}