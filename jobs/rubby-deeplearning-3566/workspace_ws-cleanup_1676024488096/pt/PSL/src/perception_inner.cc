#include "src/perception_inner.h"
#include "src/utils/utils.h"
#include <opencv2/highgui.hpp>
#include <sys/prctl.h>
#include "utils/timer.h"
#include "src/utils/file.h"
#include "src/utils/define.h"
#include "src/utils/detector_param.h"
#include "src/utils/detector_property.h"
#include "src/map/manager.h"
#include "src/utils/lidar.h"
// detect model
#include "src/module/yolov3.h"
#include "src/module/yolov5n.h"
#include "src/module/ssd.h"

#define SHOW_MAP

const int WAIT_4_DATA_TIME = 5; // milliseconds
const int WAIT_4_INIT_TIME = 50; // milliseconds
const int WAIT_4_START_TIME = 50; // milliseconds
const int WAIT_4_MAP_UPDATE_SLAM_TIME = 20; // milliseconds
const int WAIT_4_MAP_UPDATE_DL_TIME = 5; // milliseconds
const int WAIT_4_MAP_UPDATE_SLAM_TIME_MAX = 3000; // milliseconds
const int WAIT_4_MAP_UPDATE_DL_TIME_MAX = 500; // milliseconds
const int MAX_LACK_DATA = 1000; // milliseconds
const int KEEP_FREE_TIME = 5; // milliseconds
int FOV = 85; // horizontal
int FOV_LEFT = FOV / 2;
int FOV_RIGHT = FOV / 2;
const float VALID_DISTANCE_4_SENMATIC = 6; // 6m
float VALID_DISTANCE_4_SENMATIC_HORIZONTAL_LEFT =
        -tan(FOV_LEFT * ANGLE_2_RADIAN) * VALID_DISTANCE_4_SENMATIC;
float VALID_DISTANCE_4_SENMATIC_HORIZONTAL_RIGHT =
        tan(FOV_RIGHT * ANGLE_2_RADIAN) * VALID_DISTANCE_4_SENMATIC;
const int LOG_PRINT_STEPS = 100;
const std::string OUTPUT_NAME = "result";

// TODO : why use global variable
namespace
{
DetectorCallback callback = nullptr;
}

#define CALLBACK_DETECTOR(obj, map) do{ \
    Timer timer;                        \
    if (nullptr != callback) { \
        callback((obj), (map));   \
    } \
    LOG_CHECK_TIME(INFO) << timer.TimingStr("person callback"); \
}while(0)

#define EMIT_SENMATIC_MAP(map) do{ \
    Timer timer;                   \
    if (nullptr != emitSenmatic) { \
        emitSenmatic((map));   \
    }                              \
    LOG_CHECK_TIME(INFO) << timer.TimingStr("map callback"); \
}while(0)

const std::vector<std::string> BACKEND{"npu_aml", "gpu"}; // TODO : move into detector_moudle_inner

Log g_logConfig;

DetectorInner::DetectorInner()
        : outputDir(OUTPUT_NAME), ready(false), inited(false), configFile("")
          , emitSenmatic(nullptr), mapModifying(false)
          , errorMessage(nullptr), wait4Update(false)
          , getCameraParam(false)
          , withLidar(false)
          , testRunning(true)
          , mapManager(nullptr)
          , detectStep(100)
{
}

DetectorInner::DetectorInner(const psl::CameraMoudleParam &moduleParam)
        : DetectorInner()
{
    this->cameraParam = moduleParam;
    this->getCameraParam = true;
}

void DetectorInner::SetDetectorResult(DetectorCallback detectorFunc)
{
    callback = detectorFunc;
}

void DetectorInner::SetErrorMessage(ErrorMessageCallback errorFunc)
{
    ErrorMessage *errorMessage = ErrorMessage::GetInstance();

    errorMessage->Init(errorFunc);
}

DetectorInner::~DetectorInner()
{
    testRunning = false;
    if (testThread.joinable())
    {
        testThread.join();
    }
    RELEASE(mapManager);
}

bool DetectorInner::Init(const psl::DetectorParam &detectParam)
{
    detectStep = ONE_SECOND / detectParam.fps;
//    instanceManager.UnloadLidar();
    instanceManager.EnableLidar(withLidar);

    // TODO : when miss option, how to do
    Timer timer;
    this->detectParam = detectParam;
    this->detectParam.config_path += "/";

    SetProjectName( this->detectParam);

    configFile = this->detectParam.config_path + CONFIG_FILE_NAME;
    bool ret = false;
    std::string modelFile = "";

    errorMessage = ErrorMessage::GetInstance();

    try
    {
        ret = LoadConfig();
        CHECK_STATUS_EXIT(ret, "");

        obstacle.status = 1;
        obstacle.maps.clear();

        ret = CheckDevice();
        CHECK_STATUS_EXIT(ret, "");

        modelFile = this->detectParam.config_path + "net." +
                    this->detectParam.modelProperty.version; // after load config

        ret = InitModel(modelFile); // TODO : not init model, is init detect, or init ...
        CHECK_STATUS_EXIT(ret, "fail to init model");

        mapManager = new MapManager();
        mapManager->SetParam(detectParam);

        ValidData validData;
        validData.image = true;
        validData.pose = true;
        validData.lidar = this->withLidar;
        ret = data.Init(RESOLUTION, configParam, validData);
        CHECK_STATUS_EXIT(ret, "");

        // TODO : when fail to init, how to set the start
        Start(detectParam); // TODO : statr to early befor all init done


        LOG_CHECK_STATUS(INFO) << timer.TimingStr("Init");
    }
    catch (...)
    {
        LOG_CHECK_STATUS(ERROR) << "init abnormal.";
        goto exit;
    }

    inited = true;
    LOG_CHECK_STATUS(INFO) << "success to init perception.";

    if (getCameraParam) ret = SetParam(cameraParam);

    return ret && inited;

    exit:
    inited = false;
    LOG(ERROR) << "fail to init perception.";
    return inited;
}

