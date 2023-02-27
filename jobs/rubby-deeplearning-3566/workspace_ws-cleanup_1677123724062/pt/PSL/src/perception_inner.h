#ifndef DETECTOR_INNER_H_
#define DETECTOR_INNER_H_

#include "psl/sensor/CameraMoudleParam.h"
#include "psl/slam/SlamResult.h"
#include "psl/sensor/LaserScan.h"
#include "psl/perception/instance.h"
#include "psl/perception/map.h"
#include "psl/perception/detector_param.h"
#include "src/instance/manager.h"
#include <mutex>
#include <thread>
#include <functional>
#include <queue>
#include "src/perception.h"
#include "src/error.h"
#include "src/module/yolov3_tiny.h"
#include "src/module/yolov3.h"
#include "src/data/dataset.h"
#include "src/mot/sort.h"
#include "src/utils/shape/rect.h"
#include "src/module/aircrafthatch_detector.h"

extern Log g_logConfig;

class DetectorMoudle;

class MapManager;

namespace detector_inner
{
enum OperatorStatus
{
    START, STOP, PAUSE, RESTART, UNKNOWN
};
}

using namespace robot_detector;

using ModelType = inner::Detector;

class DetectorInner : public Detector
{
public:
    DetectorInner();

    DetectorInner(const psl::CameraMoudleParam &moduleParam);

    DetectorInner(const psl::CameraMoudleParam &moduleParam
                  , const psl::DetectorParam &detectorParam
                  , const int flag);

    ~DetectorInner();

    bool Init(const psl::DetectorParam &detectParam);

    bool SetParam(const psl::CameraMoudleParam &moduleParam);

    bool LoadMap(const std::string &file);

    bool SaveMap(const std::string &file);

    void SetImage(const psl::Time time, const cv::Mat &image);

    void SetImage(const psl::Time time, const cv::Mat &imageLeft
                  , const cv::Mat &imageRight);

    void SetImage(const psl::ImageData &imageData);

    void SetLidar(const swr::psl_sensor_types::LaserScan &lidar);

    void SetPose(const psl::SlamResult &pose);

    void SetSenmaticMap(const psl::SenmaticMap &senmaticMap);

    void SetDetectorResult(DetectorCallback detectorFunc);

    void SetSenmaticCallback(SenmaticCallback senmaticFun);

    void SetGetPoseByTime(GetPoseCallback getPoseFunc);

    void SetErrorMessage(ErrorMessageCallback errorFunc);

    bool Start();

    bool Pause();

    bool Stop();

    bool Restart();

    bool RotateMap(const float angle);

    bool GetDetectorResult(const DataGroup &item);

    bool SetName(const std::string name = std::to_string(GET_TIMESTAMP) + ".jpg");

    bool EnableLidar(const bool withLidar);

    void SetOutputDir(const std::string path);

private:
    bool LoadConfig();

    bool GetDetectorResult();

    bool InitInstance();

    bool InitModel(std::string &modelFile);

    bool CreateModel();

    void GetFOV(const psl::CameraMoudleParam &moduleParam) const;

    bool InitCamera(const psl::CameraMoudleParam &moduleParam);

    bool CheckDevice();

    bool ErrorMissConfig(const std::string message);

    bool ErrorMissConfigOption(const std::string message);

    bool CheckModelFile(std::string &modelFile);

    bool CheckBackend(const std::string flag);

    void SetWorldLocation(std::vector<psl::Instance> &instances
                          , const psl::SlamResult &pose) const;

    // TODO : merge the two SetWorldLocation, convert location once
    void SetWorldLocation(DetectResult &boxes
                          , const psl::SlamResult &lidarPose) const;

    void SetWorldLocation(BoxInfo &box
                          , const psl::SlamResult &lidarPose
                          , const psl::SlamResult &imagePose) const;

    void Display(const std::string name, const std::vector<BoxInfo> &boxInfos
                 , const std::vector<psl::Instance> &instanceInfos
                 , const cv::Mat &image) const;

    void Display(const std::string name, const std::vector<BoxInfo> &boxes
                 , const cv::Mat &image, const bool dot = false) const;

    void Start(const psl::DetectorParam detectorParam);

    void GetView(BoxInfo &view, const psl::SlamResult &pose) const;

    void PostProcess(const DataGroup item, DetectResult &boxes);

    void Write(const cv::Mat image, const std::string fileName) const;

    void UpdateBoxByPoseOffset(std::vector<BoxInfo> &boxes
                               , const psl::SlamResult &poseLast
                               , const psl::SlamResult &poseCurrent);

    void Tracking(std::vector<BoxInfo> &boxes);

    void Instance2EscaltorMap(DetectResult &instances
                                , psl::SenmaticMap &senmaticMap);

    void ObjectsDistanceValid(psl::SenmaticMap &senmaticMap);

    void SetProjectName(const psl::DetectorParam detectorParam);

    bool RBN100Project();

    bool RubbyProject();

    bool RandyProject();

public:
    psl::Obstacle obstacle;

private:
    std::string outputDir;
    // impl detectorInner
    std::unique_ptr<ModelType> detector;
    std::unique_ptr<inner::AircraftHatchDetector> detectorAircraft;
    ConfigParam configParam;
    // all detected objects managed here
    InstanceManager instanceManager;

    int TrueNums = 0;

    detector_inner::OperatorStatus operationStatus = detector_inner::UNKNOWN;

    psl::DetectorParam detector_param_;

    bool inited;
    bool ready;
    bool withLidar;

    std::thread testThread;
    psl::CameraMoudleParam cameraParam;
    std::string configFile;

    GetPoseCallback getPoseByTime;

    // for data sync
    DataSet data;
    SenmaticCallback emitSenmatic; // emit senmatic map
    psl::SenmaticMap senmaticMapStash;
    MapManager *mapManager;
    bool mapModifying;
    ErrorMessage *errorMessage;
    bool wait4Update;
    std::mutex senmaticMapMutex;
    std::string fileName;
    Eigen::Matrix<double, 3, 4> pMatrix;
    bool getCameraParam;

    psl::DetectorParam detectParam;
    bool testRunning;
    int detectStep; // milliseconds
    Sort multiTracker;
    std::string projectName;
};


#endif
