//
// Created by hao on 2021/7/27.
//

#include "composed.h"
#include "src/utils/define.h"
#include "src/module/box.h"

const float MIN_IOU_ESCALATOR = 0.1;
// TODO : the name
const int EXTEND_PIXEL_4_SIDE_VIEW_CHECK = 10;
const float SINGLE_BOX_RAITO_4_SIDE_VIEW_CHECK = 0.6;

static bool Right(const BoxInfo &box1, const BoxInfo &box2)
{
    return box1.cx > box2.cx or box1.cy > box2.cy; // TODO : error
}

static bool Left(const BoxInfo &box1, const BoxInfo &box2)
{
    return box1.cx < box2.cx;
}


static bool Top(const BoxInfo &box1, const BoxInfo &box2)
{
    return box1.cy < box2.cy;
}

composed::Body::Body()
{

}

composed::Body::Body(const BoxInfo &box)
{
    this->whole = box;
}

bool composed::Body::Contain(const BoxInfo &box) const
{
//    return this->IoU(box) > MIN_IOU_ESCALATOR;
    return Intersection(box);
}

void composed::Body::SortLeft()
{
    std::sort(this->parts.begin(), this->parts.end(), Left);
}

double composed::Body::IoU(const BoxInfo &box) const
{
    double intersection = Box::Intersection(
            Box(this->whole.cx, this->whole.cy, this->whole.box.width
                , this->whole.box.height)
            , Box(box.cx, box.cy, box.box.width, box.box.height));
    double unionArea = box.width * box.height;
    double iou = intersection / unionArea;
    LOG_CHECK_DEBUG(INFO) << "iou: " << iou;

    return iou;
}

psl::Location composed::Body::Average() const
{
    psl::Location avg;
    avg.x = 0;
    avg.y = 0;
    avg.z = 0;

    for (const auto &p : this->parts)
    {
        avg.x += p.location.machine.x;
        avg.y += p.location.machine.y;
        avg.z += p.location.machine.z;
    }

    avg.x /= this->parts.size();
    avg.y /= this->parts.size();
    avg.z /= this->parts.size();

    return avg;
}

std::vector<Location> composed::Body::PartLocation() const
{
    std::vector<Location> locations;

    for (const auto &p : this->parts)
    {
        locations.push_back(p.location);
    }

    return locations;
}

void composed::Body::UpdateParts2Whole()
{
    for (const auto &p : this->PartLocation())
    {
        this->whole.locationSerials.push_back(p);
    }
}

bool composed::Body::Intersection(const BoxInfo &box) const
{
    return this->whole.Intersection(box);
}

Composed::Composed()
{

}

void Composed::GetDistance(const Lidar &lidar, DetectResult &boxes)
{
    if (boxes.Empty()) return;

    this->bodys.clear();
    PrepareBody(boxes.boxes);


    for (auto &body : this->bodys)
    {
        if (body.parts.empty())
        {
            // TODO : return false and remove
            // they have there own x coordinate
//            body.whole.edgeLeft.machine.y = body.whole.location.machine.y;
//            body.whole.edgeRight.machine.y = body.whole.edgeLeft.machine.y;
        }
        else if (1 == body.parts.size())
        {
            // TODO : more
            // they have there own x coordinate
//            if (body.parts.at(0).locationSerials.empty())
//            {
//                body.whole.edgeLeft.machine.y = body.whole.location.machine.y;
//                body.whole.edgeRight.machine.y = body.whole.edgeLeft.machine.y;
//            }
//            else
//            {
//                Fit(body.parts.at(0).locationSerials, body);
//            }
        }
        else if (body.parts.size() >= 2)
        {
//            Fit(body.PartLocation(), body); // TODO : why fit

//            LOG_CHECK_DEBUG(INFO) << "origin edge left: " << body.whole.edgeLeft.machine.Print();
//            LOG_CHECK_DEBUG(INFO) << "origin edge right: " << body.whole.edgeRight.machine.Print();
//            // TODO : take care of the lidar direction
//            body.whole.edgeLeft.machine.y = body.whole.location.machine.y;
//            body.whole.edgeRight.machine.y = body.whole.edgeLeft.machine.y;
//
//            // get the max edge in x axis
//            if (body.whole.edgeLeft.machine.x < body.whole.edgeRight.machine.x)
//            {
//                // get left
//                for (const auto &p : body.parts)
//                {
//                    if (p.location.machine.x < body.whole.edgeLeft.machine.x)
//                        body.whole.edgeLeft.machine = p.location.machine;
//                    if (p.location.machine.x > body.whole.edgeRight.machine.x)
//                        body.whole.edgeRight.machine = p.location.machine;
//                }
//            }
//            else
//            {
//                // get right
//                for (const auto &p : body.parts)
//                {
//                    if (p.location.machine.x > body.whole.edgeLeft.machine.x)
//                        body.whole.edgeLeft.machine = p.location.machine;
//                    if (p.location.machine.x < body.whole.edgeRight.machine.x)
//                        body.whole.edgeRight.machine = p.location.machine;
//                }
//            }
        }

        LOG_CHECK_DEBUG(INFO) << "edge left: " << body.whole.edgeLeft.machine.Print();
        LOG_CHECK_DEBUG(INFO) << "edge right: " << body.whole.edgeRight.machine.Print();
    }

    UpdateParts2Whole();
    MergeBoxes(boxes.boxes);
}

