//
// Created by lcy on 22-4-29.
//

#ifndef RUBBYDECISION_MAPMANAGE_H
#define RUBBYDECISION_MAPMANAGE_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "Operation.h"

#include "Common.h"

class MotionMapManage : virtual public Operation{
public:
    void fill_sensorobs(cv::Point2f p);
    void fill_bumperobs_init(const SensorInfo * dataup);
    void fill_bumperobs(const SensorInfo * dataup , int flg);
    void fill_followmap(cv::Point p , int max);
    void fill_followmap_reset(cv::Point p , int id , cv::Point min , cv::Point max , int num);
    void fill_followmap_reset(cv::Point p , uint8_t cell , int num);
    void fill_followmap_force(cv::Point p , uint8_t id , int num);
    void fill_cover(cv::Point p , BoxInfo box , int num);
    void fill_cover_followobs(cv::Point p , int num);
    void fill_cover_delimit(cv::Point p , const cv::Mat & mark ,BoxInfo box , int num);

    void followInit(const PoseStamped & p , bool flg);
    int followState(const SensorInfo * info);
private:
    int radius_obs = 3;
    cv::Point2f p_bumper_last = cv::Point2f(0.0,0.0);
    float bumper_l_min = (0.025f)*(0.025f);/*0.0025;//0.05*0.05*/

    PoseStamped clean_startfollowobs = PoseStamped();
    unsigned long long Dir_360 = 0;
};
#endif //RUBBYDECISION_MAPMANAGE_H
