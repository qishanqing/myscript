#include "ssd.h"
#include "src/utils/timer.h"
#include "src/utils/image_process.h"
#include "src/utils/utils.h"

const int MAX_DETECT_NUM = 100;
const float THRESHOLD_CLASS[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
const float THRESHOLD = 0.4;
const float THRESHOLD_NMS = 0.5;

SSD::SSD()
{
    mVariance = {0.1, 0.1, 0.2, 0.2};
    mScoreThreshold = THRESHOLD;
    mNMSThreshold = THRESHOLD_NMS;
    mTopK = MAX_DETECT_NUM;
    classNum = 6; // TODO : should read from config or class name file;
}

SSD::~SSD()
{
}

bool SSD::Preprocess(void **data)
{
    Timer timer;
    cv::Mat dst;

    const ImageHead &inputSize = GetInputSize();
    if (option.param.padding)
    {
        // TODO : when false (others)
        ImageProcess::ResizeByPadding(this->item.image.left, dst, cv::Size(inputSize.width
                                                                           , inputSize.height));
    }
    else
    {
        cv::resize(this->item.image.left, dst, cv::Size(inputSize.width
                                                        , inputSize.height));
    }

    LOG_CHECK_TIME(INFO) << timer.TimingStr("resize");

    // TODO : move to model/preprocess
#ifdef USE_NPU_AML
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
#elif USE_NPU_RK
    using TYPE = uint8_t;
    TYPE* ptr;
#elif USE_GPU
    using TYPE = float;
    cv::cvtColor(dst, dst, cv::COLOR_BGR2RGB);
    dst.convertTo(dst, CV_32FC3);

    const float mean_vals[3] = {123.f, 117.f, 104.f};
    dst = dst - cv::Scalar(mean_vals[0], mean_vals[1], mean_vals[2]);
    TYPE *ptr = (TYPE *) malloc(inputSize.size * sizeof(TYPE));
    memcpy(ptr, dst.data, inputSize.size * sizeof(float));
#endif
    *data = (void *) ptr;

    return true;
}

void SSD::Tensor2Box(void *predictions)
{
#ifdef USE_NPU_AML
    // todo
#elif USE_GPU

    size_t boxSize = outputSize.at(1) / 4;
    OUTPUT_TYPE *boxes = (OUTPUT_TYPE *) predictions;
    auto anchor = boxes + outputSize.at(0);
    int originWidth = this->item.image.left.cols;
    int originHeight = this->item.image.left.rows;
    int objectStep = classNum + 4;

    for (int i = 0; i < boxSize; ++i)
    {
        const size_t iBox = i * objectStep;
        const size_t iAnchor = i;
        // location decoding
        float xcenter = boxes[iBox + classNum + 0] * mVariance[0] * anchor[iAnchor * 4 + 2] +
                        anchor[iAnchor * 4 + 0];
        float ycenter = boxes[iBox + classNum + 1] * mVariance[1] * anchor[iAnchor * 4 + 3] +
                        anchor[iAnchor * 4 + 1];
        float w = exp(boxes[iBox + classNum + 2] * mVariance[2]) * anchor[iAnchor * 4 + 2];
        float h = exp(boxes[iBox + classNum + 3] * mVariance[3]) * anchor[iAnchor * 4 + 3];

        float ymin = (ycenter - h * 0.5) * originHeight;
        float xmin = (xcenter - w * 0.5) * originWidth;
        float ymax = (ycenter + h * 0.5) * originHeight;
        float xmax = (xcenter + w * 0.5) * originWidth;

        // probability decoding, softmax
        float total_sum = exp(boxes[iBox + 0]);
        // init
        int objectID = 0;
        float objectProb = 0;

        for (int j = 1; j < classNum; j++)
        {
            float logit = exp(boxes[iBox + j]);
            total_sum += logit;
            if (objectProb < logit)
            {
                objectProb = logit;
                objectID = j;
            }
        }

        objectProb /= total_sum;


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
            box.classID = objectID;
            box.className = option.param.names[objectID];
            this->boxes.push_back(box);
        }
    }
#endif
}
