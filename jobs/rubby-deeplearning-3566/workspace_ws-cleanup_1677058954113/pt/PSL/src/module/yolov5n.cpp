#include "yolov5n.h"
#include "src/utils/timer.h"
#include "src/utils/image_process.h"
#include "src/utils/utils.h"

//#define LETTER_BOX

const int MAX_DETECT_NUM = 100;
const float THRESHOLD_CLASS[] = {0.65, 0.6, 0.6, 0.5, 0.55, 0.5, 0.65, 0.65, 0.7};;
const float THRESHOLD = 0.3;
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
        ImageProcess::LetterBox(this->item.image.left, dst, cv::Size(inputSize.width
                                                                           , inputSize.height));
#else
        ImageProcess::ResizeByPadding(this->item.image.left, dst, cv::Size(inputSize.width
                                                                           , inputSize.height));
#endif
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
//    cv::cvtColor(dst, dst, cv::COLOR_BGR2RGB);
    TYPE *ptr = (TYPE *) malloc(inputSize.size * sizeof(TYPE));
    memcpy(ptr, dst.data, inputSize.size * sizeof(TYPE));
#elif USE_GPU
    using TYPE = float;
    cv::cvtColor(dst, dst, cv::COLOR_BGR2RGB);
    dst.convertTo(dst, CV_32FC3);
    dst /= 255.0f;

    TYPE *ptr = (TYPE *) malloc(inputSize.size * sizeof(TYPE));
    memcpy(ptr, dst.data, inputSize.size * sizeof(TYPE));
#endif
    *data = (void *) ptr;

    return true;
}

static float sigmoid(float x)
{
    return 1.0 / (1.0 + expf(-x));
}

static float unsigmoid(float y)
{
    return -1.0 * logf((1.0 / y) - 1.0);
}

inline static int clamp(float val, int min, int max)
{
    return val > min ? (val < max ? val : max) : min;
}

int Yolov5n::process(OUTPUT_TYPE *input, int *anchor, int grid_h, int grid_w, int height, int width
                     , int stride, std::vector<float> &boxes, std::vector<float> &objProbs
                     , std::vector<int> &classId, float threshold)
{
    int validCount = 0;
    int grid_len = grid_h * grid_w;
    float thres = unsigmoid(threshold);
    const int boxLength = 4;
    int itemLength = this->option.param.classNum + boxLength + 1;
    if (option.param.keypoint)
    {
        itemLength = this->option.param.classNum + boxLength + 1 + 8;
    }

    for (int a = 0; a < 3; a++)
    {
        for (int i = 0; i < grid_h; i++)
        {
            for (int j = 0; j < grid_w; j++)
            {
                OUTPUT_TYPE box_confidence = input[(itemLength * a + 4) * grid_len + i * grid_w +
                                                   j];
                if (box_confidence >= thres)
                {
                    int offset = (itemLength * a) * grid_len + i * grid_w + j;
                    OUTPUT_TYPE *in_ptr = input + offset;
                    float box_x = sigmoid(*in_ptr) * 2.0 - 0.5;
                    float box_y = sigmoid(in_ptr[grid_len]) * 2.0 - 0.5;
                    float box_w = sigmoid(in_ptr[2 * grid_len]) * 2.0;
                    float box_h = sigmoid(in_ptr[3 * grid_len]) * 2.0;
                    box_x = (box_x + j) * (float) stride;
                    box_y = (box_y + i) * (float) stride;
                    box_w = box_w * box_w * (float) anchor[a * 2];
                    box_h = box_h * box_h * (float) anchor[a * 2 + 1];
                    box_x -= (box_w / 2.0);
                    box_y -= (box_h / 2.0);
                    boxes.push_back(box_x);
                    boxes.push_back(box_y);
                    boxes.push_back(box_w);
                    boxes.push_back(box_h);
                    if (option.param.keypoint)
                    {
                        for (int m = 0; m < 4; ++m)
                        {
                            float point_x = sigmoid(in_ptr[(itemLength -8 + m * 2) * grid_len]) * 4.0 -2.0;
                            float point_y = sigmoid(in_ptr[(itemLength -8 + m * 2 + 1) * grid_len]) * 4.0 -2.0;

                            point_x = point_x * (float) anchor[a * 2] + j * (float) stride ;
                            point_y = point_y * (float) anchor[a * 2 + 1] + i * (float) stride;

                            boxes.push_back(point_x);
                            boxes.push_back(point_y);
                        }
                    }

                    float maxClassProbs = in_ptr[5 * grid_len];
                    int maxClassId = 0;
                    for (int k = 1; k < this->option.param.classNum; ++k)
                    {
                        int8_t prob = in_ptr[(5 + k) * grid_len];
                        if (prob > maxClassProbs)
                        {
                            maxClassId = k;
                            maxClassProbs = prob;
                        }
                    }
                    objProbs.push_back(sigmoid(maxClassProbs) * sigmoid(box_confidence));
                    classId.push_back(maxClassId);
                    validCount++;
                }
            }
        }
    }
    return validCount;
}

