#include "perception.h"
#include <memory>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <opencv2/core.hpp>
#include <sys/time.h>
#include "utils/utils.h"
//#include "../src/utils/data_type.h"

using namespace robot_detector;

std::queue<std::string> historyFiles = {};
std::mutex fileMutex;
static bool annotation = false;

namespace sample
{
struct Lidar
{
    swr::psl_sensor_types::LaserScan data;
    psl::SlamResult pose;
};

struct Image
{
    psl::Time time;
    cv::Mat data;
    psl::SlamResult pose;
};

struct DataGroup
{
    Lidar lidar;
    Image image, orgImage;
    psl::Time time = 0;
};
}

bool
Annotation(const std::string file, const std::vector<psl::Instance> &instanceInfos)
{
    bool ret = true;

    std::ofstream stream(file);

    for (const auto &obj : instanceInfos)
    {
        ret &= obj.Write(stream);
    }

    stream.close();


//    std::vector<psl::Instance> out;
//    std::ifstream stream("file.txt");
//    while(stream)
//    {
//        psl::Instance i;
//        i.Read(stream);
//        out.push_back(i);
//    }
//    stream.close();

    return ret;
}

static void
GetDetectorResult(const robot_detector::DetectorResult &object
                  , const robot_detector::DetectorResultMap &obstacle)
{
    std::cout << "object.size: " << object.size() << ", obstaacle.size: " << obstacle.maps.size() << std::endl;
    return;
    static struct timeval start, end;
    gettimeofday(&end, nullptr);
    ERROR_PRINT("use time [algorithm](send data & run): " +
                std::to_string((1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec -
                                start.tv_usec) * 1.0 / 1000) + "ms");
    gettimeofday(&start, nullptr);

    fileMutex.lock();
    std::string file = historyFiles.front();
    WARNING_PRINT(file);
    historyFiles.pop();
    fileMutex.unlock();

    if (obstacle.maps.size() > 0)
    {
        std::cout << *std::dec << "mapinfo size: " << obstacle.maps.size() << std::endl;
    }

    if (annotation)
    {
        Annotation(file.substr(0, file.rfind(".")) + ".txt", object);
    }
}

static void GetErrorMessageResult(psl::ErrorMessage errorMessage)
{
    std::cout << "callback: " << errorMessage.state << " ["
              << errorMessage.stamp << "] "
              << std::hex << errorMessage.code << std::endl;
}

void Help()
{
    ERROR_PRINT("please input: ");
    INFO_PRINT("\tconfig dir: [string]");
    INFO_PRINT("\tinput image dir: [string]");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        Help();
        return 0;
    }

    std::string projectMode = argv[1];
    std::string inputDir = argv[3];
    std::string configDir = std::string(argv[2]) + '/';

    if (argc > 3)
    {
        annotation = argv[3];
    }

    psl::CameraMoudleParam param;
    auto pathName = getCurrentExePath();
    auto exeName = getCurrentExeName();
    std::cout << pathName << ":" << " " << exeName << std::endl;

    std::string cameraConfigFile = inputDir + "/config.yaml";

    GetCameraConfig(cameraConfigFile, param);

    robot_detector::Detector *detector = robot_detector::Create();
    std::vector<SyncDataFile> dataset;

    bool binocular = GetData(inputDir, dataset);

    psl::DetectorParam detectorParam;

    detectorParam.config_path = configDir;

    // i18R
    if ("rbn100" == projectMode)
    {
        detectorParam.modelProperty.padding = true;
        detectorParam.modelProperty.type = psl::ModelProperty::YOLOV3;
        detectorParam.modelProperty.classNum = 4;
        detectorParam.modelProperty.names = {"person", "escalator", "handrails", "person_dummy"};
        detectorParam.modelProperty.trackingExtend = {0.2, 0.3, 0.2, 0.1};
        detectorParam.modelProperty.detectExtend = {0.2, 0.3, 0.2, 0.1};
        detectorParam.modelProperty.version = "1.0.0";
        detectorParam.modelProperty.input.width = 416;
        detectorParam.modelProperty.input.height = 416;
        detectorParam.modelProperty.input.channel = 3;
        detectorParam.fps = 5;

        detector->EnableLidar(true);
    }
    else if ("rubby" == projectMode)
    {
        detectorParam.modelProperty.padding = true;
        detectorParam.modelProperty.keypoint = true;
        detectorParam.modelProperty.type = psl::ModelProperty::YOLOV5S;
        detectorParam.modelProperty.classNum = 9;
        detectorParam.modelProperty.names = {"shoes", "bin", "pedestal", "wire", "socket", "cat", "dog", "desk_rect", "desk_circle"};
        detectorParam.modelProperty.trackingExtend = {0.2, 0.27, 0.39, 0.2, 0.15, 0.28, 0.28, 1.0, 1.0};
        detectorParam.modelProperty.detectExtend = {0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 1.0, 1.0};
        detectorParam.modelProperty.version = "1.2.7";
        detectorParam.modelProperty.input.width = 320;
        detectorParam.modelProperty.input.height = 320;
        detectorParam.modelProperty.input.channel = 3;
        detectorParam.fps = 5;

        detector->EnableLidar(false);
    }
    else if ("randy" == projectMode)
    {
        detectorParam.modelProperty.padding = true;
        detectorParam.modelProperty.type = psl::ModelProperty::YOLOV3;
        detectorParam.modelProperty.classNum = 4;
        detectorParam.modelProperty.names = {"person", "escalator", "handrails", "person_dummy", "aircraft_hatch"};
        detectorParam.modelProperty.trackingExtend = {0.2, 0.3, 0.2, 0.1, 0};
        detectorParam.modelProperty.detectExtend = {0.2, 0.3, 0.2, 0.1, 0};
        detectorParam.modelProperty.version = "1.0.0";
        detectorParam.modelProperty.input.width = 416;
        detectorParam.modelProperty.input.height = 416;
        detectorParam.modelProperty.input.channel = 3;
        detectorParam.fps = 5;
        detector->EnableLidar(true);
    }
    else
    {
        ERROR_PRINT("not support project <" + projectMode + ">.");
    }


    if (not detector->Init(detectorParam)) return 0;
    if (not detector->SetParam(param)) return 0;

    auto flag = detector->Start();
    detector->SetDetectorResult(GetDetectorResult);

    const std::string mapFile = "map.txt";
