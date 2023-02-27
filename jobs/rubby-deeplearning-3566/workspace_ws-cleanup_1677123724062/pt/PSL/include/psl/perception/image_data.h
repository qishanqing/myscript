//
// Created by donglijian on 8/1/22.
//

#ifndef PERCEPTION_SAMPLE_IMAGE_DATA_H
#define PERCEPTION_SAMPLE_IMAGE_DATA_H

#include "psl/base/Time.h"
#include <opencv2/opencv.hpp>

namespace psl
{
struct ImageData
{
    psl::Time time;
    cv::Mat left;
    cv::Mat right;

    ImageData(psl::Time time, cv::Mat left, cv::Mat right)
    {
        this->time = time;
        this->left = left;
        this->right = right;
    }
};
}
#endif //PERCEPTION_SAMPLE_IMAGE_DATA_H
