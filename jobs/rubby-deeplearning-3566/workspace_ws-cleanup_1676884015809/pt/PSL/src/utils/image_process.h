//////////////////////////////////////////////////////////////////////
///  @file     image_process.h
///  @brief    some image process method
///  @author   sunhao
///  @date     2021.11.21
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_IMAGE_PROCESS_H
#define DETECTOR_SAMPLE_IMAGE_PROCESS_H

#include <opencv2/opencv.hpp>
#include <iostream>

class ImageProcess
{
public:
    using PointType = cv::Point2f;

    static void DrawDottedLine1(cv::Mat img, PointType p1, PointType p2
                                , cv::Scalar color, int thickness);

    static void DrawDottedLine2(cv::Mat img, PointType p1, PointType p2
                                , cv::Scalar color, int thickness);

    static void
    DrawDottedRect(cv::Mat img, cv::Rect box, cv::Scalar color, int thickness);

    static void Correct(PointType &p, const PointType &p1, const PointType &p2);

    static void MouseCallback(int event, int x, int y, int flags, void *param);

    static bool ResizeByPadding(const cv::Mat src, cv::Mat &dst, const cv::Size size);

    static bool LetterBox(const cv::Mat src, cv::Mat &dst, const cv::Size size);

    static bool GetRatio(const cv::Mat &src, const cv::Size size
                         , float &ratioWidth, float &ratioHeight);
};


#endif //DETECTOR_SAMPLE_IMAGE_PROCESS_H
