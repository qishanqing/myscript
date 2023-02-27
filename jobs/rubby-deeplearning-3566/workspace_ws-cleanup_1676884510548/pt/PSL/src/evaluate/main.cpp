#include "perception.h"
#include <iostream>
#include <unistd.h>
#include <queue>
#include <mutex>
#include <thread>
#include <sys/time.h>
#include "sample/utils/utils.h"

using namespace robot_detector;

std::queue<std::string> historyFiles = {};
std::mutex fileMutex;
static bool annotation = false;

bool Annotation(const std::string file, const std::vector<psl::Instance> &instanceInfos)
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
        std::cout << "mapinfo size: " << obstacle.maps.size() << std::endl;
    }

    if (annotation)
    {
        Annotation(file.substr(0, file.rfind(".")) + ".txt", object);
    }
}

static void GetErrorResult(psl::ErrorInfo errorInfo)
{
    std::cout << "callback" << std::endl;
    std::cout << "errorInfo: " << errorInfo.module << " " << " description: "
              << errorInfo.timeStamp << std::endl;
    std::cout << std::hex << errorInfo.errorCode << std::endl;
}

static void GetStatusResult(psl::WorkStatus statusInfo)
{
    std::cout << "status callback" << std::endl;
    std::cout << "statusInfo: " << statusInfo.module << " " << " description: "
              << statusInfo.timeStamp << std::endl;
    std::cout << std::hex << statusInfo.state << std::endl;
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
    std::string imagesTxt = inputDir + "/all.txt";
    std::string configDir = std::string(argv[1]) + '/';

    if (argc > 3)
    {
        annotation = argv[3];
    }

    psl::MoudleAllParam param;
    auto pathName = getCurrentExePath();
    auto exeName = getCurrentExeName();
    std::cout << pathName << ":" << " " << exeName << std::endl;

    std::string cameraConfigFile = inputDir + "/config.yaml";

    GetCameraConfig(cameraConfigFile, param);

    robot_detector::Detector *detector = robot_detector::Create(param);

    psl::SlamResult poseInput;
    poseInput.s_time = 0.001;

    poseInput.s_position[0] = 1.0;
    poseInput.s_position[1] = -1.0;
    poseInput.s_position[2] = 0.0;

    // TODO : type cast
    poseInput.s_rotation[0] = param._left_camera[RESOLUTION]._D[0];
    poseInput.s_rotation[1] = param._left_camera[RESOLUTION]._D[1];
    poseInput.s_rotation[2] = param._left_camera[RESOLUTION]._D[2];
    poseInput.s_rotation[3] = param._left_camera[RESOLUTION]._D[3];

    std::string images_txt = inputDir + "/data.txt";
    std::string lidar_txt = inputDir + "/lidar.txt";
    std::vector<std::string> image_name_list;
    std::vector<std::string> lidarNameList;

    ReadFile(images_txt, image_name_list);
    ReadFile(lidar_txt, lidarNameList);

    if (not detector->Init({configDir}))
    {
        return 0;
    }

    auto flag = detector->Start();
    detector->SetDetectorResult(GetDetectorResult);

    for (size_t i = 0; i < image_name_list.size(); ++i)
    {
//        ERROR_PRINT(i);
        auto imageName = image_name_list.at(i);
//        ERROR_PRINT(imageName);

        std::string imagePath(inputDir + "/" + imageName);
        auto image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
        if (image.empty())
        {
            ERROR_PRINT("empty data in file <" + imageName + ">");
            continue;
        }

        std::vector<psl::Instance> instanceInfosOut;

        // TODO : add status call back
        detector->SetError(GetErrorResult);

        fileMutex.lock();
        historyFiles.push(imagePath);
        fileMutex.unlock();

        std::string yamlFile = inputDir + "/" + lidarNameList[i];
        swr::sensor_types::LaserScan lidarData;

        GetLidar(yamlFile, lidarData
                 , false); // sleep 100ms : CPU 32% sleep 300ms : CPU 13~17%

//        perception->SetPose(poseInput);
        detector->SetImage(0.0001, image);
        detector->SetLidar(lidarData);
        if (annotation)
            std::this_thread::sleep_for(std::chrono::milliseconds(250)); // TODO : why it increase the perception time
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(250)); // TODO : why it increase the perception time
    robot_detector::Destroy(detector);
}
