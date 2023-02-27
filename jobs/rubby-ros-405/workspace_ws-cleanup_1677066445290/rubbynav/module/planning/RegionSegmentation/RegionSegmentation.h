//
// Created by lcy on 21-5-7.
//

#ifndef MYROBOT_RegionSegmentation_H
#define MYROBOT_RegionSegmentation_H

#include <opencv2/highgui/highgui.hpp>
#include "iostream"
#include "Common.h"
#include <list>

using namespace std;
using namespace cv;

class  RegionSegmentation{
public:
    RegionSegmentation() = default;
    ~RegionSegmentation() = default;

    int get_Partition_num(){ return num_partition;}
    int MarkingPartition(const Mat & image , Mat & dst);

private:
    int width = 5;
    int num_partition = 0;

    int count_set_type_0(const Mat & src , Point target_point , Point p_min , Point p_max , list<Point> & list_out);
    int Erode_map(const Mat & src , Mat & dst, int g_nStructRlementSize);
    int Dilate_map(const Mat & src , Mat & dst , int g_nStructRlementSize);

};

#endif //MYROBOT_RegionSegmentation_H
