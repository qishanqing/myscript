//
// Created by hao on 2022/5/23.
//

#ifndef SAMPLE_BOX_H
#define SAMPLE_BOX_H
#include "src/utils/detector_param.h"

namespace box
{
void NMS(std::vector<BoxInfo>& boxes, float threshold);
};


#endif //SAMPLE_BOX_H
