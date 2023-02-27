//
// Created by lcy on 21-7-1.
//
#include "Control.h"


float Control::DistanceP2P(Point2f p1 , Point2f p2){
    float dlt_x = p1.x - p2.x;
    float dlt_y = p1.y - p2.y;

    return sqrt(dlt_x*dlt_x + dlt_y*dlt_y);
}

float Control::DistanceP2L(Point2f p1 , Point2f p2 , Point2f p){
    float a = p2.y - p1.y;
    float b = p1.x - p2.x;
    float c = p2.x * p1.y - p1.x*p2.y;

    if(a == 0 || b == 0){
        if(a == 0 && b == 0){
            return sqrt((float)(pow((p1.x - p.x),2) + pow((p1.y - p.y),2)));
        }else if(a == 0){
            return fabs(p.y - p1.y);
        }else if(b == 0){
            return fabs(p.x - p1.x);
        }
    }else{
        return fabs(a*p.x + b*p.y + c)/sqrt((float)(pow(a,2) + pow(b,2)));
    }
}

int Control::ProjectiveP2L(Point2f p , Point2f p1 , Point2f p2 , Point2f & target){
    if(p2.x == p1.x){
        target.x = p1.x;
        target.y = p.y;
    }else if(p2.y == p1.y){
        target.x = p.x;
        target.y = p1.y;
    }else{
        double k = (p2.y - p1.y)/(p2.x - p1.x);
        target.x = (float)((k * p1.x + p.x / k + p1.y - p.y) / (1 / k + k));
        target.y = (float)(-1 / k * (target.x - p.x) + p.y);
    }
    return 1;
}

int Control::Clockwise(Point2f p1 , Point2f p2 , Point2f p){
    double a = ((p1.x-p.x)*(p2.y-p.y) - (p1.y-p.y)*(p2.x-p.x));
    //顺时a离为正,逆时针a为负,同一条直线a为0.
    if(a < 0){
        return 1;
    }else if(a > 0){
        return 0;
    }else if(a == 0){
        return 2;
    }
}

float Control::AngularP2P(Point2f p1 , Point2f p2){
    return atan2((p2.y - p1.y) , (p2.x - p1.x));
}

bool Control::MotionLP2P(Point2f p1 , Point2f p2 , PoseStamped cur , double & LinearVelocity , double & AngularVelocity){
    float dp2p = DistanceP2P(p1,p2);
    float LateralYaw = DistanceP2L(p1,p2,Point2f(cur.pose.x,cur.pose.y));
    if(isnan(LateralYaw) || isinf(LateralYaw)){
        LateralYaw = 0.0f;
    }else{
        int dirtmp = Clockwise(p1,p2,Point2f(cur.pose.x,cur.pose.y));
        if(dirtmp == 1){//顺时针距离为正,逆时针距离为负.
        }else if(dirtmp == 0){
            LateralYaw = LateralYaw * (-1);
        }else if(dirtmp == 2){
            LateralYaw = 0.0f;
        }
    }

#if 1
    float al = AngularP2P(p1,p2);
    double abssum = fabs(al) + fabs(cur.euler.yaw);
    int dir = cur.euler.yaw >= 0 ? 1:-1;
    double CourseYaw = ((al > 0 && cur.euler.yaw > 0) || (al < 0 && cur.euler.yaw < 0) || (al == 0 && cur.euler.yaw == 0)) ? (al - cur.euler.yaw):(abssum > Pi_ ? dir*(2*Pi_ - abssum):(-1*dir*abssum));

    if(dp2p < 0.15){
        if(fabs(CourseYaw) > Pi_*5/180){
            AngularVelocity = CourseYaw > 0 ? 0.8f:-0.8f;
            LinearVelocity = 0.0f;
        }else{
            if(fabs(LateralYaw) >= 0.0125){
                sPID.Integral = 0.0f;
                AngularVelocity = -PIDCalc (&sPID , LateralYaw)*sPID.gain;
            }else{
                sPID.Integral = 0.01f;
                AngularVelocity = -PIDCalc (&sPID , CourseYaw)*sPID.gain;
            }
            LinearVelocity = 0.1f;
        }
    }else{
        if(fabs(CourseYaw) > Pi_*5/180){
            AngularVelocity = CourseYaw > 0 ? 0.8f:-0.8f;
            LinearVelocity = 0.0f;
        }else{
            if(fabs(LateralYaw) >= 0.0125){
                sPID.Integral = 0.0f;
                AngularVelocity = -PIDCalc (&sPID , LateralYaw)*sPID.gain;
            }else{
                sPID.Integral = 0.01f;
                AngularVelocity = -PIDCalc (&sPID , CourseYaw)*sPID.gain;
            }
            LinearVelocity = 0.25f;
        }
    }

    if(LinearVelocity < -0.2){
        LinearVelocity = -0.2f;
    }else if(LinearVelocity > 0.35){
        LinearVelocity = 0.35f;
    }
    if(AngularVelocity < -2.0){
        AngularVelocity = -2.0f;
    }else if(AngularVelocity > 2.0){
        AngularVelocity = 2.0f;
    }
#endif
    double dlt = DistanceP2P(p2,Point2f(cur.pose.x,cur.pose.y));
    return dlt <= diatance_arrived*2;
}

