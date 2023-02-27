#include "model.h"
#include "src/utils/utils.h"
#include "src/utils/file.h"
#include "src/utils/define.h"


bool Model::Init(const std::string modelFile)
{
    LOG_CHECK_DEBUG(INFO) << "init model";
    bool exist = file_op::File::Exist(modelFile);
    CHECK_STATUS_EXIT(exist,"modle file <" + std::string(modelFile) + "> not exist");

    return CreateNetwork(modelFile);

    exit:
    END();
    return false;
}

bool Model::RunModel(void *input, float **output)
{
    Timer timer;
    bool flag = false;

    flag = UploadTensor(input);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("upload tensor");
    CHECK_STATUS(flag, exit, "fail to upload tensor");
    flag = Forward();
    LOG_CHECK_TIME(INFO) << timer.TimingStr("forward");
    CHECK_STATUS(flag, exit, "fail to Forward");
    flag = DownloadTensor(output);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("download tensor");
    CHECK_STATUS(flag, exit, "fail to download tensor");

    return true;
    exit:
    return false;
}

const ImageHead &Model::GetInputSize() const
{
    return this->inputSize;
}

Model::~Model()
{
//    DestoryNetwork();
// TODO : why cannot
}