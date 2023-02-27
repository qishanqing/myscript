#include "src/perception_inner.h"
#include "src/utils/utils.h"
#include <opencv2/highgui.hpp>
#include <sys/prctl.h>
#include "utils/timer.h"
#include "src/utils/file.h"
#include "src/utils/define.h"
#include "src/utils/detector_param.h"
#include "src/utils/detector_property.h"
#include "src/map/escalator_map.h"
#include "src/utils/lidar.h"
// detect model
#include "src/module/yolov3.h"
#include "src/module/yolov5n.h"
#include "src/module/ssd.h"

const int WAIT_4_DATA_TIME = 5; // milliseconds
const int WAIT_4_INIT_TIME = 50; // milliseconds
const int WAIT_4_START_TIME = 50; // milliseconds
const int WAIT_4_MAP_UPDATE_SLAM_TIME = 20; // milliseconds
const int WAIT_4_MAP_UPDATE_DL_TIME = 5; // milliseconds
const int WAIT_4_MAP_UPDATE_SLAM_TIME_MAX = 3000; // milliseconds
const int WAIT_4_MAP_UPDATE_DL_TIME_MAX = 500; // milliseconds
const int MAX_LACK_DATA = 1000; // milliseconds
const int DETECT_STEP = 20; // milliseconds
const int KEEP_FREE_TIME = 5; // milliseconds
const int FOV = 90; // horizontal
const float VALID_DISTANCE_4_SENMATIC = 6; // 6m
const float TAN_FOV_HALF = tan((FOV >> 1) * ANGLE_2_RADIAN);
const float VALID_DISTANCE_4_SENMATIC_HORIZONTAL =
        TAN_FOV_HALF * VALID_DISTANCE_4_SENMATIC;
const std::string RESULT_DIR = "result";
const int LOG_PRINT_STEPS = 100;

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

const std::vector<std::string> BACKEND{"npu", "gpu"}; // TODO : move into detector_moudle_inner

Log g_logConfig;

DetectorInner::DetectorInner()
        : ready(false), inited(false), configFile("")
          , emitSenmatic(nullptr), mapModifying(false)
          , errorMessage(nullptr), wait4Update(false)
          , getCameraParam(false)
          , withLidar(false)
          , testRunning(true)
{
}

DetectorInner::DetectorInner(const psl::CameraMoudleParam &moduleParam)
        : ready(false), inited(false), cameraParam(moduleParam), configFile("")
          , emitSenmatic(nullptr), mapModifying(false)
          , errorMessage(nullptr), wait4Update(false), getCameraParam(true)
          , withLidar(false)
{
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
    RELEASE(escalatorMap);
}

bool DetectorInner::Init(const psl::DetectorParam &detectParam)
{
    instanceManager.UnloadLidar();

    // TODO : when miss option, how to do
    Timer timer;
    this->detectParam = detectParam;
    this->detectParam.config_path += "/";

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
                    MODEL_VERSION; // after load config

        ret = InitModel(modelFile); // TODO : not init model, is init detect, or init ...
        CHECK_STATUS_EXIT(ret, "fail to init model");

        escalatorMap = new EscalatorMap();

        // TODO : when fail to init, how to set the start
        Start(detectParam); // TODO : statr to early befor all init done

        if (getCameraParam) ret = SetParam(cameraParam);
        CHECK_STATUS_EXIT(ret, "");

        LOG_CHECK_STATUS(INFO) << timer.TimingStr("Init");
    }
    catch (...)
    {
        LOG_CHECK_STATUS(ERROR) << "init abnormal.";
        goto exit;
    }

    inited = true;
    LOG_CHECK_STATUS(INFO) << "success to init perception.";
    return true;

    exit:
    inited = false;
    LOG(ERROR) << "fail to init perception.";
    return false;
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
    options.param = configParam.detect;
#ifdef USE_GPU
    options.backend = "gpu";
#else
    options.backend = "npu";
#endif

    ret = CheckBackend(options.backend);
    CHECK_STATUS_EXIT(ret, "");

    // TODO : check the model version, move the model version config inner
    ret = CreateModel();
    CHECK_STATUS_EXIT(ret,
            "donot suport model type " + std::to_string(configParam.detect.type));
    ret = detector->Init(options);
    // TODO : archive the ausolution path of model
    CHECK_STATUS_EXIT(ret, "fail to init detect model");
    // TODO : how to set statusInfo.state

    LOG_CHECK_DEBUG(INFO) << "success init.";
    data.Attach(detector.get());
    return true;

    exit:

    errorMessage->Push(ErrorMessageCode::DL_INVALID_MODEL, "fail to init detect model");
    return false;
}

