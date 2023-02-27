//
// Created by indemind on 4/1/22.
//
#include <gtest/gtest.h>
#include "src/utils/config.h"
#include "src/test/types.h"
#include "src/utils/lidar.h"
#include "src/utils/detector_param.h"
#include "sample/utils/utils.h"
#include "src/instance/individual.h"

extern InputParam inputParam;
const float MAX_ANGLE_DIFF = 0.12;

float Mean(const std::vector<Eigen::Vector3d> &points, const int flag)
{
    float sum = 0;
    for (auto &point : points)
    {
        sum += point[flag];
    }
    return sum / points.size();
}

psl::Location GetLocationFromLidar(std::vector<Eigen::Vector3d> &points, const int num)
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

void GetLidarDepth(Individual individual, DetectResult boxes, Lidar lidar)
{
    double radianDelta = ::GetAngle(boxes.pose, lidar.pose);
    if (abs(radianDelta) > MAX_ANGLE_DIFF)
    {
        boxes.boxes.clear();
        return;
    }
    std::vector<Eigen::Vector3d> pointsSelected;
    float radianLeft, radianRight;
    std::vector<Eigen::Vector3d> points;

    Polar2Cartesian(lidar.data, points);
    for(int i = 0; i < boxes.boxes.size(); ++ i)
    {
        individual.GetLidarFromImageRegion(radianDelta, boxes.boxes[i].box.x,
                                           boxes.boxes[i].box.x + boxes.boxes[i].box.width, points, pointsSelected,
                                           radianLeft, radianRight);

        psl::Location machine = GetLocationFromLidar(pointsSelected, 5);
        std::cout << "xMin: "<< boxes.boxes[i].box.x << ", xMax: " << boxes.boxes[i].box.x + boxes.boxes[i].box.width
        << "  " << machine.Print() << std::endl;
    }
}

TEST(FunctorTest, TestLidar)
{
    ConfigParam param;
    Individual individual;


    LoadConfig(inputParam.configPath + CONFIG_FILE_NAME, param);
    psl::MoudleAllParam cameraParam;

    GetCameraConfig(inputParam.imagePath +  + "/config.yaml", cameraParam);
    individual.InitCamera(cameraParam);
    individual.SetParam(param);

    std::cout<<inputParam.configPath<<std::endl;
    std::cout<<inputParam.imagePath<<std::endl;
    std::cout<<inputParam.annoPath<<std::endl;

    std::vector<SyncDataFile> dataset;
    bool synced = GetData(inputParam.imagePath, dataset);
    std::cout<<dataset.size()<<std::endl;
    const size_t size = dataset.size();
    for (size_t i = 0; i < size; ++i)
    {
        SyncDataFile item = dataset.at(i);
        DataGroup itemData;
        itemData.orgImage.data = cv::imread(item.image, cv::IMREAD_GRAYSCALE);

        std::cout<<"img: "<<item.image<<std::endl;

        if (not GetLidar(item.lidar, itemData.lidar.data, false)) continue;
        if (not GetPose(item.lidarPose, itemData.lidar.pose)) continue;
        if (not GetPose(item.imagePose, itemData.orgImage.pose)) continue;

        DetectResult boxes;
        BoxInfo box;
        for(int cx = 3; cx < 640; cx += 5)
        {
            int cy = 250;
            int w = 5;
            int h = 100;
            box.FromCenter(cx, cy, w, h);
            box.classID = ESCALATOR;
            boxes.boxes.push_back(box);
        }

        boxes.pose = itemData.orgImage.pose;
        GetLidarDepth(individual, boxes, itemData.lidar);
        std::cout<<i<<std::endl;
    }
}
