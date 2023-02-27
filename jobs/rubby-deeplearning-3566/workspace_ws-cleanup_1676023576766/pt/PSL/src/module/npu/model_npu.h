//
// Created by hao on 2021/6/3.
// deep learning model on npu device
//

#ifndef DETECT_DEMO_NPUMODEL_H
#define DETECT_DEMO_NPUMODEL_H

#include "src/module/model.h"
#include "src/module/npu/vnn/vnn_global.h"

class ModelNPU : public Model
{
public:
    ModelNPU();

    ~ModelNPU();

private:
    bool CreateNetwork(const std::string modelFile) override;

    void DestoryNetwork() override;

    bool UploadTensor(void *input) override;

    bool Forward() override;

    bool DownloadTensor(float **output) override;

    void ModelGetsize();

    const vsi_nn_preprocess_map_element_t *GetPrePorcessMap();
    uint32_t vnn_GetPrePorcessMapCount();
    const vsi_nn_postprocess_map_element_t *GetPostPorcessMap();
    uint32_t vnn_GetPostPorcessMapCount();

    void GetOutputSize(std::vector<unsigned long>& outputSize) const override;

private:
    NetworkStatus modelStatus;
    vsi_nn_graph_t *graph;

    const static vsi_nn_postprocess_map_element_t *postprocessMap;
    const static vsi_nn_preprocess_map_element_t *preprocessMap;
};


#endif //DETECT_DEMO_NPUMODEL_H
