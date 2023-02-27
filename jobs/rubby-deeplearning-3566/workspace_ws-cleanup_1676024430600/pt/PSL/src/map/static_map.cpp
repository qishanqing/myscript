#include "static_map.h"
#include "src/utils/timer.h"
#include "src/utils/define.h"
#include "src/utils/utils.h"
#include "src/utils/file.h"

//#define SHOW_ALL_POINT

const int INIT_WEIGHT_OBJECT = 1;
const int SEND_WEIGHT_OBJECT = 5;
const int MAX_WEIGHT_OBJECT = 20;
const float MIN_MARCHED_IOU = 0.01;
const float MIN_MARCHED_SMALL_IOU = 0.01;
const float MIN_BOUNDARY_DISTANCE = 20;
const float MAX_POINT_MACHINE = 1;
const float VALID_DISTANCE = 0.65;
const float NEAR_DISTANCE = 0.29;
const float DESK_VALID_POINT_DIS = 0.5;
const float NEW_OBJECT_SEND_DIS = 0.255;
const float INVALID_POINT_VALUE = 10.0;
const float MIN_LEFT_RIGHT = 0.05;
const float MAX_REDUCE = 1.5;
const float SMALL_EXTEND = 0.1;
const std::vector<std::string> SMALL_OBJECT_NAMES = {"shoes", "bin", "pedestal", "wire"
                                                     , "socket", "cat", "dog"};
const float SMALL_OBJECT_MIN_WIDTH[7] = {0.084, 0.19, 0.231, 0.14, 0.056, 0.07, 0.14};
const float SMALL_OBJECT_MAX_WIDTH[7] = {0.377, 0.429, 0.429, 0.26, 0.3, 0.39, 0.559};

StaticMap::StaticMap()
{

}

StaticMap::~StaticMap()
{

}

cv::Mat StaticMap::Show(const BoxInfo &view, const vector<BoxInfo> &boxes, cv::Mat image)
{
    DrawDesk(image);
    DrawSmallObject(image);
//#ifdef SHOW_ALL_POINT
    DrawPoint(image);
//#endif

    return image;
}

void StaticMap::Clear()
{
    staticObjectMap.objects.clear();
    staticObjectMap.finalUpdate = false;
    staticObjectMap.status = psl::SenmaticMap::NO_UPDATE;
    smallObjectUpdate.clear();

    pointStatus.clear();
    objectWeights.clear();

    smallObjectMapInner.clear();
}

void StaticMap::Update(const vector<BoxInfo> &boxes, const BoxInfo &view
                       , const psl::Time &time)
{
    Timer timer;
    Map::Update(boxes, view, time);

    this->Reset();
    this->mapInter.clear();

    if (not boxes.empty())
    {
        LOG_CHECK_DEBUG(INFO) << "Boxes.size: " << boxes.size();
        this->time = time;
        Fresh(boxes);
    }

    this->RemoveSmallObject(boxes);

    LOG_CHECK_DEBUG(INFO) << "map size: " << this->mapInter.size();
    LOG_CHECK_TIME(INFO) << timer.TimingStr();
}

bool StaticMap::WriteMap(ofstream &stream, const std::vector<DLObject>& objects) const
{
    size_t size = objects.size();
    file_op::File::WriteItem(stream, size);
    for (int i = 0; i < size; ++i)
    {
        auto object = this->staticObjectMap.objects[i];
        auto smallObjectUpdate = this->smallObjectUpdate[i];
        auto pointStatus = this->pointStatus[i];
        auto objectWeight = this->objectWeights[i];
        file_op::File::WriteItem(stream, object.id);
        file_op::File::WriteItem(stream, object.classID);
        file_op::File::WriteItem(stream, object.score);
        file_op::File::WriteItem(stream, object.detectExtend);
        file_op::File::WriteItem(stream, object.time);
        file_op::File::WriteItem(stream, object.nodeID);
        file_op::File::WriteItem(stream, object.history);
        WRITE_LIST(stream, object.locationWorld);
        file_op::File::WriteItem(stream, object.status);
        file_op::File::WriteItem(stream, object.statusOperator);
        file_op::File::WriteItem(stream, object.viewLocationWorld);
    }

    return true;
}

bool StaticMap::Write(std::ofstream &stream) const
{
    if (not stream.is_open()) return false;
    // TODO : not write securityTime
    WriteMap(stream, this->staticObjectMap.objects);
    WriteMap(stream, this->smallObjectMapInner);
    file_op::File::WriteList(stream, this->pointStatus);
    file_op::File::WriteList(stream, this->smallObjectUpdate);
    file_op::File::WriteList(stream, this->objectWeights);
    return true;
}

bool StaticMap::ReadMap(ifstream &stream, std::vector<DLObject> &objects)
{
    size_t size = 0;
    file_op::File::ReadItem(stream, size);
    LOG_CHECK_DEBUG(INFO) << "map size : " << size;
    for (int i = 0; i < size; ++i)
    {
        psl::Object object;

        file_op::File::ReadItem(stream, object.id);
        file_op::File::ReadItem(stream, object.classID);
        file_op::File::ReadItem(stream, object.score);
        file_op::File::ReadItem(stream, object.detectExtend);
        file_op::File::ReadItem(stream, object.time);
        file_op::File::ReadItem(stream, object.nodeID);
        file_op::File::ReadItem(stream, object.history);
        READ_LIST(stream, object.locationWorld);
        file_op::File::ReadItem(stream, object.status);
        file_op::File::ReadItem(stream, object.statusOperator);
        file_op::File::ReadItem(stream, object.viewLocationWorld);
        objects.push_back(object);
    }

    return true;
}

bool StaticMap::Read(std::ifstream &stream)
{
    if (!stream.is_open()) return false;

    ReadMap(stream, staticObjectMap.objects);
    ReadMap(stream, smallObjectMapInner);
    file_op::File::ReadList(stream, this->pointStatus);
    file_op::File::ReadList(stream, this->smallObjectUpdate);
    file_op::File::ReadList(stream, this->objectWeights);

    return true;
}

bool StaticMap::Save(std::ofstream &ofstream)
{
    if (!ofstream.is_open())
    {
        ofstream.close(); // TODO : it need close?
        LOG_CHECK_STATUS(WARNING) << "fail to create file.";
        return false;
    }

    if (!Write(ofstream)) return false;

    file_op::File::WriteItem(ofstream, lastID);
    file_op::File::WriteItem(ofstream, time);

    LOG_CHECK_DEBUG(INFO) << "static map Save Map map.size: "
                          << this->staticObjectMap.objects.size();
    LOG_CHECK_DEBUG(INFO) << "Save Map last ID: " << lastID;

    ofstream.close();
    END();
    return true;
}


bool StaticMap::Load(std::ifstream &ifstream)
{
    if (!ifstream.is_open())
    {
        ifstream.close();
        LOG_CHECK_STATUS(INFO) << "fail to open file.";
        return false;
    }

    Read(ifstream);
    file_op::File::ReadItem(ifstream, lastID);
    file_op::File::ReadItem(ifstream, time);

    LOG_CHECK_DEBUG(INFO) << "static map Load Map map.size: "
                          << this->staticObjectMap.objects.size();
    LOG_CHECK_DEBUG(INFO) << "Load Map last ID: " << lastID;

    ifstream.close();
    END();
    return true;
}

