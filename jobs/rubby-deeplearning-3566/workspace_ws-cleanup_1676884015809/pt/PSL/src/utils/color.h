//////////////////////////////////////////////////////////////////////
///  @file     color.h
///  @brief    generate color
///  Details.
///
///  @author   sunhao
///  @date     2022-10-18 14:07:58
//////////////////////////////////////////////////////////////////////

#ifndef SAMPLE_COLOR_H
#define SAMPLE_COLOR_H

#include <opencv2/core.hpp>

class Color
{
public:
    Color();

    ~Color();

    static cv::Scalar Light(const cv::Scalar &color);

    cv::Scalar GetColor(const int id);

    cv::Scalar operator[](const int id);

    cv::Scalar at(const int id);

public:
    static const cv::Scalar RED;
    static const cv::Scalar BLUE;
    static const cv::Scalar GREEN;
    static const cv::Scalar WHITE;
    static const cv::Scalar COLOR_BACK;
    static const cv::Scalar COLOR_TEXT;
    static const cv::Scalar COLOR_AXIS;
    static const cv::Scalar COLOR_VIEW;
    static const cv::Scalar COLOR_OBJECT;
    static const cv::Scalar COLOR_LAST_OBJECT;
    static const cv::Scalar COLOR_FORGROUND;

private:
    std::vector<cv::Scalar> colors;
};


#endif //SAMPLE_COLOR_H
