//
// Created by hao on 2021/11/26.
//
#include <numeric>
#include "escalator_region.h"
#include "src/utils/detector_param.h"
#include "src/utils/map_define.h"
#include "src/utils/define.h"
#include "src/utils/file.h"

const float MIN_MACHINE_MOTION_DISTANCE = 0.3; // 20 cm
const float MACHINE_MOTION_DISTANCE_4_BIND = 0.6; // 60 cm
const float REGION_MARGIN_4_BIND = 0.6; // 60 cm
const float MIN_REGION_MARGIN_4_BIND = 0.6; // 60 cm
const float MIN_INTERSECTION_AREA_4_BIND = 0.2; // m*m
const float MIN_AREA_4_INTERSECTION = 0.5; // m*m
const float DEFAULT_IUNVALID_AREA_RATIO = 0.0001; // m*m
const float MAX_MARGIN_CLOSE = 0.15; // m*m
const int MAX_AREA = INT16_MAX;
const int MIN_VALID_ANGLE = 4;
const double MIN_VALID_RADAIN = MIN_VALID_ANGLE * ANGLE_2_RADIAN;

EscalatorRegion::EscalatorRegion()
{

}

EscalatorRegion::EscalatorRegion(const std::vector<BoxInfo> &boxes, const BoxInfo &view)
{
    SetRegion(boxes, view);
}

static bool Left(const BoxInfo &box1, const BoxInfo &box2)
{
    return box1.box.x < box2.box.x;
}


