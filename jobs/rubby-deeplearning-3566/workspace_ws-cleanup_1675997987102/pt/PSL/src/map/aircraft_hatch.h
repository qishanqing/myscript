//
// Created by donglijian on 11/6/22.
//

#ifndef SAMPLE_AIRCRAFTHATCH_H
#define SAMPLE_AIRCRAFTHATCH_H
#include "utils/utils.h"
#include "src/utils/gemotry.h"
#include "src/map/map.h"
#include "src/data/observer_data.h"

struct LidarSegment
{
    enum Type
    {
        INSIDE, OUTSIDE, NORMAL
    };
    int start = 0;
    int end = 0;
    Type type = NORMAL;
    float minValue = 0;

    LidarSegment()
    {}

    LidarSegment(int start, int end, Type type, float minValue = 0)
    {
        this->start = start;
        this->end = end;
        this->type = type;
        this->minValue = minValue;
    }

    int Length() const
    {
        return this->end - this->start;
    }

    std::string Print() const
    {
        std::stringstream stream;

        stream << "(" << start << ", " << end << ") " << type << " " << this->Length();
        return stream.str();
    }
};

class AircraftHatch : public Map
{
    using cv_rect_world = cv::Rect2f;

public:
    AircraftHatch();

    ~AircraftHatch()
    {}
    cv::Mat Show(const BoxInfo &view, const std::vector <BoxInfo> &boxes, cv::Mat image) override
    {}

    bool Save(std::ofstream &ofstream) override
    {}

    bool Load(std::ifstream &ifstream) override
    {}

    void Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view
                , const psl::Time &time) override;

    void GetMap(psl::SenmaticMap &senmaticMap) const;


    void BoxesObjectIOU(const std::vector<BoxInfo> &boxes
                        , std::vector<std::vector<float>> &ious);

private:

    void Fresh(const std::vector<BoxInfo> &boxes) override;

    void FilterClass(const std::vector<BoxInfo> &boxes, std::vector<BoxInfo> &boxesGate);

    DLObject Box2Object(const BoxInfo &box) override;

    void Reset();

    void BindByIOU(const std::vector<std::vector<float>> &iouMap, std::vector<int> &indexes);

    void UpdateByBinds(const std::vector<BoxInfo> boxes, const std::vector<int> &binds);

    void Add(const BoxInfo &box);

    void Trapezoid2Rect(const psl::Location &leftCrossPoint, const psl::Location &rightCrossPoint
                        , const psl::Location &edgeLeft, const psl::Location &edgeRight
                        , cv_rect_world &rect) const;

    void LineExtendRectParallel(const Segment &line, std::vector<Point> &points, const float extend) const;

    void BoxExtendRect(const BoxInfo &box, std::vector<Point> points);

    void LineExtendRect(const Segment &line, std::vector<Point> &points
                        , const float extend) const;

    void PointsExtendRect(const Point &left, const Point &right, cv_rect_world &rect);

private:
    psl::SenmaticMap aircraftHatchesMap;
};


#endif //SAMPLE_AIRCRAFTHATCH_H
