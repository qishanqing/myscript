#ifndef DOCK_H
#define DOCK_H

#include <iostream>
#include "Common.h"

class Dock
{
private:
    enum Step
    {
        STEP_TEST_MANUAL_START,
        STEP_IDLE = 1,
        STEP_SCAN,           // 2
        STEP_AHEAD_MID,      // 3
        STEP_MOVE_TO_MID,    // 4
        STEP_TURN,           // 5
        STEP_ALIGNED,        // 6
        STEP_NONE_SIGNAL,    // 7
        STEP_FINDSIGNAL,     // 8
        STEP_TRIGGER_BUMPER, // 9
        STEP_ERROR,          // 10
        STEP_INFRARED_GUIDANCE
    } DockStep = STEP_TEST_MANUAL_START;

    enum CollisionType
    {
        COLLISIONTYPE_IDLE,
        COLLISIONTYPE_FIND_SIGNAL,
        COLLISIONTYPE_MOVE_TO_MID,
        COLLISIONTYPE_ALIGNED,
        COLLISIONTYPE_MOVE_TO_MID_NOSIGNAL
    } BumperType;
 
    PoseStamped rPose;
    PoseStamped GetRobotPose()
    {
        return rPose;
    }

    bool goMidLine = false;
    double vx = 0.0f, wz = 0.0f;
    float Kp = 0.0f;
    float modify = 0.0f;
    bool lastThetaFlag = false;
    double lastTheta = 0.0f;
    double currentTheta = 0.0f;
    double err = 0.0f;
    double targetAngle = 0.0f;
    double currentPositionX = 0.0f;
    double currentPositionY = 0.0f;
    double moveMidPositionX = 0.0f;
    double moveMidPositionY = 0.0f;
    double signalMidPositionX = 0.0f;
    double signalMidPositionY = 0.0f;
    int dockDetector = 0;
    bool gotSignal = false;
    int dockStep_bk;
    bool reversalSignal = false;
    bool infraredGuidanceMode = false;
    inline void SetVel(const double &v, const double &w)
    {
        vx = v;
        wz = w;
    }
    int TurnDirection(double now, double des);
    inline double AngleModify(double x)
    {
        if (x > M_PI)
        {
            x = x - 2 * M_PI;
        }
        else if (x < -M_PI)
        {
            x = x + 2 * M_PI;
        }
        return x;
    }

    void Scan(const SensorInfo *sensor);
    void AheadMid(const SensorInfo *sensor);
    void Move2Mid(const SensorInfo *sensor);
    void Turn(const SensorInfo *sensor);
    void Aligned(const SensorInfo *sensor);
    void FindSignal(int number, const SensorInfo *sensor);
    bool CollisionProcessing(const SensorInfo *sensor);
    void InfraredGuidance(const SensorInfo *sensor);
    
public:
    Dock() = default;
    ~Dock() = default;
    std::uint8_t number = 0;
    std::uint8_t alignedTestNum = 0;
    double beginTheta = 0.0f;
    bool ariseSignal = false;
    std::uint8_t reverseSignal = 0;
    double rotateAngle = 0.0;
    void DockInit();
    int DockRun(const SensorInfo *sensor, float &v, float &w);
};

#endif // DOCK_H