bool DetectorInner::InitInstance()
{
    instanceManager.InitCamera(cameraParam);
    instanceManager.SetParam(configParam);

    // TODO : check which is correct
//    Eigen::Matrix<double, 3, 4, Eigen::RowMajor> pMatrix = Eigen::Map<Eigen::MatrixXd>(
//            cameraParam._left_camera.at(RESOLUTION)._P, 3, 4);
    const double *p = cameraParam._left_camera.at(RESOLUTION)._P;
    pMatrix << p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11];
    LOG_CHECK_DEBUG(INFO) << "success init.";

    return true;
}

bool DetectorInner::InitModel(std::string &modelFile)
{
    // TODO : move to end (exit),

    // TODO : process the exception
    // TODO : not check the empty modelpath
    // TODO : check exist for dir and file

    // TODO : why use default
    // use default options here
    DetectorOptions options;

    bool ret = CheckModelFile(modelFile);
    CHECK_STATUS_EXIT(ret, "");

    options.modelName = modelFile;
    // remain params keep default
    options.topk = 100;
    options.nmsThreshold = 0.45;
    options.scoreThreshold = 0.3;
    options.param = detectParam.modelProperty;
#ifdef USE_GPU
    options.backend = "gpu";
#else
    options.backend = "npu_aml";
#endif

    ret = CheckBackend(options.backend);
    CHECK_STATUS_EXIT(ret, "");

    // TODO : check the model version, move the model version config inner
    ret = CreateModel();
    CHECK_STATUS_EXIT(ret,
            "donot suport model type " + std::to_string(detectParam.modelProperty.type));
    ret = detector->Init(options);
    // TODO : archive the ausolution path of model
    CHECK_STATUS_EXIT(ret, "fail to init detect model");
    // TODO : how to set statusInfo.state

    LOG_CHECK_DEBUG(INFO) << "success create model.";
    data.Attach(detector.get());
    if (RandyProject())
    {
        data.Attach(detectorAircraft.get());
    }
    return true;

    exit:

    errorMessage->Push(ErrorMessageCode::DL_INVALID_MODEL, "fail to init detect model");
    return false;
}

bool DetectorInner::CreateModel()
{
    if (RandyProject())
    {
        detectorAircraft = std::move(std::unique_ptr<inner::AircraftHatchDetector>(new inner::AircraftHatchDetector));
    }
    switch (detectParam.modelProperty.type)
    {
        case psl::ModelProperty::MOBILENETV2_SSD:
            detector = std::move(std::unique_ptr<SSD>(new SSD()));
            break;
        case psl::ModelProperty::YOLOV3:
            detector = std::move(std::unique_ptr<Yolov3>(new Yolov3()));
            break;
        case psl::ModelProperty::YOLOV5S:
        case psl::ModelProperty::YOLOV5N:
            detector = std::move(std::unique_ptr<Yolov5n>(new Yolov5n()));
            break;
        default:
            return false;
    }

    return true;
}

void DetectorInner::Start(const psl::DetectorParam detectorParam)
{
    testThread = std::thread(
            [this]()
            {
                while (testRunning)
                {
                    static bool firstIn = true;
                    if (firstIn)
                    {
                        prctl(PR_SET_NAME, "dl_detector");
                        LOG(INFO) << "[" << MODULE_NAME << "] " << "perception start.";
                        firstIn = false;
                    }

                    errorMessage->Pop(ErrorMessageCode::DL_UNKNOWN_DL, "unknown error");
                    if (operationStatus != detector_inner::START)
                    {
                        if (operationStatus == detector_inner::UNKNOWN)
                        {
                            LOG_CHECK_STATUS_WITE_STEP(ONE_MINUTE
                                                       , "detectorInner is not active ! eager for Start()");
                        }
                        else if (operationStatus == detector_inner::PAUSE)
                        {

                        }
                        else if (operationStatus == detector_inner::RESTART)
                        {
                            operationStatus = detector_inner::START;
                        }
                        else if (operationStatus == detector_inner::STOP)
                        {

                        }

                        SLEEP_MS(WAIT_4_START_TIME);
                    }
                    else
                    {
                        try
                        {
                            static bool ret = false;
                            static std::time_t lastTime = GetTimestamp();
                            if ((GetTimestamp() - lastTime) < detectStep and ret)
                            {
                                SLEEP_MS(KEEP_FREE_TIME);
                            }
                            else
                            {
                                lastTime = GetTimestamp();
                                ret = GetDetectorResult();

                                logMessageStashed.NewLine();
                            }
                        }
                        catch (...)
                        {
                            logMessageStashed.Logging();
                            errorMessage->Push(ErrorMessageCode::DL_UNKNOWN_DL
                                               , "unknown error");
                            END();
                        }
                    }
                }
            });

    testThread.detach();
}