int Yolov5n::PostprocessV5s(OUTPUT_TYPE *input0, OUTPUT_TYPE *input1, OUTPUT_TYPE *input2
                          , const int model_in_h, const int model_in_w, const float conf_threshold
                          , const float nms_threshold, const float scale_w, const float scale_h)
{
    std::vector<float> filterBoxes;
    std::vector<float> objProbs;
    std::vector<int> classId;

    const int anchor0[6] = {10, 13, 16, 30, 33, 23};
    const int anchor1[6] = {30, 61, 62, 45, 59, 119};
    const int anchor2[6] = {116, 90, 156, 198, 373, 326};

    // stride 8
    int stride0 = 8;
    int grid_h0 = model_in_h / stride0;
    int grid_w0 = model_in_w / stride0;
    int validCount0 = 0;
    validCount0 = process(input0, (int *) anchor0, grid_h0, grid_w0, model_in_h, model_in_w, stride0
                          , filterBoxes, objProbs, classId, conf_threshold);

    // stride 16
    int stride1 = 16;
    int grid_h1 = model_in_h / stride1;
    int grid_w1 = model_in_w / stride1;
    int validCount1 = 0;
    validCount1 = process(input1, (int *) anchor1, grid_h1, grid_w1, model_in_h, model_in_w, stride1
                          , filterBoxes, objProbs, classId, conf_threshold);

    // stride 32
    int stride2 = 32;
    int grid_h2 = model_in_h / stride2;
    int grid_w2 = model_in_w / stride2;
    int validCount2 = 0;
    validCount2 = process(input2, (int *) anchor2, grid_h2, grid_w2, model_in_h, model_in_w, stride2
                          , filterBoxes, objProbs, classId, conf_threshold);

    int validCount = validCount0 + validCount1 + validCount2;
    // no object detect
    if (validCount <= 0) return 0;

    std::vector<int> indexArray;
    for (int i = 0; i < validCount; ++i)
    {
        indexArray.push_back(i);
    }

//    quick_sort_indice_inverse(objProbs, 0, validCount - 1, indexArray);

    /* box valid detect target */
    for (int i = 0; i < validCount; ++i)
    {
        if (indexArray[i] == -1) continue;
        int n = indexArray[i];

        int boxLength=4;
        if (option.param.keypoint)
        {
            boxLength=12;
        }

        float x1 = filterBoxes[n * boxLength + 0];
        float y1 = filterBoxes[n * boxLength + 1];
        float x2 = x1 + filterBoxes[n * boxLength + 2];
        float y2 = y1 + filterBoxes[n * boxLength + 3];
        int id = classId[n];
        float obj_conf = objProbs[i];

        x1 = (int) (clamp(x1, 0, model_in_w) / scale_w);
        y1 = (int) (clamp(y1, 0, model_in_h) / scale_h);
        x2 = (int) (clamp(x2, 0, model_in_w) / scale_w);
        y2 = (int) (clamp(y2, 0, model_in_h) / scale_h);

        if (obj_conf > mScoreThreshold and obj_conf > THRESHOLD_CLASS[id])
        {
            BoxInfo box;
            box.score = obj_conf;
            box.classID = (ClassID) id;
            box.className = option.param.names[id];
            if (option.param.keypoint && box.className == "desk_rect" || option.param.keypoint && box.className == "desk_circle")
            {
                Keypoint keypoint = {filterBoxes[n * boxLength + 4],filterBoxes[n * boxLength + 5],
                                    filterBoxes[n * boxLength + 6],filterBoxes[n * boxLength + 7],
                                    filterBoxes[n * boxLength + 8],filterBoxes[n * boxLength + 9],
                                     filterBoxes[n * boxLength + 10],filterBoxes[n * boxLength + 11]};
                box.SetRect(cv::Rect(x1, y1, x2 - x1, y2 - y1), keypoint);
            }
            else
            {
                box.SetRect(cv::Rect(x1, y1, x2 - x1, y2 - y1));
            }

            this->boxes.push_back(box);
        }
    }

    return 0;
}

