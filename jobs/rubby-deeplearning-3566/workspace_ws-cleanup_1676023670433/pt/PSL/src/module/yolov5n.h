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

    void Tensor2Box(float *predictions) override;

private:
    std::vector<float> mVariance;

    int mTopK;
    float mScoreThreshold;
    float mNMSThreshold;
    int classNum;  // include background
};

#endif
