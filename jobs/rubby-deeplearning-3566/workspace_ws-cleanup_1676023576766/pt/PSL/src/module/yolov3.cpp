//
// Created by hao on 2021/6/9.
//

#include "yolov3.h"
#include "utils/timer.h"
#include "src/utils/define.h"
#include "src/utils/detector_property.h"
#include "src/utils/utils.h"
#include <omp.h>
#include "src/utils/image_process.h"

const int MAX_DETECT_NUM = 100;
const float THRESHOLD_CLASS[] = {0.6, 0.4, 0.55, 0.5, 0.5, 0.5, 0.5};
const float THRESHOLD= 0.4; // TODO : is min THRESHOLD_CLASS
const int MIN_TIME_INTERVAL = 10 * ONE_SECOND;//second
const int MIN_COLLECT_CONFIDENCE = 45;
const int MAX_COLLECT_CONFIDENCE = 80;
const bool PADDING = true;

Yolov3::Yolov3()
        : errorMessage(ErrorMessage::GetInstance())
{

}

Yolov3::~Yolov3()
{

}

bool Yolov3::Preprocess(void **data)
{
    Timer timer;
    cv::Mat dst;

    const ImageHead &inputSize = GetInputSize();
    if (PADDING)
    {
        ImageProcess::ResizeByPadding(this->item.image.data, dst, cv::Size(inputSize.width, inputSize.height));
    }
    else
    {
        cv::resize(this->item.image.data, dst, cv::Size(inputSize.width, inputSize.height));
    }

    LOG_CHECK_TIME(INFO) << timer.TimingStr("resize");
//    std::vector<cv::Mat> grayGroup(3, dst);
//
//    cv::merge(grayGroup, dst);
//
//    if (dst.channels() != 3) return false;
//    this->item.image.data = dst.clone();
//    PRINTF(dst.channels());

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
    TYPE *ptr = (TYPE *) malloc(inputSize.size * sizeof(TYPE));;
    memcpy(ptr, dst.data, inputSize.size);
#endif

    *data = (void *) ptr;

    return true;
}


static int MaxIndex(float *a, int n)
{
    int i, max_i = 0;
    float max = a[0];

    if (n <= 0)
        return -1;

    for (i = 1; i < n; ++i)
    {
        if (a[i] > max)
        {
            max = a[i];
            max_i = i;
        }
    }
    return max_i;
}

void
Yolov3::GetDetectionsResult(int num, float thresh, Box *boxes, float **probs, int classes)
{
    int i, boxCount = 0;
    float left = 0, right = 0, top = 0, bot = 0;
    const ImageHead &inputSize = GetInputSize();

    for (i = 0; i < num; ++i)
    {
        if (boxes[i].prob <= thresh) continue;
        int id = MaxIndex(probs[i], classes);
//        if (not Valid(id))
//            continue;
        float prob = probs[i][id];
        if (prob > thresh)
        {
            if (boxCount >= MAX_DETECT_NUM) break;
            if (PADDING)
            {
                ReversePaddingResult(boxes[i], this->item.image.data
                        , cv::Size(inputSize.width, inputSize.height));
            }

            left = boxes[i].x - boxes[i].w / 2.;
            right = boxes[i].x + boxes[i].w / 2.;
            top = boxes[i].y - boxes[i].h / 2.;
            bot = boxes[i].y + boxes[i].h / 2.;

            if (left < 0) left = 0;
            if (right > 1) right = 1.0;
            if (top < 0) top = 0;
            if (bot > 1) bot = 1.0;

            // percent to image coordinate
            left *= this->item.image.data.cols;
            right *= this->item.image.data.cols;
            top *= this->item.image.data.rows;
            bot *= this->item.image.data.rows;


            BoxInfo boxInfo;
            boxInfo.SetRect(cv::Rect(left, top, right - left, bot - top));
            boxInfo.classID = (ClassID) id;

            boxInfo.className = LABEL_NAMES[id];
            boxInfo.score = prob * 100;

            errorMessage->Pop(ErrorMessageCode::DL_COLLECT_IMG_BY_CONFIDENCE
                              , "request collect image by confidence");

            static auto lastReport = GetTimestamp() - MIN_TIME_INTERVAL;
            auto betweenReport = GetTimestamp() - lastReport;

            if (boxInfo.score > MIN_COLLECT_CONFIDENCE and
                boxInfo.score < MAX_COLLECT_CONFIDENCE
                and betweenReport > MIN_TIME_INTERVAL)
            {
                errorMessage->Push(ErrorMessageCode::DL_COLLECT_IMG_BY_CONFIDENCE
                                   , "request collect image by confidence");
                lastReport = GetTimestamp();
            }

            if (prob < THRESHOLD_CLASS[boxInfo.classID])
                continue;

            this->boxes.push_back(boxInfo);
            boxCount++;
        }
    }
}

