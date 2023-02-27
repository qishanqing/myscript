//////////////////////////////////////////////////////////////////////
///  @file     test.cpp
///  @brief    unit test for single thread
///  Details.
///
///  @author   donglijian
///  @version  1.7.9
///  @date     2021.11.01
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#include "perception.h"
#include <mutex>
#include <queue>
#include <thread>
#include <sys/time.h>
#include "sample/utils/utils.h"
#include "src/perception_inner.h"
#include "src/utils/timer.h"

using namespace robot_detector;

std::queue<std::string> historyFiles = {};
std::mutex fileMutex;
static bool annotation = false;
const std::string MAP_NAME = "map.log";

psl::SenmaticMap mapGlobal;
extern LogMessage logMessageStashed;

void SignalHandler(const char *data, int size)
{
    logMessageStashed.Logging();
    std::string str = std::string(data, size);
    LOG(INFO) << str;
}

void Senmatic(const psl::SenmaticMap &map)
{
    mapGlobal = map;
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

    return ret;
}

static void GetErrorMessageResult(psl::ErrorMessage errorMessage)
{
    std::cout << "callback: " << errorMessage.state << " ["
              << errorMessage.stamp << "]" << std::endl;
    std::cout << std::hex << errorMessage.code << std::endl;
}

void Help()
{
    ERROR_PRINT("please input: ");
    INFO_PRINT("\tproject type: [string] (\"rubby\", \"rbn100\")");
    INFO_PRINT("\tconfig dir: [string]");
    INFO_PRINT("\tinput image dir: [string]");
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        Help();
        return 0;
    }

    google::SetStderrLogging(google::GLOG_INFO);  // 输出log的最低等级是 INFO (可以设置为WARNING或者更高)
    google::InstallFailureSignalHandler();     // 配置安装程序崩溃失败信号处理器
    google::InstallFailureWriter(&SignalHandler);  // 安装配置程序失败信号的信息打印过程，设置回调函数

    std::string projectMode = argv[1];
    std::string inputDir = argv[3];
    std::string configDir = std::string(argv[2]) + '/';
    int startID = 0, endID = INT16_MAX;
    std::string outputDir = "";

    if (argc > 4) outputDir = argv[4];

    try
    {
        if (argc > 5) startID = std::stoi(argv[5]);
    }
    catch (...) {}


    try
    {
        if (argc > 6) endID = std::stoi(argv[6]);
    }
    catch (...) {}

    psl::CameraMoudleParam param;
    auto pathName = getCurrentExePath();
    auto exeName = getCurrentExeName();
    std::cout << pathName << ":" << " " << exeName << std::endl;

    std::string cameraConfigFile = inputDir + "/config.yaml";

    GetCameraConfig(cameraConfigFile, param);

    DetectorInner *detectorInner = new DetectorInner(param);

    detectorInner->EnableLidar(true);
    detectorInner->SetOutputDir(outputDir);

    std::vector<SyncDataFile> dataset;
    bool binocular = GetData(inputDir, dataset);

    psl::DetectorParam detectorParam;
    bool withLidar = false;

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
        withLidar = true;
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
        withLidar = false;
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
        withLidar = true;
    }
    else
    {
        ERROR_PRINT("not support project <" + projectMode + ">.");
    }

    detectorInner->EnableLidar(withLidar);

    if (not detectorInner->Init(detectorParam)) return 0;

    const std::string mapFile = "map.txt";

    detectorInner->SetSenmaticCallback(Senmatic);
    detectorInner->LoadMap(outputDir + "/" + MAP_NAME);

    Timer timer;
    const size_t size = dataset.size();

    for (size_t i = 0; i < size; ++i)
    {
        timer.Timing();
        SyncDataFile item = dataset.at(i);
        std::vector<psl::Instance> instanceInfosOut;

        DataGroup itemData;

        if (i == 342)
        {
            int k = 0;
        }

        if (i < startID) continue;
        if (i > endID) break;

        WARNING_PRINT(item.imageLeft);
        int id = item.imageLeft.rfind('/') + 1;
        detectorInner->SetName(
                std::to_string(i) + "_" +
                item.imageLeft.substr(id, item.imageLeft.length() - id));
        itemData.orgImage.left = cv::imread(item.imageLeft); //, cv::IMREAD_GRAYSCALE);
        if (binocular)
        {
            itemData.orgImage.right = cv::imread(item.imageRight); //, cv::IMREAD_GRAYSCALE);
        }

        if (itemData.Empty())
        {
            ERROR_PRINT("empty data in file <" + item.imageLeft + ">");
            continue;
        }

        if (not GetPose(item.imagePose, itemData.orgImage.pose)) continue;

        if (withLidar)
        {
            if (not GetLidar(item.lidar, itemData.lidar.data, false)) continue;
            if (not GetPose(item.lidarPose, itemData.lidar.pose)) continue;
        }
        else
        {
            itemData.lidar.pose = itemData.orgImage.pose;
        }

        static psl::SlamResult poseTemp;
        LOG_CHECK_DEBUG(INFO) << "s_rotation diff: " << itemData.lidar.pose.s_rotation[0] - poseTemp.s_rotation[0]
        << " , " << itemData.lidar.pose.s_rotation[1] - poseTemp.s_rotation[1]
        << " , " << itemData.lidar.pose.s_rotation[2] - poseTemp.s_rotation[2]
        << " , " << itemData.lidar.pose.s_rotation[3] - poseTemp.s_rotation[3];
        LOG_CHECK_DEBUG(INFO) << "s_position diff: " << itemData.lidar.pose.s_position[0] - poseTemp.s_position[0]
                << " , " << itemData.lidar.pose.s_position[1] - poseTemp.s_position[1]
                << " , " << itemData.lidar.pose.s_position[2] - poseTemp.s_position[2] ;
        poseTemp = itemData.lidar.pose;

        itemData.orgImage.time = GetImageTime(item.imageLeft);

        ERROR_PRINT("timestamp margin: " + std::to_string(
                ABS_DIFF(itemData.orgImage.time, itemData.lidar.data.header.stamp) /
                1000));


        timer.Timing("get data", true);

        detectorInner->GetDetectorResult(itemData);

        for (auto &m : mapGlobal.objects)
        {
            m.statusOperator = psl::Object::UPDATE_SLAM;
        }
//            detectorInner->SetSenmaticMap(mapGlobal);

        timer.Timing("GetDetectorResult", true);
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//        if (annotation)
//            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        // TODO : why it increase the perception time
    }

    detectorInner->Pause();
    detectorInner->RotateMap(30);
    detectorInner->SaveMap(outputDir + "/" + MAP_NAME);
    std::cout << "save map" << std::endl;
//    detectorInner->Stop();
}
