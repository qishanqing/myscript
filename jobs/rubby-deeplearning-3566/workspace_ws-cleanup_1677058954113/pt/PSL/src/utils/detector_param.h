//////////////////////////////////////////////////////////////////////
///  @file     data_type.h
///  @brief    base types
///  @author   sunhao
///  @date     2021.07.02
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_PARAM_SAMPLE_DATA_TYPE_H
#define DETECTOR_PARAM_SAMPLE_DATA_TYPE_H

#include <vector>
#include <iostream>
#include <string>
#include "psl/perception/instance.h"
#include "src/utils/detector_property.h"
#include "psl/slam/SlamResult.h"
#include "src/utils/config.h"
#include "psl/perception/detector_param.h"
#include "src/utils/shape/rect.h"

struct Location
{
    psl::Location machine;
    psl::Location world;

    Location();

    Location(const psl::Location &location);

    bool PoseValid() const;

    bool Inview() const;
};

struct Keypoint
{
    float point[8];
};


// TODO : refactor the struct for box and cx, cy;, set value in once
struct BoxInfo
{
    cv::Rect box;
    float score;
    float cx;
    float cy;
    // in some case, float type is needed
    float width;
    float height;

    //keypoint
    Keypoint keypoint;

    // class
    ClassID classID = psl::DEFAULT_ID;
    std::string className;
    int id = -1;

    Location location; // the distance for the whole box
    std::vector<Location> locationSerials; // many distance points for the box
    Location edgeLeft, edgeRight;
    bool dummy = false; // the box is inferred from parts; e.g. handrails, no escalator
    bool sideView = false;
    bool lidarInValid = false;

    BoxInfo();

    BoxInfo &FromCorner(int xmin, int ymin, int xmax, int ymax);

    BoxInfo(const cv::Rect box);

    BoxInfo(const cv::Rect box, Keypoint keypoint);

    void SetRect(const cv::Rect box);

    void SetRect(const cv::Rect box, Keypoint keypoint);

    void SetRect(const Rect box);

    BoxInfo &FromCenter(int x, int y, int w, int h);

    bool IsEscalator() const;

    bool IsEscalatorHandrails() const;

    bool IsPerson() const;

    bool IsDesk() const;

    bool IsAircraftHatch() const;

    psl::Instance ConvertInstace() const;

    float IoU(const BoxInfo &box) const;

    bool Intersection(const BoxInfo &box) const;

    std::string PrintKeopoint() const;
};

struct DetectResult
{
    std::vector<BoxInfo> boxes;
    uint64 time;
    psl::SlamResult pose;

    bool Empty() const;

    void Swap(DetectResult &ret);
};

struct DetectorOptions
{
    std::string modelName;
    std::string backend;
    float nmsThreshold;
    float scoreThreshold;
    int topk;
    psl::ModelProperty param;
};

#endif //DETECTOR_PARAM_SAMPLE_DATA_TYPE_H