// cam data
void DetectorInner::SetImage(const psl::Time time, const cv::Mat &image)
{
    CHECK_EQ(image.channels(), 1); // TODO : should be remove
    if (image.channels() == 0)
    {
        errorMessage->Push(ErrorMessageCode::DL_ERROR_IMAGE, "error image");
    }

    data.SetImage(time, image);
}

//
void DetectorInner::SetImage(const psl::Time time, const cv::Mat &imageLeft
                             , const cv::Mat &imageRight)
{
    CHECK_EQ(imageLeft.channels(), 1); // TODO : should be remove
    CHECK_EQ(imageRight.channels(), 1);
    if (imageLeft.channels() == 0 or imageRight.channels() == 0)
    {
        errorMessage->Push(ErrorMessageCode::DL_ERROR_IMAGE, "error image");
    }

    data.SetImage(time, imageLeft, imageRight);
}

//
void DetectorInner::SetImage(const psl::ImageData &imageData)
{
    CHECK_EQ(imageData.left.channels(), 1); // TODO : should be remove
    CHECK_EQ(imageData.right.channels(), 1);
    if (imageData.left.channels() == 0 or imageData.right.channels() == 0)
    {
        errorMessage->Push(ErrorMessageCode::DL_ERROR_IMAGE, "error image");
    }

    data.SetImage(imageData);
}

//lidar data
void DetectorInner::SetLidar(const swr::psl_sensor_types::LaserScan &lidar)
{
    data.SetLidar(lidar);
}

void DetectorInner::SetPose(const psl::SlamResult &pose)
{
    data.SetPose(pose);
}

bool DetectorInner::GetDetectorResult(const DataGroup &item)
{
    static bool firstIn = true;
    if (firstIn)
    {
        prctl(PR_SET_NAME, "dl_detector");
        LOG(INFO) << "[" << MODULE_NAME << "] " << "perception start.";
        firstIn = false;
    }

    if (not inited)
    {
        SLEEP_MS(WAIT_4_INIT_TIME);
        return false;
    }

    if (mapModifying)
    {
        psl::SenmaticMap enmaticMapStash;
        {
            std::unique_lock<std::mutex> lock(senmaticMapMutex);
            enmaticMapStash = this->senmaticMapStash;
            PrintObject(enmaticMapStash);
        }
        mapManager->Modify(enmaticMapStash);
        mapModifying = false;
    }

    Timer timer, timerDetect;
    DetectResult boxes;
    DataGroup itemCurrent = item;

    itemCurrent.Remap();
//    itemCurrent.image = itemCurrent.orgImage;
    if (itemCurrent.image.left.channels() == 1)
    {
        cv::cvtColor(itemCurrent.image.left, itemCurrent.image.left, CV_GRAY2BGR);
    }

    bool ret = false;

    if (RandyProject())
    {
        ret = detectorAircraft->Detect(itemCurrent.lidar, boxes.boxes);
        LOG_CHECK_TIME(INFO) << timerDetect.TimingStr("aircraft detect");
    }
    else
    {
        ret = detector->Detect(itemCurrent.image.left, boxes.boxes);
        LOG_CHECK_TIME(INFO) << timerDetect.TimingStr("model detect");
    }

    if (not ret)
    {
        return true;
    }

    boxes.pose = itemCurrent.image.pose;
    boxes.time = itemCurrent.image.time;

    PostProcess(itemCurrent, boxes);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("whole");
    logMessageStashed.NewLine();

    return true;
}

// get all instances exist in maps now
bool DetectorInner::GetDetectorResult()
{
    if (not ready)
    {
        SLEEP_MS(WAIT_4_INIT_TIME);
        return false;
    }

    if (mapModifying)
    {
        psl::SenmaticMap enmaticMapStash;
        {
            std::unique_lock<std::mutex> lock(senmaticMapMutex);
            enmaticMapStash = this->senmaticMapStash;
            PrintObject(enmaticMapStash);
        }
        mapManager->Modify(enmaticMapStash);
        mapModifying = false;
    }

    Timer timer, timerDetect;

    DetectResult boxes;

    DataGroup item;
    bool retDetect = false;

    if (RandyProject())
    {
//        retDetect = detectorAircraft->Detect(item.lidar, boxes.boxes);
        retDetect = detectorAircraft->Detect(boxes.boxes, item);
        boxes.pose = item.lidar.pose;
        boxes.time = item.lidar.data.header.stamp;
    }
    else
    {
        retDetect = detector->Detect(boxes.boxes, item);
        boxes.pose = item.image.pose;
        boxes.time = item.image.time;
    }


    long long timeMargin = ABS_DIFF(boxes.pose.s_time, item.lidar.pose.s_time);
    LOG_CHECK_TIME(INFO) << timerDetect.TimingStr("detect");

    static std::time_t lastLackDataTime = GetTimestamp();
    if (not retDetect and (item.image.left.empty() or (Empty(item.lidar.data) and RandyProject()))) // no data
    {
        bool lack = GetTimestamp() - lastLackDataTime > MAX_LACK_DATA;

        std::string message = "image and pose not sync for a long time (" +
                              std::to_string(MAX_LACK_DATA) + "ms)";

        if (withLidar)
        {
            message = "image, lidar and pose not sync for a long time (" +
                      std::to_string(MAX_LACK_DATA) + "ms)";
        }

        CHECK_STATUS_WITH_TIME_END(not lack, TEN_SECOND, message);
        SLEEP_MS(WAIT_4_DATA_TIME);
        return false;
    }
    else
    {
        lastLackDataTime = GetTimestamp();
    }

    SetName();
    PostProcess(item, boxes);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("whole");

    return true;
}

