#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <Eigen/Core>
#include "src/utils/define.h"
#include "src/utils/image_process.h"

static std::vector<std::string> CLASS_NAMES{"person", "escalator", "unknown"};

void DrawBoxes(const std::vector<BoxInfo> &finalBoxInfos, cv::Mat &image, const bool dot)
{
    // visualize bbox
    for (auto &box: finalBoxInfos)
    {
        cv::Rect boxRect;
        boxRect.x = (int) box.box.x;
        boxRect.y = (int) box.box.y;
        boxRect.width = (int) box.box.width;
        boxRect.height = (int) box.box.height;
        auto color = cv::Scalar(0, std::min(255, (box.classID % 3) * 80), std::min(255,
                (box.classID / 3 + 1) * 100));

        if (dot) ImageProcess::DrawDottedRect(image, boxRect, color, 1);
        else cv::rectangle(image, boxRect, color, 2);

        // show txt
        auto font = cv::FONT_HERSHEY_SIMPLEX;
        std::stringstream ss;
        ss.precision(3);
        // TODO : use the paired name
        ss << INDEMIND_NAMES[box.classID] << " " << box.score;
        auto txt = ss.str();
        int baseline;
        float font_scale = 0.5;
        cv::Size cat_size = cv::getTextSize(txt, font, font_scale, 2, &baseline);

        // get color for specific class
        cv::Point bottomLeft = cv::Point(box.box.x, std::max(20, box.box.y - 2));
        cv::rectangle(image, cv::Rect(bottomLeft.x, bottomLeft.y - cat_size.height
                                      , cat_size.width
                                      , cat_size.height), color, -1);
        int thickness = 1;
        cv::putText(image, txt, bottomLeft
                    , font, font_scale, cv::Scalar(255, 255, 255)
                    , thickness, cv::LINE_AA);

        ss.str("");
        ss << "(" << box.location.machine.x << ", " << box.location.machine.y << ")";
        txt = ss.str();
        cat_size = cv::getTextSize(txt, font, font_scale, 2, &baseline);
        bottomLeft += cv::Point(0, 15);
        cv::rectangle(image, cv::Rect(bottomLeft.x, bottomLeft.y - cat_size.height
                                      , cat_size.width
                                      , cat_size.height), color + cv::Scalar(10, 150, 50)
                      , -1);
        cv::putText(image, txt, bottomLeft
                    , font, font_scale, cv::Scalar(255, 255, 255)
                    , thickness, cv::LINE_AA);
    }
}


void
DrawInstance(const std::vector<BoxInfo> &boxes
             , const std::vector<psl::Instance> &instanceInfos
             , cv::Mat &image)
{
    if (instanceInfos.empty()) return;

    // visualize bbox
    size_t size = boxes.size();
    for (size_t i = 0; i < size; ++i)
    {
        const auto &face = instanceInfos[i];
        cv::Rect box = boxes[i].box;
        auto color = cv::Scalar(0, 0, 255);
        cv::rectangle(image, box, color, 2);
        // show txt
        auto font = cv::FONT_HERSHEY_SIMPLEX;
        std::stringstream ss;
        ss.precision(3);

        std::string name = CLASS_NAMES[face.classID];

        ss << face.locationMachine.x << " , " << face.locationMachine.y << " " << name;
        auto txt = ss.str();
        int baseline;
        float font_scale = 0.5;
        cv::Size cat_size = cv::getTextSize(txt, font, font_scale, 2, &baseline);

        // get color for specific class
        cv::rectangle(image, cv::Rect(box.x, box.y - cat_size.height - 2 +
                                             box.height, cat_size.width
                                      , cat_size.height), color, -1);
        int thickness = 1;
        cv::putText(image, txt, cv::Point(box.x, box.y - 2 + box.height)
                    , font, font_scale, cv::Scalar(255, 0, 0)
                    , thickness, cv::LINE_AA);
    }
}

void *OpenVideoStream(const std::string &f, int c, int w, int h, int fps)
{
    cv::VideoCapture *cap;
    if (!f.empty()) cap = new cv::VideoCapture(f.c_str());
    else cap = new cv::VideoCapture(c);
    if (!cap->isOpened()) return nullptr;
    if (w) cap->set(cv::CAP_PROP_FRAME_WIDTH, w);
    if (h) cap->set(cv::CAP_PROP_FRAME_HEIGHT, h);
    if (fps) cap->set(cv::CAP_PROP_FPS, w);
    return cap;
}

// TODO : move utils/box
float IoU(cv::Rect box0, cv::Rect box1)
{
    float xmin0 = box0.x;
    float ymin0 = box0.y;
    float xmax0 = box0.x + box0.width;
    float ymax0 = box0.y + box0.height;

    float xmin1 = box1.x;
    float ymin1 = box1.y;
    float xmax1 = box1.x + box1.width;
    float ymax1 = box1.y + box1.height;

    float w = fmax(0.0f, fmin(xmax0, xmax1) - fmax(xmin0, xmin1));
    float h = fmax(0.0f, fmin(ymax0, ymax1) - fmax(ymin0, ymin1));

    float i = w * h;
    float u = (xmax0 - xmin0) * (ymax0 - ymin0) +
              (xmax1 - xmin1) * (ymax1 - ymin1) - i;

    if (u <= 0.0) return 0.0f;
    else return i / u;
}

float GetSize(float w, float h)
{
    float pad = (w + h) * 0.5;
    return std::sqrt((w + pad) * (h + pad));
}

