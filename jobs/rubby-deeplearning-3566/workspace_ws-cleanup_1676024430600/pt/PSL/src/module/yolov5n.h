#ifndef __YOLOV5N_H__
#define __YOLOV5N_H__

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <memory>
#include "src/module/detector.h"
#include "src/module/box.h"

class Yolov5n : public inner::Detector
{
public:
    Yolov5n();

    ~Yolov5n();

private:
    bool Preprocess(void **data) override;

    void Tensor2Box(void *predictions) override;

    int PostprocessV5s(OUTPUT_TYPE *input0, OUTPUT_TYPE *input1, OUTPUT_TYPE *input2
                      , const int model_in_h, const int model_in_w, const float conf_threshold
                      , const float nms_threshold, const float scale_w, const float scale_h);

    int PostprocessV5n(OUTPUT_TYPE *predictions, const float scale_w, const float scale_h);

    int process(OUTPUT_TYPE *input, int *anchor, int grid_h, int grid_w, int height, int width
                , int stride
                , std::vector<float> &boxes, std::vector<float> &objProbs, std::vector<int> &classId
                , float threshold);

private:
    std::vector<float> mVariance;

    int mTopK;
    float mScoreThreshold;
    float mNMSThreshold;
    int classNum;  // include background
};

#endif
