//////////////////////////////////////////////////////////////////////
///  @file     yolov3_tiny.h
///  @brief    Yolov3Tiny tiny object detect
///  Details.
///  @author   sunhao 
///  @date     2021.09.15
//////////////////////////////////////////////////////////////////////
#ifndef DETECT_DEMO_YOLOV3_TINY_H
#define DETECT_DEMO_YOLOV3_TINY_H

#include "src/module/detector.h"
#include "src/module/box.h"


class Yolov3Tiny : public inner::Detector
{
public:
    Yolov3Tiny();

    ~Yolov3Tiny();

private:
    bool Preprocess(void **data) override;

    void Tensor2Box(void *predictions) override;
};

#endif //DETECT_DEMO_YOLOV3_TINY_H
