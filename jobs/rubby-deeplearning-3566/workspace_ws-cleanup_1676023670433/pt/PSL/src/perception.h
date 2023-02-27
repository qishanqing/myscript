//////////////////////////////////////////////////////////////////////
///  @file     perception.h
///  @brief    object detect based on deep learning for i18R
///            small wash robot
///  Details.
///
///  @author   sunhao
///  @version  1.7.0
///  @date     2021.04.19
///
///  revision statement:
///            1.0.0 beta version
///            1.1.0 add pose interface; rename interface
///            2021.10.27 1.7.0 add new error message interface
//////////////////////////////////////////////////////////////////////

#ifndef PERCEPTION_H_
#define PERCEPTION_H_

#include "perception/instance.h"
#include "perception/map.h"
#include "perception/detector_param.h"
#include "sensor/CameraMoudleParam.h"
#include "slam/SlamResult.h"
#include "sensor/LaserScan.h"
#include "error/ErrorMessage.h"
#include <functional>
#include <vector>
#include <memory>

namespace robot_detector
{
// a list of objects in the image @see I18RPublicBaseTypes/perception/detect_types.h
using DetectorResult = std::vector<psl::Instance>;
using DetectorResultMap = psl::Obstacle;

typedef std::function<void(const DetectorResult &
                           , const DetectorResultMap &)> DetectorCallback;
typedef std::function<bool(const psl::Time time , psl::SlamResult &res)> GetPoseCallback;
typedef std::function<void(const psl::SenmaticMap &)> SenmaticCallback;
typedef std::function<void(const psl::ErrorMessage &)> ErrorMessageCallback;

class Detector
{
public:
    Detector()
    {}

    virtual ~Detector()
    {}

    /**
     * @brief init the deep learning model
     * @param[in] detectParam : module param about config and so on
     *                          @see I18RPublicBaseTypes/perception/detect_types.h
     *
     * @return init status. [true] success, [false] fail
     */
    virtual bool Init(const psl::DetectorParam &detectParam) = 0;

    virtual bool SetParam(const psl::CameraMoudleParam &moduleParam) = 0;

    /**
     * @brief load map when start work
     * @param[in] file : map file path
     * @return load status. [true] success, [false] fail
     */
    virtual bool LoadMap(const std::string &file) = 0;

    /**
     * @brief save map when stop work
     * @param[in] file : map file path
     * @return load status. [true] success, [false] fail
     */
    virtual bool SaveMap(const std::string &file) = 0;

    /**
     * @brief receive the image from camera
     * @param[in] time : timestamp of image @see I18RPublicBaseTypes/base/Time.h
     * @param[in] image : gray or rbg image for object detect
     * @todo `SetImage(const imsee_types::CImseeImage &msg)` for next version;
     */
    virtual void SetImage(const psl::Time time, const cv::Mat &image) = 0;

    /**
     * @brief receive 3d tof data
     * @param[in] lidar : lidar data @see I18RPublicBaseTypes/sensor/LaserScan.h
     */
    virtual void SetLidar(const swr::sensor_types::LaserScan &lidar) = 0;

    /**
     * @brief receive the pose of robot from slam
     * @param pose : robot pose @see I18RPublicBaseTypes/slam/SlamResult.h
     */
    virtual void SetPose(const psl::SlamResult &pose) = 0;

    /**
     * @brief receive the senmatic map from slam
     * @param senmaticMap : static senmatic map update by slam @see I18RPublicBaseTypes/perception/instance.h
     */
    virtual void SetSenmaticMap(const psl::SenmaticMap &senmaticMap) = 0;

    /**
     * @brief receive the call back function about object detect result in world coordinate system
     * @param detectorFunc : a function to get the object detect result
     */
    virtual void SetDetectorResult(DetectorCallback detectorFunc) = 0;

    /**
     * @brief receive the call back function about senmatic map
     * @param senmaticFun : a function to get the senmatic map
     */
    virtual void SetSenmaticCallback(SenmaticCallback senmaticFun) = 0;

    /**
     * @brief receive the call back function about get pose by temstamp
     * @param detectorFunc : a function to get the object detect result
     */
    virtual void SetGetPoseByTime(GetPoseCallback getPoseFunc) = 0;

    /**
     * @brief receive the the call back function about module exception errorcode
     * @param errorFunc : a fucntion to get the exception errorcode
     */
    virtual void SetErrorMessage(ErrorMessageCallback errorFunc) = 0;

    virtual bool Start() = 0;

    virtual bool Pause() = 0;

    virtual bool Stop() = 0;

    virtual bool Restart() = 0;
};

/**
 * @brief create the module pointer
 * @param moduleParam : the param about the robot e.g. camera param
 * @return the moudle pointer
 * should be use @see `Destroy` to release when useless
 */
Detector *Create(const psl::CameraMoudleParam &moduleParam);

Detector *Create();

void Destroy(Detector *detectorPtr);
}

#endif
