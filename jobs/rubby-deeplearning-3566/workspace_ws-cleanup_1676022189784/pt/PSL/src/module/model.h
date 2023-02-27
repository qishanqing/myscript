//
// Created by hao on 2021/6/3.
// deep learning model
//

#ifndef DETECT_DEMO_MODEL_H
#define DETECT_DEMO_MODEL_H
#include "string"
#include "image_type.h"
#include <vector>
#include "src/utils/detector_param.h"

enum NetworkStatus {
    NETWORK_UNINIT,
    NETWORK_INIT,
    NETWORK_PREPARING,
    NETWORK_PROCESSING,
};

class Model
{
public:
    Model(){}

    virtual ~Model();

    bool Init(const std::string modelFile, const DetectorOptions &options);

    bool RunModel(void *input, void **output);

    const ImageHead& GetInputSize() const;

    virtual void GetOutputSize(std::vector<unsigned long>& outputSize) const = 0;

protected:
    virtual bool CreateNetwork(const std::string modelFile, int classNum) = 0;

    virtual void DestoryNetwork() = 0;

    virtual bool UploadTensor(void *input) = 0;

    virtual bool Forward() = 0;

    virtual bool DownloadTensor(void **output) = 0;

protected:
    ImageHead inputSize;
    DetectorOptions option;
    std::vector<unsigned long> outputSize;
};


#endif //DETECT_DEMO_MODEL_H
