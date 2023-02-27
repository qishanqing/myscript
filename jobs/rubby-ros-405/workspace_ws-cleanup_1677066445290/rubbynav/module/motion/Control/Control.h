//
// Created by lcy on 21-7-1.
//

#ifndef MY_DEMO_MOTIONPLANNING_H
#define MY_DEMO_MOTIONPLANNING_H

#include <iostream>
#include <opencv2/opencv.hpp>

#include "Common.h"
#include "pid.h"
#include "config.h"

using namespace cv;
using namespace std;

class Control{
public:
    Control(){
        sPID.Proportion = 0.5f;
        sPID.Integral = 0.0f;
        sPID.Derivative = 0.0f;
        sPID.SetPoint = 0.0f; // Set PID Setpoint
        sPID.gain = 10.0f;

        irPID.Proportion = 0.5f;
        irPID.Integral = 0.0f;
        irPID.Derivative = 0.0f;
        irPID.SetPoint = 0.0f; // Set PID Setpoint
        irPID.gain = 10.0f;
    };
    ~Control(){};

    void ControlInit(){
#ifdef NEWCONTROL
        // R_sPID.Proportion = s_config.readParam<float>("NavPID_P");
        // R_sPID.Integral = s_config.readParam<float>("NavPID_I");
        // R_sPID.Derivative = s_config.readParam<float>("NavPID_D");
        // R_sPID.SetPoint = s_config.readParam<float>("NavPID_S");
        // R_sPID.gain = s_config.readParam<float>("NavPID_G");
        // sPID = R_sPID;

        // R_irPID.Proportion = s_config.readParam<float>("WallPID_P");
        // R_irPID.Integral = s_config.readParam<float>("WallPID_I");
        // R_irPID.Derivative = s_config.readParam<float>("WallPID_D");
        // R_irPID.SetPoint = s_config.readParam<float>("WallPID_S");
        // R_irPID.gain = s_config.readParam<float>("WallPID_G");
        // irPID = R_irPID;

        // cmd_v = s_config.readParam<float>("Cmd_v");
#else
        R_sPID.Proportion = s_config.readParam<float>("NavPID_P");
        R_sPID.Integral = s_config.readParam<float>("NavPID_I");
        R_sPID.Derivative = s_config.readParam<float>("NavPID_D");
        R_sPID.SetPoint = s_config.readParam<float>("NavPID_S");
        R_sPID.gain = s_config.readParam<float>("NavPID_G");
        sPID = R_sPID;

        R_irPID.Proportion = s_config.readParam<float>("WallPID_P");
        R_irPID.Integral = s_config.readParam<float>("WallPID_I");
        R_irPID.Derivative = s_config.readParam<float>("WallPID_D");
        R_irPID.SetPoint = s_config.readParam<float>("WallPID_S");
        R_irPID.gain = s_config.readParam<float>("WallPID_G");
        irPID = R_irPID;

        cmd_v = s_config.readParam<float>("Cmd_v");
#endif
    }
    float DistanceP2P(Point2f p1 , Point2f p2);
    float DistanceP2L(Point2f p1 , Point2f p2 , Point2f p);
    int ProjectiveP2L(Point2f p , Point2f p1 , Point2f p2 , Point2f & target);

    int Clockwise(Point2f p1 , Point2f p2 , Point2f p);
    float AngularP2P(Point2f p1 , Point2f p2);

    bool MotionLP2P(Point2f p1 , Point2f p2 , PoseStamped cur , double & LinearVelocity , double & AngularVelocity);
    bool MotionLP2P_3(Point2f p1 , Point2f p2 , Point2f p3 , PoseStamped cur , float & LinearVelocity , float & AngularVelocity);

    bool MotionFollowobs_r(float ir_r , float & LinearVelocity , float & AngularVelocity);
    bool MotionFollowobs_l(float ir_l , float & LinearVelocity , float & AngularVelocity);
private:
    PID R_sPID;
    PID sPID; // PID Control Structure　导航,弓字
    PID R_irPID;
    PID irPID; // PID Control Structure　红外沿墙
    float cmd_v = 0.0f;
    float cmd_a = 0.0f;
    double diatance_arrived = 0.025;
};

#endif //MY_DEMO_MOTIONPLANNING_H
