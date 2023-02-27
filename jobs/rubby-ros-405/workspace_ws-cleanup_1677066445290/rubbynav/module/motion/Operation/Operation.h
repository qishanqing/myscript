//
// Created by lcy on 22-5-6.
//

#ifndef RUBBYDECISION_OPERATION_H
#define RUBBYDECISION_OPERATION_H

#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <Common.h>

class Operation{
public:
    double Dis_p2p(cv::Point2f p1 , cv::Point2f p2);
    double Dis_p2l();

    double SquareDis_p2p(cv::Point2f p1 , cv::Point2f p2);
    double SquareDis_p2l();

    bool ProjectiveP2L(cv::Point2f p , cv::Point2f p1 , cv::Point2f p2 , cv::Point2f & target);
    double ternaryoperator(bool a , double b , double c);

    double Dir_Offset(PoseStamped p1 , PoseStamped p2);
private:

};

#endif //RUBBYDECISION_OPERATION_H
