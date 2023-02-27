//////////////////////////////////////////////////////////////////////
///  @file     SaveDataset.h
///  @brief    save the dataset for deep learning
///  Details.
///
///  @author   sunhao
///  @date     2021.05.13
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_SAVEDATASET_H
#define DETECTOR_SAMPLE_SAVEDATASET_H

#include <string>
#include <yaml-cpp/yaml.h>
#include <opencv2/core.hpp>
#include "src/utils/data_type.h"
#include "sensor/CameraMoudleParam.h"
#include "sensor/LaserScan.h"
#include "slam/SlamResult.h"

class SaveDataset
{
public:
    SaveDataset();

    ~SaveDataset();

    bool Init(const std::string path, const psl::CameraMoudleParam &param
              , const psl::Resolution resolution);

    cv::FileStorage GetFileStream(const std::string file);

    cv::FileStorage GetFileStreamConfig();

    bool WriteConfig();

    bool WriteData(const Image &image, const Lidar &lidar);

    void NextBatch();

private:
    bool WritePose(const psl::SlamResult &pose
                   , const psl::Time time
                   , const std::string fileName);

    void InitBatch(const std::string path);


private:
    std::string rootPath;
    std::string outputDir;
    cv::FileStorage fileStream;
    int batch;
    psl::CameraMoudleParam param;
    psl::Resolution resolution;
};


#endif //DETECTOR_SAMPLE_SAVEDATASET_H
