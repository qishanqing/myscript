//////////////////////////////////////////////////////////////////////
///  @file     test_instance.cpp
///  @brief    unit test for InstanceManager
///  Details.
///  @author   sunhao
///  @date     2021.07.26
///
///  revision statement:
//////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include "src/perception.h"
#include "src/utils/utils.h"
#include "sample/utils/utils.h"
#include "types.h"
#include "src/test/utils.h"
#include "src/instance/manager.h"
#include "src/utils/config.h"

extern InputParam inputParam;

using namespace robot_detector;

InstanceManager instance;

void InstanceCheck(const ConfigParam& configParam
                   , const std::vector<BoxInfo> boxes
                   , const swr::psl_sensor_types::LaserScan &lidar
                   , const std::vector<int> &distances)
{
    std::vector<psl::Instance> instances;
    psl::Obstacle obstacle;
    DetectResult boxesSrc;
    boxesSrc.boxes = boxes;
    DetectResult boxesClean;
    Lidar lidarStruct;
    lidarStruct.data = lidar;

    instance.GetInstancesInfo(boxesSrc, lidarStruct, instances, obstacle, boxesClean);

    for (auto &obj : instances)
    {
        Lidar2Machine(configParam, obj.locationMachine);
    }

    for (int i = 0; i < instances.size(); ++i)
    {
        EXPECT_LE(abs(instances[i].locationMachine.x*100 -distances[0]), 20);
    }
}

void Init()
{
    // load config and dataset
    std::string imagesTxt = inputParam.imagePath + "/image.txt";
    std::string lidarTxt = inputParam.imagePath + "/lidar.txt";
    std::string configFile = inputParam.configPath + CONFIG_FILE_NAME;
    std::string cameraConfigFile = inputParam.imagePath + "/config.yaml";

    std::vector<std::string> imageList;
    std::vector<std::string> lidarList;
    ConfigParam configParam;
    psl::CameraMoudleParam param;

    GetCameraConfig(cameraConfigFile, param);
    ReadFile(imagesTxt, imageList);
    ReadFile(lidarTxt, lidarList);
    Status error = LoadConfig(configFile, configParam);

    if (SUCCESS != error.code)
    {
        ERROR_PRINT(error.message);
        exit(0);
    }

    // init instance
    instance.InitCamera(param);
    instance.SetParam(configParam);

    // test instance
    for (size_t i = 0; i < lidarList.size(); ++i)
    {
        std::string lidarFile = inputParam.imagePath + "/" + lidarList.at(i);
        std::string annoFile = inputParam.annoPath + "/" + imageList.at(i);

        FindReplace(annoFile, ".jpg", ".txt");

        ERROR_PRINT(lidarFile);

        swr::psl_sensor_types::LaserScan lidars;
        std::vector<BoxInfo> boxes;
        std::vector<int> distances;

        GetLidar(lidarFile, lidars, false);
        GetAnnotations(annoFile, boxes, distances);

        InstanceCheck(configParam, boxes, lidars, distances);
    }
}

// case test
TEST(InstanceTest, Init)
{
    Init();
    EXPECT_EQ(1, 1) << "error in location caculate";
}