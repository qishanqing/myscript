//
// Created by donglijian on 11/6/22.
//

#include "aircraft_hatch.h"
#include "src/utils/define.h"

const float IOU_THREAD = 0.5;
const float EXTEND_DIS = 0.1;
AircraftHatch::AircraftHatch()
{}
DLObject AircraftHatch::Box2Object(const BoxInfo &box)
{
    DLObject object = Map::Box2Object(box);

    object.locationWorld.push_back(box.edgeLeft.world);
    object.locationWorld.push_back(box.edgeRight.world);

    return object;
}

void AircraftHatch::Reset()
{
    mapUpdate = false;
}

void AircraftHatch::Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view
                           , const psl::Time &time)
{
    Timer timer;
    Map::Update(boxes, view, time);

    this->Reset();

    if (not boxes.empty())
    {
        LOG_CHECK_DEBUG(INFO) << "Boxes.size: " << boxes.size();
        this->time = time;
        Fresh(boxes);
    }

    LOG_CHECK_DEBUG(INFO) << "AircraftHatch objects size: " << this->aircraftHatchesMap.objects.size();
    LOG_CHECK_TIME(INFO) << timer.TimingStr();
}

void AircraftHatch::FilterClass(const std::vector<BoxInfo> &boxes, std::vector<BoxInfo> &boxesGate)
{
    for (const auto &box : boxes)
    {
        if (not box.IsAircraftHatch()) continue;
        boxesGate.push_back(box);

        LOG_CHECK_DEBUG(INFO) << "box.classId: " << box.classID << ", box.edgeLeft.machine: "
                              << box.edgeLeft.machine.Print() << ", box.edgeRight.machine.Print(): "
                              << box.edgeRight.machine.Print();
    }
}

void AircraftHatch::Fresh(const std::vector<BoxInfo> &boxes)
{
    std::vector<BoxInfo> boxesGate;
    std::vector<std::vector<float>> ious;
    std::vector<int> binds;

    FilterClass(boxes, boxesGate);

    BoxesObjectIOU(boxesGate, ious);

    BindByIOU(ious, binds);
//    BindByMaxIOU(ious, binds);

    UpdateByBinds(boxesGate, binds);
}

void AircraftHatch::GetMap(psl::SenmaticMap &senmaticMap) const
{
    for (const auto &object : this->aircraftHatchesMap.objects)
    {
        senmaticMap.objects.push_back(object);

        if (mapUpdate)
            senmaticMap.status = psl::SenmaticMap::Status::UPDATE;
        else
            senmaticMap.status = psl::SenmaticMap::Status::NO_UPDATE;
    }
}

void AircraftHatch::BoxesObjectIOU(const std::vector<BoxInfo> &boxes
                                   , std::vector<std::vector<float>> &ious)
{

    for (const auto &box : boxes)
    {
        vector<float> iou;
        cv_rect_world boxRect;

        LOG_CHECK_DEBUG(INFO) << "Aircraft Box.classID: " << box.classID << ", Box.id: "
                              << box.id << ", box.edgeLeft.world: " << box.edgeLeft.world.Print()
                              << " , box.edgeRight.world: " << box.edgeRight.world.Print()
                              << ", box.edgeLeft.machine: " << box.edgeLeft.machine.Print()
                              << ", box.edgeRight.machine: " << box.edgeRight.machine.Print();

        PointsExtendRect(box.edgeLeft.world, box.edgeRight.world, boxRect);

        // TODO : 2022-10-18 11:30:14 [hao] use smallObjectMapInner
        for (auto object : this->aircraftHatchesMap.objects)
        {
            if (box.classID == object.classID)
            {
                cv_rect_world objectRect;
                PointsExtendRect(object.locationWorld[0], object.locationWorld[1], objectRect);

                iou.push_back((objectRect & boxRect).area() / (objectRect | boxRect).area());
                LOG_CHECK_DEBUG(INFO) << "Small Object IOU, object.id: " << object.id
                                      << ", iou: "
                                      << (objectRect & boxRect).area() /
                                         (objectRect | boxRect).area();
            }
            else
            {
                iou.push_back(0);
            }
        }
        ious.push_back(iou);
    }
}

void AircraftHatch::BindByIOU(const std::vector<std::vector<float>> &iouMap, std::vector<int> &indexes)
{
    for (int i = 0; i < iouMap.size(); ++i)
    {
        int maxId = -1;
        for (int j = 0; j < iouMap[0].size(); ++j)
        {
            if (iouMap[i][j] > 0.5)
            {
                maxId = j;
                break;
            }
        }
        indexes.push_back(maxId);
    }
}