void softmax(float *data_in, float *data_out, int num)
{
    float exp[num];
    float total_sum = 0;
    for (int i = 0; i < num; i++)
    {
        exp[i] = std::exp(data_in[i]);
        total_sum += exp[i];
    }
    for (int i = 0; i < num; i++)
    {
        data_out[i] = exp[i] / total_sum;
    }
}

std::string GetTimestamp(std::string &date, std::string &second)
{
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
    std::time_t timestamp = ms.count();

    char now[64];
    struct tm *ttime;
    time_t t = time(0);
    ttime = localtime(&t);
    strftime(now, 64, "%Y%m%d_%H%M", ttime);
    date = now;
    strftime(now, 64, "%S", ttime);
    second = now;
    sprintf(now, "%ld", timestamp);

    return now;
}

std::time_t GetTimestamp()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
    return ms.count();
}

void PrintObject(const psl::SenmaticMap &senmaticMap)
{
    if (not g_logConfig.test) return;

    for (const auto &obj : senmaticMap.objects)
    {
        std::string message = "";
        auto id = obj.classID;

        std::ostringstream coordinate;
        coordinate.precision(2);
        coordinate.setf(std::ios::fixed);

        if (psl::ELEVATOR == id)
        {
            coordinate << " " << obj.id << " escalator [world] size "
                       << obj.locationWorld.size() << " valid: " << obj.status;;

            message = coordinate.str();
            for (auto &locationWorld: obj.locationWorld)
            {
                float minCoordinate = std::min(locationWorld.x, locationWorld.y);

                if (minCoordinate < MIN_COORDINATE)
                {
                    LOG_CHECK_TEST(INFO) << "invalid location";
                }

                std::ostringstream coordinate;
                coordinate.precision(2);
                coordinate.setf(std::ios::fixed);

                coordinate << " point : " << locationWorld.x << ", " << locationWorld.y
                           << " ";

                message = message + coordinate.str();
            }
        }
        LOG_CHECK_TEST(INFO) << " update: " << not senmaticMap.status << " [map] "<< message << "";
    }
}

void PrintObject(const std::vector<BoxInfo> &boxes)
{
    if (not g_logConfig.test) return;

    for (const auto &obj : boxes)
    {
        std::string message = "";
        auto id = obj.ConvertClassID();

        std::ostringstream coordinate;
        coordinate.precision(2);
        coordinate.setf(std::ios::fixed);

        if (obj.IsPerson())
        {
            float minCoordinate = std::min(obj.location.world.x, obj.location.world.y);

            if (minCoordinate < MIN_COORDINATE)
            {
                LOG_CHECK_TEST(INFO) << "invalid location";
            }
            coordinate << "x,y : " << obj.location.world.x << ", "
                       << obj.location.world.y;
            message = " " + CLASS_NAMES[id] + " [world] " + coordinate.str();
        }
        else if (obj.IsEscalator())
        {
            float minCoordinateLeft = std::min(obj.edgeLeft.world.x
                                               , obj.edgeLeft.world.y);
            float minCoordinateRight = std::min(obj.edgeRight.world.x
                                                , obj.edgeRight.world.y);
            float minCoordinate = std::min(minCoordinateLeft, minCoordinateRight);

            if (minCoordinate < MIN_COORDINATE)
            {
                LOG_CHECK_TEST(INFO) << "invalid location";
            }

            coordinate << "left : " << obj.edgeLeft.world.x << ", "
                       << obj.edgeLeft.world.y
                       << " right : " << obj.edgeRight.world.x << ", "
                       << obj.edgeRight.world.y;
            message = " " + CLASS_NAMES[id] + " [world] " + coordinate.str();
        }
    }
}

void PrintObject(const std::vector<psl::Instance> &instanceInfos)
{
    if (not g_logConfig.test) return;

    for (const auto &obj : instanceInfos)
    {
        std::string message = "";

        std::ostringstream coordinate;
        coordinate.precision(2);
        coordinate.setf(std::ios::fixed);

        float minCoordinate = std::min(obj.locationWorld.x, obj.locationWorld.y);

        if (minCoordinate < MIN_COORDINATE)
        {
            LOG_CHECK_TEST(INFO) << "invalid location";
        }

        coordinate << "[lidar] " << obj.locationMachine.x << ", " << obj.locationMachine.y
                   << " [world] " << obj.locationWorld.x << obj.locationWorld.y;

        if (psl::PERSON == obj.classID)
        {
            message = " " + CLASS_NAMES[obj.classID] + " " + coordinate.str();
            LOG_CHECK_TEST(INFO) << " id: " << obj.classID
                                 << message;
        }
    }
}

void Vector2Location(const Eigen::Vector3d &data, psl::Location &location)
{
    location.x = data[0];
    location.y = data[1];
    location.z = data[2];
}

void Lidar2Machine(const ConfigParam &configParam, psl::Location &pc)
{
    Eigen::Vector3d po;
    po[0] = pc.y + configParam.structure.poseLidar2Machine[0];

    if (configParam.structure.lidarDirection)
    {
        po[1] = pc.x + configParam.structure.poseLidar2Machine[1];
        po[2] = pc.z + configParam.structure.poseLidar2Machine[2];
    }
    else
    {
        po[1] = -pc.x + configParam.structure.poseLidar2Machine[1];
        po[2] = -pc.z + configParam.structure.poseLidar2Machine[2];
    }

    Vector2Location(po, pc);
}