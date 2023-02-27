//
// Created by lcy on 22-5-6.
//

#include "Operation.h"

double Operation::Dis_p2p(cv::Point2f p1 , cv::Point2f p2){
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}
double Operation::Dis_p2l(){
    return 0.0;
}

double Operation::SquareDis_p2p(cv::Point2f p1 , cv::Point2f p2){
    return (pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}
double Operation::SquareDis_p2l(){
    return 0.0;
}

bool Operation::ProjectiveP2L(cv::Point2f p , cv::Point2f p1 , cv::Point2f p2 , cv::Point2f & target){
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
    return true;
}
double Operation::ternaryoperator(bool a , double b , double c){
    double d = a ? b:c;
    return d;
}
double Operation::Dir_Offset(PoseStamped p1 , PoseStamped p2){
    double a_p2p = atan2((double)(p2.pose.y - p1.pose.y) , (double)(p2.pose.x - p1.pose.x));
    double abssum = (fabs(a_p2p) + fabs(p1.euler.yaw));
    int dir = p1.euler.yaw >= 0 ? 1:-1;
    double CourseYaw = ((a_p2p > 0 && p1.euler.yaw > 0) || (a_p2p < 0 && p1.euler.yaw < 0) || (a_p2p == 0 && p1.euler.yaw == 0)) ? (a_p2p - p1.euler.yaw):(abssum > Pi_ ? dir*(2*Pi_ - abssum):(-1*dir*abssum));
    return CourseYaw;
}