void AircraftHatch::UpdateByBinds(const std::vector<BoxInfo> boxes, const std::vector<int> &binds)
{
    if (boxes.size() != binds.size())
    {
        LOG_CHECK_STATUS(INFO) << "Boxes.size is not equal to binds, error!";
    }

    for (int i = 0; i < boxes.size(); ++i)
    {
        // TODO update Gate by binds
        if (binds[i] > -1)
        {
            mapUpdate = true;
            continue;
        }

        Add(boxes[i]);
    }
}

void AircraftHatch::Add(const BoxInfo &box)
{
    if (box.IsAircraftHatch())
    {
        mapUpdate = true;
        aircraftHatchesMap.objects.push_back(Box2Object(box));
    }
    else
    {
        LOG_CHECK_STATUS(INFO) << "error class in AirCraft Hatch add!";
    }
}

void AircraftHatch::Trapezoid2Rect(const psl::Location &leftCrossPoint, const psl::Location &rightCrossPoint
                                   , const psl::Location &edgeLeft, const psl::Location &edgeRight
                                   , cv_rect_world &rect) const
{
    float xMin = std::min({leftCrossPoint.x, rightCrossPoint.x, edgeLeft.x, edgeRight.x});
    float yMin = std::min({leftCrossPoint.y, rightCrossPoint.y, edgeLeft.y, edgeRight.y});
    float xMax = std::max({leftCrossPoint.x, rightCrossPoint.x, edgeLeft.x, edgeRight.x});
    float yMax = std::max({leftCrossPoint.y, rightCrossPoint.y, edgeLeft.y, edgeRight.y});

    rect = cv_rect_world(cv::Point2f(xMin, yMin), cv::Point2f(xMax, yMax));
}

void AircraftHatch::PointsExtendRect(const Point &left, const Point &right, cv_rect_world &rect)
{
    Segment lineOriginal = Segment(left, right);
    std::vector<Point> points;

    if (lineOriginal.start.x == lineOriginal.end.x ||
        lineOriginal.start.y == lineOriginal.end.y)
    {
        LineExtendRectParallel(lineOriginal, points, EXTEND_DIS);
    }
    else
    {
        LineExtendRect(lineOriginal, points, EXTEND_DIS);
    }

    Trapezoid2Rect(points[0], points[1], points[2], points[3], rect);
}

void AircraftHatch::LineExtendRectParallel(const Segment &line, std::vector<Point> &points
                                           , const float extend) const
{
    Point p1(0, 0, 0), p2(0, 0, 0), p3(0, 0, 0), p4(0, 0, 0);

    if (line.start.x == line.end.x)
    {
        p1.x = line.start.x - extend;
        p1.y = line.start.y;
        p2.x = line.end.x - extend;
        p2.y = line.end.y;
        p3.x = line.start.x + extend;
        p3.y = line.start.y;
        p4.x = line.end.x + extend;
        p4.y = line.end.y;
    }
    else
    {
        p1.x = line.start.x;
        p1.y = line.start.y - extend;
        p2.x = line.end.x;
        p2.y = line.end.y - extend;
        p3.x = line.start.x;
        p3.y = line.start.y + extend;
        p4.x = line.end.x;
        p4.y = line.end.y + extend;
    }
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
}

void AircraftHatch::LineExtendRect(const Segment &line, std::vector<Point> &points
                                   , const float extend) const
{
    float k, b, verticalK, bStart, bEnd;
    Point p1(0, 0, 0), p2(0, 0, 0), p3(0, 0, 0), p4(0, 0, 0);

    line.GetLine(k, b);
    verticalK = -1.0 / k;

    if (k > 0)
    {
        p1.y = line.start.y + abs(sin(atan(verticalK))) * extend;
        p1.x = line.start.x - abs(cos(atan(verticalK))) * extend;

        p2.y = line.start.y - abs(sin(atan(verticalK))) * extend;
        p2.x = line.start.x + abs(cos(atan(verticalK))) * extend;

        p3.y = line.end.y + abs(sin(atan(verticalK))) * extend;
        p3.x = line.end.x - abs(cos(atan(verticalK))) * extend;

        p4.y = line.end.y - abs(sin(atan(verticalK))) * extend;
        p4.x = line.end.x + abs(cos(atan(verticalK))) * extend;
    }
    else
    {
        p1.y = line.start.y + abs(sin(atan(verticalK))) * extend;
        p1.x = line.start.x + abs(cos(atan(verticalK))) * extend;

        p2.y = line.start.y - abs(sin(atan(verticalK))) * extend;
        p2.x = line.start.x - abs(cos(atan(verticalK))) * extend;

        p3.y = line.end.y + abs(sin(atan(verticalK))) * extend;
        p3.x = line.end.x + abs(cos(atan(verticalK))) * extend;

        p4.y = line.end.y - abs(sin(atan(verticalK))) * extend;
        p4.x = line.end.x - abs(cos(atan(verticalK))) * extend;
    }

    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
}