bool DetectorInner::Start()
{
    if (!ready) return false;

    data.SetReceive(true);
    operationStatus = detector_inner::START;

    LOG_CHECK_STATUS(INFO) << "perception has start!";
    return true;
}

bool DetectorInner::Stop()
{
    if (!ready) return false;

    mapManager->Clear();
    operationStatus = detector_inner::STOP;
    data.SetReceive(false);

    LOG_CHECK_STATUS(INFO) << "perception has stoped!";

    return true;
}

bool DetectorInner::Pause()
{
    if (!ready) return false;

    operationStatus = detector_inner::PAUSE;
    data.SetReceive(false);

    LOG_CHECK_STATUS(INFO) << "perception has been paused!";

    return true;
}

bool DetectorInner::Restart()
{
    if (!ready) return false;

    data.SetReceive(true);
    operationStatus = detector_inner::RESTART;
    LOG_CHECK_STATUS(INFO) << "perception has been restarted!";
    return true;
}

// todo : move to model
bool DetectorInner::CheckDevice()
{
    return true;

    int num_platforms = -1;
    if (num_platforms <= 0)
    {
        SLEEP_MS(100);

        errorMessage->Push(ErrorMessageCode::DL_NO_GPU, "NO GPU");

        LOG_CHECK_DEBUG(INFO) << "fail init.";
        return false;
    }

    LOG_CHECK_DEBUG(INFO) << "success init.";
    return true;
}

bool DetectorInner::ErrorMissConfig(const std::string message)
{
    errorMessage->Push(ErrorMessageCode::DL_LACK_CONFIG, "lack config, " + message);
    LOG(WARNING) << message;

    return false;
}

bool
DetectorInner::ErrorMissConfigOption(const std::string message)
{
    errorMessage->Push(ErrorMessageCode::DL_INVALID_CONFIG, "invalid config, " + message);
    LOG(WARNING) << message;

    return false;
}

bool DetectorInner::CheckModelFile(std::string &modelFile)
{
#ifdef USE_GPU
    modelFile += ".gpu";
#elif USE_NPU_AML
    modelFile += ".npu";
#elif USE_NPU_RK
    modelFile += ".npu";
#endif

    bool ret = file_op::File::Exist(modelFile);
    if (not ret)
    {
        LOG_CHECK_STATUS(ERROR) << "model file missed <" << modelFile << ">.";
    }

    return ret;
}

bool DetectorInner::CheckBackend(const std::string flag)
{
    bool ret = Contain(BACKEND, flag);

    if (not ret)
    {
        LOG_CHECK_STATUS(FATAL) << "unsupported backend: " << flag << "\n"
                                << "Supported backend: ";
    }

    return ret;
}

void DetectorInner::SetWorldLocation(std::vector<psl::Instance> &instances
                                     , const psl::SlamResult &pose) const
{
    for (auto &obj : instances)
    {
        Lidar2Machine(configParam, obj.locationMachine);
        Machine2World(obj.locationMachine, obj.locationWorld, pose);
    }
}

void DetectorInner::SetWorldLocation(DetectResult &boxes
                                     , const psl::SlamResult &lidarPose) const
{
    for (auto &obj : boxes.boxes)
    {
        SetWorldLocation(obj, lidarPose, boxes.pose);
    }
}

void DetectorInner::SetWorldLocation(BoxInfo &box
                                     , const psl::SlamResult &lidarPose
                                     , const psl::SlamResult &imagePose) const
{
    // whole location
    Lidar2Machine(configParam, box.location.machine);

    // serials location
    for (auto &parts : box.locationSerials)
    {
        Lidar2Machine(configParam, parts.machine);
    }

    // edge location
    Lidar2Machine(configParam, box.edgeLeft.machine);
    Lidar2Machine(configParam, box.edgeRight.machine);

    Machine2World(box, lidarPose, imagePose);
}

void DetectorInner::Display(const std::string name, const std::vector<BoxInfo> &boxInfos
                            , const std::vector<psl::Instance> &instanceInfos
                            , const cv::Mat &image) const
{
    if (configParam.display)
    {
        cv::Mat displayImage = image;

        if (image.empty())
        {
            LOG_CHECK_STATUS(WARNING) << "image is empty";
            return;
        }
        DrawInstance(boxInfos, instanceInfos, displayImage);
#if USE_GPU
        static int i = 0;
        cv::imwrite("/media/hao/CODE/CODE/CPP/i18R/psl/perception/build/debug/image/"
                    + std::to_string(i++) + ".png", image);
#else
        cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
        cv::imshow(name, image);
        cv::waitKey(1);
//            std::string file = "result/detect/" + std::to_string(GET_TIMESTAMP) + ".jpg";
//            file_op::File::MkdirFromFile(file);
//            cv::imwrite(file, image);
#endif
    }
    // TODO : block the start thread,
}

