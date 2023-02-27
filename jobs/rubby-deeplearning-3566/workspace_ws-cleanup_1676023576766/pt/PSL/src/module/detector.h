//
// Created by hao on 2021/6/3.
//

#ifndef DETECT_DEMO_DETECTOR_H
#define DETECT_DEMO_DETECTOR_H

#include <opencv2/highgui.hpp>
#include "src/module/image_type.h"
#include "src/utils/detector_param.h"
#include "src/data/observer_data.h"
#include "src/data/dataset.h"

class Model;

namespace inner
{
class Detector : public ObserverData
{
public:
    Detector();

    virtual ~Detector();

    bool Init(const DetectorOptions &options);

    bool Detect(const cv::Mat &image, std::vector<BoxInfo> &boxes); // for sync

    bool Detect(std::vector<BoxInfo> &boxes, DataGroup& item); // for async

    void Update(const DataGroup &item); // set data for async

protected:
    virtual bool Preprocess(void **data) = 0;

    virtual void Tensor2Box(float *predictions) = 0;

    bool Postprocess(float *predictions);

    const ImageHead &GetInputSize() const;

    bool Valid(const int id);

private:
    bool Detect();

protected:
    DataGroup item;
    DataGroup itemLastest;
    std::vector<BoxInfo> boxes;
    DetectorOptions option;
    int mNumOfClasses;
    std::vector<unsigned long> outputSize;

private:
    Model *model;
    bool running;
    std::mutex itemLock;
};
}

#endif //DETECT_DEMO_DETECTOR_H
