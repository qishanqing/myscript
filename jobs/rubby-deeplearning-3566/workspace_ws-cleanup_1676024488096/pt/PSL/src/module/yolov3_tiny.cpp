//
// Created by hao on 2021/6/9.
//

#include "yolov3_tiny.h"
#include "utils/timer.h"
#include "src/utils/define.h"
#include "src/utils/detector_property.h"
#include "src/utils/utils.h"

const int MAX_DETECT_NUM = 100;
const float THRESHOLD = 0.5;

Yolov3Tiny::Yolov3Tiny()
{

}

Yolov3Tiny::~Yolov3Tiny()
{

}

bool Yolov3Tiny::Preprocess(void **data)
{
    Timer timer;
    cv::Mat dst;

    const ImageHead &inputSize = GetInputSize();
    cv::resize(this->item.image.left, dst, cv::Size(inputSize.width, inputSize.height));

    // TODO : move to model/preprocess
#ifdef USE_NPU_AML
    using TYPE = uint8_t;
    TYPE *src = (TYPE *) dst.data;
    TYPE *ptr = (TYPE *) malloc(inputSize.size * sizeof(TYPE));

    int offset, i, j;
    // TODO : check for PIX_FMT_NV21 and PIX_FMT_RGB888, (this is PIX_FMT_RGB888)
    for (i = 0; i < inputSize.channel; i++)
    {
        offset = inputSize.width * inputSize.height *
                 (inputSize.channel - 1 - i);  // prapare BGR input data
        for (j = 0; j < inputSize.width * inputSize.height; j++)
        {
            int tmpdata = (src[j * inputSize.channel + i] >> 1);
            ptr[j + offset] = (TYPE) ((tmpdata > 127) ? 127 :
                                         (tmpdata < -128) ? -128 : tmpdata);
        }
    }
#elif USE_NPU_RK
    using TYPE = uint8_t;
    TYPE* ptr;
#elif USE_GPU
    using TYPE = float;
    cv::cvtColor(dst, dst, cv::COLOR_BGR2RGB);
    TYPE *ptr = (TYPE *) malloc(inputSize.size * sizeof(TYPE));;
    memcpy(ptr, dst.data, inputSize.size);
#endif

    *data = (void *) ptr;

    return true;
}


void Yolov3Tiny::Tensor2Box(void *predictions)
{
}
