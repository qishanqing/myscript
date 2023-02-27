//////////////////////////////////////////////////////////////////////
///  @file     sort.h
///  @brief    多目标跟踪算法——sort 头文件
///  Details.
///
///  @author   孙昊
///  @version  1.0.0
///  @date     2019.03.15
//////////////////////////////////////////////////////////////////////
#ifndef SORT_H
#define SORT_H

#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <set>
#include "src/utils/shape/shape.h"
#include "src/utils/shape/rect.h"
#include "map"

// global variables for counting
class KalmanTracker;

using ShapeType = Rect2f ;
using ShapeList = std::vector<ShapeType>;

template <class T>
struct TrackingBox
{
    unsigned int frame = 0;
    unsigned int id = 0;
    T region = Rect2f(0, 0, 0, 0);
};

class Sort
{
public:
    Sort();

    ~Sort();

    void Update(const ShapeList &boxes, std::vector<TrackingBox<Rect2f>> &results, const int &id);

    void RemoveByIDs(const std::vector<int> ids);

    void UpdateByIDs(const std::map<int, int> updateIDs);

private:
    double IOU(cv::Rect2f test, cv::Rect2f groundtruth);

    void Predict(ShapeList &boxes);

    void GetIOUMatrix(const ShapeList &boxesLast, const ShapeList &boxes
                      , std::vector<std::vector<double>> &iouMatrix);

    void Tracker(const ShapeList &boxesLast, const ShapeList &boxesCurrent
                 , std::vector<TrackingBox<Rect2f>> &result);

    void Update(const std::vector<cv::Point_<size_t>> matchedPairs
                , const std::set<unsigned int> fresh
                , const ShapeList &boxes
                , std::vector<TrackingBox<Rect2f>> &result);

    void SetMinID(const int id);

private:
    double iouThreshold;
    std::vector<KalmanTracker> trackers;
    const unsigned int MAX_AGE;
    const unsigned int MIN_HISTS;
    unsigned int frameNum;
};

#endif // SORT_H