void EscalatorRegion::Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view)
{
    std::vector<BoxInfo> boxesSorted = boxes;
    std::sort(boxesSorted.begin(), boxesSorted.end(), Left);
    std::size_t size = regions.size();

    vector<BindResult> index, indexInvalid;
    Bind(boxesSorted, view, false, index);
    Bind(boxesSorted, view, true, indexInvalid);

    // fix for the uncomplete region
    for (std::size_t i = 0; i < size; ++i)
    {
        if (index.at(i).id < 0) index.at(i) = indexInvalid.at(i);
    }

    // remove the error escalator: contain more than one escalator
    for (std::size_t i = 0; i < size; ++i)
    {
        const auto &r = regions.at(i);
        if (r.invalid)
        {
            regions.erase(regions.begin() + i);
            index.erase(index.begin() + i);
            i--;
            size--;
        }
    }

    // splite region
    /*
    for (std::size_t i = 0; i < size; ++i)
    {
        const int boxID = index.at(i).id;

        if (boxID < 0) continue;

        const auto &box = boxesSorted.at(boxID);
        const auto r = regions.at(i);
        const std::size_t partSize = box.locationSerials.size();

        if ((not index.at(i).valid) and partSize >= 2)
        {
            const auto &left = box.edgeLeft.world;
            const auto &right = box.edgeRight.world;
            bool leftInView = InViewSoft(r.vertex, r.left, r.right, left);
            bool rightInView = InViewSoft(r.vertex, r.left, r.right, right);

            LOG_CHECK_DEBUG(INFO) << "splite region(left, right): " << leftInView << ", "
                                  << rightInView;
            if (not(leftInView or rightInView)) continue;

            double angleViewLeft = GetAngle(view.location.world, view.edgeLeft.world, box.edgeLeft.world);
            double angleViewRight = GetAngle(view.location.world, view.edgeRight.world
                                             , box.edgeLeft.world);
            bool close2View = MIN(angleViewLeft, angleViewRight) < MIN_VALID_RADAIN_4_VIEW;

            if (close2View) continue;

            if (leftInView and rightInView)
            {
                auto newRegion = r;

                newRegion.left = right;
                newRegion.partCount = 0;
                regions.push_back(newRegion);
                regions.back().id = regions.size() - 1;
                index.push_back(BindResult());

                newRegion = r;
                newRegion.right = left;
                newRegion.partCount = 0;
                regions.push_back(newRegion);
                regions.back().id = regions.size() - 1;
                index.push_back(BindResult());

                regions.at(i).left = left;  // TODO : bad when r.vertex inView of box;
                regions.at(i).right = right;
                if (InViewSoft(r.vertex, r.left, r.right, view.location.world))
                {
                    regions.at(i).vertex = view.location.world;
                }
            }
            else if (leftInView or rightInView)
            {
                auto newRegion = r;

                if (leftInView)
                {
                    newRegion.left = left;
                    regions.at(i).right = left;
                }
                else if (rightInView)
                {
                    newRegion.right = right;
                    regions.at(i).left = right;
                }

                regions.push_back(newRegion);
                newRegion.partCount = partSize;
                regions.back().id = regions.size() - 1;
                index.push_back(index.at(i));
                index.back().valid = true;
                index.at(i).id = DEFALUT_INDEX;
            }

//            Bind(boxesSorted, view, false, index);
//            Bind(boxesSorted, view, true, indexInvalid);
        }
    }
*/
    size = regions.size();

    // remove small region
    for (std::size_t i = 0; i < size; ++i)
    {
        const auto &r = regions.at(i);
        double angle = GetAngle(r.vertex, r.left, r.right);
        if (angle < MIN_VALID_RADAIN)
        {
            regions.erase(regions.begin() + i);
            index.erase(index.begin() + i);
            i--;
            size--;
        }
    }

    size = regions.size();

    // update origin region
    for (std::size_t i = 0; i < size; ++i)
    {
        auto &region = regions.at(i);
        region.Reset(); // update left to leftOrg

        int boxID = index.at(i).id;

        if (boxID >= 0)
        {
            const BoxInfo &box = boxesSorted.at(boxID);
            if (not box.IsEscalator()) continue;

            // TODO : should be filter by
            if (box.locationSerials.size() < 1) continue;

            region.boxID = region.id;
            region.leftLeft = box.edgeLeft.world;
            region.rightRight = box.edgeRight.world;

            if (box.sideView)
            {
                LOG_CHECK_DEBUG(INFO) << "no update, because box in side view.";
                continue;
            }

            if (box.dummy)
            {
                if (box.locationSerials.size() < 2) // TODO : requir it ?
                {
                    LOG_CHECK_DEBUG(INFO) << "an invalid box: " << boxID << ".";
                    region.isTarget = true; // only get handrails, also update
                    continue; // TODO : not udpate the vertex
                }
            }

            bool mustUpdate = (not index.at(i).valid) and
                              region.partCount < box.locationSerials.size();

            if (mustUpdate)
            {
                region.isTarget = true;
                bool boxIsNormal = true;

                // check the region is normal, avoid false detect (more than one escalator be merged)
                // TODO : should move to Bind
                for (const auto &r : this->regions)
                {
                    if (r.id != region.id)
                    {
                        if (InViewSoft(view.location.world, box.edgeLeft.world
                                       , box.edgeRight.world, r.left)
                            and InViewSoft(view.location.world, box.edgeLeft.world
                                           , box.edgeRight.world, r.right))
                        {
                            boxIsNormal = false;
                            break;
                        }
                    }
                }

                if (not boxIsNormal) continue;

                region.partCount = box.locationSerials.size();
                region.vertex = view.location.world; // TODO : work well ?
                region.left = box.edgeLeft.world; // TODO : ugly, move to UpdateRight
                region.right = box.edgeRight.world; // TODO : ugly, move to UpdateRight
            }
            else if (index.at(i).Valid())
            {
                // origin region is not complete
                region.isTarget = true; // TODO : why not in condition box.id >= 0
                double distanceVertex = region.vertex.Distance(view.location.world);
                bool close = distanceVertex < MIN_MACHINE_MOTION_DISTANCE;

                if (not close)
                {
                    bool leftUpdate = region.UpdateLeft(box, view);
                    bool rightUpdate = region.UpdateRight(box, view);
                    region.UpdateVertex(box, view.location.world, leftUpdate
                                        , rightUpdate);
                }
                LOG_CHECK_DEBUG(INFO) << "angle: "
                                      <<
                                      GetAngle(region.vertex, region.left, region.right) /
                                      ANGLE_2_RADIAN;
            }
        }
    }

    // add new region
    if (index.empty()) SetRegion(boxesSorted, view);
    else
    {
        const std::size_t sizeBox = boxesSorted.size();
        for (std::size_t i = 0; i < sizeBox; ++i)
        {
            if (std::find(index.begin(), index.end(), BindResult(i)) == index.end())
            {
                SetRegion(boxesSorted.at(i), view);
            }
        }
    }
}

