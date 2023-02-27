//
// Created by hao on 2021/7/26.
//

#ifndef DETECTOR_TEST_TYPES_H
#define DETECTOR_TEST_TYPES_H

#include <string>

struct InputParam
{
    std::string configPath;
    std::string imagePath; // can parse image, lidar, pose from the file list
    std::string annoPath;     // the annotations file root dir
};

#endif //DETECTOR_TEST_TYPES_H
