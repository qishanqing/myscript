//
// Created by indemind on 2/21/22.
//

#ifndef DETECTOR_SAMPLE_REGION_ADD_CONVEX_H
#define DETECTOR_SAMPLE_REGION_ADD_CONVEX_H

#include <opencv2/opencv.hpp>
#include "src/utils/detector_param.h"

class WeightEscalatorMap
{
public:
    WeightEscalatorMap()
    {

    }

    void Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view);

    void GetImage(cv::Mat &image) const;

private:
    cv::Mat image;
};

#endif //DETECTOR_SAMPLE_REGION_ADD_CONVEX_H