void DetectorInner::SetGetPoseByTime(GetPoseCallback getPoseFunc)
{
    LOG_CHECK_STATUS(INFO) << "SetGetPoseByTime successful.";
    getPoseByTime = getPoseFunc;
    this->data.SetGetPoseByTime(getPoseFunc);
}

bool DetectorInner::LoadConfig()
{
    ::Status error = ::LoadConfig(this->configFile, this->configParam);
    if (error.code != SUCCESS)
    {
        if (MISS_FILE == error.code)
        {
            ErrorMissConfig(error.message);
        }
        else if (MISS_OPTION == error.code)
        {
            ErrorMissConfigOption(error.message);
        }

        LOG(WARNING) << "fail to load config.";
        return false;
    }
    else
    {
        LOG(WARNING) << "success to load config.";
        return true;
    }

}

void DetectorInner::SetSenmaticMap(const psl::SenmaticMap &senmaticMap)
{
    LOG_CHECK_STATUS_ONCE("get senmatic map from slam.");
    try
    {
        if (not senmaticMap.objects.empty())
        {
            for (auto &m : senmaticMap.objects)
            {
                mapModifying |= psl::Object::UPDATE_SLAM == m.statusOperator;
            }
            {
                std::unique_lock<std::mutex> lock(senmaticMapMutex);
                if (mapModifying)
                {
                    this->senmaticMapStash = senmaticMap;
                    LOG_CHECK_TEST(INFO) << "SetSenmaticMap function echo log";
                    if (g_logConfig.debug)
                        PrintObject(senmaticMap);
                }
            }
            // update in the main thread
        }
    }
    catch (...)
    {
        LOG_CHECK_DEBUG(WARNING) << "get senmatic map from slam error!";
    }

    if (senmaticMap.finalUpdate) wait4Update = false;
    return;
}

void DetectorInner::SetSenmaticCallback(SenmaticCallback senmaticFun)
{
    this->emitSenmatic = senmaticFun;
    LOG_CHECK_STATUS_ONCE("get Senmatic Callback.");
}

void
DetectorInner::GetView(BoxInfo &view, const psl::SlamResult &pose) const
{
    view.location = psl::Location(0, 0, 0);
    view.edgeLeft.machine = psl::Location(VALID_DISTANCE_4_SENMATIC_HORIZONTAL_LEFT
                                          , VALID_DISTANCE_4_SENMATIC
                                          , 0);
    view.edgeRight.machine = psl::Location(VALID_DISTANCE_4_SENMATIC_HORIZONTAL_RIGHT
                                           , VALID_DISTANCE_4_SENMATIC
                                           , 0);
    // TODO :RBN100 may be wrong
    SetWorldLocation(view, pose, pose);

//    Machine2World(view, pose, pose);
}

bool DetectorInner::LoadMap(const std::string &file)
{
    LOG_CHECK_DEBUG(INFO) << "LoadMap file: " << file;

    if (!ready)
    {
        LOG_CHECK_STATUS(INFO) << "Init not ready, can't load map";
        return false;
    }

    bool ret = false;

    try
    {
        ret = mapManager->Load(file);

        if (not ret) goto exit;
    }
    catch (...)
    {
        LOG_CHECK_STATUS(WARNING) << "exception.";
        goto exit;
    }

    LOG_CHECK_STATUS(WARNING) << "success to read map file.";
    return true;

    exit:
    LOG_CHECK_STATUS(WARNING) << "fail to read map file.";
    return false;
}

bool DetectorInner::SaveMap(const std::string &file)
{
    LOG_CHECK_DEBUG(INFO) << "SaveMap file: " << file;
    try
    {
        if (!ready)
        {
            LOG_CHECK_STATUS(INFO) << "Init not ready, can't save map";
            return false;
        }
        // wait for update by slam
        psl::SenmaticMap senmaticMap;
        mapManager->GetMap(senmaticMap);
        senmaticMap.finalUpdate = true;
        EMIT_SENMATIC_MAP(senmaticMap);
        LOG_CHECK_STATUS(INFO) << "send final map to slam";
        wait4Update = true;

        std::time_t start = GetTimestamp();
        while (wait4Update)
        {
            LOG_CHECK_STATUS_WITE_STEP(ONE_SECOND, "waiting for slam update");
            SLEEP_MS(WAIT_4_MAP_UPDATE_SLAM_TIME);
            if (GetTimestamp() - start > WAIT_4_MAP_UPDATE_SLAM_TIME_MAX) break;
        }

        if (GetTimestamp() - start > WAIT_4_MAP_UPDATE_SLAM_TIME_MAX)
            LOG_CHECK_STATUS(INFO) << "fail to wait.";
        else
            LOG_CHECK_STATUS(INFO) << "success to update final.";

        start = GetTimestamp();
        while (mapModifying)
        {
            SLEEP_MS(WAIT_4_MAP_UPDATE_DL_TIME);
            if (GetTimestamp() - start > WAIT_4_MAP_UPDATE_DL_TIME_MAX) break;
        }

        // write
        bool ret = mapManager->Save(file);
        if (not ret) goto exit;
    }
    catch (...)
    {
        goto exit;
    }

    LOG_CHECK_STATUS(WARNING) << "success to save map file.";
    return true;

    exit:
    LOG_CHECK_STATUS(WARNING) << "fail to save map file.";
    return false;
}

