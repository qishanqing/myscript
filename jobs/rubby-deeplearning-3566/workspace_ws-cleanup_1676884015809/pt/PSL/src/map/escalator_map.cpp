//
// Created by hao on 2021/8/11.
//

#include "escalator_map.h"
#include <numeric>
#include "psl/perception/instance.h"
#include "src/utils/utils.h"
#include "src/utils/define.h"
#include "src/utils/file.h"
#include "src/utils/gemotry.h"
#include "src/utils/image_process.h"
#include "weight_escalator_map.h"

using namespace escalator_map;
using namespace map_type;

#define REGION

#define CENTER_POINT(a, b) {(a.x+b.x)/2, (a.y+b.y)/2, (a.z + b.z)/2}

const std::string MAP_DIR = "result/map";
const float MAX_DISTANCE_LINE = 1.7; // 150cm
const float MIN_DISTANCE_HANDRAILS = 1; // 100cm
const double EPSILON = 1.0e-5;
const float MARGIN_2_REGION = 0.1;
const float MIN_DISTANCE_POINT = 0.01;

EscalatorMap::EscalatorMap()
        : weightEscalatorMap(new WeightEscalatorMap())
{

}

EscalatorMap::~EscalatorMap()
{
    RELEASE(weightEscalatorMap);
}

bool EscalatorMap::UpdateLine(const BoxInfo &box, escalator_map::Escalator &object)
{
    // TODO : iou between two line, or between all the line's points
    bool sameObject = SameByPoint(box, object);
    if (sameObject)
    {
        object.Update(box);
    }

    return sameObject;
}

bool EscalatorMap::SameByLine(const escalator_map::Escalator &object1
                              , const escalator_map::Escalator &object2) const
{
    const auto &point1 = object1.escalator->locationWorld.at(0);
    const auto &point2 = object1.escalator->locationWorld.at(1);
    const auto &point3 = object2.escalator->locationWorld.at(0);
    const auto &point4 = object2.escalator->locationWorld.at(1);

    double minDistance = LineDistance(point1, point2, point3, point4);

    bool sameLine = minDistance < MAX_DISTANCE_LINE;
    LOG_CHECK_DEBUG(INFO) << "same line object: " << sameLine << " min Distance: "
                          << minDistance;

    return sameLine;
}

bool EscalatorMap::SameByLine(const BoxInfo &box
                              , const escalator_map::Escalator &object) const
{
    // TODO : why not the handrails
    // TODO : if box.edge check is not same with handrails check, it may be have block....
    auto point1 = box.edgeLeft.world;
    auto point2 = box.edgeRight.world;
    const std::size_t partSize = box.locationSerials.size();
    LOG_CHECK_DEBUG(INFO) << "box " << box.location.world.Print();

    double minDistance = INT16_MAX;

    if (partSize > 2)
    {
        for (const auto &p : box.locationSerials)
        {
            minDistance = MIN(minDistance, LineDistance(p.world, p.world
                                                        , object.escalator->locationWorld.at(
                            0)
                                                        , object.escalator->locationWorld.at(
                            1)));
        }
    }
    else
    {
        if (partSize == 2)
        {
            // TODO : get the left and right
            point1 = box.locationSerials.begin()->world;
            point2 = box.locationSerials.rbegin()->world;
        }
        else if (partSize == 1)
        {
            point1 = box.locationSerials.begin()->world;
            point2 = box.locationSerials.begin()->world;
        }
        else
        {
            return false; // TODO : should process with inView
        }

        minDistance = LineDistance(point1, point2
                                   , object.escalator->locationWorld.at(0)
                                   , object.escalator->locationWorld.at(1));
    }

    bool sameLine = minDistance < MAX_DISTANCE_LINE;
    LOG_CHECK_DEBUG(INFO) << "same line box: " << sameLine << " min Distance: "
                          << minDistance;

    if (sameLine)
    {
//        LOG_CHECK_DEBUG(INFO) << "point1: " << point1.Print() << " : " << point2.Print();
//        LOG_CHECK_DEBUG(INFO) << "point2: "
//                              << object.escalator->locationWorld.at(0).Print()
//                              << " : " << object.escalator->locationWorld.at(1).Print();
    }

    return sameLine;
}

