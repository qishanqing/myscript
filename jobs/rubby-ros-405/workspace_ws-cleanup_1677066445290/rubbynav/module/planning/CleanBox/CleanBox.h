//
// Created by lcy on 21-6-22.
//

#ifndef ROBOT_MATLAB_CLEANBOX_H
#define ROBOT_MATLAB_CLEANBOX_H

#include <opencv2/opencv.hpp>
#include <iostream>

#include "Common.h"
#include  "config.h"

using namespace cv;
using namespace std;

class CleanBox{
public:
    CleanBox()
    {
        boxflg = s_config.readParam<int>("boxNumber");
        sidelength = (boxflg == 1) ? (s_config.readParam<int>("sideLength1")) : (s_config.readParam<int>("sideLength0"));
    }
    ~CleanBox(){
        boxarea.release();
        boxmap.release();
        boxarray.clear();
        std::vector<BoxInfo>().swap(boxarray);
        CleanBox_history.clear();
        std::vector<BoxInfo>().swap(CleanBox_history);
    }
    bool CleanBoxInit(Mat & map , Point start);
    bool GetPointInBox(Point p , BoxInfo & box);
    int GetPointInBoxId(Point p);
    BoxInfo GetCurBox();
    void SetCurBox(const BoxInfo &box);
    void SetCurBox_loadmap(const BoxInfo &box);

    bool GetPointInDelimit(Point p , BoxInfo & box);
    BoxInfo GetCurDelimit();

    void datareset(){
        boxarea.release();
        boxmap.release();
        boxarray.clear();
        std::vector<BoxInfo>().swap(boxarray);
        CleanBox_history.clear();
        std::vector<BoxInfo>().swap(CleanBox_history);
    }
    BoxInfo curbox;
    std::vector<BoxInfo> CleanBox_history;/*历史清扫框信息*/
    Mat boxarea;
    Mat boxmap;
    std::vector<BoxInfo> boxarray;
    int sidelength = 0;
    int boxflg = 0;
private:

};

#endif //ROBOT_MATLAB_CLEANBOX_H
