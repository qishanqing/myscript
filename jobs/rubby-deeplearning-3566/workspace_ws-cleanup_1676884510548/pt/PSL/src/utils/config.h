//////////////////////////////////////////////////////////////////////
///  @file     data_type.h
///  @brief    base types
///  @author   sunhao
///  @date     2021.07.02
//////////////////////////////////////////////////////////////////////

#ifndef CONFIG_SAMPLE_DATA_TYPE_H
#define CONFIG_SAMPLE_DATA_TYPE_H

#include <string>
#include "src/utils/log.h"
#include "src/utils/data_type.h"
#include "src/utils/status.h"

const std::string CONFIG_FILE_NAME = "param.yaml";
const psl::Resolution RESOLUTION = psl::Resolution::RES_640X400;

struct StructureParam
{

    std::vector<float> poseLidar2Machine;
    bool lidarDirection = true;
    float lidarAngle = 0;
    std::vector<float> leftcamera2lidar;
    std::vector<float> rightcamera2lidar;
};

struct DetectParam
{
    enum DetectorType {UNKNOWN_DETECTOR = 0, MOBILENETV2_SSD = 1, YOLOV5S = 2, YOLOV3 = 3};

    DetectorType type;
    bool padding = false;
};

struct ConfigParam
{
    enum Display {NOT_SHOW = 0, SHOW = 1, WRITE = 2};

    Log log;
    Display display;
    float radius;
    int nFrames;
    float distance;
    float angle;
    float threshold;
    Data4SaveInfo dataset;
    StructureParam structure;
    int timeMargin;
    DetectParam detect;

};

Status LoadConfig(const std::string& configFile, ConfigParam& configParam);


#endif //CONFIG_SAMPLE_DATA_TYPE_H