bool EscalatorMap::SameByPoint(const BoxInfo &box
                               , const escalator_map::Escalator &object) const
{
    if (box.locationSerials.empty()) return false; // TODO : should process with inView

    // TODO : why not the handrails
    // TODO : if box.edge check is not same with handrails check, it may be have block....
    LOG_CHECK_DEBUG(INFO) << "box " << box.location.world.Print();


    double minDistance = INT16_MAX;

    for (const auto &p : box.locationSerials)
    {
        for (const auto h : object.handrails)
        {
            minDistance = MIN(minDistance, p.world.DistanceXY(h.location));
        }
    }

    bool sameLine = minDistance < MAX_DISTANCE_LINE;
    LOG_CHECK_DEBUG(INFO) << "same line box: " << sameLine << " min Distance: "
                          << minDistance;

    return sameLine;
}

void EscalatorMap::Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view
                          , const psl::Time &time)
{
    Timer timer;
    Map::Update(boxes, view, time);

    Reset();

    // TODO : different with lidar or not
    regions.Update(boxes, view);

#ifndef REGION
    weightEscalatorMap->Update(boxes, view);
#endif
    if (not boxes.empty())
    {
        this->time = time;
        Fresh(boxes);
        Merge();
    }

    BindRegion();
    for (auto &map : mapInner) map.Fit();
    Remove(view);
    LOG_CHECK_TIME(INFO) << timer.TimingStr();
}

void EscalatorMap::Add(const BoxInfo &box)
{
    // TODO : why is empty, shuld be filter befor here
    if (box.locationSerials.empty()) return;

    // too close, do not use the box
    for (const auto &part : box.locationSerials)
    {
        if (part.Inview()) return;
    }

    this->mapInter.push_back(Box2Object(box));
    this->mapInner.push_back(
            Box2Escalator(box)); // must after escalatorMap update
    ++this->lastID;
}

psl::Object EscalatorMap::Box2Object(const BoxInfo &box)
{
    DLObject object = Map::Box2Object(box);

    // TODO : should use lidar
    object.locationWorld.push_back(box.edgeLeft.world);
    object.locationWorld.push_back(box.edgeRight.world);

    return object;
}

escalator_map::Escalator EscalatorMap::Box2Escalator(const BoxInfo &box)
{
    escalator_map::Escalator escalatorNew;
    escalatorNew.escalator = --mapInter.end();

    for (const auto &part : box.locationSerials)
    {
        LOG_CHECK_DEBUG(INFO) << "handrails location lidar: " << part.machine.Print();
        LOG_CHECK_DEBUG(INFO) << "handrails location world: " << part.world.Print();
        // TODO : for the neighbor parts
        escalatorNew.handrails.push_back(
                escalator_map::Point(part.world, escalatorNew.groupNum));
        escalatorNew.groupNum++;
    }

    escalatorNew.id = this->lastID;
    escalatorNew.classID = box.classID;
    escalatorNew.isEscalator = box.IsEscalator();
    escalatorNew.Fit();

    return escalatorNew;
}

void EscalatorMap::GetMap(psl::SenmaticMap &senmaticMap) const
{
    for (const auto &object : this->mapInter)
    {
        senmaticMap.objects.push_back(object);

        if (mapUpdate)
            senmaticMap.status = psl::SenmaticMap::Status::UPDATE;
        else
            senmaticMap.status = psl::SenmaticMap::Status::NO_UPDATE;
    }
}

void EscalatorMap::Reset()
{
    mapUpdate = false;
    bindCount.clear();

    for (auto &object : this->mapInner)
    {
        object.bindRegionID = -1;
        object.update = false;
        for (auto &h : object.handrails)
        {
            h.update = false;
//            h.useful = false;
        }
    }
}

void EscalatorMap::Fresh(const std::vector<BoxInfo> &boxes)
{
    for (auto box : boxes)
    {
        if (not box.IsEscalator()) continue;
        if (box.locationSerials.empty()) continue;
        if (box.sideView) continue;

        LOG_CHECK_DEBUG(INFO) << "size: " << box.locationSerials.size();

        bool update = false;

        for (auto &object : this->mapInner)
        {
            object.view = view;
            if (box.locationSerials.size() < 2)
                box.location = view.location; // TODO : ugly
            update |= UpdateLine(box, object);
            if (update) break;
        }

        if (not update)
        {
            Add(box);
        }

        mapUpdate = true;
    }
    END();
}

void EscalatorMap::Remove(const BoxInfo &view)
{
    LOG_CHECK_DEBUG(INFO) << "machine: " << view.location.world.Print();

    for (auto &object : this->mapInner) object.RemoveWithView(view);

    RemoveWithRegion();
    Delete();
//    WarpWithRegion(); // TODO : should remove, make sure the fit is valid
    END();
}

