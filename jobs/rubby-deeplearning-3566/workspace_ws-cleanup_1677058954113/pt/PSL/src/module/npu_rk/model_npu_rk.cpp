
//
// Created by hao on 2022/07/13.
//
#include "model_npu_rk.h"
#include "src/utils/utils.h"
#include "src/utils/file.h"
#include "src/utils/define.h"

#define CHECK_RETURN(stat, message) do {\
    if( (stat) < 0 ) {                        \
        if ("" != (message)) \
            LOG_CHECK_STATUS(WARNING) << "[" << FUNC_NAME << "] " << (message) \
            << ", error code " << ret;\
        return false;\
    }\
} while(0)

ModelNpuRK::ModelNpuRK()
{
    input_attrs.clear();
    output_attrs.clear();
}

ModelNpuRK::~ModelNpuRK()
{
    DestoryNetwork();
}

void ModelNpuRK::CheckVersion()
{
    rknn_sdk_version version;
    int ret = rknn_query(ctx, RKNN_QUERY_SDK_VERSION, &version, sizeof(rknn_sdk_version));
    if (ret < 0) LOG_CHECK_STATUS(WARNING) << "rknn_query error ret=" << ret;

    LOG_CHECK_STATUS(INFO) << "rknn version: " << version.api_version << " driver version: "
                           << version.drv_version;
}

bool ModelNpuRK::InitContext(const std::string modelFile)
{
    int modelSize = 0;
    unsigned char *data;
    int ret = file_op::File::Read(modelFile.c_str(), &data, modelSize);
    CHECK_STATUS_EXIT(ret, "");
    ret = rknn_init(&ctx, data, modelSize, 0, NULL);
    CHECK_STATUS_EXIT(ret >= 0, "rknn_init error ret=" + ret);

    return true;

    exit:
    return false;
}

bool ModelNpuRK::CreateNetwork(const std::string modelFile, int classNum)
{
    InitContext(modelFile);
    CheckVersion();
    InitIO();
}

void ModelNpuRK::DestoryNetwork()
{

}

bool ModelNpuRK::UploadTensor(void *input)
{
//    src = wrapbuffer_virtualaddr((void *) img.data, img_width, img_height, RK_FORMAT_RGB_888);
//    dst = wrapbuffer_virtualaddr((void *) resize_buf, inputSize.width, inputSize.height, RK_FORMAT_RGB_888);
//    ret = imcheck(src, dst, src_rect, dst_rect);
//    if (IM_STATUS_NOERROR != ret)
//    {
//        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS) ret));
//        return -1;
//    }
//    IM_STATUS STATUS = imresize(src, dst);
    this->inputs.at(0).buf = input;
    rknn_inputs_set(ctx, 1, this->inputs.data());

    return true;
}


inline static int32_t __clip(float val, float min, float max)
{
    float f = val <= min ? min : (val >= max ? max : val);
    return f;
}

static int8_t qnt_f32_to_affine(float f32, int32_t zp, float scale)
{
    float dst_val = (f32 / scale) + zp;
    int8_t res = (int8_t) __clip(dst_val, -128, 127);
    return res;
}

static float deqnt_affine_to_f32(int8_t qnt, int32_t zp, float scale)
{
    return ((float) qnt - (float) zp) * scale;
}

bool ModelNpuRK::DownloadTensor(void **output)
{
    const int size = outputs.size();
    int ret = rknn_outputs_get(ctx, size, outputs.data(), nullptr);
    CHECK_RETURN(ret, "fail to get output");


    for (int i = 0; i < size; ++i)
    {
        out_scales.push_back(this->output_attrs[i].scale);
        out_zps.push_back(this->output_attrs[i].zp);
    }

    using TYPE = float;
    TYPE *predictions = NULL;
    int output_len = 0;
    outputSize.resize(size);

    for (int i = 0; i < size; i++)
    {
        outputSize[i] = outputs.at(i).size;

        output_len += outputSize[i];
    }

    predictions = (TYPE *) malloc(sizeof(TYPE) * output_len);

    size_t output_cnt = 0;
    for (int i = 0; i < size; i++)
    {
        int8_t *tensor = (int8_t *) this->outputs.at(i).buf;

        for (int j = 0; j < outputSize[i]; j++)
        {
            predictions[output_cnt] =
                    deqnt_affine_to_f32(tensor[j], out_zps.at(i), out_scales.at(i));
            output_cnt++;
        }
    }

    rknn_outputs_release(ctx, size, outputs.data());
    *output = predictions;

    return true;
}

bool ModelNpuRK::Forward()
{
    int ret = rknn_run(ctx, nullptr);

    CHECK_RETURN(ret, "fail to forward");

    return true;
}

