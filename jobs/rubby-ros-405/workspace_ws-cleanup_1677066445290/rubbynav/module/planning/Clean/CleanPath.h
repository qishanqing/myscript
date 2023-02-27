//
// Created by lcy on 21-6-24.
//

#ifndef ROBOT_MATLAB_CLEANPATH_H
#define ROBOT_MATLAB_CLEANPATH_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "Common.h"

using namespace cv;

class CleanPath{
public:
    CleanPath() = default;
    ~CleanPath() = default;

    int dir_clean_lock = 0;//弓字清扫方向 1:锁Y轴+ 2:锁Y轴-
    int dir_clean_path_first = 0;//第一条清扫路径的方向 1:X+方向  2:X-方向

    int find_corner_point(const Mat & src , Point input , Point p_min , Point p_max);
    int find_corner_point_mark(const Mat & src , const Mat & mark , Point input , int id);
    int find_Endpoint(const Mat & src , Point input , Point p_min , Point p_max , Point & output);
    int find_Endpoint_mark(const Mat & src , const Mat & mark , int id , Point input , Point & output);

    bool clean_up(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e);
    bool clean_up_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e);
    bool clean_down(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e);
    bool clean_down_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e);
    bool clean_left(BoxInfo box , Point2f start , Point2f & p_s , Point2f & p_e , int dir);
    bool clean_leftAcute(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir);
    bool clean_leftAcute_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir);
    bool clean_leftObtuse(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir);
    bool clean_leftObtuse_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir);
    bool clean_left_mark(BoxInfo box , Point2f start , Point2f & p_s , Point2f & p_e , int dir);
    bool clean_right(BoxInfo box , Point2f start , Point2f & p_s , Point2f & p_e , int dir);
    bool clean_rightAcute(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir);
    bool clean_rightAcute_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir);
    bool clean_rightObtuse(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir);
    bool clean_rightObtuse_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir);
    bool clean_right_mark(BoxInfo box , Point2f start , Point2f & p_s , Point2f & p_e , int dir);

private:
/*******************划区清扫,方向指定*************************/
#define SPECIFIED_UNLOCK_DIE 0//不锁定路径方向
#define SPECIFIED_LOCK_DIE_X 1//锁定弓字长边与X轴平行
#define SPECIFIED_LOCK_DIE_Y 2//锁定弓字长边与Y轴平行
/**************************************************************/
#define UNKNOW_AREA SearchUnknowCell
#define OBS_AREA SearchObsCell
#define FREE_AREA SearchFreeCell

    int Dir_Cleanning_Path = SPECIFIED_LOCK_DIE_X;//清扫路径的方向

    bool clean_isfit(Point p);
    bool clean_isfit_NoObs(Point p);

    int find_corner_point_0(const Mat & src , Point input , Point p_min , Point p_max , Point & output);
    int ProjectiveP2L(Point2f p , Point2f p1 , Point2f p2 , Point2f & target);
};

#endif //ROBOT_MATLAB_CLEANPATH_H