bool EscalatorRegion::Empty() const
{
    return regions.empty();
}

void EscalatorRegion::SetRegion(const vector<BoxInfo> &boxes, const BoxInfo &view)
{
    const std::size_t sizeBox = boxes.size();
    for (std::size_t i = 0; i < sizeBox; ++i)
    {
        SetRegion(boxes.at(i), view);
    }
}

bool EscalatorRegion::SetRegion(const BoxInfo &box, const BoxInfo &view)
{
    if (not box.IsEscalator()) return false;
    if (box.dummy) return false;
    if (box.locationSerials.size() <= 1) return false; // TODO : how to do

    int id = 0;
    if (not regions.empty()) id = regions.back().id + 1;

    regions.push_back(Sector(box));
    regions.back().id = id;
    regions.back().boxID = regions.back().id;
    regions.back().vertex = view.location.world;
    regions.back().vertexOrigin = view.location.world;
    regions.back().isTarget = true;
    regions.back().partCount = box.locationSerials.size();


    return true;
}

void EscalatorRegion::Bind(const vector<BoxInfo> &boxes, const BoxInfo &view
                           , const bool useInvalid
                           , std::vector<BindResult> &index)
{
    if (regions.empty()) return;

    // init index
    index.resize(regions.size(), BindResult());

    if (boxes.empty()) return;

    // in view
    Point newView = view.location.world;
    const std::size_t sizeBox = boxes.size();
    const std::size_t sizeRegion = regions.size();
    std::vector<std::vector<double>> areaByRegion(sizeBox, std::vector<double>(sizeRegion
                                                                               , -1));
    std::vector<std::vector<int>> flagByRegion(sizeBox, std::vector<int>(sizeRegion
                                                                         , 1));

    for (const auto &r :regions)
    {
        LOG_CHECK_DEBUG(INFO) << "Sector(Point(" << r.vertex.x << ", " << r.vertex.y
                              << ", " << 0 << "), Point("
                              << r.left.x << ", " << r.left.y << ", " << 0 << "), Point("
                              << r.right.x << ", " << r.right.y << ", " << 0 << "))";
    }

    LOG_CHECK_DEBUG(INFO) << "vertex Point(" << newView.x << ", " << newView.y << ", "
                          << 0
                          << ")";
    for (const auto &b :boxes)
    {
        if (not b.IsEscalator()) continue;
        LOG_CHECK_DEBUG(INFO) << "Point(" << b.edgeLeft.world.x << ", "
                              << b.edgeLeft.world.y << ", " << 0 << ")"
                              << " Point(" << b.edgeRight.world.x << ", "
                              << b.edgeRight.world.y << ", " << 0 << ")";
    }

    // intersection
    std::vector<int> intersection(sizeBox, 0);

    for (std::size_t i = 0; i < sizeRegion; ++i)
    {
        std::vector<double> areaByBox(sizeBox, -1);
        auto &r = regions.at(i);

        for (std::size_t j = 0; j < sizeBox; ++j)
        {
            auto box = boxes.at(j);
            box.location = newView;
            if (not box.IsEscalator()) continue;
            if (box.dummy ^ useInvalid) continue;

            double distanceVertex = r.vertex.Distance(newView);
            bool close = distanceVertex < MACHINE_MOTION_DISTANCE_4_BIND;

            Segment leftOrg(r.vertex, r.left);
            Segment rightOrg(r.vertex, r.right);
            Segment leftCurrent(newView
                                , box.edgeLeft.world); // TODO : use view or b.location
            Segment rightCurrent(newView, box.edgeRight.world);

            Sector newRegion(box);
            newRegion.vertex = newView;
            std::vector<psl::Location> crossPoints;
            float areaIntersection = -1;
//            GetCrossPointTestMulti(r, newRegion, areaIntersection, crossPoints);
            GetTriangleArea(r.GetPoint(), newRegion.GetPoint(), areaIntersection
                            , crossPoints, MAX_DISTANCE_4_DETECT_REGION);

            LOG_CHECK_DEBUG(INFO) << "intersection area : " << areaIntersection
                                  << " cross point num: " << crossPoints.size();

            if (areaIntersection < MIN_AREA_4_INTERSECTION)
            {
                LOG_CHECK_DEBUG(INFO) << "too lower intersection.";
                continue;
            }

            if (crossPoints.size() > 0)
            {
                Point crossPointLL, crossPointRR;

                GetCrossPoint4Line(leftOrg, leftCurrent, crossPointLL);
                GetCrossPoint4Line(rightOrg, rightCurrent, crossPointRR);

                double leftLineDistance = LineDistance(newView, newRegion.left, r.vertex
                                                       , r.left); // TODO : what is this
                bool leftClose = leftLineDistance < MAX_MARGIN_CLOSE;
                double rightLineDistance = LineDistance(newView, newRegion.right, r.vertex
                                                        , r.right);
                bool rightClose = rightLineDistance < MAX_MARGIN_CLOSE;
                bool leftCrossOnFront = OnFront(leftOrg.start, leftOrg.end, crossPointLL);
                bool rightCrossOnFront = OnFront(rightOrg.start, rightOrg.end
                                                 , crossPointRR);

                bool leftInTriangle = leftClose or (
                        InTriangleSoft(newView, newRegion.left, newRegion.right
                                       , crossPointLL)
                        and (leftCrossOnFront or
                             InViewSoft(newView, newRegion.left, newRegion.right
                                        , r.vertex)));
                bool rightInTriangle = rightClose or (
                        InTriangleSoft(newView, newRegion.left
                                       , newRegion.right, crossPointRR)
                        and (rightCrossOnFront
                             or InViewSoft(newView, newRegion.left
                                           , newRegion.right, r.vertex)));

                // remove invalid intersection by far cross point
                {
                    double angle = GetAngle(newView, newRegion.left, newRegion.right);
                    LOG_CHECK_DEBUG(INFO) << "in triangle(L, R): " << leftInTriangle
                                          << " " << rightInTriangle;

                    bool newLeftInView = InViewSoft(r.vertex, r.left, r.right
                                                    , newRegion.left);
                    bool originLeftInView = InViewSoft(newView, newRegion.left
                                                       , newRegion.right, r.left);
                    bool newRightInView = InViewSoft(r.vertex, r.left, r.right
                                                     , newRegion.right);
                    bool originRightInView = InViewSoft(newView, newRegion.left
                                                        , newRegion.right
                                                        , r.right);

                    if (not(
                            (leftInTriangle or newLeftInView or originLeftInView)
                            and (rightInTriangle or newRightInView or originRightInView)
                    ))
                    {
                        LOG_CHECK_DEBUG(INFO)
                            << "invalid intersection."; // TODO : remove it
                        areaIntersection *= DEFAULT_IUNVALID_AREA_RATIO;
                        flagByRegion.at(j).at(i) = false;
                    }
                }

                // remove invalid intersection by distance between cross and vertex;
                {
                    double distanceLeft2Vertex = crossPointLL.Distance(r.vertex);
                    double distanceRight2Vertex = crossPointRR.Distance(r.vertex);
                    LOG_CHECK_DEBUG(INFO) << "distance to vertex(L, R): "
                                          << distanceLeft2Vertex << " "
                                          << distanceRight2Vertex;

//                    if (not(leftCrossOnFront and
//                            distanceLeft2Vertex < MIN_DISTANCE_EDGE_2_VERTEX
//                            or distanceRight2Vertex < MIN_DISTANCE_EDGE_2_VERTEX))
//                    {
//                        flagByRegion.at(j).at(i) = false;
//                        areaByBox.at(j) = areaIntersection;;
//                        continue;
//                    }
                }

                // no valid cross point
                {
                    if (not(close or leftInTriangle or rightInTriangle))
                    {
                        flagByRegion.at(j).at(i) = false;
                    }
                }
            }

            if (areaIntersection > (MAX_OUTVIEW - 1)) continue; // TODO : not security

            areaByBox.at(j) = areaIntersection;
        }

        // region to box, max intersection
//        auto it = std::max_element(areaByBox.begin(), areaByBox.end());
//        if (*it > 0)
//        {
//            auto boxID = it - areaByBox.begin();
//            areaByRegion.at(boxID).at(i) = *it;
//        }

        for (std::size_t j = 0; j < sizeBox; ++j)
        {
            const auto &area = areaByBox.at(j);
            areaByRegion.at(j).at(i) = area;
        }
    }

    // box to region, max area
    for (std::size_t boxID = 0; boxID < sizeBox; ++boxID)
    {
        const BoxInfo &box = boxes.at(boxID);

        auto it = std::max_element(areaByRegion.at(boxID).begin(), areaByRegion.at(
                boxID).end());
        if (*it > 0)
        {
            auto regionID = it - areaByRegion.at(boxID).begin();

            index.at(regionID).id = boxID;
            index.at(regionID).valid = true;

            if (*it < MIN_INTERSECTION_AREA_4_BIND
                or not flagByRegion.at(boxID).at(regionID))
            {
                index.at(regionID).valid = false;
            }

            // check with intersection angle
            const auto &r = regions.at(regionID);
            bool leftInOrg = InViewSoft(newView, box.edgeLeft.world
                                        , box.edgeRight.world
                                        , r.left);
            bool rightInOrg = InViewSoft(newView, box.edgeLeft.world
                                         , box.edgeRight.world
                                         , r.right);
            bool leftInNew = InViewSoft(r.vertex, r.left, r.right
                                        , box.edgeLeft.world);
            bool rightInNew = InViewSoft(r.vertex, r.left, r.right
                                         , box.edgeRight.world);

            if ((leftInOrg ^ rightInOrg) and (leftInNew ^ rightInNew))
            {
                double intersectionAngle = 0;
                double distance = 1;
                Point cross;

                if (leftInOrg)
                {
                    intersectionAngle = GetAngle(Segment(r.vertex, r.left)
                                                 , Segment(newView
                                                           , box.edgeRight.world));
                    GetCrossPoint4Line(Segment(r.vertex, r.right)
                                       , Segment(newView, box.edgeRight.world)
                                       , cross);
                }
                else if (rightInOrg)
                {
                    intersectionAngle = GetAngle(Segment(r.vertex, r.right)
                                                 , Segment(newView
                                                           , box.edgeLeft.world));
                    GetCrossPoint4Line(Segment(r.vertex, r.left)
                                       , Segment(newView, box.edgeLeft.world)
                                       , cross);
                }

                distance = cross.DistanceXY(r.vertex);
                intersectionAngle *= distance;
                double angleOrigin = GetAngle(r.vertex, r.left, r.right);

                if (angleOrigin * 0.02 > intersectionAngle)
                {
                    index.at(regionID).valid = false;
                }
            }
        }
        else
        {
        }
    }


    for (std::size_t i = 0; i < sizeRegion; ++i)
    {
        LOG_CHECK_DEBUG(INFO) << "region: " << regions.at(i).id << " box: " << index[i].id
                              << " valid: " << index.at(i).valid;
    }
}

bool EscalatorRegion::Read(ifstream &stream)
{
    if (not stream.is_open()) return false;

    READ_LIST(stream, regions);

    return true;
}

bool EscalatorRegion::Write(ofstream &stream) const
{
    if (not stream.is_open()) return false;

    WRITE_LIST(stream, regions);

    return true;
}

bool BindResult::Valid() const
{
    return valid and id >= 0;
}

bool BindResult::operator==(const BindResult &b1) const
{
    return this->id == b1.id;
}

BindResult::BindResult()
{

}

BindResult::BindResult(int id)
{
    this->id = id;
}