void EscalatorMap::Delete()
{
    int size = this->mapInner.size();

    for (int i = 0; i < size; ++i)
    {
        auto &object = this->mapInner.at(i);

        // remove invalid handrails
        object.Delete();

        // remove invalid object
        if (object.handrails.empty())
        {
            LOG_CHECK_DEBUG(INFO) << "remove, part empty";
            this->mapInter.erase(object.escalator);
            this->mapInner.erase(this->mapInner.begin() + i);
            i--;
            size--;
            continue;
        }
    }

    END();
}

cv::Mat EscalatorMap::Show(const BoxInfo &view, const std::vector<BoxInfo> &boxes, cv::Mat image)
{
    const size_t size = this->mapInner.size();
    char message[1000];

#ifndef REGION
    weightEscalatorMap->GetImage(image);
    if (image.empty()) image = cv::Mat::zeros(W, H, CV_8UC3);
#endif

    // draw view
    auto machine = view.location.world + LOCATION_ADD;
    auto left = view.edgeLeft.world + LOCATION_ADD;
    auto right = view.edgeRight.world + LOCATION_ADD;
    ImageProcess::DrawDottedLine2(image, cv::Point(int(machine.x * DELTA)
                                                   , int(machine.y * DELTA))
                                  , cv::Point(int(left.x * DELTA), int(left.y * DELTA))
                                  , Color::COLOR_VIEW, 2);
    ImageProcess::DrawDottedLine2(image, cv::Point(int(machine.x * DELTA)
                                                   , int(machine.y * DELTA))
                                  , cv::Point(int(right.x * DELTA), int(right.y * DELTA))
                                  , Color::COLOR_VIEW, 2);
    ImageProcess::DrawDottedLine1(image, cv::Point(int(left.x * DELTA)
                                                   , int(left.y * DELTA))
                                  , cv::Point(int(right.x * DELTA), int(right.y * DELTA))
                                  , Color::COLOR_VIEW, 2);

    // draw grid [-10, 10]
    for (int i = -RANGE; i <= RANGE; ++i)
    {
        int x = i + BASE;
        cv::line(image, cv::Point(int(x * STEP), 0)
                 , cv::Point(int(x * STEP), H)
                 , Color::COLOR_BACK, 1);

        cv::line(image, cv::Point(0, int(x * STEP))
                 , cv::Point(W, int(x * STEP))
                 , Color::COLOR_BACK, 1);

        cv::putText(image, std::to_string(i)
                    , cv::Point(10, MAX(20, int(x *STEP)))
                    , cv::FONT_HERSHEY_PLAIN, 1, Color::COLOR_TEXT);
        cv::putText(image, std::to_string(i), cv::Point(
                MAX(20, int(x *STEP - 20)), W - 10)
                    , cv::FONT_HERSHEY_PLAIN, 1, Color::COLOR_TEXT);
    }

    //  draw coordinate
    cv::Point origin = cv::Point(W >> 1, H >> 1);
    int lingLength = 20;
    cv::Point originEndX = origin + cv::Point(lingLength, 0);
    cv::Point originEndY = origin + cv::Point(0, lingLength);
    cv::arrowedLine(image, origin, originEndX, Color::COLOR_AXIS, 2);
    cv::arrowedLine(image, origin, originEndY, Color::COLOR_AXIS, 2);
    cv::putText(image, "x", originEndX + cv::Point(0, 10), cv::FONT_HERSHEY_PLAIN, 1
                , Color::COLOR_AXIS);
    cv::putText(image, "y", originEndY + cv::Point(-10, 0), cv::FONT_HERSHEY_PLAIN, 1
                , Color::COLOR_AXIS);

    DrawObject(view, boxes, this->regions.regions, image);

    // draws parts
    for (size_t i = 0; i < size; ++i)
    {
        const auto &object = this->mapInner.at(i);
        cv::Scalar colorCurrent = colors.at(i);
        const size_t handSize = object.handrails.size();

        for (size_t j = 0; j < handSize; ++j)
        {
            const auto &p = object.handrails.at(j);
            cv::Scalar color = colorCurrent;

            if (p.update)
            {
                color = cv::Scalar(0, 255, 0);
            }
            if (not p.useful)
            {
                color = cv::Scalar(0, 0, 255);
            }

            auto temp = p.location + LOCATION_ADD;

            cv::circle(image, cv::Point(int(temp.x * DELTA), int(temp.y * DELTA))
                       , 3, color);
            sprintf(message, "weight: %d x: %.2f y: %.2f id: %d", p.weight, p.location.x
                    , p.location.y, p.classID);
            cv::putText(image, message
                        , cv::Point(i * 300 + 50, (j + 1) * 20 + 200)
                        , cv::FONT_HERSHEY_PLAIN, 1, Color::COLOR_TEXT);
        }

        sprintf(message, "left: %.2f, %.2f right: %.2f, %.2f"
                , object.escalator->locationWorld[0].x
                , object.escalator->locationWorld[0].y
                , object.escalator->locationWorld[1].x
                , object.escalator->locationWorld[1].y);
        cv::putText(image, message, cv::Point(100, (i + 1) * 60 + 60)
                    , cv::FONT_HERSHEY_PLAIN, 2.5, colorCurrent);

        auto temp1 = object.escalator->locationWorld[0] + LOCATION_ADD;
        auto temp2 = object.escalator->locationWorld[1] + LOCATION_ADD;
        cv::line(image, cv::Point(int(temp2.x * DELTA), int(temp2.y * DELTA))
                 , cv::Point(int(temp1.x * DELTA), int(temp1.y * DELTA))
                 , colorCurrent, 2);
    }
    cv::putText(image, "num: " + std::to_string(size), cv::Point(100, 60)
                , cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255));

    return image;
}