bool Control::MotionLP2P_3(Point2f p1 , Point2f p2 , Point2f p3 , PoseStamped cur , float & LinearVelocity , float & AngularVelocity)
{
    float dp2p = DistanceP2P(p1,p2);
    float LateralYaw = DistanceP2L(p1,p2,Point2f(cur.pose.x,cur.pose.y));
    if(isnan(LateralYaw) || isinf(LateralYaw)){
        LateralYaw = 0.0f;
    }else{
        int dirtmp = Clockwise(p1,p2,Point2f(cur.pose.x,cur.pose.y));
        if(dirtmp == 1){//顺时针距离为正,逆时针距离为负.
        }else if(dirtmp == 0){
            LateralYaw = LateralYaw * (-1);
        }else if(dirtmp == 2){
            LateralYaw = 0.0f;
        }
    }
#if 1
    float al = AngularP2P(p1,p2);
    double abssum = (fabs(al) + fabs(cur.euler.yaw));
    int dir = cur.euler.yaw >= 0 ? 1:-1;
    double CourseYaw = ((al > 0 && cur.euler.yaw > 0) || (al < 0 && cur.euler.yaw < 0) || (al == 0 && cur.euler.yaw == 0)) ? (al - cur.euler.yaw):(abssum > Pi_ ? dir*(2*Pi_ - abssum):(-1*dir*abssum));

    if(dp2p < 0.15){
        if(fabs(CourseYaw) > Pi_*5/180){
            AngularVelocity = CourseYaw > 0 ? 0.8f:-0.8f;
            LinearVelocity = 0.0f;
        }else{
            if(fabs(LateralYaw) >= 0.0125){
                sPID.Integral = 0.0f;
                AngularVelocity = (float)(-PIDCalc (&sPID , LateralYaw)*sPID.gain);
            }else{
                sPID.Integral = R_sPID.Integral;
                AngularVelocity = (float)(-PIDCalc (&sPID , CourseYaw)*sPID.gain);
            }
            LinearVelocity = 0.1f;
        }
    }else{
        if(fabs(CourseYaw) > Pi_*10/180){
            AngularVelocity = CourseYaw > 0 ? 0.8f:-0.8f;
            LinearVelocity = 0.0f;
        }else{
            if(fabs(LateralYaw) >= 0.0125){
                sPID.Integral = 0.0f;
                AngularVelocity = (float)(-PIDCalc (&sPID , LateralYaw)*sPID.gain);
            }else{
                sPID.Integral = R_sPID.Integral;
                AngularVelocity = (float)(-PIDCalc (&sPID , CourseYaw)*sPID.gain);
            }
            LinearVelocity = cmd_v;
        }
    }
    if(AngularVelocity > 5){
        AngularVelocity = 5.0f;
    } else if(AngularVelocity < -5){
        AngularVelocity = -5.0f;
    }

#endif
    return DistanceP2L(p2,p3,Point2f(cur.pose.x,cur.pose.y)) <= diatance_arrived;
}

bool Control::MotionFollowobs_r(float ir_r , float & LinearVelocity , float & AngularVelocity){
    irPID.Proportion = 0.5f;
    irPID.SetPoint = 0.2018f;
    AngularVelocity = (float)(PIDCalc (&irPID , ir_r)*80);

    if(AngularVelocity < -2.0){
        AngularVelocity = -2.0f;
    }else if(AngularVelocity > 2.0){
        AngularVelocity = 2.0f;
    }

    LinearVelocity = 0.2f;
    return true;
}

bool Control::MotionFollowobs_l(float ir_l , float & LinearVelocity , float & AngularVelocity){
    irPID.Proportion = 0.5;
    irPID.SetPoint = 0.2018;
    AngularVelocity = (float)(-PIDCalc (&irPID , ir_l)*80);

    if(AngularVelocity < -2.0){
        AngularVelocity = -2.0f;
    }else if(AngularVelocity > 2.0){
        AngularVelocity = 2.0f;
    }

    LinearVelocity = 0.2f;
    return true;
}