static float Logistic(float x)
{
    return 1. / (1. + exp(-x));
}

static Box Region2Box(float *x, float *biases, int n, int index
                      , int i, int j, int w, int h)
{
    Box b;
    b.x = (i + Logistic(x[index + 0])) / w;
    b.y = (j + Logistic(x[index + 1])) / h;
    b.w = exp(x[index + 2]) * biases[2 * n] / w;
    b.h = exp(x[index + 3]) * biases[2 * n + 1] / h;
    return b;
}

static void Flatten(float *x, int size, int layers, int batch, int forward)
{
    float *swap = (float *) calloc(size * layers * batch, sizeof(float));
    int i, c, b;
    for (b = 0; b < batch; ++b)
    {
        for (c = 0; c < layers; ++c)
        {
            for (i = 0; i < size; ++i)
            {
                int i1 = b * layers * size + c * size + i;
                int i2 = b * layers * size + i * layers + c;
                if (forward) swap[i2] = x[i1];
                else swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size * layers * batch * sizeof(float));
    free(swap);
}

static int PostProcessOnescale(float *predictions, int *input_size, float *biases
                               , Box *boxes, float **probs, float threshold)
{
    int i, j;
    int coords = 4;
    int bb_size = coords + NUM_CLASSES + 1;
    int num_box = input_size[2] / bb_size;
    int modelWidth = input_size[0];
    int modelHeight = input_size[1];

    for (j = 0; j < modelWidth * modelHeight * num_box; ++j)
        probs[j] = (float *) calloc(NUM_CLASSES + 1, sizeof(float));

    int ck0, batch = 1;
    Flatten(predictions, modelWidth * modelHeight, bb_size * num_box, batch, 1);

    // logistic
    for (i = 0; i < modelHeight * modelWidth * num_box; ++i)
    {
        for (ck0 = coords; ck0 < bb_size; ck0++)
        {
            int index = bb_size * i;

            predictions[index + ck0] = Logistic(predictions[index + ck0]);
            if (ck0 == coords)
            {
                if (predictions[index + ck0] <= threshold)
                {
                    break;
                }
            }
        }
    }


    for (i = 0; i < modelWidth * modelHeight; ++i)
    {
        int row = i / modelWidth;
        int col = i % modelWidth;
        int n = 0;
        for (n = 0; n < num_box; ++n)
        {
            int index = i * num_box + n;
            int p_index = index * bb_size + 4;
            float scale = predictions[p_index];
            int box_index = index * bb_size;
            int class_index = 0;
            class_index = index * bb_size + 5;

            if (scale > threshold)
            {
                for (j = 0; j < NUM_CLASSES; ++j)
                {
                    float prob = scale * predictions[class_index + j];
                    probs[index][j] = (prob > threshold) ? prob : 0;
                }
                boxes[index] = Region2Box(predictions, biases, n, box_index, col, row
                                          , modelWidth, modelHeight);
            }
            boxes[index].prob = (scale > threshold) ? scale : 0;
        }
    }
    return 0;
}


void Yolov3::Tensor2Box(float *predictions)
{
#ifdef USE_NPU
    const ImageHead &inputSize = GetInputSize();
    int nn_output_channel = inputSize.channel * (NUM_CLASSES + 5);
    int size[3] = {inputSize.width / 32, inputSize.height / 32, nn_output_channel};

    float threshold = 0.05;
    float iou_threshold = 0.4;

    float biases[18] = {10 / 8., 13 / 8., 16 / 8., 30 / 8., 33 / 8., 23 / 8., 30 / 16.
                        , 61 / 16., 62 / 16., 45 / 16., 59 / 16., 119 / 16., 116 / 32.
                        , 90 / 32., 156 / 32., 198 / 32., 373 / 32., 326 / 32.};
    int size2[3] = {size[0] * 2, size[1] * 2, size[2]};
    int size4[3] = {size[0] * 4, size[1] * 4, size[2]};
    // TODO : refactor the code style
    int len1 = size[0] * size[1] * size[2];
    int box1_id = len1 / (NUM_CLASSES + 5);

    Box *boxes = (Box *) calloc(box1_id * (1 + 4 + 16)
                                , sizeof(Box)); // box1_id * (layer1 + layer2 + layer3)
    float **probs = (float **) calloc(box1_id * (1 + 4 + 16), sizeof(float *));

    PostProcessOnescale(&predictions[0], size, &biases[12], boxes, &probs[0]
                        , threshold); //final layer
    PostProcessOnescale(&predictions[len1], size2, &biases[6], &boxes[box1_id]
                        , &probs[box1_id], threshold);
    PostProcessOnescale(&predictions[len1 * (1 + 4)], size4, &biases[0], &boxes[
            box1_id * (1 + 4)], &probs[box1_id * (1 + 4)], threshold);

    // TODO : move NMS to the perception
    Box::NMSSort(boxes, probs, box1_id * 21, NUM_CLASSES, iou_threshold);
    GetDetectionsResult(box1_id * 21, THRESHOLD, boxes, probs, NUM_CLASSES);

    FREE(boxes);

    for (int j = 0; j < box1_id * (1 + 4 + 16); ++j)
    {
        FREE(probs[j]);
    }
    FREE(probs);
#elif USE_GPU
    const size_t size = 10647; // TODO : shuld calclate from the base layer info

    GetDetectionsResult(size, THRESHOLD, predictions, NUM_CLASSES);
#endif

    NMSOnlyOne(this->boxes);
}

void Yolov3::GetDetectionsResult(int num, float thresh
                                 , const float *predictions, int classes)
{
    int boxCount = 0;
    const size_t sizeBox = 10647 * 4;
    std::vector<Box> boxes;
    std::vector<float> probs;

    // TODO : merge with other GetDetectionsResult
    for (size_t i = 0; i < num; ++i)
    {
        size_t id = sizeBox + i * classes;

        size_t objectID = 0;
        float objectProb = 0;

        // get confidence
        for (size_t j = 0; j < classes; ++j)
        {
            if (predictions[id + j] > objectProb)
            {
                objectID = j;
                objectProb = predictions[id + j];
            }
        }

        if (objectProb < thresh) continue;

        if (boxCount >= MAX_DETECT_NUM) break;

        ERROR_PRINT(Join(predictions, num, ", "));
        float left = predictions[i * 4];
        float top = predictions[i * 4 + 1];
        float right = predictions[i * 4 + 2];
        float bottom = predictions[i * 4 + 3];
        if (left < 0) left = 0;
        if (right > 1) right = 1.0;
        if (top < 0) top = 0;
        if (bottom > 1) bottom = 1.0;
        left *= this->item.image.data.cols;
        right *= this->item.image.data.cols;
        top *= this->item.image.data.rows;
        bottom *= this->item.image.data.rows;

        BoxInfo boxInfo;
        boxInfo.SetRect(cv::Rect(left, top, right - left, bottom - top));
        boxInfo.classID = (ClassID) objectID;

        boxInfo.className = LABEL_NAMES[objectID];
        boxInfo.score = objectProb * 100;
        ERROR_PRINT(objectProb);

        this->boxes.push_back(boxInfo);
        boxCount++;
    }
}

void Yolov3::NMSOnlyOne(std::vector<BoxInfo> &boxes)
{
    size_t size = boxes.size();
    for (int i = 0; i < size; ++i)
    {
        const auto &box1 = boxes.at(i);

        if (not box1.IsEscalator()) continue;

        for (int j = i + 1; j < size; ++j)
        {
            const auto &box2 = boxes.at(j);
            if (not box2.IsEscalator()) continue;

            if (box1.Intersection(box2))
            {
                if (box1.box.width > box2.box.width)
                {
                    boxes.erase(boxes.begin() + j--);
                    size--;
                }
                else
                {
                    boxes.erase(boxes.begin() + i--);
                    size--;
                    break;
                }
            }
        }
    }
}

void Yolov3::ReversePaddingResult(Box &box, const cv::Mat &src, const cv::Size dstSize)
{
    float ratioWidth = 1.0;
    float ratioHeight = 1.0;
    float ratio = 1.0;

    if(not (ImageProcess::GetRatio(src, dstSize, ratioWidth, ratioHeight)))
    {
        LOG_CHECK_DEBUG(INFO)<<"invalid image size or dstImage size";
        return;
    }

    bool heightBigger = ratioHeight > ratioWidth;

    if (heightBigger)
    {
        ratio = (1 / ratioWidth) * ratioHeight;
        box.x *= ratio;
        box.w *= ratio;
    }
    else
    {
        ratio = (1 / ratioHeight) * ratioWidth;
        box.y *= ratio;
        box.h *= ratio;
    }
}