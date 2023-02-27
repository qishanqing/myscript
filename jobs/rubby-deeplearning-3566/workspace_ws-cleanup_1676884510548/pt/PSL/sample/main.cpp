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

using namespace robot_detector;

std::queue<std::string> historyFiles = {};
std::mutex fileMutex;
static bool annotation = false;


struct Lidar
{
    swr::sensor_types::LaserScan data;
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

    std::string inputDir = argv[2];
    std::string configDir = std::string(argv[1]) + '/';

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

    bool synced = GetData(inputDir, dataset);
    if (not detector->Init({configDir})) return 0;
    if (not detector->SetParam(param)) return 0;

    auto flag = detector->Start();
    detector->SetDetectorResult(GetDetectorResult);

    const std::string mapFile = "map.txt";
//    perception->LoadMap(mapFile);

    const size_t size = dataset.size();

    for (size_t i = 0; i < size; ++i)
    {
        SyncDataFile item = dataset.at(i);

        std::string imagePath(item.image);
        auto image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
        if (image.empty())
        {
            ERROR_PRINT("empty data in file <" + imagePath + ">");
            continue;
        }

        std::vector<psl::Instance> instanceInfosOut;

        // TODO : add status call back
        detector->SetErrorMessage(GetErrorMessageResult);

        if (i == 100)
        {
            detector->SaveMap(mapFile);
        }

        fileMutex.lock();
        historyFiles.push(imagePath);
        fileMutex.unlock();

        DataGroup itemData;

        if (not GetLidar(item.lidar, itemData.lidar.data, false)) continue;
        if (not GetPose(item.lidarPose, itemData.lidar.pose)) continue;
        if (not GetPose(item.imagePose, itemData.orgImage.pose)) continue;

        itemData.orgImage.time = GetImageTime(item.image);

        detector->SetPose(itemData.lidar.pose);
        detector->SetPose(itemData.orgImage.pose);
        detector->SetImage(itemData.lidar.data.header.stamp + 1, image); // TODO : use timestamp
        detector->SetLidar(itemData.lidar.data);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (annotation)
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            // TODO : why it increase the perception time
    }

    detector->Stop();
    std::this_thread::sleep_for(
            std::chrono::milliseconds(250)); // TODO : why it increase the perception time
    robot_detector::Destroy(detector);
}
