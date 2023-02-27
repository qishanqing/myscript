
#ifndef ALONGWALL_H
#define ALONGWALL_H

#include <iostream>
#include "nav_log.h"
#include "Common.h"

class IrAlongWall
{
public:
    IrAlongWall();
    ~IrAlongWall() = default;

    bool allongWallInit();
    int allongWallLeft(const SensorInfo * sensor , SpeedControlInfo & cmd);
    int allongWallRight(const SensorInfo * sensor , SpeedControlInfo & cmd);

private:
    bool UpdateVelocity(const SensorInfo * sensor , SpeedControlInfo & cmd);
    void LeftStraightWall(const SensorInfo * sensor , SpeedControlInfo & cmd);
    void RightStraightWall(const SensorInfo * sensor , SpeedControlInfo & cmd);

    double originalLeftWallDis;//贴墙距离
    double originalRightWallDis;
    double objectLeftWallDis;//目标距离
    double objectRightWallDis;
    double originAngle;

    enum Step
    {
        STEP_TEST_MANUAL_START,
        STEP_IDLE = 1,
		
        STEP_LEFTSTRAIGHTWALL,
        STEP_RIGHTSTRAIGHTWALL,
		
        STEP_LEFTARCWALL,
        STEP_RIGHTARCWALL,
		
        STEP_LEFTSTRAIGHT_LEFTPUMBER,
        STEP_LEFTSTRAIGHT_RIGHTPUMBER,
		STEP_LEFTSTRAIGHT_LEFTPUMBER_RE,
        STEP_LEFTSTRAIGHT_RIGHTPUMBER_RE,
		
        STEP_RIGHTSTRAIGHT_LEFTPUMBER,		
		STEP_RIGHTSTRAIGHT_RIGHTPUMBER,
		STEP_RIGHTSTRAIGHT_LEFTPUMBER_RE,
        STEP_RIGHTSTRAIGHT_RIGHTPUMBER_RE,
        STEP_RIGHT_OBJECT_SIZE1,
        STEP_RIGHT_OBJECT_SIZE2,
        STEP_RIGHT_ALONG_OBSTACLE,
        STEP_RIGHT_ROTATUON_ANGLE,

        STEP_RIGHTPUMBER
    } AlongWallStep;

    enum Mode
    {
       MODE_ALONG_WALL,
       MODE_ALONG_OBJECT
    } AlongWallMode;

    void Init();
    double vx, wz;
    float Kp,Ki,Kd;
    float modify;
    bool lastThetaFlag;
    double lastTheta;
    double currentTheta;
    double err;
    double err_last;
    double integral;
    double voltage;
    double targetAngle;
    double minDistance=100.0;
    float  startAngle;
    double rotationAngle;
    double calculationVelocity;
    float lastPositionX;
    float lastPositionY; 
    bool alongObjectFlag;
    double  bumperStartAngle;
    bool  distanceTooClose=false;

	cv::Point2f go_start = cv::Point(0.0f,0.0f);
	
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
};

#endif // ALONGWALL_H
