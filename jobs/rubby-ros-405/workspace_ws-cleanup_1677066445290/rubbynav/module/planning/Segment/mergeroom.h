#ifndef MERGEROOM_H_
#define MERGEROOM_H_
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <opencv2/opencv.hpp>
#include<config.h>
#include "MapSegmentationGoal.h"

class MergeRoom
{
public:
void mergeContours(cv::Mat Map,std::vector<cv::Point> contour_a,std::vector<cv::Point> contour_b,std::vector < std::vector<cv::Point> >& room_contours);
void mergeContours(cv::Mat Map,std::vector < std::vector<cv::Point> > contours,std::vector<std::vector<cv::Point>>&room_contours);
void merge_room(cv::Mat&Map,cv::Mat Map1,MapSegmentationGoal &goal,std::vector < std::vector<cv::Point>>&door);
double room_area(cv::Mat Map,std::vector<cv::Point> room,MapSegmentationGoal &goal);
void correct(cv::Mat Map1, std::vector < std::vector<cv::Point>>save_contours, std::vector < std::vector<cv::Point>>&correct_contours);
std::vector<cv::Point>vertical(std::vector<cv::Point> door_contour,cv::Mat threshold1);
std::vector<cv::Point>horizontal(std::vector<cv::Point> door_contour,cv::Mat threshold1);
void expand(cv::Mat map,std::vector<std::vector<cv::Point>>&room_contours,int n);
void manual_segment(cv::Mat Map,std::vector<cv::Point> contour,cv::Point point_a,cv::Point point_b,std::vector<std::vector<cv::Point>>&manual_contours,std::vector<cv::Point>&door_contour);
};


#endif