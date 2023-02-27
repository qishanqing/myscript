#ifndef __SSD_H__
#define __SSD_H__

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <memory>
#include "src/module/detector.h"
#include "src/module/box.h"

class SSD : public inner::Detector
{
public:
    SSD();

    ~SSD();

private:
    bool Preprocess(void **data) override;

    void Tensor2Box(void *predictions) override;

private:
    std::vector<float> mVariance;

    int mTopK;
    float mScoreThreshold;
    float mNMSThreshold;
    int classNum;  // include background
};

#endif
