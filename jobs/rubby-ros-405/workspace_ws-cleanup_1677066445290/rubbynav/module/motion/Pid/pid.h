//
// Created by lcy on 21-6-29.
//

#ifndef MY_DEMO_PID_H
#define MY_DEMO_PID_H

#include <iostream>
#include <string.h>             //C语言中memset函数头文件

typedef struct PID {
    double SetPoint;      // 设定目标Desired value
    double Proportion;    // 比例常数Proportional Const
    double Integral;      // 积分常数Integral Const
    double Derivative;    // 微分常数Derivative Const
    double LastError;     // Error[-1]
    double PrevError;    // Error[-2]
    double SumError;    // Sums of Errors
    double gain;        //增益
}PID;
double PIDCalc( PID *pp, double NextPoint );
void PIDInit (PID *pp);

#endif
