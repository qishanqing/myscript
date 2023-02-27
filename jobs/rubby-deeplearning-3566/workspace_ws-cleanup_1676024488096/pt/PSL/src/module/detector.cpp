//
// Created by hao on 2021/6/3.
//

#include "detector.h"

#ifdef USE_NPU_AML
#include "src/module/npu_aml/model_npu.h"
#elif USE_NPU_RK
#include "src/module/npu_rk/model_npu_rk.h"
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
#ifdef USE_NPU_AML
    model = new ModelNpuAml();
#elif USE_GPU
    model = new ModelGPU();
#elif USE_NPU_RK
    model = new ModelNpuRK();
#else
    return false;
#endif

    ret = model->Init(options.modelName, option);

    if (ret) LOG_CHECK_DEBUG(INFO) << "success to init model {" << options.modelName << "}.";
    else LOG_CHECK_DEBUG(INFO) << "model not match init.";

    return ret;
}

bool Detector::Detect(const cv::Mat &image, std::vector<BoxInfo> &boxes)
{
    Timer timer;

    this->item.image.left = image;
    LOG_CHECK_TIME(INFO) << timer.TimingStr("image clone");
    bool ret = Detect();
    boxes.swap(this->boxes);
    return ret;
}

Detector::~Detector()
{
    RELEASE(model);
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

bool Detector::Postprocess(void *predictions)
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
        LOG_CHECK_DEBUG(INFO) << "image time: " << item.orgImage.time;
    }
}

bool Detector::Detect(std::vector<BoxInfo> &boxes, DataGroup &item)
{
    {
        std::unique_lock<std::mutex> lock(itemLock);
        if (this->itemLastest.Empty()) // TODO : lack of data, what message shuld to throw
        {
            return false;
        }

        this->item = this->itemLastest;
        this->item.orgImage.left = this->itemLastest.orgImage.left.clone();
        if (not this->itemLastest.orgImage.right.empty())
        {
            this->item.orgImage.right = this->itemLastest.orgImage.right.clone();
        }

        this->itemLastest = DataGroup();
    }

    if (!this->item.Remap()) return false;

    item = this->item;
    bool status = this->Detect();
    boxes.swap(this->boxes);
    this->item.image.Reset();

    return status;
}

bool Detector::Detect()
{
    running = true;
    Timer timer;
    void *data = nullptr;
    void *predictions = nullptr;

    if (this->item.image.left.empty() || this->item.image.left.channels() != 3)
    {
        LOG_CHECK_STATUS(ERROR) << "image channels is wrong, want 3, but got " << this->item.image.left.channels();
        return false;
    }

    Preprocess(&data);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("preprocess");

    if (not model->RunModel(data, &predictions))
    {
        FREE(data);
        FREE(predictions);
        return false;
    }

    FREE(data);
    model->GetOutputSize(outputSize);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("run model");
    Postprocess(predictions);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("postprocess");
    running = false;

    return true;
}

void Detector::Update(const Lidar &lidar)
{
}