void DetectorInner::GetFOV(const psl::CameraMoudleParam &moduleParam) const
{
    auto leftParam = moduleParam._left_camera.at(RESOLUTION);
    float f = leftParam._P[0];
    int cx = leftParam._P[2];

    FOV_LEFT = atan(cx / f) / ANGLE_2_RADIAN;
    FOV_RIGHT = atan((WIDTH - cx) / f) / ANGLE_2_RADIAN;
    FOV = FOV_LEFT + FOV_RIGHT;

    VALID_DISTANCE_4_SENMATIC_HORIZONTAL_LEFT =
            -tan(FOV_LEFT * ANGLE_2_RADIAN) * VALID_DISTANCE_4_SENMATIC;
    VALID_DISTANCE_4_SENMATIC_HORIZONTAL_RIGHT =
            tan(FOV_RIGHT * ANGLE_2_RADIAN) * VALID_DISTANCE_4_SENMATIC;

    LOG_CHECK_STATUS(INFO) << "camera fov: " << FOV;
}

bool DetectorInner::InitCamera(const psl::CameraMoudleParam &moduleParam)
{
    GetFOV(moduleParam);
    bool ret = data.InitCamera(cameraParam);

    if (ret)
    {
        return true;
    }
    else
    {
        SLEEP_MS(5); // TODO : why sleep

        errorMessage->Push(ErrorMessageCode::DL_INVALID_CAMERA_PARAM
                           , "invalid camera param config");
        LOG_CHECK_STATUS(INFO) << "fail init camera.";
    }

    return ret;
}

void DetectorInner::Display(const std::string name
                            , const std::vector<BoxInfo> &boxes
                            , const cv::Mat &image, const bool dot) const
{
    if (configParam.NOT_SHOW != configParam.display)
    {
        if (image.empty()) return;
        cv::Mat displayImage = image; // TODO : what
        DrawBoxes(boxes, displayImage, dot);
    }
}

