
//
// Created by hao on 2021/6/3.
//
#include "model_npu.h"
#include "src/module/npu_aml/vnn/vnn_yolov3.h"
#include "src/module/npu_aml/vnn/vnn_yolov3_tiny.h"
#include "src/utils/utils.h"
#include "src/utils/file.h"
#include "src/utils/define.h"

const vsi_nn_postprocess_map_element_t *ModelNpuAml::postprocessMap = nullptr;
const vsi_nn_preprocess_map_element_t *ModelNpuAml::preprocessMap = nullptr;

ModelNpuAml::ModelNpuAml()
        : modelStatus(NETWORK_UNINIT)
{

}

ModelNpuAml::~ModelNpuAml()
{
    DestoryNetwork();
}

bool ModelNpuAml::CreateNetwork(const std::string modelFile, int classNum)
{
    vsi_status status = VSI_SUCCESS;
    // TODO : add for check env
    // TODO : refer to the origin logging
    // TODO : add network status
    // TODO : check support model
    CHECK_BEFOR
        // TODO : input the func
        graph = vnn_CreateYolov3(modelFile.c_str(), NULL, GetPrePorcessMap()
                                 , vnn_GetPrePorcessMapCount(), GetPostPorcessMap()
                                 , vnn_GetPostPorcessMapCount()
                                 , classNum);
        TEST_CHECK_PTR(graph, exit);

        status = vsi_nn_VerifyGraph(graph);
        CHECK_STATUS(VSI_SUCCESS == status, exit,
                "fail to create modle, file_path = " + std::string(modelFile));

        modelStatus = NETWORK_INIT;

        ModelGetsize();
    CHECK_AFTER

    return true;

    exit:
    END();
    return false;
}

void ModelNpuAml::DestoryNetwork()
{
    vnn_Release(graph, true);

    graph = nullptr;
}

bool ModelNpuAml::UploadTensor(void *input)
{
    vsi_nn_tensor_t *tensor = vsi_nn_GetTensor(graph, graph->input.tensors[0]);
    vsi_status status = vsi_nn_CopyDataToTensor(graph, tensor, (uint8_t *) input);

    TEST_CHECK_STATUS(status, exit);

    return true;

    exit:
    return false;
}

bool ModelNpuAml::DownloadTensor(void **output)
{
    vsi_nn_tensor_t *tensor = NULL;

    int sz[10];
    int i, j, stride;
    int output_cnt = 0;
    int output_len = 0;
    uint8_t *tensor_data = NULL;
    float *predictions = NULL;

    for (i = 0; i < graph->output.num; i++)
    {
        tensor = vsi_nn_GetTensor(graph, graph->output.tensors[i]);
        sz[i] = 1;
        // TODO : check tensor->attr.dim_num == 4
        ImageHead outputSize(tensor->attr.size[0], tensor->attr.size[1]
                             , tensor->attr.size[2]);
        int batchSize = tensor->attr.size[3];
        sz[i] = outputSize.size * batchSize;
        output_len += sz[i];
    }
    predictions = (float *) malloc(sizeof(float) * output_len);

    for (i = 0; i < graph->output.num; i++)
    {
        tensor = vsi_nn_GetTensor(graph, graph->output.tensors[i]);

        stride = vsi_nn_TypeGetBytes(tensor->attr.dtype.vx_type);
        tensor_data = (uint8_t *) vsi_nn_ConvertTensorToData(graph, tensor);

        // equal the vsi_nn_DtypeToFloat32
        float fl = pow(2., -tensor->attr.dtype.fl);
        for (j = 0; j < sz[i]; j++)
        {
            int val = tensor_data[stride * j];
            int tmp1 = (val >= 128) ? val - 256 : val;
            predictions[output_cnt] = tmp1 * fl;
            output_cnt++;
        }
        vsi_nn_Free(tensor_data);
    }

    *output = predictions;

    return true;
}

void ModelNpuAml::ModelGetsize()
{
    int width = -1, height = -1, channel = -1;

    if (graph)
    {
        vsi_nn_tensor_t *tensor = NULL;
        tensor = vsi_nn_GetTensor(graph, graph->input.tensors[0]);

        width = tensor->attr.size[0];
        height = tensor->attr.size[1];
        channel = tensor->attr.size[2];
    }

    inputSize.Set(width, height, channel);
}


const vsi_nn_preprocess_map_element_t *ModelNpuAml::GetPrePorcessMap()
{
    return preprocessMap;
}

uint32_t ModelNpuAml::vnn_GetPrePorcessMapCount()
{
    if (preprocessMap == NULL)
        return 0;
    else
        return sizeof(preprocessMap) / sizeof(vsi_nn_preprocess_map_element_t);
}

const vsi_nn_postprocess_map_element_t *ModelNpuAml::GetPostPorcessMap()
{
    return postprocessMap;
}

uint32_t ModelNpuAml::vnn_GetPostPorcessMapCount()
{
    if (postprocessMap == NULL)
        return 0;
    else
        return sizeof(postprocessMap) / sizeof(vsi_nn_postprocess_map_element_t);
}

bool ModelNpuAml::Forward()
{
    vsi_status status = vsi_nn_RunGraph(graph);
    TEST_CHECK_STATUS(status, exit);

    return true;

    exit:
    return false;
}

void ModelNpuAml::GetOutputSize(std::vector<unsigned long> &outputSize) const
{
}