bool StaticMap::Close2Boundary(const BoxInfo &box)
{
    if (abs(box.box.x) < MIN_BOUNDARY_DISTANCE ||
        abs(box.box.x + box.box.width - WIDTH) < MIN_BOUNDARY_DISTANCE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void StaticMap::FilterDeskBox(const std::vector<BoxInfo> &boxes
                              , std::vector<BoxInfo> &boxesDesk)
{
    for (auto objet : this->staticObjectMap.objects)
    {
        if (objet.classID != 7) continue;
        if (ViewClosePoints(objet.locationWorld))
        {
            LOG_CHECK_DEBUG(INFO)
                    << "view close to object, don't ues this image update or add desk!";
            return;
        }
    }

    for (const auto &box : boxes)
    {
        if (box.className != "desk_rect") continue;

        bool keyPointBelow = false;
        for (int i = 0; i < 8; ++i)
        {
            if (i % 2 != 0)
            {
                if (box.keypoint.point[i] > box.box.y + box.box.height)
                {
                    keyPointBelow = true;
                    break;
                }
            }
        }

        if (keyPointBelow) continue;

        LOG_CHECK_DEBUG(INFO) << "Desk Box.classID: " << box.classID
                              << ", Box.className: "
                              << box.className
                              << ", locationSerials[0].world: "
                              << box.locationSerials[0].world.Print()
                              << ", locationSerials[1].world: "
                              << box.locationSerials[1].world.Print()
                              << "\n Desk Box. locationSerials[0].machine: "
                              << box.locationSerials[0].machine.Print()
                              << ", locationSerials[1].machine: "
                              << box.locationSerials[1].machine.Print();
        boxesDesk.push_back(box);
    }
}

void StaticMap::SetPointsStatus(const std::vector<BoxInfo> &boxes
                                , std::vector<std::vector<int>> &statuses)
{
    statuses.clear();
    for (int i = 0; i < boxes.size(); ++i)
    {
        statuses.push_back({true, true, false, false});

        if (Close2Boundary(boxes[i]))
        {
            for (int j = 0; j < statuses[i].size(); ++j)
            {
                statuses[i][j] = false;
            }
            continue;
        }
        for (int j = 0; j < boxes[i].locationSerials.size(); ++j)
        {
            if (abs(boxes[i].locationSerials[j].machine.x) > MAX_POINT_MACHINE)
            {
                statuses[i][j] = false;
            }
        }
    }
}

void StaticMap::GetMatchedByTrack(const std::vector<BoxInfo> &boxes
                                  , std::vector<int> &matchedTrack)
{
    for (int i = 0; i < boxes.size(); ++i)
    {
        auto box = boxes[i];
        int matched = -1;
        for (int j = 0; j < this->staticObjectMap.objects.size(); ++j)
        {
            auto object = this->staticObjectMap.objects[j];
            if (box.id == object.id)
            {
                matched = j;
                break;
            }
        }
        if (matched > -1)
        {
            matchedTrack[i] = box.id;
        }
    }
}

void StaticMap::View2PointsAxisDistance(const vector<Point> &points
                                        , float &distanceAxisMin, float &distanceAxisMax)
{
    float minDistance = FLT_MAX;
    float maxDistance = FLT_MIN;
    LOG_CHECK_DEBUG(INFO) << "view.location.world: " << view.location.world.Print()
                          << " , view.edgeLeft.world: " << view.edgeLeft.world.Print()
                          << " , view.edgeRight.world: " << view.edgeRight.world.Print();
    for (auto point : points)
    {
        float distance;
        View2PointAxis(point, distance);
        LOG_CHECK_DEBUG(INFO) << " , Point: " << point.Print() << ", distance: "
                              << distance;

        if (distance < minDistance)
        {
            minDistance = distance;
        }
        if (distance > maxDistance)
        {
            maxDistance = distance;
        }
    }
    distanceAxisMin = minDistance;
    distanceAxisMax = maxDistance;
}

void StaticMap::View2PointAxis(const Point &location, float &distance)
{
    float angle = 0;
    Point middleViewPoint = view.edgeLeft.world;
    UpdatePoint(view.edgeRight.world, middleViewPoint);
    angle = GetAngle(view.location.world, middleViewPoint, location);
    distance = cos(angle) * view.location.world.DistanceXY(location);
}

void StaticMap::FreshDeskMap(const std::vector<BoxInfo> &boxes)
{
    std::vector<BoxInfo> boxesDesk;
    std::vector<std::vector<float>> ious;
    std::vector<int> binds;
    std::vector<psl::Object> objects;
    std::vector<std::vector<int>> statuses;
    std::vector<TrackingBox<ShapeType>> boxesTrack;

    FilterDeskBox(boxes, boxesDesk);

    //TODO: if add RemoveDeskObject need remove this code
    if (boxesDesk.size() == 0) return;

    std::vector<int> matchedTrack(boxesDesk.size(), -1);

    SetPointsStatus(boxesDesk, statuses);

    Bind(binds, boxesDesk, matchedTrack);

    UpdateDeskMap(binds, boxesDesk, statuses, matchedTrack);
}

void StaticMap::Fresh(const vector<BoxInfo> &boxes)
{
    std::vector<BoxInfo> trackingResult;
    vector<TrackingBox<ShapeType>> rectResult;
    Tracking(boxes, rectResult, this->tracker);
    TrackingWorld(boxes, view.location.world, trackingResult);

    FreshDeskMap(boxes);

    FreshSmallBox(boxes);

    END();
}

void StaticMap::AddDeskMap(const BoxInfo &box, const std::vector<int> &status)
{
    this->staticObjectMap.objects.push_back(DeskBox4Object(box));
    this->smallObjectMapInner.push_back(DeskBox4Object(box));
    this->pointStatus.push_back(status);
    this->smallObjectUpdate.push_back(true);
    this->objectWeights.push_back(INIT_WEIGHT_OBJECT);
}

void StaticMap::Add(const BoxInfo &box)
{
    this->mapInter.push_back(Box2Object(box));
    ++this->lastID;
}

bool StaticMap::UpdateLocation(const BoxInfo &box)
{
    return false;
}

void StaticMap::BindNearest2Points(std::vector<std::vector<float >> &distances
                                   , std::vector<int> &BindResult)
{
    // TODO: get nearest keyPoints to view, not first two
    std::vector<size_t> KeyPoint1, KeyPoint2;
    KeyPoint1 = SortIndexes(distances[0]);
    KeyPoint2 = SortIndexes(distances[1]);
    if (KeyPoint1[0] == KeyPoint2[0])
    {
        if (distances[0][KeyPoint1[0]] > distances[0][KeyPoint2[0]])
        {
            BindResult.push_back(KeyPoint2[0]);
            BindResult.push_back(KeyPoint1[0]);
        }
        else
        {
            BindResult.push_back(KeyPoint1[0]);
            BindResult.push_back(KeyPoint2[0]);
        }
    }
    else
    {
        BindResult.push_back(KeyPoint1[0]);
        BindResult.push_back(KeyPoint2[0]);
    }
}

bool StaticMap::ViewClosePointsRect(const vector<Point> &points)
{
    cv::Rect rect;
    float x = std::min({points[0].x, points[1].x, points[2].x, points[3].x});
    float y = std::min({points[0].y, points[1].y, points[2].y, points[3].y});
    float width = std::max({points[0].x, points[1].x, points[2].x, points[3].x}) - x;
    float height = std::max({points[0].y, points[1].y, points[2].y, points[3].y}) - y;
    x = x - NEAR_DISTANCE;
    width += 2 * NEAR_DISTANCE;
    y = y - NEAR_DISTANCE;
    height += 2 * NEAR_DISTANCE;

    if (view.location.world.x >= x && view.location.world.x <= x + width
        && view.location.world.y >= y && view.location.world.y <= y + height)
    {
        return true;
    }
    return false;
}

bool StaticMap::ViewClosePoints(const vector<Point> &points)
{
    float minDistance = 0;
    float maxDistance = 0;
    View2PointsAxisDistance(points, minDistance, maxDistance);
    if (minDistance * maxDistance <= 0
        || min(abs(minDistance), abs(maxDistance)) < NEAR_DISTANCE)
    {
        return true;
    }

    return false;
}

void StaticMap::UpdateMapByBox(const BoxInfo &box, const int index
                               , const std::vector<int> &status)
{
    std::vector<std::vector<float >> distances;
    std::vector<int> bindResult;
    std::vector<psl::Location> boxLocations;
    psl::Object &object = this->staticObjectMap.objects[index];

    this->smallObjectUpdate[index] = true;

    if (DeskBoxInvalid(box)) return;

    boxLocations.push_back(box.locationSerials[0].world);
    boxLocations.push_back(box.locationSerials[1].world);

    StoreLocation(index, box);

    BoxLocationsObjectDistance(boxLocations, object, distances);

    BindNearest2Points(distances, bindResult);

    for (int i = 0; i < bindResult.size(); ++i)
    {
        if (status[i])
        {
            if (pointStatus[index][bindResult[i]])
            {
                UpdatePoint(box.locationSerials[i].world
                            , object.locationWorld[bindResult[i]]);
            }
            else
            {
                object.locationWorld[bindResult[i]] = box.locationSerials[i].world;
                pointStatus[index][bindResult[i]] = true;
            }
        }
    }
}

DLObject StaticMap::DeskBox4Object(const BoxInfo &box)
{
    DLObject object = Map::Box2Object(box);
    if (box.id > -1)
    {
        object.id = box.id;
    }

    LineExtendVerticalBisector(view, box.locationSerials[0].world
                               , box.locationSerials[1].world
                               , object.locationWorld
                               , detectParam.modelProperty.trackingExtend[box.classID]);

    return object;
}

DLObject StaticMap::DeskBox2Object(const BoxInfo &box)
{
    DLObject object = Map::Box2Object(box);
    if (box.id > -1)
    {
        object.id = box.id;
    }
    if (box.className == "desk_rect")
    {
        for (int j = 0; j < box.locationSerials.size(); ++j)
        {
            object.locationWorld.push_back(box.locationSerials[j].world);
        }
    }
    else
    {
        object.locationWorld.push_back(box.location.world);
        object.locationWorld.push_back(box.location.world);
    }

    return object;
}

void StaticMap::Remove()
{

}

void StaticMap::Delete(const int index)
{
    this->smallObjectUpdate.erase(this->smallObjectUpdate.begin() + index);
    this->staticObjectMap.objects.erase(this->staticObjectMap.objects.begin() + index);
    this->pointStatus.erase(this->pointStatus.begin() + index);
    this->objectWeights.erase(this->objectWeights.begin() + index);
    this->smallObjectMapInner.erase(this->smallObjectMapInner.begin() + index);
}

void StaticMap::ReduceWeight(const std::vector<BoxInfo> &boxes, const int &index)
{
    float distance = this->view.location.world.DistanceXY(
            staticObjectMap.objects[index].locationWorld[0]);
    if (distance > VALID_DISTANCE || distance < NEAR_DISTANCE)
    {
//        bool weightReduce = false;
//        for(auto box : boxes)
//        {
//            if (box.classID == this->staticObjectMap.objects[index].classID)
//            {
//                std::vector<psl::Location> points;
//                LineExtendVerticalBisector(view, box.edgeLeft.world
//                                           , box.edgeRight.world, points
//                                           , detectParam.modelProperty.trackingExtend[box.classID]);
//                for (auto point : points)
//                {
//                    if (InTriangle(view.location.world, this->staticObjectMap.objects[index].locationWorld[0]
//                                   , this->staticObjectMap.objects[index].locationWorld[1], point)
//                        && distance < MAX_REDUCE)
//                    {
//                        weightReduce = true;
//                        break;
//                    }
//                }
//
//                if (weightReduce)
//                {
//                    this->objectWeights[index]--;
//                    break;
//                }
//            }
//        }
    }
    else
    {
        this->objectWeights[index]--;
    }

}

void StaticMap::RemoveSmallObject(const std::vector<BoxInfo> &boxes)
{
    for (int i = 0; i < this->smallObjectUpdate.size(); ++i)
    {
        if (std::find(SMALL_OBJECT_NAMES.begin(), SMALL_OBJECT_NAMES.end()
                      , detectParam.modelProperty.names[this->staticObjectMap.objects[i].classID])
            == SMALL_OBJECT_NAMES.end())
        {
            continue;
        }
        bool inView = InTriangle(view.location.world, view.edgeLeft.world
                                 , view.edgeRight.world
                                 , this->staticObjectMap.objects[i].locationWorld[0]);
        inView &= InTriangle(view.location.world, view.edgeLeft.world
                             , view.edgeRight.world
                             , this->staticObjectMap.objects[i].locationWorld[1]);
        inView &= InTriangle(view.location.world, view.edgeLeft.world
                             , view.edgeRight.world
                             , this->staticObjectMap.objects[i].locationWorld[2]);
        inView &= InTriangle(view.location.world, view.edgeLeft.world
                             , view.edgeRight.world
                             , this->staticObjectMap.objects[i].locationWorld[3]);

        if ((!this->smallObjectUpdate[i]) && inView)
        {
            ReduceWeight(boxes, i);

            if (this->objectWeights[i] < INIT_WEIGHT_OBJECT)
            {
                Delete(i);
                --i;
            }
        }
    }
}

void
StaticMap::Tracking(const std::vector<BoxInfo> &boxes
                    , vector<TrackingBox<ShapeType>> &rectResult
                    , Sort &tracker)
{
    //Tracking by sort
//    vector<TrackingBox<ShapeType>> rectResult;
    vector<ShapeType> rectangles;

    for (auto box: boxes)
    {
        rectangles.push_back(ShapeType(box.box));
    }

    Timer timer;
    tracker.Update(rectangles, rectResult, this->lastID);
    LOG_CHECK_TIME(INFO) << timer.TimingStr();

    for (int i = 0; i < rectResult.size(); ++i)
    {
        rectResult[i].id = rectResult[i].id - 1;
    }
}

void
StaticMap::TrackingWorld(const std::vector<BoxInfo> &boxes
                         , const psl::Location &machineLocation
                         , std::vector<BoxInfo> &trackingResult)
{
    //Tracking by sort
    vector<TrackingBox<ShapeType>> rectResult;
    vector<ShapeType> rectangles;

    for (auto box: boxes)
    {
        cv::Rect boxRect;
        Box2WorldRect(box, boxRect, machineLocation);
        rectangles.push_back(ShapeType(boxRect));
    }

    Timer timer;
//    trackerWorld.Update(rectangles, rectResult);
    LOG_CHECK_TIME(INFO) << timer.TimingStr();
    trackingResult.resize(rectResult.size());

    if (rectResult.size() == rectangles.size())
    {
        for (int i = 0; i < rectResult.size(); ++i)
        {
            trackingResult[i].SetRect(rectResult[i].region.data);
            // TODO： confirm whether need to - 1
            trackingResult[i].id = rectResult[i].id - 1;
        }
    }
}

void StaticMap::BoxView4CrossPoints(const BoxInfo &box, const psl::Location &view
                                    , std::vector<Point> &points)
{
    points.resize(4);

    if (OnSegment(box.edgeLeft.machine, box.edgeRight.machine, box.location.machine))
    {
        points[0] = box.edgeLeft.world;
        points[1] = box.edgeRight.world;
    }
    else
    {
        GetParallelLinesCrossPoints(box, view, points[0], points[1]);
    }

    double lengthLeft = view.DistanceXY(points[0]);
    double lengthRight = view.DistanceXY(points[1]);
    ExtendLine(view, points[0], points[3]
               , lengthLeft + detectParam.modelProperty.trackingExtend[box.classID]);
    ExtendLine(view, points[1], points[2]
               , lengthRight + detectParam.modelProperty.trackingExtend[box.classID]);
}

void
StaticMap::Box2WorldRect(const BoxInfo &box, cv::Rect &rect, const psl::Location &view)
{
    std::vector<Point> points;
    BoxView4CrossPoints(box, view, points);
    Trapezoid2Rect(points[0], points[1], points[2], points[3], rect);
}

void StaticMap::GetParallelLinesCrossPoints(const BoxInfo &box, psl::Location view
                                            , psl::Location &p1, psl::Location &p2)
{
    Segment parallelLine, leftLine, rightLine;
    leftLine = Segment(view, box.edgeLeft.world);
    rightLine = Segment(view, box.edgeRight.world);

    if (box.edgeLeft.world.x == box.edgeRight.world.x)
    {
        parallelLine.start.x = box.location.world.x;
        parallelLine.end.x = box.location.world.x;
        parallelLine.start.y = 20;
        parallelLine.end.y = -20;
    }
    else
    {
        float k = (box.edgeLeft.world.y - box.edgeRight.world.y) /
                  (box.edgeLeft.world.x - box.edgeRight.world.x);
        float b = box.location.world.y - box.location.world.x * k;
        if (abs(k) > 1)
        {
            parallelLine.start.y = -20;
            parallelLine.end.y = 20;
            parallelLine.start.x = (parallelLine.start.y - b) / k;
            parallelLine.end.x = (parallelLine.end.y - b) / k;
        }
        else
        {
            parallelLine.start.x = -20;
            parallelLine.end.x = 20;
            parallelLine.start.y = parallelLine.start.x * k + b;
            parallelLine.end.y = parallelLine.end.x * k + b;
        }
    }
    GetCrossPoint4Segment(parallelLine, leftLine, p1);
    GetCrossPoint4Segment(parallelLine, rightLine, p2);
}

void
StaticMap::Trapezoid2Rect(const psl::Location &leftCrossPoint
                          , const psl::Location &rightCrossPoint
                          , const psl::Location &edgeLeft, const psl::Location &edgeRight
                          , cv::Rect &rect) const
{
    float x = std::min({leftCrossPoint.x, rightCrossPoint.x, edgeLeft.x, edgeRight.x});
    float y = std::min({leftCrossPoint.y, rightCrossPoint.y, edgeLeft.y, edgeRight.y});
    float width =
            std::max({leftCrossPoint.x, rightCrossPoint.x, edgeLeft.x, edgeRight.x}) - x;
    float height =
            std::max({leftCrossPoint.y, rightCrossPoint.y, edgeLeft.y, edgeRight.y}) - y;

    rect.x = std::max(0.0, x * map_type::DELTA + map_type::W / 2.0);
    rect.y = std::max(0.0, y * map_type::DELTA + map_type::H / 2.0);
    rect.width = std::min(map_type::W - rect.x, int(width * map_type::DELTA));
    rect.height = std::min(map_type::H - rect.y, int(height * map_type::DELTA));
}


void StaticMap::Trapezoid2Rect(const psl::Location &leftCrossPoint
                               , const psl::Location &rightCrossPoint
                               , const psl::Location &edgeLeft
                               , const psl::Location &edgeRight
                               , StaticMap::cv_rect_world &rect) const
{
    float xMin = std::min({leftCrossPoint.x, rightCrossPoint.x, edgeLeft.x, edgeRight.x});
    float yMin = std::min({leftCrossPoint.y, rightCrossPoint.y, edgeLeft.y, edgeRight.y});
    float xMax = std::max({leftCrossPoint.x, rightCrossPoint.x, edgeLeft.x, edgeRight.x});
    float yMax = std::max({leftCrossPoint.y, rightCrossPoint.y, edgeLeft.y, edgeRight.y});

    rect = cv_rect_world(cv::Point2f(xMin, yMin), cv::Point2f(xMax, yMax));
}

void StaticMap::Location2Points(const std::vector<psl::Location> &locations
                                , std::vector<cv::Point> &points) const
{
    for (auto location : locations)
    {
        cv::Point point;
        point.x = location.x * map_type::DELTA + map_type::W / 2.0;
        point.y = location.y * map_type::DELTA + map_type::H / 2.0;
        points.push_back(point);
    }
}

void StaticMap::BindByMaxIOU(std::vector<std::vector<float>> &ious, std::vector<int> &binds)
{
    cv::Mat matIOU;

    for (int i = 0; i < ious.size(); ++i)
    {
        binds.push_back(-1);
    }

    if (!ious.empty() && !ious[0].empty())
    {
        cv::Mat tempIou(0, ious[0].size(), cv::DataType<float>::type);
        for (unsigned int i = 0; i < ious.size(); ++i)
        {
            cv::Mat lineIou(1, ious[0].size(), cv::DataType<float>::type, ious[i].data());
            tempIou.push_back(lineIou);
        }
        matIOU = tempIou;
    }
    else
    {
        return;
    }
    LOG_CHECK_DEBUG(INFO) << matIOU;
    while (true)
    {
        double minValue, maxValue;
        cv::Point minIdx, maxIdx;
        cv::minMaxLoc(matIOU, &minValue, &maxValue, &minIdx, &maxIdx);

        if (maxValue <= 0)
        {
            break;
        }

        binds[maxIdx.y] = maxIdx.x;

        if (matIOU.cols > 1 && matIOU.rows > 1)
        {
            LOG_CHECK_DEBUG(INFO) << matIOU.cols << " , " << matIOU.rows;
            int k = 10;
        }

        for (int row = 0; row < matIOU.rows; row++)
        {
            auto *data_Ptr = matIOU.ptr<cv::Vec<float, 1>>(row);
            for (int col = 0; col < matIOU.cols; col++)
            {
                if (row == maxIdx.y or col == maxIdx.x)
                {
                    data_Ptr[col][0] = -1;
                }
            }
        }
    }
}

void StaticMap::RemoveByIDs(const std::vector<int> ids)
{
    trackerDesk.RemoveByIDs(ids);
}

void StaticMap::UpdateByIDs(const std::map<int, int> updateIDs)
{
    trackerDesk.UpdateByIDs(updateIDs);
}

void StaticMap::SetIouByMatchedTrack(std::vector<std::vector<float>> &ious
                                     , const std::vector<int> &matchedTrack)
{
    for (int i = 0; i < matchedTrack.size(); ++i)
    {
        if (matchedTrack[i] > -1)
        {
            for (int j = 0; j < this->staticObjectMap.objects.size(); j++)
            {
                if (this->staticObjectMap.objects[j].id == matchedTrack[i])
                {
                    ious[i][j] = 1;
                    break;
                }
            }
        }
    }
}

void StaticMap::UpdateSmallBoxByBinds(const std::vector<BoxInfo> boxesSmallObject
                                      , const std::vector<int> &binds)
{
    for (int i = 0; i < binds.size(); ++i)
    {
        if (binds[i] > -1)
        {
            UpdateSmallBox(boxesSmallObject[i], binds[i]);
        }
        else
        {
            AddSmallBox(boxesSmallObject[i]);
        }
        mapUpdate = true;
    }
}

void StaticMap::FreshSmallBox(const std::vector<BoxInfo> &boxes)
{
    std::vector<BoxInfo> boxesSmallObject;
    std::vector<std::vector<float>> ious;
    std::vector<int> binds;

    FilterBoxes(boxes, boxesSmallObject);

    BoxesSmallObjectsIOU(boxesSmallObject, ious);

    BindByIOUEightQueen(ious, binds);
//    BindByMaxIOU(ious, binds);

    UpdateSmallBoxByBinds(boxesSmallObject, binds);

}

void StaticMap::FilterSmallObject(const std::vector<BoxInfo> &boxes
                                  , std::vector<BoxInfo> &boxesSmallObject)
{
    for (auto box : boxes)
    {
        LOG_CHECK_DEBUG(INFO) << "box.classId: " << box.classID << ", box.xMin: "
                              << box.box.x << ", box.yMin: " << box.box.y
                              << ", box.xMax: " << box.box.x + box.box.width
                              << ", box.yMax: " << box.box.y + box.box.height;
        if (std::find(SMALL_OBJECT_NAMES.begin(), SMALL_OBJECT_NAMES.end()
                      , box.className) != SMALL_OBJECT_NAMES.end())
        {
            boxesSmallObject.push_back(box);
        }
    }
}

void StaticMap::FilterBoxes(const std::vector<BoxInfo> &boxes
                            , std::vector<BoxInfo> &boxesSmallObject)
{
    FilterSmallObject(boxes, boxesSmallObject);
//    FilterClose2Boundary(boxesSmallObject);
}

void
StaticMap::UpdatePoint(const psl::Location &locationSrc, psl::Location &locationDst) const
{
    locationDst = locationSrc + locationDst;
    locationDst.x = locationDst.x / 2;
    locationDst.y = locationDst.y / 2;
    locationDst.z = locationDst.z / 2;
}

void StaticMap::BoxLocationsObjectDistance(const std::vector<psl::Location> &boxLocations
                                           , const psl::Object &object
                                           , std::vector<std::vector<float >> &distances)
{
    for (auto boxLocation : boxLocations)
    {
        std::vector<float> distance;
        for (auto &location: object.locationWorld)
        {
            distance.push_back(boxLocation.DistanceXY(location));
        }
        distances.push_back(distance);
    }
}

void StaticMap::Point2SegmentDistance(const Segment &l, const psl::Location location, float &distance)
{
    if (l.start.x == l.end.x)
    {
        distance = abs(l.start.x - location.x);
    }
    else
    {
        float k, b;
        l.GetLine(k, b);
        distance = abs(k * location.x - location.y + b)/sqrt(1 + k*k);
    }
}

void StaticMap::UpdateSmallBoxPoint(const BoxInfo &box, const int bind)
{
    // this is view to object L1 distance, just used to compare
    float objectDistance, boxDistance;
    psl::Object object;
    BoxExtendSingle4(box, object.locationWorld);

    psl::Location objectViewWorld = this->staticObjectMap.objects[bind].viewLocationWorld;
    Segment objectLine = Segment(this->staticObjectMap.objects[bind].locationWorld[0]
                                , this->staticObjectMap.objects[bind].locationWorld[1]);
    Point2SegmentDistance(objectLine, objectViewWorld, objectDistance);

    Segment boxLine = Segment(object.locationWorld[0], object.locationWorld[1]);
    Point2SegmentDistance(boxLine, view.location.world, boxDistance);

    if (boxDistance < objectDistance)
    {
        this->staticObjectMap.objects[bind].locationWorld.swap(object.locationWorld);
        this->staticObjectMap.objects[bind].viewLocationWorld = view.location.world;
        this->staticObjectMap.objects[bind].score = box.score;
        this->staticObjectMap.objects[bind].time = this->time;

        LOG_CHECK_DEBUG(INFO) << "update small object points";
    }
}

void StaticMap::UpdateSmallBox(const BoxInfo &box, const int bind)
{

    if (this->objectWeights[bind] < MAX_WEIGHT_OBJECT)
    {
        this->objectWeights[bind]++;
    }

    if (Close2Boundary(box))
    {
        this->smallObjectUpdate[bind] = true;
        return;
    }

    if (box.location.machine.x < NEAR_DISTANCE || box.location.machine.x > VALID_DISTANCE)
    {
        this->smallObjectUpdate[bind] = true;
        return;
    }

    StoreLocation(bind, box);

//    if (this->objectWeights[bind] <= SEND_WEIGHT_OBJECT)
//    {
    UpdateSmallBoxPoint(box, bind);
//    }

    this->smallObjectUpdate[bind] = true;
}

void StaticMap::AddSmallBox(const BoxInfo &box)
{
    if (Close2Boundary(box))
    {
        LOG_CHECK_DEBUG(INFO) << "new box, box.classID: " << box.classID << ", box.id: "
                              << box.id
                              << " close to image boundary, don't creat new small object in static map";
        return;
    }

    if (box.location.machine.x < NEAR_DISTANCE || box.location.machine.x > VALID_DISTANCE)
    {
        LOG_CHECK_DEBUG(INFO) << "new box, box.classID: " << box.classID << ", box.id: "
                              << box.id
                              << " close too far or near machine.x: "
                              << box.location.machine.x
                              << ", don't creat new small object in static map";
        return;
    }

    if (abs(box.edgeLeft.machine.y - box.edgeRight.machine.y) <
        SMALL_OBJECT_MIN_WIDTH[box.classID])
    {
        LOG_CHECK_DEBUG(INFO) << "new box, box.classID: " << box.classID << ", box.id: "
                              << box.id
                              << " box.left close to right : "
                              << box.edgeLeft.machine.Print()
                              << " , " << box.edgeRight.machine.Print() << " < "
                              << SMALL_OBJECT_MIN_WIDTH[box.classID]
                              << ", don't creat new small object in static map";
        return;
    }

    if (abs(box.edgeLeft.machine.y - box.edgeRight.machine.y) >
        SMALL_OBJECT_MAX_WIDTH[box.classID])
    {
        LOG_CHECK_DEBUG(INFO) << "new box, box.classID: " << box.classID << ", box.id: "
                              << box.id
                              << " box.left close to right : "
                              << box.edgeLeft.machine.Print()
                              << " , " << box.edgeRight.machine.Print() << " > "
                              << SMALL_OBJECT_MAX_WIDTH[box.classID]
                              << ", don't creat new small object in static map";
        return;
    }

    LOG_CHECK_DEBUG(INFO) << "new box to SmallObject: "
                          << box.classID << ", box.id: " << box.id
                          << " box.left close to right : " << box.edgeLeft.machine.Print()
                          << " , " << box.edgeRight.machine.Print();

    this->staticObjectMap.objects.push_back(SmallBox2Object(box));
    this->smallObjectMapInner.push_back(SmallBox2Object(box));
    this->smallObjectUpdate.push_back(true);
    this->pointStatus.push_back({false, false, false, false});
    this->objectWeights.push_back(INIT_WEIGHT_OBJECT);
}

void StaticMap::LineExtendParallel(const BoxInfo &view, const Segment &line
                                   , std::vector<Point> &points, const float extend) const
{
    Point pointStart(0, 0, 0), pointEnd(0, 0, 0);

    if (line.start.x == line.end.x)
    {
        if (view.location.world.x > line.start.x)
        {
            pointStart.x = line.start.x - extend;
            pointStart.y = line.start.y;
            pointEnd.x = line.end.x - extend;
            pointEnd.y = line.end.y;

            if (pointStart.y > pointEnd.y)
            {
                points.push_back(line.end);
                points.push_back(line.start);
                points.push_back(pointStart);
                points.push_back(pointEnd);
            }
            else
            {
                points.push_back(line.start);
                points.push_back(line.end);
                points.push_back(pointEnd);
                points.push_back(pointStart);
            }
        }
        else
        {
            pointStart.x = line.start.x + extend;
            pointStart.y = line.start.y;
            pointEnd.x = line.end.x + extend;
            pointEnd.y = line.end.y;

            if (pointStart.y > pointEnd.y)
            {
                points.push_back(line.start);
                points.push_back(line.end);
                points.push_back(pointEnd);
                points.push_back(pointStart);
            }
            else
            {
                points.push_back(line.end);
                points.push_back(line.start);
                points.push_back(pointStart);
                points.push_back(pointEnd);
            }
        }
    }
    else
    {
        if (view.location.world.y > line.start.y)
        {
            pointStart.x = line.start.x;
            pointStart.y = line.start.y - extend;
            pointEnd.x = line.end.x;
            pointEnd.y = line.end.y - extend;

            if (pointStart.x > pointEnd.x)
            {
                points.push_back(line.start);
                points.push_back(line.end);
                points.push_back(pointEnd);
                points.push_back(pointStart);
            }
            else
            {
                points.push_back(line.end);
                points.push_back(line.start);
                points.push_back(pointStart);
                points.push_back(pointEnd);
            }
        }
        else
        {
            pointStart.x = line.start.x;
            pointStart.y = line.start.y + extend;
            pointEnd.x = line.end.x;
            pointEnd.y = line.end.y + extend;

            if (pointStart.x > pointEnd.x)
            {
                points.push_back(line.end);
                points.push_back(line.start);
                points.push_back(pointStart);
                points.push_back(pointEnd);
            }
            else
            {
                points.push_back(line.start);
                points.push_back(line.end);
                points.push_back(pointEnd);
                points.push_back(pointStart);
            }
        }
    }
}

void
StaticMap::BoxExtendVerticalBisector(const std::vector<BoxInfo> boxes
                                     , std::vector<cv::Rect> &rects)
{
    for (auto box : boxes)
    {
        cv::Rect rect;
        BoxExtendVerticalBisectorRect(box, rect);
        rects.push_back(rect);
    }
}

void StaticMap::BoxExtendVerticalBisectorRect(const BoxInfo &box
                                              , StaticMap::cv_rect_world &rect)
{
    Point p1, p2;
    std::vector<Point> points;
    if (box.IsDesk())
    {
        p1 = box.locationSerials[0].world;
        p2 = box.locationSerials[1].world;
    }
    else
    {
        p1 = box.edgeLeft.world;
        p2 = box.edgeRight.world;
    }

    LineExtendVerticalBisector(view, p1, p2, points
                               , detectParam.modelProperty.trackingExtend[box.classID]);

    Trapezoid2Rect(points[0], points[1], points[2], points[3], rect);
}


void StaticMap::BoxExtendVerticalBisectorRect(const BoxInfo &box, cv::Rect &rect)
{
    Point p1, p2;
    std::vector<Point> points;
    if (box.className == "desk_rect")
    {
        p1 = box.locationSerials[0].world;
        p2 = box.locationSerials[1].world;
    }
    else
    {
        p1 = box.edgeLeft.world;
        p2 = box.edgeRight.world;
    }

    LineExtendVerticalBisector(view, p1, p2, points
                               , detectParam.modelProperty.trackingExtend[box.classID]);

    Trapezoid2Rect(points[0], points[1], points[2], points[3], rect);
}


void StaticMap::LineExtendVerticalBisector(const BoxInfo &view, const Point &p1
                                           , const Point &p2
                                           , std::vector<Point> &points
                                           , const float extend) const
{
    Segment lineOriginal = Segment(p1, p2);
    if (lineOriginal.start.x == lineOriginal.end.x ||
        lineOriginal.start.y == lineOriginal.end.y)
    {
        LineExtendParallel(view, lineOriginal, points, extend);
    }
    else
    {
        PointOnVerticalBisector(view, lineOriginal, extend, points);
    }
}

void
StaticMap::ViewKMiddleVerticalBisector(const float k, const float b, const float extend
                                       , const Point &middlePoint, const Point &viewPoint
                                       , Point &dst) const
{
    float k1 = -1.0 / k;
    if (k > 0)
    {
        if (k * viewPoint.x + b > viewPoint.y)
        {
            dst.y = middlePoint.y + abs(sin(atan(k1))) * extend;
            dst.x = middlePoint.x - abs(cos(atan(k1))) * extend;
        }
        else
        {
            dst.y = middlePoint.y - abs(sin(atan(k1))) * extend;
            dst.x = middlePoint.x + abs(cos(atan(k1))) * extend;
        }
    }
    else
    {
        if (k * viewPoint.x + b > viewPoint.y)
        {
            dst.y = middlePoint.y + abs(sin(atan(k1))) * extend;
            dst.x = middlePoint.x + abs(cos(atan(k1))) * extend;
        }
        else
        {
            dst.y = middlePoint.y - abs(sin(atan(k1))) * extend;
            dst.x = middlePoint.x - abs(cos(atan(k1))) * extend;
        }
    }
}

void StaticMap::SetPointOrders(const Segment &line, const Point viewPoint
                               , const Point &startPoint
                               , const Point &endPoint, std::vector<Point> &points) const
{
    float k, b;
    line.GetLine(k, b);
    if (k * startPoint.x > startPoint.y)
    {
        if (line.start.x > line.end.x)
        {
            points.push_back(line.start);
            points.push_back(line.end);
            points.push_back(endPoint);
            points.push_back(startPoint);
        }
        else
        {
            points.push_back(line.end);
            points.push_back(line.start);
            points.push_back(startPoint);
            points.push_back(endPoint);
        }
    }
    else
    {
        if (line.start.x > line.end.x)
        {
            points.push_back(line.end);
            points.push_back(line.start);
            points.push_back(startPoint);
            points.push_back(endPoint);
        }
        else
        {
            points.push_back(line.start);
            points.push_back(line.end);
            points.push_back(endPoint);
            points.push_back(startPoint);
        }
    }
}

void StaticMap::PointOnVerticalBisector(const BoxInfo &view, const Segment &line
                                        , const float extend
                                        , std::vector<Point> &points) const
{
    float k, b;
    Point verticalBisectorPoint;
    Point middlePoint = line.start;
    Point viewPoint = view.location.world;
    UpdatePoint(line.end, middlePoint);

    line.GetLine(k, b);

    ViewKMiddleVerticalBisector(k, b, extend, middlePoint, viewPoint
                                , verticalBisectorPoint);

    float kLine1, bLine1, kLine2, bLine2, kLineParallel, bLineParallel;

    Segment verticalBisector = Segment(middlePoint, verticalBisectorPoint);

    verticalBisector.GetLine(kLine1, bLine1);
    bLine1 = line.start.y - kLine1 * line.start.x;

    verticalBisector.GetLine(kLine2, bLine2);
    bLine2 = line.end.y - kLine2 * line.end.x;

    kLineParallel = k;
    bLineParallel = verticalBisectorPoint.y - kLineParallel * verticalBisectorPoint.x;

    Point crossPointStart(0, 0, 0), crossPointEnd(0, 0, 0);
    GetCrossPoint(kLine1, kLineParallel, bLine1, bLineParallel, crossPointStart);
    GetCrossPoint(kLine2, kLineParallel, bLine2, bLineParallel, crossPointEnd);

    SetPointOrders(line, viewPoint, crossPointStart, crossPointEnd, points);
}

void StaticMap::GetCrossPoint(const float k1, const float k2
                              , const float b1, const float b2, Point &crossPoint) const
{
    crossPoint.x = (b2 - b1) / (k1 - k2);
    crossPoint.y = crossPoint.x * k1 + b1;
}

void
StaticMap::PointsExtendSingle4(const Point &left, const Point &right, const float &extend
                               , std::vector<Point> &points)
{
    points.clear();
    float distanceX = abs(left.x - right.x);
    float distanceY = abs(left.y - right.y);
    Point locationLeftExtend, locationRightExtend;
    if (distanceX > distanceY)
    {
        if (min(left.y, right.y) > view.location.world.y)
        {
            locationLeftExtend = {left.x, left.y + extend, 0};
            locationRightExtend = {right.x, right.y + extend, 0};
            if (left.x > right.x)
            {
                points.push_back(right);
                points.push_back(left);
                points.push_back(locationLeftExtend);
                points.push_back(locationRightExtend);
            }
            else
            {
                points.push_back(left);
                points.push_back(right);
                points.push_back(locationRightExtend);
                points.push_back(locationLeftExtend);
            }
        }
        else
        {
            locationLeftExtend = {left.x, left.y - extend, 0};
            locationRightExtend = {right.x, right.y - extend, 0};
            if (left.x > right.x)
            {
                points.push_back(left);
                points.push_back(right);
                points.push_back(locationRightExtend);
                points.push_back(locationLeftExtend);
            }
            else
            {
                points.push_back(right);
                points.push_back(left);
                points.push_back(locationLeftExtend);
                points.push_back(locationRightExtend);
            }
        }
    }
    else
    {
        if (min(left.x, right.x) > view.location.world.x)
        {
            locationLeftExtend = {left.x + extend, left.y, 0};
            locationRightExtend = {right.x + extend, right.y, 0};

            if (left.y > right.y)
            {
                points.push_back(left);
                points.push_back(right);
                points.push_back(locationRightExtend);
                points.push_back(locationLeftExtend);
            }
            else
            {
                points.push_back(right);
                points.push_back(left);
                points.push_back(locationLeftExtend);
                points.push_back(locationRightExtend);
            }
        }
        else
        {
            locationLeftExtend = {left.x - extend, left.y, 0};
            locationRightExtend = {right.x - extend, right.y, 0};

            if (left.y > right.y)
            {
                points.push_back(right);
                points.push_back(left);
                points.push_back(locationLeftExtend);
                points.push_back(locationRightExtend);
            }
            else
            {
                points.push_back(left);
                points.push_back(right);
                points.push_back(locationRightExtend);
                points.push_back(locationLeftExtend);
            }
        }
    }
}

void StaticMap::BoxExtendSingle4(const BoxInfo &box, std::vector<Point> &points)
{
    float extend = detectParam.modelProperty.trackingExtend[box.classID];
    psl::Location left = box.edgeLeft.world;
    psl::Location right = box.edgeRight.world;

//    if (left.DistanceXY(right) < extend)
//    {
//        psl::Location extendPoint;
//        psl::Location middle = left;
//        UpdatePoint(right, middle);
//
//        ExtendLine(middle, left, extendPoint, extend / 2);
//        left = extendPoint;
//        ExtendLine(middle, right, extendPoint, extend / 2);
//        right = extendPoint;
//    }

    LineExtendVerticalBisector(view, left, right, points, extend);
}

void StaticMap::SmallRectExtend(const float extendPoint, cv_rect_world &rect) const
{
    if (rect.height < extendPoint)
    {
        rect.y = rect.y + rect.height / 2 - extendPoint / 2;
        rect.height = extendPoint;
    }

    if (rect.width < extendPoint)
    {
        rect.x = rect.x + rect.width / 2 - extendPoint / 2;
        rect.width = extendPoint;
    }
}

void
StaticMap::BoxesSmallObjectsIOU(const vector<BoxInfo> &boxes
                                , std::vector<std::vector<float>> &ious)
{
    for (auto box : boxes)
    {
        vector<float> iou;
        cv_rect_world boxRect;

        LOG_CHECK_DEBUG(INFO) << "small Box.classID: " << box.classID << ", Box.id: "
                              << box.id
                              << ", box.edgeLeft.world: " << box.edgeLeft.world.Print()
                              << " , box.edgeRight.world: "
                              << box.edgeRight.world.Print() << ", box.edgeLeft.machine: "
                              << box.edgeLeft.machine.Print()
                              << ", box.edgeRight.machine: "
                              << box.edgeRight.machine.Print();

        BoxExtendVerticalBisectorRect(box, boxRect);
        SmallRectExtend(detectParam.modelProperty.trackingExtend[box.classID], boxRect);
        Extend<float>(SMALL_EXTEND, boxRect);

        // TODO : 2022-10-18 11:30:14 [hao] use smallObjectMapInner
        for (auto object : this->staticObjectMap.objects)
        {
            if (box.classID == object.classID)
            {
                cv_rect_world objectRect;

                Trapezoid2Rect(object.locationWorld[0], object.locationWorld[1]
                               , object.locationWorld[2], object.locationWorld[3]
                               , objectRect);
                SmallRectExtend(detectParam.modelProperty.trackingExtend[object.classID]
                                , objectRect);

                Extend<float>(SMALL_EXTEND, objectRect);

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

DLObject StaticMap::SmallBox2Object(const BoxInfo &box)
{
    DLObject object = Map::Box2Object(box);

    BoxExtendSingle4(box, object.locationWorld);

    return object;
}

void StaticMap::Reset()
{
    for (int i = 0; i < this->smallObjectUpdate.size(); ++i)
    {
        this->smallObjectUpdate[i] = false;
    }
    mapUpdate = false;
}

void StaticMap::GetMap(psl::SenmaticMap &senmaticMap) const
{
    // TODO : 2022-10-18 11:31:13 [hao] use smallObjectMapInner
    for (int i = 0; i < this->staticObjectMap.objects.size(); ++i)
    {
        auto object = this->staticObjectMap.objects[i];

        if (this->objectWeights[i] <= SEND_WEIGHT_OBJECT
            && std::find(SMALL_OBJECT_NAMES.begin(), SMALL_OBJECT_NAMES.end()
                         , detectParam.modelProperty.names[this->staticObjectMap.objects[i].classID])
               != SMALL_OBJECT_NAMES.end())
        {
            LOG_CHECK_DEBUG(INFO) << "static map object, object.classID: "
                                  << this->staticObjectMap.objects[i].classID
                                  << ", object.id: "
                                  << this->staticObjectMap.objects[i].id
                                  << ", objectWeight: " << this->objectWeights[i]
                                  << " <= " << SEND_WEIGHT_OBJECT << " don't send it！";
            continue;
        }

        if (object.classID == 7)
        {
            BoxInfo directionPoint;
            directionPoint.location.world.x =
                    2 * object.locationWorld[0].x - object.locationWorld[2].x;
            directionPoint.location.world.y =
                    2 * object.locationWorld[0].y - object.locationWorld[2].y;
            std::vector<psl::Location> points;
            float distance = object.locationWorld[0].DistanceXY(object.locationWorld[1]);

            LineExtendVerticalBisector(directionPoint, object.locationWorld[0]
                                       , object.locationWorld[1], points, distance);
            object.locationWorld.swap(points);
            senmaticMap.objects.push_back(object);
        }
        else
        {
            senmaticMap.objects.push_back(object);
        }

        LOG_CHECK_DEBUG(INFO) << "object ready to send classID:" << object.classID
                              << " , object.id: " << object.id << ", statuses:"
                              << this->pointStatus[i][0] << ", "
                              << this->pointStatus[i][1]
                              << " , " << this->pointStatus[i][2] << " , "
                              << this->pointStatus[i][3];

        if (mapUpdate)
            senmaticMap.status = psl::SenmaticMap::Status::UPDATE;
        else
            senmaticMap.status = psl::SenmaticMap::Status::NO_UPDATE;
    }
}

void StaticMap::Bind(std::vector<int> &binds, std::vector<BoxInfo> &boxesDesk
                     , std::vector<int> &matchedTrack)
{
    // TODO: change boxes or objects get cv::Rect function to get max IOU;
    std::vector<std::vector<float>> ious;

    BoxesObjectsIOU(boxesDesk, ious);

    Tracking(boxesDesk, this->trackerDesk);

    GetMatchedByTrack(boxesDesk, matchedTrack);
    SetIouByMatchedTrack(ious, matchedTrack);

    BindByIOUEightQueen(ious, binds);
    //    BindByMaxIOU(ious, binds);
}

void
StaticMap::BoxesObjectsIOU(const vector<BoxInfo> &boxesDesk
                           , std::vector<std::vector<float>> &ious)
{
    std::vector<cv::Rect> objectRects, boxRects;

    GetDeskMap4Rects(objectRects);

    BoxExtendVerticalBisector(boxesDesk, boxRects);
    for (int i = 0; i < boxRects.size(); ++i)
    {
        vector<float> iou;
        cv::Rect boxRect = boxRects[i];

        for (int j = 0; j < objectRects.size(); ++j)
        {
            if (boxesDesk[i].classID == this->staticObjectMap.objects[j].classID)
            {
                cv::Rect objectRect = objectRects[j];
                iou.push_back(IoU(objectRect, boxRect));
            }
            else
            {
                iou.push_back(0);
            }
        }
        ious.push_back(iou);
    }

    for (int i = 0; i < boxRects.size(); ++i)
    {
        for (int j = 0; j < objectRects.size(); ++j)
        {
            LOG_CHECK_DEBUG(INFO) << "Desk box.classID: " << boxesDesk[i].classID
                                  << " , object.classID: "
                                  << this->staticObjectMap.objects[j].classID
                                  << ", object id: "
                                  << this->staticObjectMap.objects[j].id
                                  << ", iou: " << ious[i][j];
        }
    }
}

void StaticMap::GetDeskMap4Rects(std::vector<cv::Rect> &objectRects)
{
    for (auto object : this->staticObjectMap.objects)
    {
        cv::Rect objectRect;
        Trapezoid2Rect(object.locationWorld[0], object.locationWorld[1]
                       , object.locationWorld[2]
                       , object.locationWorld[3], objectRect);
        objectRects.push_back(objectRect);
    }
}

void StaticMap::SetLastID(const vector<TrackingBox<ShapeType>> &rectResult)
{
    for (int i = 0; i < rectResult.size(); ++i)
    {
        if (this->lastID <= rectResult[i].id)
        {
            this->lastID = rectResult[i].id + 1;
        }
    }
}

void StaticMap::Tracking(std::vector<BoxInfo> &boxes, Sort &tracker)
{
    vector<TrackingBox<ShapeType>> rectResult;

    Tracking(boxes, rectResult, tracker);
    SetLastID(rectResult);

    for (int i = 0; i < rectResult.size(); ++i)
    {
        boxes[i].SetRect(rectResult[i].region.data);
        boxes[i].id = rectResult[i].id;
    }
}

void StaticMap::UpdateDeskMap(const std::vector<int> &binds
                              , const std::vector<BoxInfo> &boxesDesk
                              , const std::vector<std::vector<int>> &statuses
                              , const std::vector<int> &matchedTrack)
{
    std::vector<int> removeIDs;
    std::map<int, int> updateIDs;
    for (int i = 0; i < boxesDesk.size(); ++i)
    {
        if (matchedTrack[i] > -1)
        {
            UpdateMapByBox(boxesDesk[i], binds[i], statuses[i]);
        }
        else if (binds[i] > -1)
        {
            removeIDs.push_back(this->staticObjectMap.objects[binds[i]].id);
            updateIDs[boxesDesk[i].id] = this->staticObjectMap.objects[binds[i]].id;
            UpdateMapByBox(boxesDesk[i], binds[i], statuses[i]);
        }
        else
        {
            if (PointsValid(statuses[i]) < 2)
            {
                LOG_CHECK_DEBUG(INFO) << "Desk PointsValid < 2, don't add new desk";
                continue;
            }
            if (DeskBoxInvalid(boxesDesk[i]))
            {
                LOG_CHECK_DEBUG(INFO) << "Desk two points' distance < "
                                      << DESK_VALID_POINT_DIS << ", don't add new desk";
                continue;
            }
            AddDeskMap(boxesDesk[i], statuses[i]);
        }
    }

    RemoveByIDs(removeIDs);
    UpdateByIDs(updateIDs);
}

int StaticMap::PointsValid(const std::vector<int> &status)
{
    int validPoints = 0;

    for (auto valid : status)
    {
        if (valid) validPoints++;
    }

    return validPoints;
}

bool StaticMap::DeskBoxInvalid(const BoxInfo &box)
{
    float distance = abs(
            box.locationSerials[0].machine.y - box.locationSerials[1].machine.y);
    LOG_CHECK_DEBUG(INFO) << "new box's locationSerials [machine] (x,y,z): ("
                          << box.locationSerials[0].machine.Print() << "),("
                          << box.locationSerials[1].machine.Print() << "),("
                          << box.locationSerials[2].machine.Print() << "),("
                          << box.locationSerials[3].machine.Print() << ")";

    if (distance < DESK_VALID_POINT_DIS)
    {
        return true;
    }
    return false;
}

void StaticMap::StoreLocation(const int id, const BoxInfo &box)
{
#ifdef SHOW_ALL_POINT
    if (box.IsDesk())
    {
        if (box.locationSerials.size() < 2)
        {
            LOG_CHECK_STATUS(INFO) << "invalid desk point count";
            this->smallObjectMapInner.at(id).locationWorld.push_back(box.location.world);
        }
        else
        {
            this->smallObjectMapInner.at(id).locationWorld.push_back(
                    box.locationSerials.at(0).world);
            this->smallObjectMapInner.at(id).locationWorld.push_back(
                    box.locationSerials.at(1).world);
        }
    }
    else
    {
        this->smallObjectMapInner.at(id).locationWorld.push_back(box.location.world);
    }
    //    this->staticObjectMap.objects[bind].locationWorld.push_back(box.edgeLeft.world);
//    this->staticObjectMap.objects[bind].locationWorld.push_back(box.edgeRight.world);
#endif
}

void StaticMap::DrawPoint(cv::Mat image) const
{
    for (int i = 0; i < this->smallObjectMapInner.size(); ++i)
    {
        psl::Object object = this->smallObjectMapInner[i];
        cv::Scalar colorCurrent = colors.at(object.classID);
        auto colorCurrentPoint = Color::Light(colorCurrent);

        std::vector<cv::Point> points;
        Location2Points(object.locationWorld, points);

        for (int j = 0; j < points.size(); ++j)
        {
            auto color = colorCurrentPoint;

            if (2 == j or 3 == j) color = Color::WHITE;

            cv::circle(image, cv::Point(points[j].x, points[j].y), 1, color, -1);
        }
    }
}

void StaticMap::DrawSmallObject(cv::Mat image) const
{

    for (int i = 0; i < this->staticObjectMap.objects.size(); ++i)
    {
        psl::Object object = this->staticObjectMap.objects[i];

        // TODO : 2022-10-18 14:53:27 [hao] check desk, not only id
        if (object.classID == 7) continue;

        cv::Scalar colorCurrent = colors.at(object.classID);
        auto colorCurrentPoint = Color::Light(colorCurrent);

        if (this->objectWeights[i] <= SEND_WEIGHT_OBJECT)
        {
//            continue;
            colorCurrent = cv::Scalar(0, 0, 255);
        }

        cv_rect_world boxResult;

        Trapezoid2Rect(object.locationWorld[0], object.locationWorld[1]
                       , object.locationWorld[2]
                       , object.locationWorld[3], boxResult);

        SmallRectExtend(detectParam.modelProperty.trackingExtend[object.classID], boxResult);
        Extend<float>(SMALL_EXTEND, boxResult);

        cv::putText(image, std::to_string(object.id), cv::Point(boxResult.x, boxResult.y)
                    , cv::FONT_HERSHEY_PLAIN, 1, colorCurrent);

        std::vector<cv::Point> points;
        Location2Points(object.locationWorld, points);

        auto result = cv::boundingRect(points);
        cv::rectangle(image, cv::Point(result.x, result.y)
                      , cv::Point((result.x + result.width), (result.y + result.height))
                      , colorCurrent, 2);
    }
}

void StaticMap::DrawDesk(cv::Mat image) const
{
    for (int i = 0; i < this->pointStatus.size(); ++i)
    {
        psl::Object object = this->staticObjectMap.objects[i];
        LOG_CHECK_DEBUG(INFO) << "object id: " << object.id << ", four key point: ";
        for (auto location : object.locationWorld)
        {
            LOG_CHECK_DEBUG(INFO) << location.Print();
        }

        if (object.classID != 7) continue;
        cv::Scalar colorCurrent = colors.at(object.classID);
        cv::Rect boxResult;

        if (object.locationWorld.size() != 4)
        {
            LOG_CHECK_STATUS(INFO) << "desk key point is not 4 but :"
                                   << object.locationWorld.size();
            continue;
        }
        Trapezoid2Rect(object.locationWorld[0], object.locationWorld[1]
                       , object.locationWorld[2]
                       , object.locationWorld[3], boxResult);

        cv::rectangle(image, cv::Point(boxResult.x, boxResult.y), cv::Point(
                (boxResult.x + boxResult.width), (boxResult.y + boxResult.height))
                      , Color::GREEN, 2);
        cv::putText(image, std::to_string(object.id), cv::Point(boxResult.x, boxResult.y)
                    , cv::FONT_HERSHEY_PLAIN, 2.5, colorCurrent);

        //Draw send points
        std::vector<cv::Point> pts;
        BoxInfo directionPoint;
        directionPoint.location.world.x =
                2 * object.locationWorld[0].x - object.locationWorld[2].x;
        directionPoint.location.world.y =
                2 * object.locationWorld[0].y - object.locationWorld[2].y;
        std::vector<psl::Location> points;
        float distance = object.locationWorld[0].DistanceXY(object.locationWorld[1]);
        LineExtendVerticalBisector(directionPoint, object.locationWorld[0]
                                   , object.locationWorld[1], points, distance);
        pts.push_back(cv::Point(points[0].x * map_type::DELTA + map_type::W / 2.0
                                , points[0].y * map_type::DELTA + map_type::H / 2.0));
        pts.push_back(cv::Point(points[1].x * map_type::DELTA + map_type::W / 2.0
                                , points[1].y * map_type::DELTA + map_type::H / 2.0));
        pts.push_back(cv::Point(points[2].x * map_type::DELTA + map_type::W / 2.0
                                , points[2].y * map_type::DELTA + map_type::H / 2.0));
        pts.push_back(cv::Point(points[3].x * map_type::DELTA + map_type::W / 2.0
                                , points[3].y * map_type::DELTA + map_type::H / 2.0));

        polylines(image, pts, true, colorCurrent, 2, 8, 0);
    }
}

void StaticMap::RotationPoint(const float angle, psl::Location &location)
{
    psl::Location newPoint;
    float x = location.x;
    float y = location.y;
    float angleTemp = angle * ANGLE_2_RADIAN;
    location.x = x * cos(angleTemp) - y * sin(angleTemp);
    location.y = y * cos(angleTemp) + x * sin(angleTemp);
}

void StaticMap::RotationAllWorld(const float angle)
{
    RotationObject(angle, this->staticObjectMap.objects);
    RotationObject(angle, this->smallObjectMapInner);
}

void StaticMap::RotationObject(const float angle, std::vector<DLObject> &objects)
{
    for (auto &object : objects)
    {
        for (auto &location : object.locationWorld)
        {
            RotationPoint(angle, location);
        }
    }
}

void StaticMap::CompareBinds(const std::vector<int> &queenPlace
                  , const std::vector<std::vector<float>> &iouMap
                  , std::vector<int> &indexes)
{
    int currentNumbers = 0;
    float currentIouSum = 0;

    for (int i = 0; i < queenPlace.size(); ++i)
    {
        if (queenPlace[i] >=0)
        {
            currentNumbers++;
            currentIouSum += iouMap[i][queenPlace[i]];
        }
    }

    if (currentNumbers > indexesBindsNumbers
        || (currentNumbers == indexesBindsNumbers && currentIouSum > indexesIouSum))
    {
        indexes = queenPlace;
        indexesIouSum = currentIouSum;
        indexesBindsNumbers = currentNumbers;
    }
}

bool StaticMap::IsValidCompare(int row, int col
                               , const std::vector<std::vector<int>> &indexMap
                               , std::vector<int> &queenPlace)
{
    if (indexMap[row][col] == -1)
    {
        return true;
    }

    for (int i = row - 1; i >= 0; --i)
    {
        if (queenPlace[i] == indexMap[row][col])
        {
            return false;
        }
    }

    return true;
}

void StaticMap::EightQueen(int row,const std::vector<std::vector<int>> &indexMap
        ,const std::vector<std::vector<float>> &iouMap, std::vector<int> &queenPlace
        , std::vector<int> &indexes)
{
    if (row == indexMap.size())
    {
        CompareBinds(queenPlace,iouMap, indexes);
        return;
    }

    for (int col = 0; col < indexMap[row].size(); ++col)
    {
        if (IsValidCompare(row, col, indexMap, queenPlace))
        {
            queenPlace[row] = indexMap[row][col];
            EightQueen(row+1, indexMap, iouMap, queenPlace, indexes);
        }
    }
}

void StaticMap::BindByIOUEightQueen(const std::vector<std::vector<float>> &iouMap
                                    , std::vector<int> &indexes)
{
    std::vector<int> queenPlace;
    std::vector<std::vector<int>> indexMap;

    queenPlace.resize(iouMap.size(),-1);
    indexes.resize(iouMap.size(),-1);
    indexesIouSum = 0;
    indexesBindsNumbers = 0;

    for (auto map : iouMap)
    {
        std::vector<int> index;
        for (int i = 0; i < map.size(); ++i)
        {
            if (map[i] > 0)
            {
                index.push_back(i);
            }
        }
        index.push_back(-1);
        indexMap.push_back(index);
    }

    EightQueen(0, indexMap, iouMap, queenPlace, indexes);
}

void StaticMap::SetDistanceValidStatus(BoxInfo view)
{
    for (int i = 0; i < this->staticObjectMap.objects.size(); ++i)
    {
        auto object = this->staticObjectMap.objects[i];

        if (this->smallObjectUpdate[i] && this->objectWeights[i] == (SEND_WEIGHT_OBJECT + 1) && this->staticObjectMap.objects[i].classID < 5)
        {
            this->staticObjectMap.objects[i].status = psl::Object::DISABLE;
        }

        if (this->objectWeights[i] <= SEND_WEIGHT_OBJECT || (this->staticObjectMap.objects[i].status &&
                                            not GetObjectsDistance(this->staticObjectMap.objects[i], view.location.world)))
        {
            this->staticObjectMap.objects[i].status = psl::Object::ENABLE;
        }
    }
}

bool StaticMap::GetObjectsDistance(psl::Object &object, psl::Location &p)
{
    Point A = object.locationWorld.at(0);
    Point B = object.locationWorld.at(1);
    Point C = object.locationWorld.at(2);
    Point D = object.locationWorld.at(3);
    Point P = p;

    double distanceAB = DistanceMin(A, B, P);
    double distanceBC = DistanceMin(B, C, P);
    double distanceCD = DistanceMin(C, D, P);
    double distanceDA = DistanceMin(D, A, P);

    double distance = min({distanceAB, distanceBC, distanceCD, distanceDA});

    if (distance < NEW_OBJECT_SEND_DIS)
    {
        LOG_CHECK_STATUS(INFO) << "filter distance： " << distance << ", class id: " << object.classID << ", id: "
                               << object.id;
        return true;
    }
    return false;
}