int Yolov5n::PostprocessV5n(OUTPUT_TYPE *boxes, const float scale_w, const float scale_h)
{
    const size_t boxLength = 4;
    const size_t boxConfidenceLength = 1;
    int objectStep = classNum + boxLength + boxConfidenceLength;
    size_t boxSize = outputSize.at(0) / objectStep;

    for (int i = 0; i < boxSize; ++i, boxes += objectStep)
    {
        if (boxes[4] > 1.01) continue;

        // location decoding
        float xcenter = boxes[0];
        float ycenter = boxes[1];
        float w = boxes[2];
        float h = boxes[3];

        if (xcenter < 0 or ycenter < 0 or w < 0 or h < 0) continue;

        float ymin = (ycenter - h * 0.5) / scale_h;
        float xmin = (xcenter - w * 0.5) / scale_w;
        float ymax = (ycenter + h * 0.5) / scale_h;
        float xmax = (xcenter + w * 0.5) / scale_w;

        // probability decoding, softmax
        float boxConfidence = boxes[4];
        const size_t confidenceStartID = boxConfidenceLength + boxLength;
        float objectProb = 0, total_sum = 0;
        int objectID = 0;

        for (int j = 0; j < classNum; j++)
        {
            float logit = boxes[j + confidenceStartID] * boxConfidence;
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
            box.classID = (ClassID) objectID;
            box.className = option.param.names[objectID];

            if (objectProb < THRESHOLD_CLASS[box.classID])
                continue;

            this->boxes.push_back(box);
        }
    }
}

void Yolov5n::Tensor2Box(void *predictions)
{
    int originWidth = this->item.image.left.cols;
    int originHeight = this->item.image.left.rows;
    float ratioWidth = (float) option.param.input.width / originWidth;
    float ratioHeight = (float) option.param.input.height / originHeight;
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
        heightPadding = (option.param.input.height - ratio * originHeight) / 2;
        widthPadding = (option.param.input.width - ratio * originWidth) / 2;
#endif

        ratioWidth = ratio;
        ratioHeight = ratio;
    }

#ifdef USE_NPU_AML
    // todo
#elif USE_NPU_RK
    OUTPUT_TYPE *tensor = (float *) predictions;
    PostprocessV5s(tensor, tensor + this->outputSize.at(0),
            tensor + this->outputSize.at(0) + this->outputSize.at(1)
                 , this->option.param.input.height, this->option.param.input.width
                 , THRESHOLD, THRESHOLD_NMS, ratioWidth, ratioHeight);
#elif USE_GPU
    if (option.param.type == psl::ModelProperty::YOLOV5S)
    {
        OUTPUT_TYPE *tensor = (float *) predictions;
        PostprocessV5s(tensor, tensor + this->outputSize.at(0),
                     tensor + this->outputSize.at(0) + this->outputSize.at(1)
                , this->option.param.input.height, this->option.param.input.width
                , THRESHOLD, THRESHOLD_NMS, ratioWidth, ratioHeight);

    }
    else
    {
        OUTPUT_TYPE *tensor = (float *) predictions;
        PostprocessV5n(tensor, ratioWidth, ratioHeight);
    }

#endif
    if (option.param.padding)
    {
        for (auto &box : this->boxes)
        {
            int ymin = (box.box.y - heightPadding / ratioHeight);
            int xmin = (box.box.x - widthPadding / ratioWidth);
            int ymax = MIN(originHeight, ymin + box.box.height);
            int xmax = MIN(originWidth, xmin + box.box.width);
            ymin = MAX(0, ymin);
            xmin = MAX(0, xmin);
            if (option.param.keypoint && box.className == "desk_rect" || option.param.keypoint && box.className == "desk_circle")
            {
                for (int j = 0; j < 4; ++j)
                {
                    box.keypoint.point[j * 2] = (box.keypoint.point[j * 2] - widthPadding) / ratioWidth;
                    box.keypoint.point[j * 2 + 1] = (box.keypoint.point[j * 2 + 1] - heightPadding) / ratioHeight;
                }
                box.SetRect(cv::Rect(xmin, ymin, xmax - xmin, ymax - ymin), box.keypoint);
            }
            else
            {
                box.SetRect(cv::Rect(xmin, ymin, xmax - xmin, ymax - ymin));
            }
        }
    }
}