void DetectorInner::PostProcess(const DataGroup item, DetectResult &boxes)
{
    DetectResult boxesClean;
    BoxInfo view;
    cv::Mat imageDisplay;
    std::vector<psl::Instance> instanceInfos;

    if (RandyProject())
    {
        GetView(view, item.lidar.pose);

        psl::SenmaticMap senmaticMap;
        SetWorldLocation(boxes, item.lidar.pose);
        mapManager->Update(boxes.boxes, view, item.image.time);

        mapManager->GetMap(senmaticMap);

        cv::Mat worldMap4Display;
        if (ConfigParam::NOT_SHOW != configParam.display)
        {

            worldMap4Display = mapManager->Show(view, boxes.boxes);
            worldMap4Display = detectorAircraft->Show(configParam, worldMap4Display);
        }


        if (configParam.SHOW == configParam.display)
        {
            cv::imshow("result", worldMap4Display);
            cv::waitKey(1);
        } else if (configParam.WRITE == configParam.display)
        {
            Write(worldMap4Display, fileName);
        }
        EMIT_SENMATIC_MAP(senmaticMap);
        LOG_CHECK_DEBUG(INFO) << "send aircraft hatch!";
    }
    else
    {
        GetView(view, item.image.pose);
        Timer timerDistance;
        bool ret = instanceManager.GetInstancesInfo(boxes, item.lidar
                                                    , instanceInfos, obstacle
                                                    , boxesClean);

        // remove error distance obj
        {
            int boxSize = boxes.boxes.size();
            for (int i = 0; i < boxSize; ++i)
            {
                // far than 6m
                float front = boxes.boxes.at(i).location.machine.x;
                if (front > 6)
                {
                    boxes.boxes.erase(boxes.boxes.begin() + i);
                    i--;
                    boxSize--;
                }
            }

            boxSize = boxesClean.boxes.size();
            for (int i = 0; i < boxSize; ++i)
            {
                // far than 6m
                float front = boxesClean.boxes.at(i).location.machine.x;
                if (front > 6)
                {
                    boxesClean.boxes.erase(boxesClean.boxes.begin() + i);
                    i--;
                    boxSize--;
                }
            }

        }
        // update boxes with boxesClean
        {
            size_t boxSize = boxes.boxes.size();
            for (int i = 0; i < boxSize; ++i)
            {
                if (boxes.boxes.at(i).IsEscalator())
                {
                    boxes.boxes.erase(boxes.boxes.begin() + i);
                    i--;
                    boxSize--;
                }
            }

            boxSize = boxesClean.boxes.size();
            for (int i = 0; i < boxSize; ++i)
            {
                if (boxesClean.boxes.at(i).IsEscalator() and
                    not boxesClean.boxes.at(i).dummy)
                {
                    boxes.boxes.push_back(boxesClean.boxes.at(i));
                }
            }
        }

        //PrintObject(boxesClean.boxes);

        if (ret)
        {
            imageDisplay = item.image.left;

            SetWorldLocation(instanceInfos, item.lidar.pose);
            SetWorldLocation(boxes, item.lidar.pose);
            SetWorldLocation(boxesClean, item.lidar.pose);

            if (instanceManager.ErrorDomain()) TrueNums = 0;
            else TrueNums += 1;

            if (withLidar)
            {
                UpdateBoxByPoseOffset(boxesClean.boxes, item.image.pose, item.lidar.pose);
            }

            mapManager->Update(boxesClean.boxes, view, item.image.time);
        }

        LOG_CHECK_TIME(INFO) << timerDistance.TimingStr("distance estimate");

        // TODO : id for check have object or not; to remove - (id)
        psl::SenmaticMap senmaticMap;
        cv::Mat worldMap4Display;

        BoxInfo machineCenter;
        machineCenter.location.machine = psl::Location(0, 0, 0);
        Machine2World(machineCenter.location.machine, machineCenter.location.world, item.lidar.pose);
        mapManager->GetDistanceValidStatus(machineCenter);

        mapManager->GetMap(senmaticMap);

        if(not senmaticMap.objects.empty())
        {
            int k = 0;
        }
        LOG_CHECK_TIME(INFO) << timerDistance.TimingStr("get map.");

#ifdef SHOW_MAP
        if (ConfigParam::NOT_SHOW != configParam.display)
        {
            worldMap4Display = mapManager->Show(view, boxes.boxes);
            LOG_CHECK_TIME(INFO) << timerDistance.TimingStr("show map.");
        }
#endif

//    Instance2EscaltorMap(boxes, senmaticMap);
        ObjectsDistanceValid(senmaticMap);
        PrintObject(senmaticMap);
        EMIT_SENMATIC_MAP(senmaticMap);
        CALLBACK_DETECTOR(instanceInfos, obstacle);

        // show box and lidar
        Display("origin", boxes.boxes, imageDisplay);
#ifdef SHOW_MAP
        if (withLidar)
        {
            UpdateBoxByPoseOffset(boxes.boxes, item.image.pose, item.lidar.pose);
            Display("origin", boxes.boxes, imageDisplay, true);
            imageDisplay = ShowLidar(pMatrix, configParam, item);
        }
#endif
        // concate the display result
        if (configParam.NOT_SHOW != configParam.display and not imageDisplay.empty())
        {
            if (imageDisplay.channels() == 1)
            {
                cv::cvtColor(imageDisplay, imageDisplay, cv::COLOR_GRAY2BGR);
            }
#ifdef SHOW_MAP
            cv::Mat displayAll;
            cv::Mat rest = cv::Mat::zeros(worldMap4Display.rows - imageDisplay.rows, imageDisplay.cols,
                                          imageDisplay.type());

            cv::vconcat(imageDisplay, rest, displayAll);
            cv::hconcat(worldMap4Display, displayAll, displayAll);
#else
            cv::Mat displayAll = imageDisplay;
#endif
            if (configParam.SHOW == configParam.display)
            {
                cv::imshow("result", displayAll);
                cv::waitKey(1);
            } else if (configParam.WRITE == configParam.display)
            {
                Write(displayAll, fileName);
            }
            LOG_CHECK_TIME(INFO) << timerDistance.TimingStr("show box.");
        }
    }
}

void DetectorInner::Write(const cv::Mat image, const std::string fileName) const
{
    static bool first = true;
    if (first)
    {
        file_op::File::RemoveDir(outputDir);
        first = false;
    }

    std::string file = outputDir + "/" + fileName;
    file_op::File::MkdirFromFile(file);
    cv::imwrite(file, image);
    LOG_CHECK_TEST(INFO) << "write image to: " << file;
}

bool DetectorInner::SetName(const std::string name)
{
    fileName = name;
}

void DetectorInner::UpdateBoxByPoseOffset(std::vector<BoxInfo> &boxes
                                          , const psl::SlamResult &poseLast
                                          , const psl::SlamResult &poseCurrent)
{
    double radianDelta = GetAngle(poseLast, poseCurrent);
    const auto &fx = pMatrix(0, 0);
    const auto &cx = pMatrix(0, 2);

    float radianLeft, radianRight;
    for (auto &b : boxes)
    {
        GetAngle(pMatrix, b.box.x, radianLeft);
        GetAngle(pMatrix, b.box.x + b.width, radianRight);
        radianLeft += radianDelta;
        radianRight += radianDelta;

        auto newLeft = tan(radianLeft) * fx + cx;
        auto newRight = tan(radianRight) * fx + cx;

        b.SetRect(cv::Rect(newLeft, b.box.y, newRight - newLeft, b.box.height));
    }
}

bool DetectorInner::SetParam(const psl::CameraMoudleParam &moduleParam)
{
    cameraParam = moduleParam;
    getCameraParam = true;

    // TODO : how to control the order of submodule init
    bool ret = InitCamera(cameraParam);
    CHECK_STATUS_EXIT(ret, "");

    ret = InitInstance();
    CHECK_STATUS_EXIT(ret, "");

    ready = inited;
    LOG_CHECK_STATUS(INFO) << "success to set param.";

    return true;

    exit:
    LOG_CHECK_STATUS(ERROR) << "fail to set param.";
    return false;
}

bool DetectorInner::EnableLidar(const bool withLidar)
{
    this->withLidar = withLidar;
//    if (not withLidar)
//    {
//        this->configParam.structure.cameraAngle = this->configParam.structure.cameraAngle / RADIAN_2_ANGLE;
//    }
}

