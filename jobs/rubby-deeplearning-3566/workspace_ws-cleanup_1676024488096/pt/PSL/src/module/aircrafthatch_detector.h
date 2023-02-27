//
// Created by donglijian on 11/8/22.
//

#ifndef SAMPLE_AIRCRAFTHATCH_DETECTOR_H
#define SAMPLE_AIRCRAFTHATCH_DETECTOR_H
#include "src/data/observer_data.h"
#include "utils/utils.h"
#include "src/utils/gemotry.h"

const float MIN_OBJ_LENGTH = 7;
const float MIN_OBJ_RATION = 2.5;

namespace inner
{
using PointType = cv::Point2f;

struct Convex
{
    std::vector<PointType> rotateBox;
    std::vector<PointType> minRect;
    cv::RotatedRect minRectInfo;
    float area = 0;
    float ratio = 0;
    float axisAngle = 0;

    Convex();

    Convex(std::vector<PointType> &rotateBox);

    void SetConvex(std::vector<PointType> &rotateBox);

    bool Invalid() const;

    float GetAxis(const std::vector<PointType> &points);

    std::vector<PointType> Rotate(const std::vector<PointType> &points, const float theta);

};

struct LidarSegment
{
    enum Type
    {
        INSIDE, OUTSIDE, NORMAL
    };
    int start = 0;
    int end = 0;
    Type type = NORMAL;
    float head = 0, tail = 0, medianValue = 0;

    LidarSegment()
    {}

    LidarSegment(int start, int end, Type type, float head = 0
            , float tail = 0, float medianValue = 0)
    {
        this->start = start;
        this->end = end;
        this->type = type;
        this->head = head;
        this->tail = tail;
        this->medianValue = medianValue;
    }

    int Length() const
    {
        return this->end - this->start;
    }

    std::string Print() const
    {
        std::stringstream stream;

        stream << "(" << start << ", " << end << ") " << type << " " << this->Length()
               << " min value: " << head
               << " max value: " << tail
               << " max value: " << medianValue;
        return stream.str();
    }
};

class AircraftHatchDetector : public ObserverData
{
public:
    AircraftHatchDetector();

    ~AircraftHatchDetector() override;

    bool Detect(std::vector<BoxInfo> &boxes, DataGroup &item);

    bool Detect(const Lidar &lidar, std::vector<BoxInfo> &boxes);

    void Update(const DataGroup &item); // set data for async

    void Update(const Lidar &lidar); // not used

    cv::Mat Show(const ConfigParam &configParam, cv::Mat image);

private:
    bool Detect();

    void Clear();

    void Polar2Cartesian(const swr::psl_sensor_types::LaserScan &lidarOrigin
                         , std::vector<PointType> &lidarPoints);

    void Gradient(const std::vector<float> &ranges, std::vector<float> &gradient);

    bool WaveSegment(const std::vector<float> &ranges, const int start, const int end
                     , const std::vector<float> &gradient, std::vector<LidarSegment> &segment);

    float Median(const std::vector<float> &ranges);

    void GradientSmooth(const std::vector<float> &ranges, std::vector<float> &gradient);

    bool IsINF(const float value);

    bool IsINF(const std::vector<float> &ranges, const int start, const int end);

    std::vector<PointType>
    GetConvex(const std::vector<float> &ranges, std::vector<PointType> &pointsClean
            , const std::vector<LidarSegment> &segments);

    template<class T>
    std::vector<int> SortByIndex(const std::vector<T> &data, bool inverse = false)
    {
        std::vector<int> index;

        for (int i = 0; i < data.size(); i++)
        {
            index.push_back(i);
        }

        sort(index.begin(), index.end(), [inverse, data, index](int a, int b)
        {
            if (inverse) return (data[index[a]] > data[index[b]]);
            else return (data[index[a]] < data[index[b]]);
        });

        return index;
    }

    std::vector<PointType> GrahamScan(std::vector<PointType> location);

    double CrossProduct(PointType p0, PointType p1, PointType p2);

    double Dis(PointType p1, PointType p2);

    float Distance(const PointType &p1, const PointType &p2);

    std::vector<PointType> Dilation(const double radius, const std::vector<PointType> &location);

    PointType Centroid(const std::vector<PointType> &vertexes);

    std::vector<std::pair<int, int>>
    CheckGate(const std::vector<float> &ranges, const std::vector<PointType> &points
              , const std::vector<float> &gradient, std::vector<LidarSegment> &segment
              , const Convex &convex, const bool axisSame);

    void GetGate(const std::vector<std::pair<int, int>> &gates
                 , const std::vector<PointType> &points);

    void ReSet();

    void ShowLidar(cv::Mat &image, std::vector<PointType> points);

    PointType Scale(const PointType &points);

    void DrawConvex(cv::Mat &image, const Convex &convex);

    void Lidar2World(std::vector<PointType> &points, const ConfigParam &configParam);

private:
    Lidar lidarCurrent;
    Lidar lidarLastest;
    std::mutex lidarLock;
    std::vector<BoxInfo> boxes;
    bool running;
    std::vector<PointType> points;
    Convex convex;
};
}

#endif //SAMPLE_AIRCRAFTHATCH_DETECTOR_H
