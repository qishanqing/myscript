//
// Created by lcy on 21-6-22.
//

#ifndef ROBOT_MATLAB_SEARCH_H
#define ROBOT_MATLAB_SEARCH_H

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <chrono>

#include "CleanBox.h"
#include "Common.h"

//#define HUNT_POINT_TEST //打开该行,可查看搜索范围
#ifdef DEBUG_PATH_PLANNING
#define HUNT_POINT_TIME  //打开该行,可查看搜索耗时
#endif
using namespace std;
using namespace cv;

class Search{

public:
    Search() = default;;
    ~Search() = default;

    int Search_point_set(const Mat & maze , Point startPoint , uint8_t type, Point &target_point);
    int Search_point_out_set(const Mat & maze , Point startPoint , uint8_t type, Point &target_point);
    int Search_p2p(const Mat & maze , Point startPoint , Point target_point);
    /**
     * 统计图片中一定范围内和指定点具有相同像素值的个数和点
     * @param src 地图数据
     * @param target_point 目标点
     * @param p_min 角点(小端)
     * @param p_max 角点(大端)
     * @param num 数量
     * @param list_out 目标点
     * @return 1 失败 0 完成
     */
    int count_set_type(const Mat & src , Point target_point , Point p_min , Point p_max , int & num ,list<Point> & list_out);//统计图片中指定像素的个数.
    int count_set_type(const Mat & src , Point target_point , Point p_min , Point p_max , list<Point> & list_out);//统计图片中指定点所在的区域大小.

    /**
     * 探索目标点
     * @param maze 地图数据
     * @param startPoint 起始点
     * @param target_point 目标点
     * @return 0 未找到 1 找到
     */
    int Search_point_explore(const Mat & maze , Point startPoint , Point &target_point);

    /**
     * 按清扫框搜索
     * @param maze 地图数据
     * @param startPoint 起始点
     * @param target_point 目标点
     * @param maprange_min 角点(小端)
     * @param maprange_max 角点(大端)
     * @return 0 未找到  1 找到内部点 2 找到外部点
     */
    int Search_box_point(const Mat & maze , CleanBox * boxinfo , Point startPoint , Point &target_point , vector<Point> & vpignore);
    int Search_box_point_remainder(const Mat & maze , CleanBox * boxinfo , Point startPoint , Point &target_point , vector<Point> & vpignore);
    int Search_mark_point(const Mat & maze , const Mat & dividemap , CleanBox * boxinfo , Point startPoint , Point &target_point , vector<Point> & vpignore);

    bool Search_obs_point(const Mat & maze , Point range_min , Point range_max , Point startPoint , Point &target_point , Mat & map);
    bool Search_obs_point_marking(const Mat & maze , const Mat & dividemap , int id , Point range_min , Point range_max , Point startPoint , Point &target_point , Mat & map);
private:
    int num_hunt_local_min = 14;//搜索到的区域内目标点周围必须有大于这个数量的相同目标点
    int num_hunt_global_min = 28 * 1;//搜索到的区域外目标点周围必须有大于这个数量的相同目标点

    int count_set_type_0(const Mat & src , const Mat & mark , Point target_point , int & num ,list<Point> & list_out);//统计图片中指定像素的个数.
    int count_set_type_1(const Mat & src , const Mat & mark , Point target_point , int & num ,list<Point> & list_out);//统计图片中指定像素的个数.
    int count_set_type_2(const Mat & src , Point target_point , uint8_t type , Point p_min , Point p_max , list<Point> & list_out);//统计图片中与起点所在的区域相邻的指定像素

};

#endif //ROBOT_MATLAB_SEARCH_H
