#ifndef DETECTOR_INNER_H_
#define DETECTOR_INNER_H_

#include "sensor/CameraMoudleParam.h"
#include "slam/SlamResult.h"
#include "sensor/LaserScan.h"
#include "perception/instance.h"
#include "perception/map.h"
#include "perception/detector_param.h"
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

extern Log g_logConfig;

class DetectorMoudle;

class EscalatorMap;

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

    void SetLidar(const swr::sensor_types::LaserScan &lidar);

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

    bool GetDetectorResult(const DataGroup &item);

    bool SetName(const std::string name = std::to_string(GET_TIMESTAMP) + ".jpg");

private:
    bool LoadConfig();

    bool GetDetectorResult();

    bool InitInstance();

    bool InitModel(std::string &modelFile);

    bool CreateModel();

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

    void Machine2World(BoxInfo &box
                       , const psl::SlamResult &lidarPose
                       , const psl::SlamResult &imagePose) const;

    void Machine2World(const psl::Location &pm, psl::Location &pw
                       , const psl::SlamResult &pose) const;

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

public:
    psl::Obstacle obstacle;

private:
    // impl detectorInner
    std::unique_ptr<ModelType> detector;
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
    EscalatorMap *escalatorMap;
    bool mapModifying;
    ErrorMessage *errorMessage;
    bool wait4Update;
    std::mutex senmaticMapMutex;
    std::string fileName;
    Eigen::Matrix<double, 3, 4> pMatrix;
    bool getCameraParam;

    psl::DetectorParam detectParam;
    bool testRunning;
};


#endif
