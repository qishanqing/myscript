//
// Created by lcy on 21-6-29.
//

#include "pid.h"

double PIDCalc( PID *pp, double NextPoint )
{
    double dError, Error;
    Error = pp->SetPoint - NextPoint;           // 偏差
    pp->SumError += Error;                   // 积分
    dError = Error - pp->LastError;             // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    return (pp->Proportion * Error // 比例项
            + pp->Integral * pp->SumError // 积分项
            + pp->Derivative * dError // 微分项
    );
}

void PIDInit (PID *pp)
{
    memset ( pp,0,sizeof(PID));
}