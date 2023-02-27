//////////////////////////////////////////////////////////////////////
///  @file     escalator.h
///  @brief    manage escalator map
///  Details.
///
///  @author   sunhao
///  @date     2021.08.11
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_ESCALATOR_H
#define DETECTOR_SAMPLE_ESCALATOR_H

#include <vector>
#include <list>
#include <map>
#include <numeric>
#include "src/utils/detector_param.h"
#include "src/utils/map_define.h"
#include "psl/perception/instance.h"
#include "src/utils/data_type.h"
#include "src/utils/shape/sector.h"

using DLObject = psl::Object;
using DLObjectLink = std::list<DLObject>::iterator;
using TimeType = long long;

const int MAX_WEIGHT = 700;
const int MAX_POINT_NUM_PER = 7;
const int MAX_POINT_NUM = 40;
const int WEIGHT_ADD = 1;
const int WEIGHT_ADD_BY_REGION = WEIGHT_ADD * 2;
const int WEIGHT_REMOVE = 3;

namespace escalator_map
{
enum Axis
{
    X = 0, Y, UNKNOWN
};

const int INIT_ID = -1;

struct Point
{
    psl::Location location;
    int weight;
    float distance2Line; // distance to line from points
    bool useful;
    bool update;
    int failCheckoutCount;
    int classID = INIT_ID;

    Point();

    Point(const psl::Location &location, const int id);

    bool InvalidWeight() const;

    void AddWeight(const int delta);

    bool Keep() const;

    void Merge(const Point &point);

    bool Read(std::ifstream &stream);

    bool Write(std::ofstream &stream) const;
};

struct Escalator
{
    std::vector<Point> handrails;
    DLObjectLink escalator;
    float k = 0, b = 0; // for the line of escalator
    bool update = true;
    int handrailsUncompleteCount = 0; // the update count from create
    Axis axis = UNKNOWN;
    static int groupNum;
    std::vector<int> useful;
    long long validPartNum = 0;
    std::map<int, TimeType> securityTime;
    BoxInfo view;
    int bindRegionID = -1;
    int id = -1;
    ClassID classID = psl::DEFAULT_ID;
    Sector region;
    bool isEscalator = false;

public:
    bool Update(const BoxInfo &box);

    bool Delete();

    void RemoveWithView(const BoxInfo &view);

    bool Fit();

    bool HaveRegion() const;

    int GetClassNum(const int &id) const;

    bool Read(std::ifstream &stream);

    bool Write(std::ofstream &stream) const;

private:
    bool Ransac(float &k, float &b);

    bool LeasrSquares(const std::vector<int> &useful, float &k, float &b);

    void UpdateEdge();

    bool UpdateHandrail(const BoxInfo &box);

    void RemoveWithDistance();

    bool IsNew() const;

    bool Uncomplete() const;

    bool InvalidPair(const Point &point1, const Point &point2) const;

    float Distance2Line(const Point &point, const float k, const float b);

    escalator_map::Axis  GetLineFromPoint(const psl::Location &point1
                                          , const psl::Location &point2
                                          , float &k, float &b);

    Axis VarianceAnalysis() const;

    void HandrailsPush(const Point &point);

    bool Close2Edge(const BoxInfo &box, const Point &point);
};
}

#endif //DETECTOR_SAMPLE_ESCALATOR_H