void Composed::GetWhole(const std::vector<BoxInfo> &boxes)
{
    const size_t size = boxes.size();

    for (size_t i = 0; i < size; ++i)
    {
        const auto &box = boxes.at(i);
        if (box.IsEscalator() and -1 != box.location.machine.x)
        {
            bodys.push_back(composed::Body(box));
        }
    }
}

void Composed::GetPartial(const std::vector<BoxInfo> &boxes, std::vector<int> &orphan)
{
    const size_t size = boxes.size();
    std::fill(orphan.begin(), orphan.end(), 0);

    for (size_t i = 0; i < size; ++i)
    {
        const auto &box = boxes.at(i);

        if (box.IsEscalatorHandrails())
        {
            if (box.location.machine.IsDefault())
            {
                LOG_CHECK_DEBUG(INFO) << "not get the lidar for box.";
                continue;  // TODO : lidar is empty
            }

            bool update = false;

            for (auto &body : this->bodys)
            {
                if (body.Contain(box))
                {
                    if (body.whole.location.machine.y < box.location.machine.y)
                    {
                        body.parts.push_back(box);
                    }
                    update = true;
                }
            }

            if (not update)
            {
                // no escalator box;
                orphan.at(i) = true;
            }
        }
    }
}

void Composed::AlignmentWhole()
{
    for (auto &body : bodys)
    {
        if (body.parts.size() > 0)
        {
            int xMin = body.whole.box.x;
            int xMax = body.whole.box.x + body.whole.box.width;

            for (auto part : body.parts)
            {
                xMin = std::min(xMin, part.box.x);
                xMax = std::max(xMax, part.box.x + part.box.width);
            }

            body.whole.box.x = xMin;
            body.whole.box.width = xMax - xMin;
            body.whole.SetRect(body.whole.box);
        }
    }
}

void Composed::PrepareBody(const std::vector<BoxInfo> &boxes)
{
    std::vector<int> orphan(boxes.size(), 0);

    GetWhole(boxes);
    GetPartial(boxes, orphan);
    GroupInSideView();
    AlignmentWhole();

    // generate escalator which lack of escalator box
    const size_t size = boxes.size();
    for (int i = 0; i < size; ++i)
    {
        if (orphan.at(i))
        {
            composed::Body body;
            BoxInfo box = boxes.at(i);

            if (box.lidarInValid) continue;

            body.parts.push_back(box);

            box.classID = ESCALATOR;
            box.dummy = true;
            body.whole = box;
            this->bodys.push_back(body);
        }
    }

    for (auto &body : this->bodys)
    {
        body.SortLeft();
    }
}