void ModelNpuRK::GetOutputSize(std::vector<unsigned long> &outputSize) const
{
    outputSize = this->outputSize;
}

bool ModelNpuRK::GetIONumber(rknn_input_output_num &io_num) const
{
    int ret = rknn_query(ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
    if (ret < 0)
    {
        LOG_CHECK_STATUS(INFO) << "rknn_query error ret=" << ret;
        return false;
    }
    LOG_CHECK_STATUS(INFO) << "model input num: " << io_num.n_input
                           << ", output num: " << io_num.n_output;

    return true;
}

bool ModelNpuRK::InitIO()
{
    rknn_input_output_num io_num;
    CHECK_STATUS_EXIT(GetIONumber(io_num), "");
    CHECK_STATUS_EXIT(CreateInput(io_num.n_input), "");
    CHECK_STATUS_EXIT(CreateOutput(io_num.n_output), "");

    return true;

    exit:
    return false;
}

void ModelNpuRK::PrintTensorAttribute(const rknn_tensor_attr &attr)
{
    char message[2000];
    sprintf(message, "  index=%d, name=%s, n_dims=%d, dims=[%d, %d, %d, %d], n_elems=%d, size=%d"
                     ", fmt=%s, type=%s, qnt_type=%s, "
                     "zp=%d, scale=%f"
            , attr.index, attr.name, attr.n_dims, attr.dims[0]
            , attr.dims[1], attr.dims[2], attr.dims[3], attr.n_elems, attr.size
            , get_format_string(attr.fmt), get_type_string(attr.type), get_qnt_type_string(
                    attr.qnt_type)
            , attr.zp, attr.scale);
    LOG_CHECK_DEBUG(INFO) << message;
}

bool ModelNpuRK::CreateTensorAttribute(const rknn_query_cmd mode, const int num
                                       , std::vector<rknn_tensor_attr> &tensorAttribute)
{
    tensorAttribute.resize(num);
//    memset(input_attrs.data, 0, sizeof(input_attrs));
    for (int i = 0; i < num; i++)
    {
        tensorAttribute[i].index = i;
        int ret = rknn_query(ctx, mode, &(tensorAttribute[i]), sizeof(rknn_tensor_attr));
        if (ret < 0)
        {
            LOG_CHECK_STATUS(INFO) << "rknn_query error ret=" << ret;
            return false;
        }
        PrintTensorAttribute(tensorAttribute[i]);
    }

    return true;
}

bool ModelNpuRK::CheckInputSize() const
{
    ImageHead inputSize;

    if (input_attrs[0].fmt == RKNN_TENSOR_NCHW)
    {
        LOG_CHECK_DEBUG(INFO) << "model is NCHW input format";
        inputSize.channel = input_attrs[0].dims[1];
        inputSize.width = input_attrs[0].dims[2];
        inputSize.height = input_attrs[0].dims[3];
    }
    else
    {
        LOG_CHECK_DEBUG(INFO) << "model is NHWC input format";
        inputSize.width = input_attrs[0].dims[1];
        inputSize.height = input_attrs[0].dims[2];
        inputSize.channel = input_attrs[0].dims[3];
    }

    if (inputSize != this->inputSize)
    {
        LOG_CHECK_STATUS(INFO) << "expect " << inputSize.Print() << " but got "
                               << this->inputSize.Print();
        return false;
    }
    else
    {
        LOG_CHECK_DEBUG(INFO) << "model input " << inputSize.Print();

        return true;
    }
}

bool ModelNpuRK::CreateInput(const int num)
{
    if (num < 1)
    {
        LOG_CHECK_STATUS(ERROR) << "error input size: " << num;
        return false;
    }

    void *resize_buf;

    CHECK_STATUS_EXIT(CreateTensorAttribute(RKNN_QUERY_INPUT_ATTR, num, input_attrs)
                      , "fail to create input tensor.");
    CHECK_STATUS_EXIT(CheckInputSize(), "");

    inputs.resize(num);

    for (int i = 0; i < num; ++i)
    {
        inputs[i].index = i;
        inputs[i].type = RKNN_TENSOR_UINT8;
        inputs[i].size = this->inputSize.size;
        inputs[i].fmt = RKNN_TENSOR_NHWC;
        inputs[i].pass_through = 0; // TODO : what

        resize_buf = malloc(inputs[i].size);
        inputs[i].buf = resize_buf;
    }

    return true;

    exit:
    return false;
}

bool ModelNpuRK::CreateOutput(const int num)
{
    CHECK_STATUS_EXIT(CreateTensorAttribute(RKNN_QUERY_OUTPUT_ATTR, num, output_attrs)
                      , "fail to create output tensor.");

    outputs.resize(num);
    for (int i = 0; i < num; i++)
    {
        outputs[i].want_float = 0;
    }

    return true;

    exit:
    return false;
}
