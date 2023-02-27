//
// Created by indemind on 11/22/21.
//

#include "init_yolov3.h"

std::unique_ptr<ModelType> InitYoloV3()
{
    std::string configFile = inputParam.configPath + CONFIG_FILE_NAME;
    ConfigParam configParam;
    Status error = LoadConfig(configFile, configParam);
    const std::string MODEL_VERSION = "1.2.0";

    DetectorOptions options;
    //TODO change to related path
    options.modelName = inputParam.configPath + "/net." + MODEL_VERSION + ".npu_aml";
    options.topk = 100;
    options.nmsThreshold = 0.45;
    options.scoreThreshold = 0.3;
    options.backend = "npu_aml";

    std::unique_ptr<ModelType> detector;
    detector = std::move(std::unique_ptr<ModelType>(new ModelType()));
    bool ret = detector->Init(options);
    if (!ret)
    {
        std::cout<<"Init wrong!"<<std::endl;
    }
    return std::move(detector);
}