bool Composed::LeasrSquares(const std::vector<Location> &locations, float &k
                            , float &b)
{
    if (locations.size() < 2) return false;

    double sumX = 0, sumXSquare = 0, sumY = 0, sumXY = 0;

    for (const auto &l : locations)
    {
        sumX += l.machine.x;
        sumY += l.machine.y;
        sumXSquare += l.machine.x * l.machine.x;
        sumXY += l.machine.x * l.machine.y;
    }

    double temp = locations.size() * sumXSquare - sumX * sumX;

    if (0 != temp)
    {
        k = (sumXY * locations.size() - sumX * sumY) / temp;
        b = (sumY * sumXSquare - sumXY * sumX) / temp;
    }
    else
    {
        k = 0;
        b = 0;
        return false;
    }

    return true;
}

bool Composed::Fit(const std::vector<Location> &locations, composed::Body &body)
{
    // TODO : remove the noise
    float k = 0, b = 0;

    std::vector<Location> locationsNew = locations;
    if ((not locations.empty()) and locations.size() % 2 == 0)
    {
        locationsNew.push_back(body.whole.edgeLeft);
        locationsNew.push_back(body.whole.edgeRight);
    }

    if (LeasrSquares(locationsNew, k, b))
    {
        body.whole.edgeLeft.machine.y = k * body.whole.edgeLeft.machine.x + b;
        body.whole.edgeRight.machine.y = k * body.whole.edgeRight.machine.x + b;
    }
    else
    {
        // TODO : fail to get the line;
        return false;
    }

    return true;
}

void Composed::MergeBoxes(std::vector<BoxInfo> &boxes) const
{
    std::vector<BoxInfo> boxesMerged;

    for (const auto &b : boxes)
    {
        if (b.IsPerson() and not b.location.machine.IsDefault())
        {
            boxesMerged.push_back(b);
        }
    }

    for (const auto &b : this->bodys)
    {
        boxesMerged.push_back(b.whole);
    }
    boxes.swap(boxesMerged);
}

void Composed::UpdateParts2Whole()
{
    for (auto &body : this->bodys)
    {
        LOG_CHECK_DEBUG(INFO) << "handrails size: " << body.parts.size();
        if (body.parts.size() < 2)
        {
//            body.whole.locationSerials.push_back(body.whole.edgeLeft);
//            body.whole.locationSerials.push_back(body.whole.edgeRight);
        }

        body.UpdateParts2Whole();
    }
}

void Composed::GroupInSideView()
{
    if (bodys.size() > 1) return; // TODO : bad

    for (auto &body : bodys)
    {
        body.SortLeft();
        const size_t partSize = body.parts.size();

        if (partSize >= 3)
        {
            bool sideView = true;

            for (int i = 0; i < partSize - 1; ++i)
            {
                auto pre = body.parts.at(i);
                auto next = body.parts.at(i + 1);

                sideView &= ((pre.box.y < next.box.y)
                             and ((pre.box.y + pre.box.height) >
                                  (next.box.y + next.box.height)));
            }

            body.whole.sideView = sideView;
        }
        else if (partSize == 2)
        {
            const auto &pre = body.parts.at(0);
            const auto &next = body.parts.at(1);

            double intersection = Box::Intersection(
                    Box(pre.cx, pre.cy, pre.box.width + EXTEND_PIXEL_4_SIDE_VIEW_CHECK,
                            pre.box.height + EXTEND_PIXEL_4_SIDE_VIEW_CHECK)
                    , Box(next.cx, next.cy,
                            next.box.width + EXTEND_PIXEL_4_SIDE_VIEW_CHECK,
                            next.box.height + EXTEND_PIXEL_4_SIDE_VIEW_CHECK));
            body.whole.sideView = intersection > 0.001;
        }
        else if (partSize == 1)
        {
            const auto &box = body.parts.at(0);
            double ratioWidth = box.width / body.whole.width;

            body.whole.sideView = ratioWidth > SINGLE_BOX_RAITO_4_SIDE_VIEW_CHECK;
        }

        if (body.whole.sideView)
            LOG_CHECK_DEBUG(INFO) << "side view: true";
    }
}