void EscalatorMap::Merge()
{
    const size_t size = this->mapInner.size();

    for (size_t i = 0; i < size; ++i)
    {
        auto &object1 = this->mapInner.at(i);

        for (size_t j = i + 1; j < size; ++j)
        {
            auto &object2 = this->mapInner.at(j);

            if (object2.handrails.empty()) continue;

            if (SameByLine(object1, object2))
            {
                LOG_CHECK_DEBUG(INFO) << "merge";
                // TODO : should control the number by check the handrail id
                object1.handrails.insert(object1.handrails.end()
                                         , object2.handrails.begin()
                                         , object2.handrails.end());
                object2.handrails.clear();
                object1.Fit();
            }
        }
    }

    Delete();
    END();
}

void EscalatorMap::Write(const cv::Mat image) const
{
    static bool first = true;
    if (first)
    {
        char *p = getcwd(NULL, 0);
        file_op::File::RemoveDir(std::string(p) + "/" + MAP_DIR);
        first = false;
    }

    std::string file = MAP_DIR + "/" + std::to_string(GET_TIMESTAMP) + ".jpg";
    file_op::File::MkdirFromFile(file);
    cv::imwrite(file, image);
    LOG_CHECK_DEBUG(INFO) << file;
}

void EscalatorMap::Clear()
{
    this->mapInner.clear();
    this->mapInter.clear();
}

bool EscalatorMap::Save(std::ofstream &ofstream)
{
    if (not Map::Save(ofstream)) return false;
    regions.Write(ofstream);

    END();
    return true;
}


bool EscalatorMap::Load(std::ifstream &ifstream)
{
    if (not Map::Load(ifstream)) return false;
    regions.Read(ifstream);

    END();
    return true;
}

// TODO : 2022-08-01 10:40:16 [hao]  fix for all types apart from escalator.
void EscalatorMap::Modify(psl::SenmaticMap &newMap)
{
    LOG_CHECK_DEBUG(INFO) << "modify map";
    Timer timer;

    // update parts
    for (const auto &currentObject : newMap.objects)
    {
        if (psl::Object::UPDATE_SLAM != currentObject.statusOperator) continue;

        for (auto &originObjectInner : this->mapInner)
        {
            auto &originObject = originObjectInner.escalator;

            if (currentObject.id == originObject->id)
            {
                ModifyHandrial(originObjectInner, currentObject);
                *originObjectInner.escalator = currentObject;
                originObject->statusOperator = psl::Object::UPDATE_DL;
                originObjectInner.Fit();
            }
        }
    }

    // merge multi lines
    Merge();

    LOG_CHECK_TIME(INFO) << timer.TimingStr();
    END();
}