void DetectorInner::Tracking(std::vector<BoxInfo> &boxes)
{
    //Tracking by sort
    vector<TrackingBox<ShapeType>> rectResult;
    vector<ShapeType> rectangles;

    for (auto box: boxes)
    {
        rectangles.push_back(ShapeType(box.box));
    }

    Timer timer;
//    multiTracker.Update(rectangles, rectResult);
    LOG_CHECK_TIME(INFO) << timer.TimingStr();

    if (rectResult.size() == rectangles.size())
    {
        for (int i = 0; i < rectResult.size(); ++i)
        {
            boxes[i].SetRect(rectResult[i].region.data);
            boxes[i].id = rectResult[i].id - 1;
        }
    }
}

void DetectorInner::Instance2EscaltorMap(DetectResult &boxes
                                         , psl::SenmaticMap &senmaticMap)
{
    const float maxFirstPointDistance2Camera = 0.5;
    const float maxFirstPointDistance2Machine = this->configParam.structure.leftcamera2lidar[0]
                                                + this->configParam.structure.poseLidar2Machine[0]
                                                + maxFirstPointDistance2Camera;

    // TODO add nodeID,
    for (auto box : boxes.boxes)
    {
        if (box.IsEscalator() || box.IsEscalatorHandrails())
        {
            continue;
        }

        psl::Object object;
        object.id = box.id;
        object.classID = box.classID;
        object.score = box.score;
        object.detectExtend = detectParam.modelProperty.detectExtend[object.classID];
        object.time = boxes.time;
        object.statusOperator = psl::Object::INITED_DL;
        object.status = psl::Object::Status::ENABLE;
        object.history = false;
        if (box.className == "desk_rect" || box.className == "desk_circle")
        {
            if (this->configParam.structure.cameraHeight < 0.2)
            {
                bool clean = true;
                for (int i = 0; i < 4; ++i)
                {
                    // far than 0.5m
                    float front = box.locationSerials[i].machine.x;
                    if (front < maxFirstPointDistance2Machine)
                    {
                        clean = false;
                        break;
                    }
                }
                if (clean)
                {
                    continue;
                }
            }

            for (int j = 0; j < 4; ++j)
            {
                object.locationWorld.push_back(box.locationSerials[j].world);
            }
        }
        else
        {
            object.locationWorld.push_back(box.location.world);
        }

        senmaticMap.objects.push_back(object);

    }
    // TODO maybe set from Obstacle::status
    senmaticMap.status = psl::SenmaticMap::UPDATE;
}

void DetectorInner::SetOutputDir(const std::string path)
{
    this->outputDir = path + "/" + OUTPUT_NAME;
}

bool DetectorInner::RotateMap(const float angle)
{
    float rotationAngle = angle * RADIAN_2_ANGLE;

    if (abs(rotationAngle) < 1)
    {
        LOG_CHECK_STATUS(INFO) << "cancel rotate; angle too small：" << rotationAngle;
        return true;
    }
    mapManager->RotateMap(rotationAngle);

    LOG_CHECK_STATUS(INFO) << "rotate map angle: " << rotationAngle;

    psl::SenmaticMap senmaticMap;
    mapManager->GetMap(senmaticMap);
    senmaticMap.finalUpdate = true;

    if (configParam.WRITE == configParam.display)
    {
        cv::Mat image;
        std::string file = outputDir + "/../last_rotate.jpg";
        file_op::File::MkdirFromFile(file);

        image = mapManager->Show(BoxInfo(), vector<BoxInfo>{});

        cv::imwrite(file, image);
        LOG_CHECK_TEST(INFO) << "write rotate image to: " << file;
    }

    EMIT_SENMATIC_MAP(senmaticMap);
    LOG_CHECK_STATUS(INFO) << "send final map when rotate";

    return true;
}

void DetectorInner::ObjectsDistanceValid(psl::SenmaticMap &senmaticMap) {

    for (int i = 0; i < senmaticMap.objects.size(); ++i) {

        if (senmaticMap.objects[i].status) {
            senmaticMap.objects.erase(senmaticMap.objects.begin() + i);
            --i;

        }
    }
}

void DetectorInner::SetProjectName(const psl::DetectorParam detectorParam)
{
    if (std::find(detectorParam.modelProperty.names.begin()
                  , detectorParam.modelProperty.names.end(), "shoes")
                  != detectorParam.modelProperty.names.end())
    {
        this->projectName = "rubby";
    }
    else if (std::find(detectorParam.modelProperty.names.begin()
            , detectorParam.modelProperty.names.end(), "aircraft_hatch")
            != detectorParam.modelProperty.names.end())
    {
        this->projectName = "randy";
    }
    else if (std::find(detectorParam.modelProperty.names.begin()
            , detectorParam.modelProperty.names.end(), "escalator")
             != detectorParam.modelProperty.names.end())
    {
        this->projectName = "rbn100";
    }
}

bool DetectorInner::RBN100Project()
{
    return this->projectName == "rbn100";
}

bool DetectorInner::RubbyProject()
{
    return this->projectName == "rubby";
}

bool DetectorInner::RandyProject()
{
    return this->projectName == "randy";
}