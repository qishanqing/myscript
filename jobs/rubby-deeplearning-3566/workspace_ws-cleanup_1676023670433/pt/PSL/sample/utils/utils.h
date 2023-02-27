//
// Created by hao on 2021/6/15.
//

#ifndef DETECTOR_SAMPLE_UTILS_H
#define DETECTOR_SAMPLE_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <dirent.h>
#include <yaml-cpp/yaml.h>
// robot
#include "sensor/CameraMoudleParam.h"
#include "slam/SlamResult.h"
#include "sensor/LaserScan.h"

#define ERROR_PRINT(x) std::cout << "" << (x) << "" << std::endl
#define WARNING_PRINT(x) std::cout << "" << (x) << "" << std::endl
#define INFO_PRINT(x) std::cout << "" << (x) << "" << std::endl

struct SyncDataFile
{
    std::string image;
    std::string imagePose;
    std::string lidar;
    std::string lidarPose;

    void Print();

    SyncDataFile& SetPrefix(const std::string message)
    {
        this->image = message + this->image;
        this->imagePose = message + this->imagePose;
        this->lidarPose = message + this->lidarPose;
        this->lidar = message + this->lidar;

        return *this;
    }
};

uint64_t GetImageTime(const std::string imageFile);

bool GetData(const std::string inputDir, std::vector<SyncDataFile>& dataset);

void ReadFilesFromDir(const std::string &path_to_dir
                      , std::vector<std::string> *image_name_list);

bool ReadSyncFile(std::string srcFile, std::vector<SyncDataFile> &files);

bool ReadFile(std::string srcFile, std::vector<std::string> &image_files);

void ReadArray(const YAML::Node &config, std::vector<float> &array);

bool GetCameraConfig(std::string file, psl::CameraMoudleParam &param);

bool GetLidar(std::string yamlFile, swr::sensor_types::LaserScan &data, bool mode);

bool GetPose(std::string yamlFile, psl::SlamResult &pose);

std::string getCurrentExePath();

std::string getCurrentExeName();

#endif //DETECTOR_SAMPLE_UTILS_H