void EscalatorMap::ModifyHandrial(escalator_map::Escalator &escalator
                                  , const DLObject &newObject)
{
    // TODO : should update the handrails by distance to line
    //          or at least, keep the num of handrails and weights same to origin
    if (newObject.locationWorld.size() != 2 or
        escalator.escalator->locationWorld.size() != 2)
    {
        LOG_CHECK_DEBUG(WARNING) << "locationWorld.size() != 2"
                                 << ", newObject.locationWorld.size():"
                                 << newObject.locationWorld.size()
                                 << ", escalator->locationWorld.size(): "
                                 << escalator.escalator->locationWorld.size();

        return;
    }

    UpdateHandrial(escalator, newObject);
}

void EscalatorMap::RemoveWithRegion()
{
    RemoveMapNoRegion();
    RemovePartsBeyondRegion();
    Delete();
    RemoveMapFromOccluder();
}

void EscalatorMap::RotationAngle(const psl::Location &src
                                 , const psl::Location &dst
                                 , float &rotationAngle)
{
    double x1 = src.x;
    double y1 = src.y;
    double x2 = dst.x;
    double y2 = dst.y;
    double dist, dot, degree, angle;

    rotationAngle = 0;

    dist = sqrt(x1 * x1 + y1 * y1);
    if (dist < EPSILON) return;
    x1 /= dist;
    y1 /= dist;

    dist = sqrt(x2 * x2 + y2 * y2);
    if (dist < EPSILON) return;
    x2 /= dist;
    y2 /= dist;

    dot = x1 * x2 + y1 * y2;

    if (fabs(dot - 1.0) <= EPSILON)
        angle = 0.0;
    else if (fabs(dot + 1.0) <= EPSILON)
        angle = M_PI;
    else
    {
        double cross;
        angle = acos(dot);
        cross = x1 * y2 - x2 * y1;
        if (cross < 0)
        {
            angle = 2 * M_PI - angle;
        }
    }

    degree = angle * 180.0 / M_PI;
    rotationAngle = degree;
}

void EscalatorMap::UpdateHandrial(escalator_map::Escalator &escalator
                                  , const DLObject &newObject)
{
    float angle;
    auto &originObject = escalator.escalator;
    psl::Location src =
            originObject->locationWorld[1] - originObject->locationWorld[0];
    psl::Location dst = newObject.locationWorld[1] - newObject.locationWorld[0];

    RotationAngle(src, dst, angle);

    Location startCenter;
    Location endCenter;
    startCenter.world = CENTER_POINT(originObject->locationWorld[0]
                                     , originObject->locationWorld[1]);
    endCenter.world = CENTER_POINT(newObject.locationWorld[0]
                                   , newObject.locationWorld[1]);

    for (auto &point : escalator.handrails)
    {
        UpdatePoint(startCenter, endCenter, angle, point.location);
    }
}

void EscalatorMap::UpdatePoint(const Location &startCenter, const Location &endCenter
                               , const float &angle, psl::Location &srcPoint)
{
    srcPoint = srcPoint - startCenter.world;
    float rotationX = srcPoint.x * cos(M_PI * angle / 180)
                      - srcPoint.y * sin(M_PI * angle / 180);
    float rotationY = srcPoint.x * sin(M_PI * angle / 180)
                      + srcPoint.y * cos(M_PI * angle / 180);
    srcPoint = {rotationX, rotationY, 0};
    srcPoint = srcPoint + endCenter.world;
}

