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
    int startID = 0, endID = INT16_MAX;

    if (argc > 3) startID = std::stoi(argv[3]);
    if (argc > 4) endID = std::stoi(argv[4]);

    psl::CameraMoudleParam param;
    auto pathName = getCurrentExePath();
    auto exeName = getCurrentExeName();
    std::cout << pathName << ":" << " " << exeName << std::endl;

    std::string cameraConfigFile = inputDir + "/config.yaml";

    GetCameraConfig(cameraConfigFile, param);

    DetectorInner *detectorInner = new DetectorInner(param);

    std::vector<SyncDataFile> dataset;
    bool synced = GetData(inputDir, dataset);

    if (not detectorInner->Init({configDir})) return 0;

    const std::string mapFile = "map.txt";

    detectorInner->SetSenmaticCallback(Senmatic);
    detectorInner->LoadMap(MAP_NAME);

    Timer timer;
    const size_t size = dataset.size();

    for (size_t i = 0; i < size; ++i)
    {
        timer.Timing();
        SyncDataFile item = dataset.at(i);
        std::vector<psl::Instance> instanceInfosOut;

        DataGroup itemData;

        if (i == 265)
        {
            int k = 0;
        }

        if (i < startID) continue;
        if (i > endID) break;

        WARNING_PRINT(item.image);
        int id = item.image.rfind('/') + 1;
        detectorInner->SetName(
                std::to_string(i) + "_" +
                item.image.substr(id, item.image.length() - id));
        itemData.orgImage.data = cv::imread(item.image); //, cv::IMREAD_GRAYSCALE);
        if (itemData.Empty())
        {
            ERROR_PRINT("empty data in file <" + item.image + ">");
            continue;
        }

        if (not GetLidar(item.lidar, itemData.lidar.data, false)) continue;
        if (not GetPose(item.lidarPose, itemData.lidar.pose)) continue;
        if (not GetPose(item.imagePose, itemData.orgImage.pose)) continue;

        itemData.orgImage.time = GetImageTime(item.image);

        ERROR_PRINT("timestamp margin: " + std::to_string(
                ABS_DIFF(itemData.orgImage.time, itemData.lidar.data.header.stamp) /
                1000));


        timer.Timing("get data", true);

        detectorInner->GetDetectorResult(itemData);
        SLEEP_MS(10);

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

    detectorInner->SaveMap(MAP_NAME);
}
