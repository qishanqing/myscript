//
// Created by hao on 2021/07/13.
// deep learning model on npu_rk device
//

#ifndef DETECT_DEMO_NPUMODEL_H
#define DETECT_DEMO_NPUMODEL_H

#include "src/module/model.h"
#include "rknn_api.h"

class ModelNpuRK : public Model
{
public:
    ModelNpuRK();

    ~ModelNpuRK();

private:
    void CheckVersion();

    bool InitContext(const std::string modelFile);

    bool CreateNetwork(const std::string modelFile, int classNum) override;

    void DestoryNetwork() override;

    bool UploadTensor(void *input) override;

    bool Forward() override;

    bool DownloadTensor(void **output) override;

    void GetOutputSize(std::vector<unsigned long> &outputSize) const override;

    bool InitIO();

    bool GetIONumber(rknn_input_output_num &io_num) const;

    static void PrintTensorAttribute(const rknn_tensor_attr &attr);

    bool CreateTensorAttribute(const rknn_query_cmd mode, const int num
                               , std::vector<rknn_tensor_attr> &tensorAttribute);

    bool CheckInputSize() const;

    bool CreateInput(const int num);

    bool CreateOutput(const int num);

private:
    rknn_context ctx;
    std::vector<rknn_tensor_attr> input_attrs;
    std::vector<rknn_input> inputs;
    std::vector<rknn_tensor_attr> output_attrs;
    std::vector<rknn_output> outputs;
    std::vector<float> out_scales;
    std::vector<int32_t> out_zps;
};


#endif //DETECT_DEMO_NPUMODEL_H