void EscalatorMap::DrawObject(const BoxInfo &view, const std::vector<BoxInfo> &boxes
                              , const std::vector<Sector> &regions, const cv::Mat &image)
{
    auto machine = view.location.world + LOCATION_ADD;

    for (auto &b : boxes)
    {
//        if (not b.IsEscalator()) continue;
        auto vertexNew = machine; // r.newVertex + addLocation;
        auto leftLeft = b.edgeLeft.world + LOCATION_ADD;
        auto rightRight = b.edgeRight.world + LOCATION_ADD;
        cv::fillConvexPoly(image, std::vector<cv::Point>{
                        cv::Point(int(vertexNew.x * DELTA), int(vertexNew.y * DELTA))
                        , cv::Point(int(leftLeft.x * DELTA), int(leftLeft.y * DELTA))
                        , cv::Point(int(rightRight.x * DELTA), int(rightRight.y * DELTA))}
                           , Color::COLOR_OBJECT);
    }

    for (auto &r : regions)
    {
        auto originVertex = r.vertexOrigin + LOCATION_ADD;
        auto vertexNew = machine; // r.newVertex + addLocation;
        auto leftOrg = r.leftOrg + LOCATION_ADD;
        auto rightOrg = r.rightOrg + LOCATION_ADD;
        auto left = r.left + LOCATION_ADD;
        auto right = r.right + LOCATION_ADD;
        auto boxLeft = r.leftLeft + LOCATION_ADD;
        auto boxRight = r.rightRight + LOCATION_ADD;
        auto leftRight = r.leftRight + LOCATION_ADD;
        auto rightLeft = r.rightLeft + LOCATION_ADD;


        ImageProcess::DrawDottedLine2(image, cv::Point(int(originVertex.x * DELTA), int(
                originVertex.y * DELTA)), cv::Point(int(leftOrg.x * DELTA), int(
                leftOrg.y * DELTA))
                                      , Color::COLOR_LAST_OBJECT, 2);
        ImageProcess::DrawDottedLine2(image, cv::Point(int(originVertex.x * DELTA), int(
                originVertex.y * DELTA)), cv::Point(int(rightOrg.x * DELTA), int(
                rightOrg.y * DELTA))
                                      , Color::COLOR_LAST_OBJECT, 2);

        cv::putText(image, std::to_string(r.id)
                    , cv::Point(int(rightOrg.x * DELTA + 20), int(
                        rightOrg.y * DELTA + 20))
                    , cv::FONT_HERSHEY_PLAIN, 1.5, Color::COLOR_FORGROUND, 2);

        if (r.boxID >= 0)
        {
            cv::putText(image, std::to_string(r.boxID)
                        , cv::Point(int(boxRight.x * DELTA + 20), int(
                            boxRight.y * DELTA + 20))
                        , cv::FONT_HERSHEY_PLAIN, 1.5, Color::COLOR_AXIS, 2);
        }
        if (r.isTarget)
        {
//            cv::fillConvexPoly(image, std::vector<cv::Point>{
//                            cv::Point(int(vertexNew.x * delta), int(vertexNew.y * delta))
//                            , cv::Point(int(boxLeft.x * delta), int(boxLeft.y * delta))
//                            , cv::Point(int(boxRight.x * delta), int(boxRight.y * delta))}
//                               , COLOR_OBJECT);

            const int radius = 2, thickness = 3;
            cv::circle(image, cv::Point(int(leftRight.x * DELTA)
                                        , int(leftRight.y * DELTA)), radius
                       , Color::COLOR_FORGROUND
                       , thickness);
            cv::circle(image, cv::Point(int(rightLeft.x * DELTA)
                                        , int(rightLeft.y * DELTA)), radius
                       , Color::BLUE, thickness);

            cv::circle(image, cv::Point(int(left.x * DELTA)
                                        , int(left.y * DELTA)), radius, Color::GREEN, thickness);
            cv::circle(image, cv::Point(int(right.x * DELTA)
                                        , int(right.y * DELTA)), radius, Color::RED, thickness);
        }
    }
}

void EscalatorMap::WarpWithRegion()
{
    if (regions.regions.empty()) return;
    if (mapInner.empty()) return;

    // TODO : old method update by new method
    int sizeMap = mapInner.size();

    // check all map
    for (int j = 0; j < sizeMap; ++j)
    {
        auto &map = mapInner.at(j);

        if (map.HaveRegion())
        {
            const auto &r = regions.regions.at(map.bindRegionID);

            auto &side1 = map.escalator->locationWorld.at(0);
            auto &side2 = map.escalator->locationWorld.at(1);
            ::Point cross1, cross2;
            GetCrossPoint4Line(Segment(r.vertex, r.left), Segment(side1, side2), cross1);
            GetCrossPoint4Line(Segment(r.vertex, r.right), Segment(side1, side2), cross2);

            if (not InViewSoft(r.vertex, r.left, r.right, side1, MARGIN_2_REGION))
            {
                if (not SameDirect(cross1, side1, side2)) side1 = cross1;
                else if (not SameDirect(cross2, side1, side2)) side1 = cross2;
            }

            if (not InViewSoft(r.vertex, r.left, r.right, side2, MARGIN_2_REGION))
            {
                if (not SameDirect(cross1, side1, side2)) side2 = cross1;
                else if (not SameDirect(cross2, side1, side2)) side2 = cross2;
            }
        }
    }
}

