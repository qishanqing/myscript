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
#include "perception/instance.h"
#include "src/utils/detector_property.h"
#include "slam/SlamResult.h"
#include "src/utils/config.h"

struct Location
{
    psl::Location machine;
    psl::Location world;

    Location();

    Location(const psl::Location &location);

    bool PoseValid() const;

    bool Inview() const;
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

    // class
    ClassID classID = UNKNOWN;
    std::string className;

    Location location; // the distance for the whole box
    std::vector<Location> locationSerials; // many distance points for the box
    Location edgeLeft, edgeRight;
    bool dummy = false; // the box is inferred from parts; e.g. handrails, no escalator
    bool sideView = false;
    bool lidarInValid = false;

    BoxInfo();

    BoxInfo &FromCorner(int xmin, int ymin, int xmax, int ymax);

    BoxInfo(const cv::Rect box);

    void SetRect(const cv::Rect box);

    BoxInfo &FromCenter(int x, int y, int w, int h);

    bool IsEscalator() const;

    bool IsEscalatorHandrails() const;

    bool IsPerson() const;

    psl::ClassID ConvertClassID() const;

    psl::Instance ConvertInstace() const;

    float IoU(const BoxInfo &box) const;

    bool Intersection(const BoxInfo &box) const;
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
    DetectParam param;
};

#endif //DETECTOR_PARAM_SAMPLE_DATA_TYPE_H
