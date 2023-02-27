//
// Created by hao on 2021/6/9.
//

#ifndef DETECT_DEMO_YOLOV3_H
#define DETECT_DEMO_YOLOV3_H

#include "src/module/detector.h"
#include "src/module/box.h"


class Yolov3 : public inner::Detector
{
public:
    Yolov3();

    ~Yolov3();

private:
    bool Preprocess(void **data) override;

    void Tensor2Box(void *predictions) override;

    void GetDetectionsResult(int num, float thresh, Box *boxes
                             , float **probs, int classes);

    void GetDetectionsResult(float thresh, const float *predictions, int classesNum);

    void NMSOnlyOne(std::vector<BoxInfo>& boxes);

    void ReversePaddingResult(Box &box, const cv::Mat &src, const cv::Size dstSize);

private:
    ErrorMessage *errorMessage;
    friend void Tensor2BoxTest(float *predictions, std::unique_ptr<Yolov3>  yolov3);

};

#endif //DETECT_DEMO_YOLOV3_H
