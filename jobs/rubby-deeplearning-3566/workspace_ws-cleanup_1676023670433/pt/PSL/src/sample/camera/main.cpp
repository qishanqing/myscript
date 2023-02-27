//////////////////////////////////////////////////////////////////////
///  @file     main.cpp
///  @brief    capture image for detection
///  Details.
///
///  @author   donglijian
///  @version  1.8.34
///  @date     2022.01.10
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#include "perception.h"
#include <mutex>
#include <thread>
#include "sample/utils/utils.h"
#include "src/detector_inner.h"
#include "src/sample/camera/capture_image.h"
#include "src/utils/utils.h"
#include "src/utils/timer.h"

using namespace robot_detector;
std::queue<cv::Mat> imgs;
#define EXIST(file) (access((file).c_str(), 0) == 0)

void Help()
{
    ERROR_PRINT("please input: ");
    INFO_PRINT("\tconfig dir: [string]");
    INFO_PRINT("\tcamera config: [string]");
}

std::unique_ptr<ModelType> InitYoloV3(std::string configPath)
{
    std::string configFile = configPath + CONFIG_FILE_NAME;
    ConfigParam configParam;
    Status error = LoadConfig(configFile, configParam);

    DetectorOptions options;
    //TODO change to related path
    options.modelName = configPath + "/net." + MODEL_VERSION + ".npu";
    options.topk = 100;
    options.nmsThreshold = 0.45;
    options.scoreThreshold = 0.3;
    options.backend = "npu";

    std::unique_ptr<ModelType> detector;
    detector = std::move(std::unique_ptr<ModelType>(new ModelType()));
    bool ret = detector->Init(options);
    if (!ret)
    {
        std::cout<<"Init wrong!"<<std::endl;
    }
    return std::move(detector);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        Help();
        return 0;
    }

    std::string cameraConfigFile = argv[2];
    std::string configDir = std::string(argv[1]);

    psl::MoudleAllParam param;
    auto pathName = getCurrentExePath();
    auto exeName = getCurrentExeName();
    std::cout << pathName << ":" << " " << exeName << std::endl;

    INFO_PRINT("start to generate: " + cameraConfigFile + "/config.yaml");
    CaptureImage * captureImage = new CaptureImage();
    captureImage->InitDriver();
    captureImage->SaveConfigFile(cameraConfigFile);

    if (not captureImage->GetParam(param))
    {
        ERROR_PRINT("Init ReadPara wrong, return!");
        return 0;
    }

    if (!(DeDistortion::ReadParamLeft(param) and DeDistortion::ReadParamRight(param)))
    {
        ERROR_PRINT("InitCamera wrong, return!");
        return 0;
    }

    const float f = param._left_camera.at(RESOLUTION)._P[0];
    const float B = param._right_camera.at(RESOLUTION).b;

    std::unique_ptr<ModelType> detector;
    detector = InitYoloV3(configDir);
    int key;
    while(1)
    {
        Timer timer;
        ImageModule images = captureImage->GetImg();
        cv::Mat leftImage = images.left;
        cv::Mat rightImage = images.right;

        if (not leftImage.empty())
        {
            DeDistortion::Remap(LEFT, leftImage);
            DeDistortion::Remap(RIGHT, rightImage);

            std::vector<BoxInfo> boxesLeft, boxesRight;
            detector->Detect(leftImage, boxesLeft);
            detector->Detect(rightImage, boxesRight);

            const size_t leftCount = boxesLeft.size();
            const size_t rightCount = boxesRight.size();
            if (leftCount == rightCount and leftCount == 1)
            {
                BoxInfo& boxLeft = boxesLeft.at(0);
                BoxInfo& boxRight = boxesRight.at(0);
                float d = boxLeft.box.x - boxRight.box.x;
                float z = B * f / d;
                float x =  z * boxLeft.box.x / d;
                boxLeft.location.machine.x = z;
                boxLeft.location.machine.y = x;
                boxRight.location.machine.x = z;
                boxRight.location.machine.y = x;
            }

//            for (auto box : boxes)
//            {
//                std::cout<<box.classID<<std::endl;
//            }

            DrawBoxes(boxesLeft, leftImage);
            DrawBoxes(boxesRight, rightImage);

            cv::Mat imageShow;
            cv::hconcat(leftImage, rightImage, imageShow);
            cv::imshow("detect", imageShow);

            key = cv::waitKey(1);
            if (27 == key) {
                break;
            }
        }
        else
        {
            SLEEP_MS(50);
        }
        timer.Timing("detect", true);
    }
    if (captureImage != nullptr)
    {
        free(captureImage);
        captureImage = nullptr;
    }
    return 0;
}


