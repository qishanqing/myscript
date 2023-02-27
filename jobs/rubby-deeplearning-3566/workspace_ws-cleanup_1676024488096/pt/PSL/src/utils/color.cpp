//
// Created by hao on 2022/10/18.
//

#include "color.h"
#include "src/utils/define.h"

const cv::Scalar Color::RED{0, 0, 255};
const cv::Scalar Color::BLUE{255, 0, 0};
const cv::Scalar Color::GREEN{0, 255, 0};
const cv::Scalar Color::WHITE{255, 255, 255};
const cv::Scalar Color::COLOR_BACK(80, 80, 80);
const cv::Scalar Color::COLOR_TEXT(150, 150, 150);
const cv::Scalar Color::COLOR_AXIS(0, 0, 200);
const cv::Scalar Color::COLOR_VIEW(255, 50, 50);
const cv::Scalar Color::COLOR_OBJECT(50, 150, 50);
const cv::Scalar Color::COLOR_LAST_OBJECT(0, 250, 250);
const cv::Scalar Color::COLOR_FORGROUND(0, 140, 255);

Color::Color()
{

}

Color::~Color()
{

}

cv::Scalar Color::GetColor(const int id)
{
    if (id < 0) return WHITE;

    while (id >= colors.size())
    {
        colors.push_back(cv::Scalar(RAND_INT(0, 255), RAND_INT(0, 255)
                                          , RAND_INT(0, 255)));
    }

    return colors.at(id);
}

cv::Scalar Color::operator[](const int id)
{
    return GetColor(id);
}

cv::Scalar Color::at(const int id)
{
    return GetColor(id);
}

cv::Scalar Color::Light(const cv::Scalar &color)
{
    return cv::Scalar(MIN(255, color[0] + 50)
               , MIN(255, color[1] + 50)
               , MIN(255, color[2] + 50));
}
