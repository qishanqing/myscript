//////////////////////////////////////////////////////////////////////
///  @file     gemotry.h
///  @brief    some gemotry function about const Point&, line, and region
///  @author   sunhao
///  @date     2021.10.14
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_GEMOTRY_H
#define DETECTOR_SAMPLE_GEMOTRY_H

#include <iostream>
#include <cmath>
#include <algorithm>

#include "psl/perception/instance.h"
#include "src/utils/data_type.h"
#include "src/utils/shape/point.h"
#include "src/utils/shape/sector.h"

const float MAX_INVIEW = 1e+6;//点在内部面积无限大
const float MAX_OUTVIEW = MAX_INVIEW + 500;//没有点在内部时候面积无限大
const float DEFAULT_VIEW_MARGIN = 0.0001; // for view check
const float MAX_ESCALATOR_LENGTH = 5; // 500cm // TODO : move to another file
const double ANGLE_90 = 90 * ANGLE_2_RADIAN;


using namespace std;

struct Segment
{
    Point start;
    Point end;

    Segment();

    Segment(const Point &p1, const Point &p2);

    void GetLine(float &k, float &b) const;
};

double Cross(const Point &a, const Point &b, const Point &p);      //向量AB与向量AP的外积（叉积）

double Cross(const Point &a, const Point &b);  // 外积

double Dot(const Point &a, const Point &b, const Point &p);         //向量AB与向量AP的内积（点积）

double GetAngle(const Point &v, const Point &a, const Point &b);         // angle of 向量va与向量vb

double GetAngle(const Segment &a, const Segment &b);         // angle of 向量a与向量b

double Dis2(const Point &a, const Point &b);                //点a、b距离的平方

int Dir(const Point &a, const Point &b, const Point &p);            //点P与线段AB位置关系

int OnSegment(const Point &a, const Point &b, const Point &p);            //点P与线段AB位置关系

int OnFront(const Point &a, const Point &b, const Point &p);            //点P与线AB位置关系

bool Front(const Point &a, const Point &b, const Point &p);            // p in front of ab

double DistanceMin(const Point &a, const Point &b, const Point &p);      //点P到线段AB的最短距离

double LineDistance(const Point a1, const Point a2, const Point b1, const Point b2);

double Product(const Point &p1, const Point &p2, const Point &p3);

bool InTriangle(const Point &p1, const Point &p2, const Point &p3, const Point &point);

bool InTriangleSoft(const Point &p1, const Point &p2, const Point &p3
                    , const Point &point);

bool InView(const Point &p1, const Point &p2, const Point &p3, const Point &point);

bool InViewSoft(const Point &p1, const Point &p2, const Point &p3, const Point &point
                , const float margin = DEFAULT_VIEW_MARGIN);

bool GetCrossPoint4Segment(const Segment &s1, const Segment &s2, Point &point);

bool GetCrossPoint4Line(const Segment &s1, const Segment &s2, Point &point);

bool SegmentCross(const Segment &s1, const Segment &s2);

void GetCrossPoint(Segment l0, Segment l1, std::vector<psl::Location> &crossPoints);

bool PointsArea(std::vector<psl::Location> &crossPoints, float &area, bool calSort);

bool SameLine(Segment l0, Segment l1);

bool GetCrossPointTestMulti(Sector &sector1, Sector &sector2, float &area
                            , std::vector<psl::Location> &crossPoints);

bool GetCrossPointTestMulti(const std::vector<psl::Location> &sectorOriginal
                            , const std::vector<psl::Location> &sectorTest
                            , float &area, std::vector<psl::Location> &crossPoints);

bool InLine(const Point &p, const Segment &l);

bool SameDirect(const Point &v, const Point &a, const Point &b);         // 向量va与向量vb方向

bool OnLine(const Point &a, const Point &b, const Point &p);

void ExtendLine(const Point &start, const Point &end, Point &extend
        , const double &lineLength);

void ExtendSector(const std::vector<Point> &sectorOriginal
        , std::vector<Point> &sectorExtend
        ,  const double &lineLength);

void DeleteRepeatPoint(std::vector<Point> &crossPoints);

bool ComparePointX(const Point &a, const Point &b);

void ConvexPolygonArea(std::vector<Point> &crossPoints, float &area);

void CrossPointInTriangle(const std::vector<Point> &sectorExtend
        , const std::vector<Point> &sectorTest
        , std::vector<Point> &crossPoints);

void CrossPointInTriangleLine(const std::vector<Point> &sectorExtend
        , const std::vector<Point> &sectorTest
        , std::vector<Point> &crossPoints);

bool GetTriangleArea(const std::vector<Point> &sectorOriginal
        , const std::vector<Point> &sectorTest
        , float &area, std::vector<Point> &crossPoints
        , const double &lineLength);

#endif //DETECTOR_SAMPLE_GEMOTRY_H