void EscalatorMap::BindRegion()
{
    if (mapInner.empty() or regions.Empty()) return;

    // TODO : old method update by new method
    const int sizeMap = mapInner.size();
    const int sizeRegion = regions.regions.size();
    std::vector<std::vector<int>> allSum2Region(sizeMap, std::vector<int>(sizeRegion, 0));
    bindCount.resize(sizeMap);

    // check all map
    for (int j = 0; j < sizeMap; ++j)
    {
        auto &map = mapInner.at(j);

        if (map.handrails.empty()) continue;

        int sizePart = map.handrails.size();
        std::vector<int> sum(sizeRegion, 0);
        std::vector<std::vector<int>> localBindCount(sizeRegion
                                                     , std::vector<int>(sizePart, 0));

        // TODO : what ?
        for (int i = 0; i < sizeRegion; ++i)
        {
            const auto &r = regions.regions.at(i);

            for (int k = 0; k < sizePart; ++k)
            {
                auto &part = map.handrails.at(k);
                if (InViewSoft(r.vertex, r.left, r.right, part.location, MARGIN_2_REGION))
                {
                    localBindCount.at(i).at(k) = 1;

                    if (r.boxID >= 0 and part.useful)
                        part.AddWeight(WEIGHT_ADD_BY_REGION);
                }
            }

            sum.at(i) = std::accumulate(localBindCount.at(i).begin(), localBindCount.at(
                    i).end()
                                        , 0);
        }

        allSum2Region.at(j) = sum;
    }

    std::vector<int> bindCountByRegion(sizeRegion, 0);

    for (int i = 0; i < sizeRegion; ++i)
    {
        for (int j = 0; j < sizeMap; ++j)
        {
            if (allSum2Region.at(j).at(i) > 0)
            {
                bindCountByRegion.at(i)++;
            }
        }
    }

    int updateCount = sizeRegion;
    while (updateCount--)
    {
        for (int i = 0; i < sizeRegion; ++i)
        {
            if (bindCountByRegion.at(i) == 1)
            {
                // when one region only bind a map, make pare
                // std::max_element
                for (int j = 0; j < sizeMap; ++j)
                {
                    // only one
                    if (allSum2Region.at(j).at(i) > 0)
                    {
                        auto &map = mapInner.at(j);
                        map.bindRegionID = i;
                        map.region = regions.regions.at(map.bindRegionID);

                        // clear other bind result
                        for (int k = 0; k < sizeRegion; ++k)
                        {
                            if (allSum2Region.at(j).at(k) > 0)
                            {
                                allSum2Region.at(j).at(
                                        k) = 0; // TODO : keep current i sum
                                bindCountByRegion.at(k)--;
                            }
                        }
                    }
                }
            }
        }
    }

    // set other map bind id
    for (int j = 0; j < sizeMap; ++j)
    {
        auto &map = mapInner.at(j);

        if (map.bindRegionID >= 0) continue;

        const auto &sum = allSum2Region.at(j);
        auto it = std::max_element(sum.begin(), sum.end());
        if (0 == *it) continue;
        map.bindRegionID = it - sum.begin();
        map.region = regions.regions.at(map.bindRegionID);
    }

    // update part status
    for (int j = 0; j < sizeMap; ++j)
    {
        auto &map = mapInner.at(j);
        const size_t sizePart = map.handrails.size();
        std::vector<int> localBindCount(sizePart, 0);

        // update part status
        for (int k = 0; k < sizePart; ++k)
        {
            auto &part = map.handrails.at(k);
            if (InViewSoft(map.region.vertex, map.region.left
                           , map.region.right, part.location
                           , MARGIN_2_REGION))
            {
                localBindCount.at(k) = 1;
            }
        }

        bindCount.at(j) = localBindCount;
    }
}

void EscalatorMap::RemoveMapNoRegion()
{
    if (mapInner.empty()) return;

    // TODO : old method update by new method
    // check all map
    for (auto &map : mapInner)
    {
        if (map.bindRegionID < 0)
        {
            map.handrails.clear();
            LOG_CHECK_DEBUG(INFO) << "remove handrails: no region. "
                                  << map.escalator->locationWorld.at(0).Print();
        }
    }
}

