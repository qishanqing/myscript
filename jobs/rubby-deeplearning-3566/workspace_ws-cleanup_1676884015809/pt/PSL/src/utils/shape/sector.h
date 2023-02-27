//////////////////////////////////////////////////////////////////////
///  @file     sector.h
///  @brief    sector shape
///  Details.
///
///  @author   sunhao
///  @date     2021.07.28
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#ifndef SAMPLE_SECTOR_H
#define SAMPLE_SECTOR_H
#include "psl/perception/instance.h"
#include "src/utils/shape/point.h"

class BoxInfo;

struct Sector
{
    Point vertex;
    Point left, right;
    Point leftOrg, rightOrg;

    Point leftLeft, rightRight;
    Point vertexOrigin;
    Point leftRight, rightLeft;

    bool leftUpdate, rightUpdate;
    bool isTarget;
    int id, boxID;
    int partCount;
    bool leftClose2Edge = false, rightCLose2Edge = false;
    bool invalid = false;

    Sector();

    Sector(const BoxInfo &box);

    Sector(const Point& vertex, const Point& left, const Point& right);

    bool UpdateEdge(const BoxInfo &box, const BoxInfo &view);

    bool UpdateLeft(const BoxInfo &box, const BoxInfo &view);

    bool UpdateRight(const BoxInfo &box, const BoxInfo &view);

    void UpdateVertex(const BoxInfo &box, const Point &view
                      , const bool leftUpdate = true, const bool rightUpdate = true);

    void Reset();

    void SetVertex(const Point& point);

    std::vector<Point> GetPoint() const;

    bool Read(std::ifstream &stream);

    bool Write(std::ofstream &stream) const;
};


#endif //SAMPLE_SECTOR_H
