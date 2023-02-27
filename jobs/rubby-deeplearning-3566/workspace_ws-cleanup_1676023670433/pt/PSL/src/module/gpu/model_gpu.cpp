#include <fstream>
#include "model_gpu.h"
#include "src/utils/detector_property.h"
#include "src/utils/define.h"
#include "src/utils/timer.h"
#include "src/utils/utils.h"
#include "src/utils/file.h"

//#define WRITE_TENSOR

#ifdef WRITE_TENSOR
const std::vector<std::string> outputNames = {"cls_and_bbox", "anchors"};
size_t num = 0;
#else
const std::vector<std::string> outputNames = {"cls_and_bbox", "anchors"};
#endif

ModelGPU::ModelGPU()
{

}

ModelGPU::~ModelGPU()
{
    DestoryNetwork();
}

bool ModelGPU::CreateNetwork(const std::string modelFile)
{
    Timer timer;
    mModelName = modelFile;
    int threads = 4;
    int precision = 2;

    int forward = MNN_FORWARD_OPENCL;
    mConfig.numThread = threads;
    mConfig.type = static_cast<MNNForwardType>(forward);

    // backend config
    MNN::BackendConfig backendConfig;
    backendConfig.precision = (MNN::BackendConfig::PrecisionMode) precision;
    backendConfig.power = MNN::BackendConfig::Power_High;
    mConfig.backendConfig = &backendConfig;

    // input config
    mInputSize.push_back(MODEL_HEIGHT);
    mInputSize.push_back(MODEL_WIDTH);

    // PrepareInputAndOutputNames();

    // create net first
    mNet.reset(MNN::Interpreter::createFromFile(mModelName.c_str()));

    // create session
    mSession = mNet->createSession(mConfig);
    mNet->releaseModel();
    mFirst = true;
    InitInputAndOutput();

    LOG_CHECK_TIME(INFO) << timer.TimingStr("create model");

    return true; // TODO : when false
}

void ModelGPU::DestoryNetwork()
{
// TODO : check session is null
    mNet->releaseSession(mSession);
    for (auto &t: mOutputTensorsHost)
    {
        delete t;
    }
}

bool ModelGPU::UploadTensor(void *input)
{
    // TODO : size check
    // copy to tensor
    std::vector<int> dims{1, mInputSize[0], mInputSize[1], 3};
    auto nhwc_Tensor = MNN::Tensor::create(dims, mInputTensors[0]->getType(), NULL
                                           , MNN::Tensor::TENSORFLOW);
    auto nhwc_data = nhwc_Tensor->host<float>();
    auto nhwc_size = nhwc_Tensor->size();
    ::memcpy(nhwc_data, input, nhwc_size);

    mInputTensors[0]->copyFromHostTensor(nhwc_Tensor);

    delete nhwc_Tensor;

    return true; // TODO : when false
}

bool ModelGPU::Forward()
{
    CHECK_BEFOR
        mNet->runSession(mSession);
    CHECK_AFTER
    return true;

    exit:
    return false;
}

bool ModelGPU::DownloadTensor(float **output)
{
    size_t output_len = 0;
    const int outputSize = mOutputTensors.size();
    for (int i = 0; i < outputSize; i++)
    {
        output_len += mOutputTensors[i]->elementSize();
    }

    float *predictions = (float *) malloc(sizeof(float) * output_len);
    size_t count = 0;

    // TODO : remove, and parse from mOutputTensors
    for (int i = 0; i < outputSize; i++)
    {
        auto tensorGPU = mOutputTensors[i];
        auto tensorCPU = mOutputTensorsHost[i];

        tensorGPU->copyToHostTensor(tensorCPU);
        auto tensor = tensorCPU->host<float>();
        memcpy(predictions + count, tensor, tensorCPU->elementSize() * sizeof(float));
        count += tensorCPU->elementSize();

#ifdef WRITE_TENSOR
        LOG_CHECK_STATUS(INFO) << tensorCPU->elementSize();
        std::string fileName =
                "result/" + std::to_string(num) + "_" + std::to_string(i) + "_" +
                mOutputNames.at(i) + ".txt";
        std::ofstream file(fileName);
        file_op::File::MkdirFromFile(fileName);
        float *boxes = tensor;
        size_t objectStep = 10;
        size_t boxSize = tensorCPU->elementSize() / objectStep;

        for (int i = 0; i < boxSize; ++i, boxes += objectStep)
        {
            file << Join(boxes, objectStep, ",") << "\n";
        }
        file.close();
#endif
    }
#ifdef WRITE_TENSOR
    num++;
#endif


    int lassNum = mOutputTensorsHost[0]->shape()[3] - 4;
    int num_boxes = mOutputTensorsHost[0]->channel();

    mFirst = false;
    *output = predictions;

    // TODO : tensor to float**

    return true; // TODO : when false
}

void ModelGPU::InitInputAndOutput()
{
    // TODO : exception for names error
    PrepareParamNames(outputNames);
    SetUpInputAndOutputTensors();
    auto dims = mInputTensors[0]->shape();
    mInputSize[1] = dims[3];
    mInputSize[0] = dims[2];
    inputSize.Set(dims[3], dims[2], MODEL_CHANNEL); // TODO : check to dims
}

void ModelGPU::PrepareParamNames(const std::vector<std::string> &names)
{
    mOutputNames = names;
}

void ModelGPU::SetUpInputAndOutputTensors()
{
    if (mInputNames.size() == 0)
    {
        mInputTensors.push_back(mNet->getSessionInput(mSession, NULL));
    }
    else
    {
        for (auto &input_name: mInputNames)
        {
            mInputTensors.push_back(mNet->getSessionInput(mSession, input_name.c_str()));
        }
    }

    // get output tensor
    for (auto &output_name: mOutputNames)
    {
        auto t = mNet->getSessionOutput(mSession, output_name.c_str());
        mOutputTensors.push_back(t);
        LOG_CHECK_STATUS(INFO) << t->elementSize() << " "
                               << Join(t->shape().data(), 5, ",");
        auto t_host = MNN::Tensor::create(t->shape(), t->getType(), NULL
                                          , MNN::Tensor::CAFFE);
        mOutputTensorsHost.push_back(t_host);
    }
}

void ModelGPU::GetOutputSize(std::vector<unsigned long> &outputSize) const
{
    outputSize.clear();
    for (int i = 0; i < mOutputTensors.size(); ++i)
    {
        outputSize.push_back(mOutputTensors[i]->elementSize());
    }
}