void EscalatorMap::RemoveMapFromOccluder()
{
    if (mapInner.empty() or regions.Empty()) return;

    const int sizeMap = mapInner.size();
    const int sizeRegion = regions.regions.size();
    std::vector<std::vector<int>> mapIDByRegion(sizeRegion, std::vector<int>());

    for (int j = 0; j < sizeMap; ++j)
    {
        int bindRegionID = mapInner.at(j).bindRegionID;

        if (bindRegionID < 0) continue;
        mapIDByRegion.at(bindRegionID).push_back(j);
    }

    // remove the noise point in same region
    for (int i = 0; i < sizeRegion; ++i)
    {
        std::vector<double> distance2Region;
        const auto &region = regions.regions.at(i);
        const auto &mapID = mapIDByRegion.at(i);

        if (mapID.size() >= 2)
        {
            LOG_CHECK_DEBUG(INFO) << "some map(" << Join(mapID.data(), mapID.size(), ", ")
                                  << ") in one region: " << i
                                  << " (vertex) " << region.vertex.Print();
            for (const auto &j : mapID)
            {
                double minDistance = INT16_MAX;
                auto &map = this->mapInner.at(j);
                int sizePart = map.handrails.size();
                if (map.escalator->locationWorld.size() == 2)
                {
                    minDistance = MIN(
                            map.escalator->locationWorld.at(0).DistanceXY(region.vertex)
                            , map.escalator->locationWorld.at(1).DistanceXY(
                            region.vertex));
                }
                else if (map.escalator->locationWorld.size() == 1)
                {
                    minDistance = map.escalator->locationWorld.at(0).DistanceXY(
                            region.vertex);
                }
                else
                {
                    bool getDistance = false;
                    // TODO : error value
                    for (int k = 0; k < sizePart; ++k)
                    {
                        auto &part = map.handrails.at(k);
                        if (part.weight > 0) // TODO : check
                        {
                            part.location.z = 0;
                            minDistance = MIN(minDistance, part.location.DistanceXY(
                                    region.vertex));
                            getDistance = true;
                        }
                    }
                    if (not getDistance) continue;
                }

                distance2Region.push_back(minDistance);
            }

            if (distance2Region.size() < 2) continue;

            int maxID =
                    std::max_element(distance2Region.begin(), distance2Region.end()) -
                    distance2Region.begin();

            for (int j = 0; j < mapID.size(); ++j)
            {
                const int currentMapID = mapID.at(j);
                auto &map = this->mapInner.at(currentMapID);
                auto &maxMap = this->mapInner.at(mapID.at(maxID));

                if (map.handrails.empty() or distance2Region.at(j) < 0)
                {
                    continue;
                }

                LOG_CHECK_DEBUG(INFO) << " map " << map.escalator->id << ": "
                                      << map.escalator->locationWorld.at(0).Print()
                                      << " distance to region: " << distance2Region[j];
                if (maxID != j)
                {
                    if (EscalatorLonger(maxMap.escalator->locationWorld
                                        , map.escalator->locationWorld))
                    {
                        LOG_CHECK_DEBUG(INFO) << "remove farthest map: too short, maybe noise.";
                        maxMap.handrails.clear();
                        distance2Region.at(maxID) = -1;
                        maxID = std::max_element(distance2Region.begin(), distance2Region.end()) -
                                distance2Region.begin();
                        j--;
                    }
                    else
                    {
                        LOG_CHECK_DEBUG(INFO) << "remove map: too close, maybe noise.";
                        map.handrails.clear(); // equal remove
                    }
                }
            }
        }
    }
}

void EscalatorMap::RemovePartsBeyondRegion()
{
    if (mapInner.empty() or regions.Empty()) return;

    // TODO : old method update by new method
    const int sizeMap = mapInner.size();

    // check all map
    for (int j = 0; j < sizeMap; ++j)
    {
        auto &map = mapInner.at(j);
        int sizePart = map.handrails.size();
        int bindRegionID = map.bindRegionID;
        const auto &valid = bindCount.at(j);

        if (bindRegionID < 0) continue;

        for (int k = 0; k < sizePart; ++k)
        {
            auto &part = map.handrails.at(k);
            if (not valid.at(k))
            {
                LOG_CHECK_DEBUG(INFO) << "remove handrails: beyond region. "
                                      << part.location.Print();
                part.weight = 0; // equal to remove
            }
        }
    }
}

bool EscalatorMap::EscalatorLonger(const std::vector<psl::Location> &locationWorld1
                                   , const std::vector<psl::Location> &locationWorld2)
{
    bool flag = locationWorld1.at(0).DistanceXY(locationWorld1.at(1)) < MIN_DISTANCE_POINT;
    flag = flag && locationWorld2.at(0).DistanceXY(locationWorld2.at(1)) > MIN_DISTANCE_POINT;

    return flag;
}