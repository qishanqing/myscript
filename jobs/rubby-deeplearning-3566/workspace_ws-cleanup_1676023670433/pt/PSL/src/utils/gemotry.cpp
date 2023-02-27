#include "gemotry.h"
#include "src/utils/define.h"

const double TRIANGLE_MAGIN = 0.01;
const double NEAREST_CROSS_POINT = 0.003;

double Cross(const Point &a, const Point &b, const Point &p)
{
    Point AB = {b.x - a.x, b.y - a.y, 0};
    Point AP = {p.x - a.x, p.y - a.y, 0};
    return AB.x * AP.y - AB.y * AP.x;
}

double Dot(const Point &a, const Point &b, const Point &p)
{
    Point AB = {b.x - a.x, b.y - a.y, 0};
    Point AP = {p.x - a.x, p.y - a.y, 0};
    return AB.x * AP.x + AB.y * AP.y;
}

double Dis2(const Point &a, const Point &b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int Dir(const Point &a, const Point &b, const Point &p)
{
    if (Cross(a, b, p) < 0) return -1;     //逆时针
    else if (Cross(a, b, p) > 0) return 1;   //顺时针
    else if (Dot(a, b, p) < 0) return -2;  //反延长线
    else if (Dot(a, b, p) >= 0 && Dis2(a, b) >= Dis2(a, p))
    {
        if (Dis2(a, b) < Dis2(a, p)) return 2;  //延长线
        return 0;                          //在线上
    }
    else
    {
        return 3; // A and B is the same point
    }
}

bool Front(const Point &a, const Point &b, const Point &p)
{

    return (Dis2(a, b) < Dis2(a, p));
}

double DistanceMin(const Point &a, const Point &b, const Point &p)
{
    double distAB = Dis2(a, b);

    if (distAB < 0.1) return Dis2(a, p);

    double r = ((p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y)) / distAB;
    if (r <= 0) return sqrt(Dis2(a, p));
    else if (r >= 1) return sqrt(Dis2(b, p));
    else
    {
        double AC = r * sqrt(Dis2(a, b));
        return sqrt(Dis2(a, p) - AC * AC);
    }
}

double LineDistance(const Point a1, const Point a2, const Point b1, const Point b2)
{
    double dist = -1;

    if (Dir(a1, a2, b1) * Dir(a1, a2, b2) <= 0 &&
        Dir(b1, b2, a1) * Dir(b1, b2, a2) <= 0)  //两线段相交, 距离为0
    {
        dist = 0;
    }
    else      //如不相交, 则最短距离为每个端点到另一条线段距离的最小值
    {
        dist = min({DistanceMin(a1, a2, b1)
                    , DistanceMin(a1, a2, b2)
                    , DistanceMin(b1, b2, a1)
                    , DistanceMin(b1, b2, a2)});
    }

    return dist;
}

bool InTriangle(const psl::Location &p1, const psl::Location &p2
                , const psl::Location &p3
                , const psl::Location &point)
{
    // ensure p1，p2，p3 are in counterclockwise
    if (Product(p1, p2, p3) < 0)
    {
        return InTriangle(p1, p3, p2, point);
    }
//
//    if (OnLine(p1, p2, point) or OnLine(p2, p3, point) or OnLine(p3, p1, point))
//        return true;

    if (Product(p1, p2, point) > 0 && Product(p2, p3, point) > 0 &&
        Product(p3, p1, point) > 0)
        return true;
    return false;
}

double Product(const psl::Location &p1, const psl::Location &p2
               , const psl::Location &p3)
{
    //get p1p2 and  p1p3 vector，then get the cross product
    //p1p2 vector:  (p2.x-p1.x,p2.y-p1.y)
    //p1p3 vector: (p3.x-p1.x,p3.y-p1.y)

    return (p2.x - p1.x) * (p3.y - p1.y)
           - (p2.y - p1.y) * (p3.x - p1.x);
}

double Cross(const Point &a, const Point &b)
{
    return a.x * b.y - a.y * b.x;
}

bool GetCrossPoint4Line(const Segment &s1, const Segment &s2, Point &point)
{
    float k1, b1, k2, b2;
    s1.GetLine(k1, b1);
    s2.GetLine(k2, b2);

    point.z = 0;
    float x, y;

    if (abs(k1 - k2) > 0)
    {
        x = (b2 - b1) / (k1 - k2);
        y = k1 * x + b1;
        point.x = x;
        point.y = y;
    }
    else
    {
        return false;
    }

    return true;
}

bool GetCrossPoint4Segment(const Segment &s1, const Segment &s2, Point &point)
{
    // TODO : if not cross return 0
    if (not(Dir(s1.start, s1.end, s2.start) * Dir(s1.start, s1.end, s2.end) <= 0 &&
            Dir(s2.start, s2.end, s1.start) * Dir(s2.start, s2.end, s1.end) <= 0))
    {
        return false;
    }

    // get cross point
    Point base = s2.end - s2.start;
    double d1 = abs(Cross(base, s1.start - s2.start));
    double d2 = abs(Cross(base, s1.end - s2.start));
    double t = d1 / (d1 + d2);
    point = s1.start +
            (s1.end - s1.start) / Point(1 / t, 1 / t, 1 / t); // TODO : use mult
    point.z = 0;

    return true;
}

bool SegmentCross(const Segment &s1, const Segment &s2)
{
    return Dir(s1.start, s1.end, s2.start) * Dir(s1.start, s1.end, s2.end) <= 0 &&
           Dir(s2.start, s2.end, s1.start) * Dir(s2.start, s2.end, s1.end) <= 0;
}

bool InView(const Point &p1, const Point &p2, const Point &p3, const Point &point)
{
    if (Product(p2, p1, p3) < 0) return InView(p1, p3, p2, point);

    if (Product(p1, p2, point) < 0 && Product(p1, p3, point) > 0) return true;
    return false;
}

bool InViewSoft(const Point &p1, const Point &p2, const Point &p3, const Point &point
                , const float margin)
{
    if (Product(p2, p1, p3) < 0) return InViewSoft(p1, p3, p2, point);

    float product1 = Product(p1, p2, point);
    float product2 = Product(p1, p3, point);
    if ((product1 < 0 or abs(product1) < margin)
        && (product2 > 0 or abs(product2) < margin))
        return true;
    return false;
}

double GetAngle(const Point &v, const Point &a, const Point &b)
{
    Point va = {a.x - v.x, a.y - v.y, 0};
    Point vb = {b.x - v.x, b.y - v.y, 0};
    double dot = (va.x * vb.x + va.y * vb.y);
    return acos(dot / sqrt((va.x * va.x + va.y * va.y) * (vb.x * vb.x + vb.y * vb.y)));
}

double GetAngle(const Segment &a, const Segment &b)
{
    Point va = {a.end.x - a.start.x, a.end.y - a.start.y, 0};
    Point vb = {b.end.x - b.start.x, b.end.y - b.start.y, 0};
    double dot = (va.x * vb.x + va.y * vb.y);
    return acos(dot / sqrt((va.x * va.x + va.y * va.y) * (vb.x * vb.x + vb.y * vb.y)));
}


Segment::Segment()
{

}

Segment::Segment(const psl::Location &p1, const psl::Location &p2)
{
    start = p1;
    end = p2;

    start.z = 0;
    end.z = 0;
}

void Segment::GetLine(float &k, float &b) const
{
    k = (end.y - start.y) / (end.x - start.x);
    b = end.y - k * end.x;
}

void GetCrossPoint(Segment l0, Segment l1, std::vector<psl::Location> &crossPoints)
{
    psl::Location crossPoint;

    if (GetCrossPoint4Line(l0, l1, crossPoint))
    {
        if ((l0.end.x - l0.start.x) * (crossPoint.x - l0.start.x) >= 0
            && (l1.end.x - l1.start.x) * (crossPoint.x - l1.start.x) >= 0)
        {
            crossPoints.push_back(crossPoint);
        }
    }
}

bool PointsArea(std::vector<psl::Location> &crossPoints, float &area, bool calSort)
{
    psl::Location l1;
    psl::Location l2;
    if (crossPoints.size() == 3)
    {
        l1 = crossPoints[1] - crossPoints[0];
        l2 = crossPoints[2] - crossPoints[1];
        area = (1 / 2.0) * abs(l1.x * l2.y - l1.y * l2.x);
    }
    else if (crossPoints.size() == 4)
    {
        if (calSort)
        {
            l1 = crossPoints[1] - crossPoints[0];
            l2 = crossPoints[3] - crossPoints[2];
        }
        else
        {
            l1 = crossPoints[2] - crossPoints[0];
            l2 = crossPoints[3] - crossPoints[1];
        }
        area = (1 / 2.0) * abs(l1.x * l2.y - l1.y * l2.x);
    }
    else
    {
        // PointsArea error! and points' number wrong
        return false;
    }

    return true;
}

bool SameLine(Segment l0, Segment l1)
{
    psl::Location line0 = l0.end - l0.start;
    psl::Location line1 = l1.end - l1.start;

    return (line0.x * line1.y - line0.y * line1.x) == 0;
}

bool GetCrossPointTestMulti(Sector &sector1, Sector &sector2, float &area
                            , std::vector<psl::Location> &crossPoints)
{

    cv::Scalar colorObject(50, 250, 50);
    const float nowInView = -1;

    psl::Location vertex1, left1, right1, vertex2, left2, right2;
    vertex1 = sector1.vertex;
    left1 = sector1.left;
    right1 = sector1.right;
    vertex2 = sector2.vertex;
    left2 = sector2.left;
    right2 = sector2.right;

    Segment l0 = Segment(sector1.vertex, sector1.left);
    Segment l1 = Segment(sector1.vertex, sector1.right);
    Segment l2 = Segment(sector2.vertex, sector2.left);
    Segment l3 = Segment(sector2.vertex, sector2.right);

    if (abs(vertex1.x - vertex2.x) < 1e-6 && abs(vertex1.y - vertex2.y) < 1e-6)
    {
        if (InViewSoft(vertex1, left1, right1, left2) or
            InViewSoft(vertex1, left1, right1, right2)
            or InViewSoft(vertex2, left2, right2, left1) or
            InViewSoft(vertex2, left2, right2, right1))
        {
            area = MAX_INVIEW;
        }
        else
        {
            area = nowInView;
        }
        return true;
    }

    //如果两个顶点都在内部，则会有4个交点，且能计算面积
    if (InViewSoft(vertex1, left1, right1, vertex2) &&
        InViewSoft(vertex2, left2, right2, vertex1))
    {
        crossPoints.push_back(vertex1);
        crossPoints.push_back(vertex2);
        GetCrossPoint(l0, l2, crossPoints);
        GetCrossPoint(l0, l3, crossPoints);
        GetCrossPoint(l1, l2, crossPoints);
        GetCrossPoint(l1, l3, crossPoints);

        PointsArea(crossPoints, area, true);

        return true;
    }

    //其中一个顶点在内部，讨论3个交点时候能不能计算面积
    if (InViewSoft(vertex1, left1, right1, vertex2))
    {
        crossPoints.push_back(vertex2);
        GetCrossPoint(l0, l2, crossPoints);
        GetCrossPoint(l0, l3, crossPoints);
        int pointSize = crossPoints.size();
        if (pointSize == 3)
        {
            PointsArea(crossPoints, area, true);
            return true;
        }

        GetCrossPoint(l1, l2, crossPoints);
        GetCrossPoint(l1, l3, crossPoints);

        if (pointSize == 1 && crossPoints.size() == 3)
        {
            PointsArea(crossPoints, area, true);
            return true;
        }
        area = MAX_INVIEW;
        return true;
    }

    //另外一个顶点在内部，讨论3个交点时候能不能计算面积
    if (InViewSoft(vertex2, left2, right2, vertex1))
    {
        crossPoints.push_back(vertex1);
        GetCrossPoint(l2, l0, crossPoints);
        GetCrossPoint(l2, l1, crossPoints);
        int pointSize = crossPoints.size();
        if (pointSize == 3)
        {
            PointsArea(crossPoints, area, true);
            return true;
        }

        GetCrossPoint(l3, l1, crossPoints);
        GetCrossPoint(l3, l1, crossPoints);

        if (pointSize == 1 && crossPoints.size() == 3)
        {
            PointsArea(crossPoints, area, true);
            return true;
        }
        area = MAX_INVIEW;
        return true;
    }

    GetCrossPoint(l0, l2, crossPoints);
    GetCrossPoint(l0, l3, crossPoints);
    GetCrossPoint(l1, l3, crossPoints);
    GetCrossPoint(l1, l2, crossPoints);

    //4个交点能计算面积
    if (crossPoints.size() == 4)
    {
        PointsArea(crossPoints, area, false);
        return true;
    }
        //三个交点，且其中存在线段共线情况，共线直线目前没有交点
    else if (crossPoints.size() == 3)
    {
        if (SameLine(l0, l2) or SameLine(l0, l3) or SameLine(l1, l2) or SameLine(l2, l3))
        {
            PointsArea(crossPoints, area, false);
        }
        else
        {
            area = MAX_INVIEW;
        }
        return true;
    }
    else if (crossPoints.size() == 2)
    {
        area = MAX_INVIEW;
        return true;
    }
        //顶点都不在内部，面积不能计算情况
    else if (crossPoints.size() > 0 && crossPoints.size() < 4)
    {
        area = MAX_OUTVIEW;
        return true;
    }
        //顶点都不在内部，无交点情况
    else if (crossPoints.size() == 0)
    {
        area = nowInView;
        return true;
    }
    else
    {
        // area calcualte error!crossPoints.size()
        return false;
    }

    return true;
}

bool GetCrossPointTestMulti(const std::vector<psl::Location> &sectorOriginal
                            , const std::vector<psl::Location> &sectorTest
                            , float &area, std::vector<psl::Location> &crossPoints)
{
    Sector origin(sectorOriginal[0], sectorOriginal[1], sectorOriginal[2]);
    Sector test(sectorTest[0], sectorTest[1], sectorTest[2]);
    GetCrossPointTestMulti(origin, test, area, crossPoints);
}

int OnSegment(const Point &a, const Point &b, const Point &p)
{
    int ret = Dir(a, b, p);

    return ret == 0 or ret == 3;
}

int OnFront(const Point &a, const Point &b, const Point &p)
{
    auto ab = a - b;
    auto ap = a - p;

    return ab.x * ap.x > 0 and ab.y * ap.y > 0;
}

bool InTriangleSoft(const Point &p1, const Point &p2, const Point &p3, const Point &point)
{
    // ensure p1，p2，p3 are in counterclockwise
    if (Product(p1, p2, p3) < 0)
    {
        return InTriangleSoft(p1, p3, p2, point);
    }
//
//    if (OnLine(p1, p2, point) or OnLine(p2, p3, point) or OnLine(p3, p1, point))
//        return true;

    double ret = MIN(MIN(Product(p1, p2, point), Product(p2, p3, point)), Product(p3, p1
                                                                                  , point));

    return ret > -TRIANGLE_MAGIN;
}

bool InLine(const Point &p, const Segment &l)
{
    return (not(p.x < l.start.x ^ p.x > l.end.x))
           and (not(p.y < l.start.y ^ p.y > l.end.y));
}

bool SameDirect(const Point &v, const Point &a, const Point &b)
{
    return GetAngle(v, a, b) < 1.5; // 90
}

void ExtendLine(const Point &start, const Point &end, Point &extend
        , const double &lineLength)
{
    double scale = lineLength / end.DistanceXY(start);

    Point temp = end - start;
    temp.x = temp.x * scale;
    temp.y = temp.y * scale;
    extend = temp + start;
}

bool OnLine(const Point &a, const Point &b, const Point &p)
{
    auto ap = p - a;
    auto bp = p - b;
    return ap.y * bp.y <= 0 and ap.x * bp.x <= 0;
}

void ExtendSector(const std::vector<Point> &sectorOriginal
        , std::vector<Point> &sectorExtend, const double &lineLength)
{
    Point extendFirst, extendSecond;
    ExtendLine(sectorOriginal[0], sectorOriginal[1], extendFirst, lineLength);
    ExtendLine(sectorOriginal[0], sectorOriginal[2], extendSecond, lineLength);
    sectorExtend.push_back(sectorOriginal[0]);
    sectorExtend.push_back(extendFirst);
    sectorExtend.push_back(extendSecond);
}

void DeleteRepeatPoint(std::vector<Point> &crossPoints)
{
    for (int i = 0; i < crossPoints.size(); ++i)
    {
        for (int j = i + 1; j < crossPoints.size(); ++j)
        {
            if (crossPoints[i].DistanceXY(crossPoints[j]) < 1e-6)
            {
                crossPoints.erase(crossPoints.begin() + j);
                --j;
            }
        }
    }
}

bool ComparePointX(const Point &a, const Point &b)
{
    return a.x < b.x;
}

void ConvexPolygonArea(std::vector<Point> &crossPoints, float &area)
{
    area = 0;
    if (crossPoints.size() < 3) return;
    sort(crossPoints.begin(), crossPoints.end(), ComparePointX);

    Point p1, p2;
    p1 = crossPoints[0];
    p2 = crossPoints[1];

    for (int i = 2; i < crossPoints.size(); ++i)
    {
        Point l1 = crossPoints[i] - p1;
        Point l2 = crossPoints[i] - p2;
        area += (1 / 2.0) * abs(l1.x * l2.y - l1.y * l2.x);
        if (i != crossPoints.size() - 1)
        {
            if (SegmentCross(Segment(p1, crossPoints[i]), Segment(p2, crossPoints[i+1])))
            {
                p2 = crossPoints[i];
            }
            else if (SegmentCross(Segment(p2, crossPoints[i]), Segment(p1, crossPoints[i+1])))
            {
                p1 = crossPoints[i];
            }
            else
            {
                if (p1.DistanceXY(crossPoints[i]) < p2.DistanceXY(crossPoints[i]))
                {
                    p1 = crossPoints[i];
                }
                else
                {
                    p2 = crossPoints[i];
                }

                LOG_CHECK_DEBUG(INFO)<<"CrossPoints on one line ,change nearest p1 or p2 to update";
            }
        }
    }
}

void CrossPointInTriangle(const std::vector<Point> &sectorExtend
        , const std::vector<Point> &sectorTest
        , std::vector<Point> &crossPoints)
{
    for (int i = 0; i < 3; ++i)
    {
        if (InTriangleSoft(sectorExtend[0], sectorExtend[1], sectorExtend[2], sectorTest[i]))
        {
            bool close2Edge = false;

            for (int j = 0; j < crossPoints.size(); j++)
            {
                if (sectorTest[i].DistanceXY(crossPoints[j]) < NEAREST_CROSS_POINT)
                {
                    close2Edge = true;
                    break;
                }
            }

            if (not close2Edge) crossPoints.push_back(sectorTest[i]);
        }

        if (InTriangleSoft(sectorTest[0], sectorTest[1], sectorTest[2], sectorExtend[i]))
        {
            bool close2Edge = false;

            for (int j = 0; j < crossPoints.size(); j++)
            {
                if (sectorExtend[i].DistanceXY(crossPoints[j]) < NEAREST_CROSS_POINT)
                {
                    close2Edge = true;
                    break;
                }
            }

            if (not close2Edge) crossPoints.push_back(sectorExtend[i]);
        }
    }
}

void CrossPointInTriangleLine(const std::vector<Point> &sectorExtend
        , const std::vector<Point> &sectorTest
        , std::vector<Point> &crossPoints)
{
    std::vector<Segment> segmentExtend, segmentTest;
    segmentExtend.push_back(Segment(sectorExtend[0], sectorExtend[1]));
    segmentExtend.push_back(Segment(sectorExtend[0], sectorExtend[2]));
    segmentExtend.push_back(Segment(sectorExtend[1], sectorExtend[2]));

    segmentTest.push_back(Segment(sectorTest[0], sectorTest[1]));
    segmentTest.push_back(Segment(sectorTest[0], sectorTest[2]));
    segmentTest.push_back(Segment(sectorTest[1], sectorTest[2]));

    Point crossPoint;
    for (int i = 0; i < segmentExtend.size(); ++i)
    {
        for (int j = 0; j < segmentTest.size(); ++j)
        {
            if (GetCrossPoint4Segment(segmentExtend[i], segmentTest[j], crossPoint))
            {
                if (OnLine(segmentExtend[i].start, segmentExtend[i].end, crossPoint)
                    and (OnLine(segmentTest[j].start, segmentTest[j].end, crossPoint)))
                {
                    crossPoints.push_back(crossPoint);
                }
            }
        }
    }
}

bool GetTriangleArea(const std::vector<Point> &sectorOriginal
        , const std::vector<Point> &sectorTest
        , float &area, std::vector<Point> &crossPoints
        , const double &lineLength)
{
    if (sectorOriginal.size() != 3 or sectorTest.size() != 3)
    {
        std::cout<<"Triangle with wrong points"<<std::endl;
        return false;
    }

    std::vector<Point> sectorExtend;
    ExtendSector(sectorOriginal, sectorExtend, lineLength);
    CrossPointInTriangleLine(sectorExtend, sectorTest, crossPoints);

    CrossPointInTriangle(sectorExtend, sectorTest, crossPoints);

    DeleteRepeatPoint(crossPoints);
    if (crossPoints.size() < 3)
    {
        area = 0;
        return true;
    }
    ConvexPolygonArea(crossPoints, area);
}