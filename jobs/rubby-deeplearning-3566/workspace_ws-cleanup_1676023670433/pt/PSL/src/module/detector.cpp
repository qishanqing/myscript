//
// Created by hao on 2021/6/3.
//

#include "detector.h"
#ifdef USE_NPU
#include "src/module/npu/model_npu.h"
#elif USE_GPU
#include "src/module/gpu/model_gpu.h"
#endif
#include "src/utils/utils.h"
#include "src/utils/define.h"
#include "src/utils/de_distortion.h"
#include "src/utils/box.h"

using namespace inner;

const float NMS_THRESHOLD = 0.4;

bool Detector::Init(const DetectorOptions &options)
{
    this->option = options;
    bool ret = false;

    // select backend from compile, or may be can from the backend of option
#ifdef USE_NPU
    model = new ModelNPU();
#elif USE_GPU
    model = new ModelGPU();
#else
    return false;
#endif

    ret = model->Init(options.modelName);

    if (ret) LOG_CHECK_DEBUG(INFO) << "success to init model.";
    else LOG_CHECK_DEBUG(INFO) << "model not match init.";

    return ret;
}

bool Detector::Detect(const cv::Mat &image, std::vector<BoxInfo> &boxes)
{
    Timer timer;

    this->item.image.data = image;
    LOG_CHECK_TIME(INFO) << timer.TimingStr("image clone");
    Detect();
    boxes.swap(this->boxes);
    return true;
}

Detector::~Detector()
{
    if (model != nullptr)
    {
        delete model;
        model = nullptr;
    }
}

Detector::Detector()
        : model(nullptr)
          , running(false)
          , mNumOfClasses(0)
{

}

const ImageHead &Detector::GetInputSize() const
{
    return model->GetInputSize();
}

bool Detector::Postprocess(float *predictions)
{
    Tensor2Box(predictions);
    FREE(predictions);
    box::NMS(this->boxes, NMS_THRESHOLD);
    return true;
}

bool Detector::Valid(const int id)
{
    return (id == 0);
}

void Detector::Update(const DataGroup &item)
{
    if (not running)
    {
        {
            std::unique_lock<std::mutex> lock(itemLock);
            this->itemLastest = item;
        }
//        LOG_CHECK_DEBUG(INFO) << "lidar time: " <<  item.lidar.data.header.stamp;
        LOG_CHECK_DEBUG(INFO) << "image time: " <<  item.orgImage.time;
    }
}

bool Detector::Detect(std::vector<BoxInfo> &boxes, DataGroup& item)
{
    {
        std::unique_lock<std::mutex> lock(itemLock);
        if (this->itemLastest.Empty()) // TODO : lack of data, what message shuld to throw
        {
            return false;
        }

        this->item = this->itemLastest;
        this->item.orgImage.data = this->itemLastest.orgImage.data.clone();

        this->itemLastest = DataGroup();
    }

    if (!this->item.Remap()) return false;

    item = this->item;
    bool status =  this->Detect();
    boxes.swap(this->boxes);
    this->item.image.Reset();

    return status;
}

bool Detector::Detect()
{
    running = true;
    Timer timer;
    void *data = nullptr;
    float *predictions;

    if (this->item.image.data.empty() || this->item.image.data.channels() != 3)
    {
        LOG_CHECK_STATUS(ERROR) << "image channels is wrong!";
        return false;
    }

    Preprocess(&data);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("preprocess");
    if (not model->RunModel(data, &predictions)) return false;
    FREE(data);
    model->GetOutputSize(outputSize);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("run model");
    Postprocess(predictions);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("postprocess");
    running = false;

    return true;
}