bool DetectorInner::CreateModel()
{
    switch (configParam.detect.type)
    {
        case 1:
            detector = std::move(std::unique_ptr<SSD>(new SSD()));
            break;
        case 2:
            detector = std::move(std::unique_ptr<Yolov3>(new Yolov3()));
            break;
        case 3:
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
                        logMessageStashed.NewLine();
                    }
                    else
                    {
                        try
                        {
                            static bool ret = false;
                            static std::time_t lastTime = GetTimestamp();
                            if ((GetTimestamp() - lastTime) < DETECT_STEP and ret)
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

//lidar data
void DetectorInner::SetLidar(const swr::sensor_types::LaserScan &lidar)
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
        escalatorMap->Modify(enmaticMapStash);
        mapModifying = false;
    }

    Timer timer, timerDetect;
    DetectResult boxes;
    DataGroup itemCurrent = item;

//    itemCurrent.Remap();
    itemCurrent.image = itemCurrent.orgImage;
    if (itemCurrent.image.data.channels() == 1)
    {
        cv::cvtColor(itemCurrent.image.data, itemCurrent.image.data, CV_GRAY2BGR);
    }
    int c = itemCurrent.image.data.channels();
    detector->Detect(itemCurrent.image.data, boxes.boxes);
    boxes.pose = itemCurrent.image.pose;
    boxes.time = itemCurrent.image.time;
    LOG_CHECK_TIME(INFO) << timerDetect.TimingStr("detect");

    PostProcess(itemCurrent, boxes);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("whole");
    if (not boxes.boxes.empty())
        LOG_CHECK_STATUS(INFO) << "box size: " << boxes.boxes.size();

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
        escalatorMap->Modify(enmaticMapStash);
        mapModifying = false;
    }

    Timer timer, timerDetect;

    DetectResult boxes;

    DataGroup item;
    bool retDetect = detector->Detect(boxes.boxes, item);
    boxes.pose = item.image.pose;
    boxes.time = item.image.time;

    long long timeMargin = ABS_DIFF(boxes.pose.s_time, item.lidar.pose.s_time);
    LOG_CHECK_TIME(INFO) << timerDetect.TimingStr("detect");

    static std::time_t lastLackDataTime = GetTimestamp();
    if (not retDetect and item.image.data.empty()) // no data
    {
        bool lack = GetTimestamp() - lastLackDataTime > MAX_LACK_DATA;
        CHECK_STATUS_WITH_TIME_END(not lack, TEN_SECOND,
                "image and lidar not sync for a long time (" +
                std::to_string(MAX_LACK_DATA) + "ms)");
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

    escalatorMap->Clear();
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

    escalatorMap->Clear();
    data.SetReceive(true);
    operationStatus = detector_inner::RESTART;
    LOG_CHECK_STATUS(INFO) << "perception has been restarted!";
    return true;
}

// todo : move to model
bool DetectorInner::CheckDevice()
{
    uint num_platforms = -1;
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
#elif USE_NPU
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

void DetectorInner::Machine2World(const psl::Location &pm, psl::Location &pw
                                  , const psl::SlamResult &pose) const
{
    if (pose.s_rotation[0] > 100) // invalid pose
    {
        pw = pm;
        return;
    }
    Eigen::Quaterniond poseRotation;

    poseRotation.w() = pose.s_rotation[0];
    poseRotation.x() = pose.s_rotation[1];
    poseRotation.y() = pose.s_rotation[2];
    poseRotation.z() = pose.s_rotation[3];

    Eigen::Vector3d po;
    po[0] = pm.x;
    po[1] = pm.y;
    po[2] = pm.z;
    // machine coordinate to world
    Eigen::Vector3d two(pose.s_position[0], pose.s_position[1]
                        , pose.s_position[2]);
    Eigen::Vector3d pwo = poseRotation.toRotationMatrix() * po + two;
    Vector2Location(pwo, pw);
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
        cv::imwrite("/media/hao/CODE/CODE/CPP/i18R/perception/build/debug/image/"
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
DetectorInner::Machine2World(BoxInfo &box
                             , const psl::SlamResult &lidarPose
                             , const psl::SlamResult &imagePose) const
{
    // whole location
    Machine2World(box.location.machine, box.location.world, lidarPose);

    // serials location
    for (auto &parts : box.locationSerials)
    {
        Machine2World(parts.machine, parts.world, lidarPose);
    }

    // edge location
    Machine2World(box.edgeLeft.machine, box.edgeLeft.world, imagePose);
    Machine2World(box.edgeRight.machine, box.edgeRight.world, imagePose);
}

void
DetectorInner::GetView(BoxInfo &view, const psl::SlamResult &pose) const
{
    view.location = psl::Location(0, 0, 0);
    view.edgeLeft.machine = psl::Location(VALID_DISTANCE_4_SENMATIC
                                          , -VALID_DISTANCE_4_SENMATIC_HORIZONTAL
                                          , 0);
    view.edgeRight.machine = psl::Location(VALID_DISTANCE_4_SENMATIC
                                           , VALID_DISTANCE_4_SENMATIC_HORIZONTAL
                                           , 0);
    Machine2World(view, pose, pose);
}

bool DetectorInner::LoadMap(const std::string &file)
{
    bool ret = false;

    try
    {
        ret = escalatorMap->Load(file);

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
    try
    {
        // wait for update by slam
        psl::SenmaticMap senmaticMap;
        escalatorMap->GetMap(senmaticMap);
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
        bool ret = escalatorMap->Save(file);
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

bool DetectorInner::InitCamera(const psl::CameraMoudleParam &moduleParam)
{
    bool ret = data.Init(cameraParam, RESOLUTION, configParam);

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

    GetView(view, item.image.pose);

    Timer timerDistance;
    bool ret = instanceManager.GetInstancesInfo(boxes, item.lidar, instanceInfos
                                                , obstacle, boxesClean);

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

    if (ret)
    {
        imageDisplay = item.image.data;

        SetWorldLocation(instanceInfos, item.lidar.pose);
        SetWorldLocation(boxes, item.lidar.pose);
        SetWorldLocation(boxesClean, item.lidar.pose);

        if (instanceManager.ErrorDomain()) TrueNums = 0;
        else TrueNums += 1;

        UpdateBoxByPoseOffset(boxesClean.boxes, item.image.pose, item.lidar.pose);
        escalatorMap->Update(boxesClean.boxes, view, item.image.time);

        //    PrintObject(instanceInfos);
        PrintObject(boxesClean.boxes);
    }

    LOG_CHECK_TIME(INFO) << timerDistance.TimingStr("distance estimate");

    // TODO : id for check have object or not; to remove - (id)
    psl::SenmaticMap senmaticMap;
    cv::Mat mapDisplay;
    escalatorMap->GetMap(senmaticMap);

    LOG_CHECK_TIME(INFO) << timerDistance.TimingStr("get map.");

    if (ConfigParam::NOT_SHOW != configParam.display)
    {
        mapDisplay = escalatorMap->Show(view, boxesClean.boxes);
        LOG_CHECK_TIME(INFO) << timerDistance.TimingStr("show map.");
    }
    PrintObject(senmaticMap);

    // check map with part size
    int size = senmaticMap.objects.size();
    for (int i = 0; i < size; ++i)
    {
        auto &map = senmaticMap.objects.at(i);
        const int partSize = senmaticMap.objects.at(i).locationWorld.size();
        if (partSize != 2)
        {
            LOG_CHECK_STATUS(WARNING) << "error size for senmatic map. size " << partSize;
            if (1 == partSize)
            {
                map.locationWorld.push_back(map.locationWorld.at(0));
            }
            else if (0 == partSize)
            {
                senmaticMap.objects.erase(senmaticMap.objects.begin() + i);
                i--;
                size--;
            }
            else
            {

            }
        }
    }

    EMIT_SENMATIC_MAP(senmaticMap);
    CALLBACK_DETECTOR(instanceInfos, obstacle);

    // show box and lidar
    Display("origin", boxes.boxes, imageDisplay);
    if (withLidar)
    {
        UpdateBoxByPoseOffset(boxes.boxes, item.image.pose, item.lidar.pose);
        Display("origin", boxes.boxes, imageDisplay, true);
        imageDisplay = ShowLidar(pMatrix, configParam, item);
    }

    // concate the display result
    if (configParam.NOT_SHOW != configParam.display and not imageDisplay.empty())
    {
        if (imageDisplay.channels() == 1)
        {
            cv::cvtColor(imageDisplay, imageDisplay, cv::COLOR_GRAY2BGR);
        }
        cv::Mat displayAll;
        cv::Mat rest = cv::Mat::zeros(mapDisplay.rows - imageDisplay.rows
                                      , imageDisplay.cols, imageDisplay.type());

        cv::vconcat(imageDisplay, rest, displayAll);
        cv::hconcat(mapDisplay, displayAll, displayAll);

        if (configParam.SHOW == configParam.display)
        {
            cv::imshow("result", displayAll);
            cv::waitKey(1);
        }
        else if (configParam.WRITE == configParam.display)
        {
            Write(displayAll, fileName);
        }
        LOG_CHECK_TIME(INFO) << timerDistance.TimingStr("show box.");
    }
}

void DetectorInner::Write(const cv::Mat image, const std::string fileName) const
{
    static bool first = true;
    if (first)
    {
        char *p = getcwd(NULL, 0);
        file_op::File::RemoveDir(std::string(p) + "/" + RESULT_DIR);
        first = false;
    }

    std::string file = RESULT_DIR + "/" + fileName;
    file_op::File::MkdirFromFile(file);
    cv::imwrite(file, image);
    LOG_CHECK_DEBUG(INFO) << "write image to: " << file;
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

