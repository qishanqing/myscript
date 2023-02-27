//////////////////////////////////////////////////////////////////////
///  @file     model_gpu.h
///  @brief    cnn inference based on gpu by [mnn, ]
///  @author   sunhao
///  @date     2021.08.05
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_MODEL_GPU_H
#define DETECTOR_SAMPLE_MODEL_GPU_H

#include "src/module/model.h"
#include "MNN/Interpreter.hpp"
#include "MNN/MNNDefine.h"
#include "MNN/Tensor.hpp"
#include <vector>
#include <memory.h>


class ModelGPU : public Model
{
public:
    ModelGPU();

    ~ModelGPU();

    void GetOutputSize(std::vector<unsigned long>& outputSize) const override;

private:
    bool CreateNetwork(const std::string modelFile) override;

    void DestoryNetwork() override;

    bool UploadTensor(void *input) override;

    bool Forward() override;

    bool DownloadTensor(float **output) override;

    void InitInputAndOutput();

    void PrepareParamNames(const std::vector<std::string> &names);

    void SetUpInputAndOutputTensors();

private:
    std::string mModelName;
    MNN::ScheduleConfig mConfig;
    // hw
    std::vector<int> mInputSize;


    std::shared_ptr<MNN::Interpreter> mNet;
    MNN::Session *mSession = nullptr;

    // input and output
    std::vector<MNN::Tensor *> mOutputTensors;
    std::vector<MNN::Tensor *> mInputTensors;
    std::vector<MNN::Tensor *> mOutputTensorsHost;

    std::vector<std::string> mInputNames;
    std::vector<std::string> mOutputNames;
    bool mFirst;
};


#endif //DETECTOR_SAMPLE_MODEL_GPU_H