//    perception->LoadMap(mapFile);

    const size_t size = dataset.size();

    for (size_t i = 0; i < size; ++i)
    {
        if (i == 10)
        {
            detector->Stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            if (not detector->SetParam(param)) return 0;
            detector->Start();
        }

        SyncDataFile item = dataset.at(i);

        std::string imageLeftPath(item.imageLeft);
        auto imageLeft = cv::imread(imageLeftPath, cv::IMREAD_GRAYSCALE);
        std::string imageRightPath;
        cv::Mat imageRight;

        if (imageLeft.empty())
        {
            ERROR_PRINT("empty data in file <" + imageLeftPath + ">");
            continue;
        }

        if (binocular)
        {
            imageRightPath = item.imageRight;
            imageRight = cv::imread(imageRightPath, cv::IMREAD_GRAYSCALE);
            if (imageRight.empty())
            {
                ERROR_PRINT("empty data in file <" + imageLeftPath + ">");
                continue;
            }
        }

        std::vector<psl::Instance> instanceInfosOut;

        // TODO : add status call back
        detector->SetErrorMessage(GetErrorMessageResult);

        if (i == 100)
        {
            detector->SaveMap(mapFile);
        }

        fileMutex.lock();
        historyFiles.push(imageLeftPath);
        fileMutex.unlock();

        sample::DataGroup itemData;

        if ("rubby" != projectMode)
        {
            if (not GetLidar(item.lidar, itemData.lidar.data, false)) continue;
        }
        if (not GetPose(item.lidarPose, itemData.lidar.pose)) continue;
        if (not GetPose(item.imagePose, itemData.orgImage.pose)) continue;

        itemData.orgImage.time = GetImageTime(item.imageLeft);

        // confirm early s_time be pushed into poses
        if (itemData.lidar.pose.s_time < itemData.orgImage.pose.s_time)
        {
            detector->SetPose(itemData.lidar.pose);
            detector->SetPose(itemData.orgImage.pose);
        }
        else
        {
            detector->SetPose(itemData.orgImage.pose);
            detector->SetPose(itemData.lidar.pose);
        }

        if (!binocular)
        {
            detector->SetImage(itemData.lidar.data.header.stamp + 1, imageLeft); // TODO : use timestamp
        }
        else
        {
            detector->SetImage(itemData.lidar.data.header.stamp + 1, imageLeft, imageRight); // TODO : use timestamp
        }

        if ("rubby" != projectMode)
        {
            detector->SetLidar(itemData.lidar.data);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (annotation)
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            // TODO : why it increase the perception time
    }

    detector->Stop();
    std::this_thread::sleep_for(
            std::chrono::milliseconds(50)); // TODO : why it increase the perception time
    robot_detector::Destroy(detector);

    return 0;
}
