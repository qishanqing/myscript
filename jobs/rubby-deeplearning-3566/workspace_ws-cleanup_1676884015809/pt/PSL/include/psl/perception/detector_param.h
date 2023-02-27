#pragma once

#include <string>

namespace psl
{
struct InputHead
{
    int width;
    int height;
    int channel;
};

struct ModelProperty
{
    enum ModelType {UNKNOWN_DETECTOR = 0, MOBILENETV2_SSD = 1, YOLOV5S = 2, YOLOV3 = 3, YOLOV5N = 4};

    ModelType type;
    std::string version;
    InputHead input;
    bool padding = false;
    bool keypoint = false;
    int classNum;
    std::vector<std::string> names;
    std::vector<float> trackingExtend;
    std::vector<float> detectExtend;
};

struct DetectorParam
{
    ModelProperty modelProperty;
    std::string config_path;
    int fps;
};
}