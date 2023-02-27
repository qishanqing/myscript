//////////////////////////////////////////////////////////////////////
///  @file     perception.h
///  @brief    perception mindos node
///  Details.
///
///  @author   sunhao
///  @version  1.0.0
///  @date     2022.03.31
///
///  revision statement:
//////////////////////////////////////////////////////////////////////
#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>
#include "srv/perception/Init.h"
#include "srv/perception/Start.h"
#include "srv/perception/Stop.h"
#include "srv/perception/Pause.h"
#include "srv/perception/Restart.h"
#include "srv/perception/LoadMap.h"
#include "srv/perception/SaveMap.h"
#include "srv/perception/RotateMap.h"
#include "srv/slam/GetPoseSrv.h"
#include "msg/sensor/ImgMsg.h"
#include "msg/slam/SlamResult.h"
#include "msg/slam/OccupancyGrid.h"
#include "msg/perception/SenmaticMap.h"
#include "PSL/src/perception.h"
#include "srv/sensor/ImseeParamSrv.h"

class PerceptionSimulator : public mind_os::NodePlugin
{
public:
    PerceptionSimulator();

    ~PerceptionSimulator();

private:
    void onLoaded() override;

    void onUnloaded() override;

    void SetImage();

    void SetMap();

    void SetPose();

    void SetTopicMap();

    void SetTopicError();

    void SetServiceInit();

    void SetServiceStart();

    void SetServiceStop();

    void SetServicePause();

    void SetServiceRestart();

    void SetServiceLoadMap();

    void SetServiceSaveMap();

    void SetServiceRotateMap();

    void SetDetectorParam(psl::DetectorParam &detectorParam);

    void Init(mind_os::ConstPtr<perception_srvs::Init::Request> &request
              , mind_os::Ptr<perception_srvs::Init::Response> &response);

    void Start(mind_os::ConstPtr<perception_srvs::Start::Request> &request
               , mind_os::Ptr<perception_srvs::Start::Response> &response);

    void Stop(mind_os::ConstPtr<perception_srvs::Stop::Request> &request
              , mind_os::Ptr<perception_srvs::Stop::Response> &response);

    void Pause(mind_os::ConstPtr<perception_srvs::Pause::Request> &request
            , mind_os::Ptr<perception_srvs::Pause::Response> &response);

    void Restart(mind_os::ConstPtr<perception_srvs::Restart::Request> &request
            , mind_os::Ptr<perception_srvs::Restart::Response> &response);

    void LoadMap(mind_os::ConstPtr<perception_srvs::LoadMap::Request> &request
                 , mind_os::Ptr<perception_srvs::LoadMap::Response> &response);

    void SaveMap(mind_os::ConstPtr<perception_srvs::SaveMap::Request> &request
                 , mind_os::Ptr<perception_srvs::SaveMap::Response> &response);

    void RotateMap(mind_os::ConstPtr<perception_srvs::RotateMap::Request> &request
            , mind_os::Ptr<perception_srvs::RotateMap::Response> &response);

    void GetSensorImage(mind_os::ConstPtr<sensor_msgs::ImgMsg> &imsee);

    void GetSlamMap(mind_os::ConstPtr<perception_msgs::SenmaticMap> &map);

    void GetSlamPose(mind_os::ConstPtr<slam_msgs::SlamResult> &pose);

    void SetGetPoseByTimeCallBack();

    void SetSenmaticMapCallBack();

    void Pose2PSL(const slam_msgs::SlamResult &slamPose
                  , psl::SlamResult &pslPose);

    void Map2Perception(const psl::SenmaticMap &src
                        , mind_os::Ptr<perception_msgs::SenmaticMap> &dst);

    void Map2PSL(const perception_msgs::SenmaticMap &src
                 , psl::SenmaticMap &dst);

    void GetSensorCameraParam();

    void SetCameraParam();

    bool GetResolution(const std::map<sensor_srvs::Resolution, sensor_srvs::CameraParameter> &camera
                       , sensor_srvs::Resolution &sensorResolution
                       , psl::Resolution &plsResolution);

    void ConvertCameraParameter(const sensor_srvs::CameraParameter &sensorCameraParameter
                                , psl::CameraParam &cameraParam);


private:
    mind_os::NodeHandle &nodeHandle = getPrivateNodeHandle();
    mind_os::util::LoopThread *threadError;
    mind_os::Publisher pubSenmaticMap;
    mind_os::Publisher pubError;
    mind_os::ServiceServer ssInit, ssStart, ssStop, ssLoadMap, ssSaveMap;
    mind_os::Subscriber subSensorImg;
    mind_os::Subscriber subSlamMap;
    mind_os::Subscriber subSlamPose;
    mind_os::Subscriber subEncoder;
    mind_os::ServiceClient sCCameraParam;

};

extern "C" mind_os::NodePlugin *Create()
{
    return new PerceptionSimulator();
}
