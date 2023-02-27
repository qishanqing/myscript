#include "yolov5n.h"
#include "src/utils/timer.h"
#include "src/utils/image_process.h"
#include "src/utils/utils.h"
#ifdef USE_NPU
#include "MNN/Backend.hpp"
#elif USE_GPU
#include "MNN/Interpreter.hpp"
#include "MNN/MNNDefine.h"
#include "MNN/Tensor.hpp"
#endif

//#define LETTER_BOX

const int MAX_DETECT_NUM = 100;
const float THRESHOLD_CLASS[] = {0.3, 0.3, 0.3, 0.42, 0.4, 0.5, 0.5};
const float THRESHOLD = 0.25;
const float THRESHOLD_NMS = 0.5;

Yolov5n::Yolov5n()
{
    mVariance = {0.1, 0.1, 0.2, 0.2};
    mScoreThreshold = THRESHOLD;
    mNMSThreshold = THRESHOLD_NMS;
    mTopK = MAX_DETECT_NUM;
    classNum = 5; // TODO : should read from config or class name file;
}

Yolov5n::~Yolov5n()
{
}

bool Yolov5n::Preprocess(void **data)
{
    Timer timer;
    cv::Mat dst;

    const ImageHead &inputSize = GetInputSize();
    if (option.param.padding)
    {
        // TODO : when false (others)
#ifdef LETTER_BOX
        ImageProcess::LetterBox(this->item.image.data, dst, cv::Size(inputSize.width
                                                                           , inputSize.height));
#else
        ImageProcess::ResizeByPadding(this->item.image.data, dst, cv::Size(inputSize.width
                                                                           , inputSize.height));
#endif
    }
    else
    {
        cv::resize(this->item.image.data, dst, cv::Size(inputSize.width
                                                        , inputSize.height));
    }

    LOG_CHECK_TIME(INFO) << timer.TimingStr("resize");

    // TODO : move to model/preprocess
#ifdef USE_NPU
    using TYPE = uint8_t;
    TYPE *src = (TYPE *) dst.data;
    TYPE *ptr = (TYPE *) malloc(inputSize.size * sizeof(TYPE));

    int offset, i, j;
    // TODO : check for PIX_FMT_NV21 and PIX_FMT_RGB888, (this is PIX_FMT_RGB888)
    size_t size = inputSize.width * inputSize.height;

//    #pragma omp parallel for
    for (i = 0; i < inputSize.channel; i++)
    {
        offset = size * (inputSize.channel - 1 - i);  // prapare BGR input data
        for (j = 0; j < size; j++)
        {
            int tmpdata = (src[j * inputSize.channel + i] >> 1);
            ptr[j + offset] = (TYPE) (tmpdata);
        }
    }
    LOG_CHECK_TIME(INFO) << timer.TimingStr("normalize");
#elif USE_GPU
    using TYPE = float;
    cv::cvtColor(dst, dst, cv::COLOR_BGR2RGB);
    dst.convertTo(dst, CV_32FC3);
    dst /= 255.0f;

    TYPE *ptr = (TYPE *) malloc(inputSize.size * sizeof(TYPE));
    memcpy(ptr, dst.data, inputSize.size*sizeof(float));
#endif
    *data = (void *) ptr;

    return true;
}

void Yolov5n::Tensor2Box(float *predictions)
{
#ifdef USE_NPU
    // todo
#elif USE_GPU

    const size_t  boxLength = 4;
    const size_t boxConfidenceLength = 1;
    int objectStep = classNum + boxLength + boxConfidenceLength;
    size_t boxSize = outputSize.at(0) / objectStep;
    auto boxes = predictions;
    int originWidth = this->item.image.data.cols;
    int originHeight = this->item.image.data.rows;
    float ratioWidth = (float)MODEL_WIDTH / originWidth;
    float ratioHeight = (float)MODEL_HEIGHT / originHeight;
    float ratio = MIN(ratioWidth, ratioHeight);
    int heightPadding = 0;
    int widthPadding = 0;

    if (option.param.padding)
    {
#ifdef LETTER_BOX
        int widthUnpad = int(originWidth * ratio);
        int heightUnpad = int(originHeight * ratio);

        widthPadding = round(((MODEL_WIDTH - widthUnpad) % 32) / 2);
        heightPadding = round(((MODEL_HEIGHT - heightUnpad) % 32) / 2);
#else
        heightPadding = (MODEL_HEIGHT - ratio * originHeight) / 2;
        widthPadding = (MODEL_WIDTH - ratio * originWidth) / 2;
#endif

        ratioWidth = ratio;
        ratioHeight = ratio;
    }

    for (int i = 0; i < boxSize; ++i, boxes += objectStep)
    {
        if (boxes[4] > 1.01) continue;

        // location decoding
        float xcenter = boxes[0];
        float ycenter = boxes[1];
        float w = boxes[2];
        float h = boxes[3];

        if (xcenter < 0 or ycenter < 0 or w < 0 or h < 0) continue;

        float ymin = ((ycenter - h * 0.5) - heightPadding) / ratioHeight;
        float xmin = ((xcenter - w * 0.5) - widthPadding) / ratioWidth;
        float ymax = ((ycenter + h * 0.5) - heightPadding) / ratioHeight;
        float xmax = ((xcenter + w * 0.5) - widthPadding) / ratioWidth;

        // probability decoding, softmax
        float boxConfidence = boxes[4];
        const size_t confidenceStartID = boxConfidenceLength + boxLength;
        float objectProb = 0, total_sum = 0;
        int objectID = 0;

        for (int j = 0; j < classNum; j++)
        {
            float logit = boxes[j + confidenceStartID]  * boxConfidence;
            if (objectProb < logit)
            {
                objectProb = logit;
                objectID = j;
            }
        }

        if (objectProb > mScoreThreshold)
        {
            BoxInfo box;
            box.box.x = xmin;
            box.box.y = ymin;
            box.box.width = xmax - xmin;
            box.box.height = ymax - ymin;

            // center
            box.cx = (xmin + xmax) / 2.0;
            box.cy = (ymin + ymax) / 2.0;

            box.height = box.box.height;
            box.width = box.box.width;

            box.score = objectProb;
            box.classID = (ClassID)objectID;
            box.className = LABEL_NAMES[objectID];

            if (objectProb < THRESHOLD_CLASS[box.classID])
                continue;

            this->boxes.push_back(box);
        }
    }
#endif
}
