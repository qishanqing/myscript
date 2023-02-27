//
// Created by gby on 23-01-31.
//

#ifndef RECOVERY_H_
#define RECOVERY_H_

#include "BasicAction.h"

#include "Map.h"
#include <opencv2/highgui/highgui.hpp>
#include <vector>

enum StepObs
{
    StepObs_INIT = 0,
    StepObs_CLEAR,
    StepObs_SEARCH,
    StepObs_ACTION
};

enum EStepObsClear
{
    EStepObsClear_DEFAULT = 0,
    // Rotate_INIT,
    // Rotate_TURN,
    EStepObsClear_CLEAR
};

enum ESteapObsAction
{
    ESteapObsAction_DEFAULT = 0,
    ESteapObsAction_CALDIFF,
    ESteapObsAction_TURNINIT,
    ESteapObsAction_TURN,
    ESteapObsAction_DRIVEINIT,
    ESteapObsAction_DRIVE,
    ESteapObsAction_BUMPER,
    ESteapObsAction_BACK
};

enum StepSurround
{
    StepSurround_INIT = 0,
    StepSurround_DRIVE,
    StepSurround_BUMPER
};

class Recovery: virtual public BasicAction
{
private:
    cv::Mat _NavMap;  // 导航图
    cv::Mat _ForbidenMap;  // 禁区图

    StepObs _StepObs = StepObs::StepObs_INIT;
    EStepObsClear _StepObsClear = EStepObsClear::EStepObsClear_DEFAULT;
    PoseStamped _RobotPoseLattice;
    int _ClearNum;
    vector<cv::Point> _SearchPoint;
    ESteapObsAction _StepObsAction = ESteapObsAction::ESteapObsAction_DEFAULT;
    double _DiffYaw;
    cv::Point _targetActionPoint;

    PoseStamped _RobotPoseSurround;
    StepSurround _StepSurround = StepSurround::StepSurround_INIT;

private:
    void StepObsClear(int num_, DataDown *dataDown_);
    bool StepObsSearch(int num_);
    double ESteapObsAction_CalDiff(const SensorInfo *sensor_, cv::Point robotPoint_, cv::Point targetPoint_);
    void ESteapObsAction_RunDrive(const SensorInfo *sensor_, PoseStamped curPose_, cv::Point targetPoint_, DataDown *dataDown_);
    void ESteapObsAction_Bumper(const SensorInfo *sensor_, PoseStamped targetPose_, DataDown *dataDown_);
    void ESteapObsAction_Back(const SensorInfo *sensor_, PoseStamped targetPose_, DataDown *dataDown_);
    bool StepObsAction(const SensorInfo *sensor_, DataDown *dataDown_);

public:
    bool RecoveryInit(const RecoveryStyle& recoveryStyle_);
    NodeStatus RecoveryForbidenRun(DataDown *dataDown_);
    NodeStatus RecoveryObsRun(const SensorInfo *sensor_, DataDown *dataDown_);
    NodeStatus RecoverySurroundedRun(const SensorInfo *sensor_, DataDown *dataDown_);
    NodeStatus RecoveryPhysicsRun(DataDown *dataDown_);
};

#endif