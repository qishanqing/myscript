#ifndef UTILS_H_
#define UTILS_H_

#include <opencv2/opencv.hpp>
#include <dlfcn.h>
#include <glog/logging.h>
#include <yaml-cpp/yaml.h>
#include <Eigen/Core>
#include "src/utils/timer.h"
#include "src/utils/detector_param.h"
#include "perception/instance.h"
#include "src/utils/config.h"

const float MIN_COORDINATE = -1000;//min coordinate

void DrawBoxes(const std::vector<BoxInfo> &finalBoxInfos, cv::Mat &image, const bool dot = false);

void
DrawInstance(const std::vector<BoxInfo> &boxes, const std::vector<psl::Instance> &
             , cv::Mat &raw_image);

void *OpenVideoStream(const std::string &f, int c, int w, int h, int fps);

float IoU(cv::Rect box0, cv::Rect box1);

float GetSize(float w, float h);

void Softmax(float *data, int num);

template<class T>
bool Contain(const std::vector<T> &array, const T &item)
{
    return std::find(array.begin(), array.end(), item) != array.end();
}

template<class T>
std::string Join(const T *data, const uint size, const char *separator)
{
    std::ostringstream ss;
    const uint beforLast = size - 1;
    std::copy(data, data + beforLast, std::ostream_iterator<T>(ss, separator));
    std::copy(data + beforLast, data + size, std::ostream_iterator<T>(ss));
    std::string message = ss.str();

    return message;
}

std::string GetTimestamp(std::string &date, std::string &second);

std::time_t GetTimestamp();

void PrintObject(const std::vector<psl::Instance> &instanceInfos);

void PrintObject(const psl::SenmaticMap &senmaticMap);

void PrintObject(const std::vector<BoxInfo> &boxes);

void Vector2Location(const Eigen::Vector3d &data, psl::Location &location);

void Lidar2Machine(const ConfigParam &configParam, psl::Location &pc);

template <class T>
void PrintMatrix(const T *data, const int width, const int maxCol, const int maxRow)
{
    for (int i = 0; i < maxRow; i++)
    {
        for (int j = 0; j < maxCol; j++)
            std::cout << std::fixed << std::setprecision(2) << std::setw(7) << data[i*width+j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

#endif