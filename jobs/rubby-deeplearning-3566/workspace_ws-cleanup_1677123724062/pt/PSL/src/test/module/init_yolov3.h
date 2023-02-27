//
// Created by indemind on 11/22/21.
//

#ifndef DETECTOR_SAMPLE_INIT_YOLOV3_H
#define DETECTOR_SAMPLE_INIT_YOLOV3_H

#include "src/utils/utils.h"
#include <time.h>
#include "src/module/yolov3.h"
#include "src/test/types.h"

extern InputParam inputParam;
using ModelType = Yolov3;

std::unique_ptr<ModelType> InitYoloV3();

#endif //DETECTOR_SAMPLE_INIT_YOLOV3_H
