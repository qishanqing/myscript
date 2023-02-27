#ifndef SEGMENT_H_
#define SEGMENT_H_
#include <fstream>
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <ipa_room_segmentation/distance_segmentation.h>
#include <ipa_room_segmentation/morphological_segmentation.h>
#include <ipa_room_segmentation/voronoi_segmentation.h>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include<config.h>
#include "MapSegmentationGoal.h"
#include "ipa_room_segmentation/meanshift2d.h"
#include "mergeroom.h"
//struct SegmentInfo
//{
//    cv::Mat SegmentMap;  // 输出的分区图
//    std::vector< std::vector<cv::Point> > border;  // 轮廓列表
//    std::vector< std::vector<cv::Point> > door;//门列表
//    int SegNumber;  // 分区数量
//};
//struct SegmentOperationInfo{//分区操作
//    int flag;
//    std::vector<int> merge;//合并
//    std::pair<int , std::pair<cv::Point , cv::Point>> split;//拆分
//};
class RoomSegment
{
private:
//    config robot_config;
    MapSegmentationGoal goal;
    MergeRoom mergeroom;
public:
    cv::Mat processed_map;
    void SegmentInit(const cv::Mat& Map , const char * path);
    void SegmentRun(const cv::Mat& Map, SegmentInfo& info_);
    bool SegmentOperation(const SegmentOperationInfo & operationInfo , SegmentInfo& info_);
    bool LoadInfo(const char * path , SegmentInfo& info_);
    bool SaveInfo(const char * path , const SegmentInfo& info_);
};


#endif