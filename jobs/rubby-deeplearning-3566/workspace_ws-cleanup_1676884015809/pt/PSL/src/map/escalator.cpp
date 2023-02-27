//
// Created by hao on 2021/8/11.
//

#include "escalator.h"
#include <cmath>
#include "psl/perception/instance.h"
#include "src/utils/utils.h"
#include "src/utils/define.h"
#include "src/utils/file.h"
#include "src/utils/gemotry.h"

#define RAND_INT(a, b) (rand() % (b-a+1))+ a
//#define DRAW_PARTS

const float SQRT_2 = 1.414;
const std::string MAP_DIR = "result/map";

const float POINT_DISTANCE_4_SAME = 0.3; // 10cm, view them as one point when distance is less than it
const float POINT_DISTANCE_4_ERROR = 0.8; // 80cm
const float POINT_2_LINE_4_WEIGHT = 0.1; // 10cm
const float MARGIN_4_RANSAC = 0.2; // 20cm
const float REMOVE_TIME = 120; // second
const int FPS = 5; // algorithm fps
const float MIN_ESCALATOR_LENGTH = 0.5;

// TODO : make sure for default value
const int DEFAULT_WEIGHT = REMOVE_TIME * FPS / 2;
const int WEIGHT_REMOVE_WITH_VIEW = MAX(1, MAX_WEIGHT / (REMOVE_TIME * FPS));
const int OUTOF_VIEW_COUNT = 10;
const int MAX_DISTANCE = 1000;

const int INVALID_DISTANCE = 0;
const TimeType SECURITY_TIME_BASE = 1000; // 1s
const TimeType SECURITY_TIME_MAX = 6000; // 6s

const int MIN_VALID_ANGLE = 10;
const double MIN_VALID_RADAIN = MIN_VALID_ANGLE * ANGLE_2_RADIAN;
const float MIN_VALID_ANGLE_TAN = tan(MIN_VALID_ANGLE);

const float MIN_DISTANCE_HANDRAILS = 1.7; // 170cm

int escalator_map::Escalator::groupNum = 0;

escalator_map::Point
SumPointLocation(const escalator_map::Point &pX, const escalator_map::Point &pY);

escalator_map::Point::Point()
{
    this->weight = 0;
    this->useful = true;
    this->update = false;
    this->failCheckoutCount = 0;
    this->distance2Line = 0;
}

escalator_map::Point::Point(const psl::Location &location, const int id)
{
    this->location = location;
    this->weight = DEFAULT_WEIGHT;
    this->useful = false;
    this->update = true;
    this->failCheckoutCount = 0;
    this->distance2Line = 0;
    this->classID = id;
}

bool escalator_map::Point::InvalidWeight() const
{
    return this->weight <= 0;
}

void escalator_map::Point::AddWeight(const int delta)
{
    if (this->weight < MAX_WEIGHT)
    {
        this->weight += delta;
    }
}

bool escalator_map::Point::Keep() const
{
    return this->failCheckoutCount < OUTOF_VIEW_COUNT;
}

void escalator_map::Point::Merge(const escalator_map::Point &point)
{
    this->location = this->location + point.location;
    this->location.x /= 2;
    this->location.y /= 2;
    this->location.z /= 2;
}

bool escalator_map::Escalator::Fit()
{
    this->escalator->status = psl::Object::DISABLE;
    if (handrails.empty()) return false;
//    if (this->escalator->locationWorld.empty()) return false; // TODO : what is this?
    if (Uncomplete()) goto exit;

    this->axis = this->VarianceAnalysis(); // TODO : should be run only once

    if (not Ransac(this->k, this->b))
    {
        LOG_CHECK_DEBUG(INFO) << "fit invalid."; // TODO : should not exist
        goto exit;
    }

    this->UpdateEdge();

    if (this->escalator->locationWorld.size() == 2 // TODO : should not exist
        and this->escalator->locationWorld.at(0).DistanceXY(
            this->escalator->locationWorld.at(1)) > MIN_ESCALATOR_LENGTH)
        this->escalator->status = psl::Object::ENABLE;

    return true;

    exit:
    // TODO : use the useful part
    this->escalator->locationWorld.at(0) = this->handrails.at(0).location;
    this->escalator->locationWorld.at(1) = this->handrails.at(0).location;

    return false;
}

bool escalator_map::Escalator::UpdateHandrail(const BoxInfo &box)
{
    bool change = false; // point num change

    const bool complete = box.locationSerials.size() >= MIN_HANDRAIL_NUM;

    LOG_CHECK_DEBUG(INFO) << "handrails size: " << handrails.size();

    // update
    for (auto &newLocation : box.locationSerials)
    {
        LOG_CHECK_DEBUG(INFO) << "handrails world: " << newLocation.world.Print();

        if (newLocation.machine.DistanceXY(psl::Location(0, 0, 0)) >
            MAX_DISTANCE_4_DETECT)
        {
            continue;
        }

        const size_t sizeParts = this->handrails.size();
        auto newPoint = Point(newLocation.world, groupNum);

        double maxDistance = 0;
        int maxDifferentID = -1;
        int classCount = 0;
        double minDistanceDiff = INT16_MAX;
        int minDifferentID = -1;

        // check for closer point
        for (size_t i = 0; i < sizeParts; ++i)
        {
            auto &orgPoint = this->handrails.at(i);
            double distanceDiff = abs(orgPoint.location.DistanceXY(newLocation.world));

            if (distanceDiff < minDistanceDiff)
            {
                minDistanceDiff = distanceDiff;
                minDifferentID = i;
            }

            bool same = distanceDiff < POINT_DISTANCE_4_SAME;

            if (same)
            {
                securityTime[orgPoint.classID] = MAX(SECURITY_TIME_MAX,
                        securityTime[orgPoint.classID] + SECURITY_TIME_BASE);
                if (not complete) return true; // TODO : ? avoid the remove by view
                newPoint.classID = orgPoint.classID;
                classCount++;

                if (distanceDiff > maxDistance)
                {
                    maxDistance = distanceDiff;
                    maxDifferentID = i;
                }

                // remove the point which far away
                // TODO : why do this, it will be remove in remove stage
                if (distanceDiff > POINT_DISTANCE_4_ERROR)
                {
                    orgPoint.weight -= WEIGHT_REMOVE;
                    continue;
                }

                orgPoint.update = true;

                // TODO : update location
//                orgPoint.location = (orgPoint.location + newLocation) / 2;
                orgPoint.AddWeight(WEIGHT_ADD);
                ++newPoint.weight;
                newPoint.weight = MAX(newPoint.weight, orgPoint.weight);
            }
        }

        if (maxDifferentID >= 0)
        {
            // same point
            // goto remove the most far point
            this->handrails.at(maxDifferentID).weight -= WEIGHT_REMOVE;
            // TODO : remove the sizeParts < MAX_POINT_NUM
            if (classCount < MAX_POINT_NUM_PER and sizeParts < MAX_POINT_NUM)
            {
                HandrailsPush(newPoint);
            }
        }
        else
        {
            // different point
            // TODO : maybe not same
            if (sizeParts < MAX_POINT_NUM
//                and (not Close2Edge(box, newPoint))
                and (minDifferentID >= 0 and minDistanceDiff < MIN_DISTANCE_HANDRAILS))
                // TODO : some parts here, but update the far point firsts
            {
                groupNum++;
                newPoint.classID = groupNum;
                HandrailsPush(newPoint);
            }
            LOG_CHECK_DEBUG(WARNING) << "error for check point same."; // TODO : why
        }

        change = true;
    }

    return change;
}

bool escalator_map::Escalator::Delete()
{
    int size = this->handrails.size();
    bool deleteID = false;

    for (int i = 0; i < size; ++i)
    {
        if (this->handrails.at(i).weight <= 0)
        {
            LOG_CHECK_DEBUG(INFO) << "remove part " << i;
            this->handrails.erase(this->handrails.begin() + i);
            size--;
            i--;
            deleteID = true; // TODO : the delete should be delete a whole id, not one node;
        }
    }

    if (not this->handrails.empty() and deleteID)
    {
        Fit();
        escalator->statusOperator = psl::Object::UPDATE_DL;
    }

    END();
    return deleteID;
}

void DrawParts(const escalator_map::Escalator &escalator, const std::vector<int> &useful)
{
    const int DELTA = 50, ADD = 10;
    const int W = 1000, H = 1000;
    const cv::Scalar COLOR_BACK(100, 100, 100);
    const psl::Location addLocation(ADD, ADD, ADD);

    cv::Mat image = cv::Mat::zeros(W, H, CV_8UC3);

    const size_t size = std::min(escalator.handrails.size(), useful.size());

    // draw line [-10, 10]
    cv::Scalar color(255, 100, 100);

    for (size_t i = 0; i < size; ++i)
    {
        auto &point = escalator.handrails.at(i);
        if (not useful.at(i)) continue;
        // draw view
        auto newPoint = point.location + addLocation;

        cv::circle(image, cv::Point(int(newPoint.x * DELTA), int(newPoint.y * DELTA))
                   , 3, color, 2);
    }

    cv::imshow("useful", image);
    cv::waitKey(1);
}

bool
escalator_map::Escalator::LeasrSquares(const std::vector<int> &useful, float &k, float &b)
{
    if (UNKNOWN == axis) return false;

    double sumX = 0, sumXSquare = 0, sumY = 0, sumXY = 0;

    const size_t size = this->handrails.size();
    size_t pointSize = 0;

    for (size_t i = 0; i < size; ++i)
    {
        const auto &h = this->handrails.at(i);

        bool t = useful.empty();
        if ((not useful.empty()) && (not useful.at(i)))
        {
            this->handrails.at(i).useful = false; // TODO : ugly, no use
            continue;
        }

        this->handrails.at(i).useful = true;
//        if (size > 4 and h.weight < 5) continue; // TODO : how to do
        const auto &l = h.location;

        if (Y == axis)
        {
            sumX += l.y;
            sumY += l.x;
            sumXSquare += l.y * l.y;
            sumXY += l.x * l.y;
        }
        else if (X == axis)
        {
            sumX += l.x;
            sumY += l.y;
            sumXSquare += l.x * l.x;
            sumXY += l.x * l.y;
        }

        pointSize++;
    }

    double temp = pointSize * sumXSquare - sumX * sumX;

    if (temp > 0.0001)
    {
        k = (sumXY * pointSize - sumX * sumY) / temp;
        b = (sumY * sumXSquare - sumXY * sumX) / temp;
    }
    else
    {
        k = 0;
        b = 0;
        return false;
    }

    // TODO : calc r

    return true;
}

void escalator_map::Escalator::UpdateEdge()
{
    try
    {
        if (Uncomplete()) return;
        if (useful.empty()) return;
        if (UNKNOWN == axis) return; // equal Uncomplete;

        if (validPartNum < 1)
        {
            auto usefulIt = std::find(useful.begin(), useful.end(), 1);
            if (usefulIt != useful.end())
            {
                int validID = usefulIt - useful.begin();
                this->escalator->locationWorld.at(0) = this->handrails.at(
                        validID).location;
                this->escalator->locationWorld.at(1) = this->handrails.at(
                        validID).location;
            }
            return;
        }

        psl::Location max = psl::Location(-MAX_DISTANCE, -MAX_DISTANCE
                                                  , INVALID_DISTANCE);
        psl::Location min = psl::Location(MAX_DISTANCE, MAX_DISTANCE
                                                  , INVALID_DISTANCE);

        const size_t size = this->handrails.size();

        for (size_t i = 0; i < size; ++i)
        {
            if (not this->useful.at(i)) continue;

            const auto &point = this->handrails.at(i);

            if (X == axis)
            {
                max.x = MAX(max.x, point.location.x);
                min.x = MIN(min.x, point.location.x);
            }
            else if (Y == axis)
            {
                max.y = MAX(max.y, point.location.y);
                min.y = MIN(min.y, point.location.y);
            }
        }

        if (X == axis)
        {
            max.y = max.x * this->k + b;
            min.y = min.x * this->k + b;
        }
        else if (Y == axis)
        {
            max.x = max.y * this->k + b;
            min.x = min.y * this->k + b;
        }

        LOG_CHECK_TEST(INFO) << "object.locationWorld.size: "
                             << this->escalator->locationWorld.size();

        this->escalator->locationWorld.at(0) = max;
        this->escalator->locationWorld.at(1) = min;
    }
    catch (...)
    {
        LOG_CHECK_STATUS(WARNING) << "unknown error.";
    }

    return;
}

void escalator_map::Escalator::RemoveWithDistance()
{
    const bool uncomplete = Uncomplete();

    for (auto &point : this->handrails)
    {
        if (uncomplete)
        {
            point.distance2Line = point.location.DistanceXY(
                    this->escalator->locationWorld.at(0));
        }
        else
        {
            point.distance2Line = Distance2Line(point, k, b);
        }
        point.useful = point.distance2Line <= POINT_2_LINE_4_WEIGHT;

        // TODO : use distance for weight's weight
        if (point.useful)
        {
            point.AddWeight(WEIGHT_ADD);
        }
        else
        {
            point.weight -= WEIGHT_REMOVE;
        }
    }

    Delete();
}

void escalator_map::Escalator::RemoveWithView(const BoxInfo &view)
{
    if (this->update) return;

    int size = this->handrails.size();

    for (int i = 0; i < size; ++i)
    {
        auto &part = this->handrails.at(i);
        bool inView = InTriangleSoft(view.location.world, view.edgeLeft.world
                                     , view.edgeRight.world, part.location);

        if (inView)
        {
            part.failCheckoutCount++; // support to remove the uncomplete object
        }

        float distance = part.location.DistanceXY(view.location.world);

        bool valid = (distance < MAX_DISTANCE_4_DETECT and
                      distance > MIN_DISTANCE_4_DETECT) or not part.useful;

        if (inView and valid)
        {
            part.failCheckoutCount++;
            if (Uncomplete() and part.Keep()) continue;
            part.weight -= WEIGHT_REMOVE_WITH_VIEW;
        }
    }

    END();
}

bool escalator_map::Escalator::Update(const BoxInfo &box)
{
    if (box.locationSerials.size() < MIN_HANDRAIL_NUM)
    {
        LOG_CHECK_DEBUG(INFO) << "lack of handrails";
    }

    update = UpdateHandrail(box);
    LOG_CHECK_DEBUG(INFO) << "change: " << update;

    if (update)
    {
        this->Fit(); // TODO : merge Fit and UpdateEdge
        this->RemoveWithDistance(); // TODO : why should remove before updateEdge
        this->escalator->statusOperator = psl::Object::UPDATE_DL;
    }

    return update;
}

bool escalator_map::Escalator::Uncomplete() const
{
    bool flag = this->handrails.size() < MIN_HANDRAIL_NUM;

    if (handrails.size() > 2)
    {
        int id = handrails.at(0).classID;

        for (const auto &h : handrails)
        {
            if (h.classID != id)
            {
                return false;
            }
        }
        flag = true;
    }

    return flag;
}

bool MorePoint(const std::vector<int> &p1, const std::vector<int> &p2)
{
    return *p1.rbegin() > *p2.rbegin();
}

bool escalator_map::Escalator::Ransac(float &k, float &b)
{
    const size_t size = this->handrails.size();
    std::vector<std::vector<int>> allUseful;
    std::vector<int> usefullSum;
    std::vector<Axis> allAxis;

    for (size_t i = 0; i < size; ++i)
    {
        const auto &point1 = this->handrails.at(i);

        // reset the useful
        for (size_t j = 0; j < size; ++j)
        {
            auto &pointTemp = this->handrails.at(j);
            if (pointTemp.classID == point1.classID) continue;
            pointTemp.useful = false;
        }

        // get another point
        for (size_t j = i + 1; j < size; ++j)
        {
            const auto &point2 = this->handrails.at(j);

            if (point2.classID == point1.classID) continue;

            // check the point is already the inner point
            if (point2.useful) continue; // TODO : care for

            // TODO : not work, should sort with axis
//            if (InvalidPair(point1, point2)) continue; // remove the illegal point

            auto tmpAxis = GetLineFromPoint(point1.location, point2.location, k, b);
            allAxis.push_back(tmpAxis);

            // TODO : only check the center of every handrails, not all points
            // calc the inner point
            std::vector<int> useful(size, 0);
            for (size_t m = 0; m < size; ++m)
            {
                auto &point = this->handrails.at(m);
                float distance2Line = Distance2Line(point, k, b);
                useful[m] = distance2Line <= MARGIN_4_RANSAC;
                point.useful = useful[m];
            }

            // remove by point distance
            // TODO : select more point
            bool update = true;
            while (update)
            {
                update = false;
                for (int p = 0; p < size; ++p)
                {
                    if (useful.at(p))
                    {
                        float minDistance = INT16_MAX;
                        int minID = -1;
                        for (int q = p + 1; q < size; ++q)
                        {
                            if (useful.at(q) and
                                handrails.at(p).classID != handrails.at(q).classID)
                            {
                                float tmpDistance = handrails.at(
                                        p).location.DistanceXY(handrails.at(q).location);
                                if (tmpDistance < minDistance)
                                {
                                    minDistance = tmpDistance;
                                    minID = q;
                                }
                            }
                        }

                        // TODO : should consider the point num
                        if (minDistance < (INT16_MAX - 1) and
                            minDistance > MIN_DISTANCE_HANDRAILS)
                        {
                            if (bindRegionID >= 0)
                            {
                                if (this->handrails.at(p).location.DistanceXY(
                                        region.vertex)
                                    < this->handrails.at(minID).location.DistanceXY(
                                        region.vertex))
                                {
                                    minID = p;
                                }
                            }
                            else
                            {
                                if (GetClassNum(p) < GetClassNum(minID))
                                    minID = p;
                            }

                            for (int l = 0; l < size; ++l)
                            {
                                if (this->handrails.at(l).classID ==
                                    this->handrails.at(minID).classID)
                                {
                                    useful.at(minID) = false;
                                }
                            }

                            update = true;
                        }
                    }
                }
            }

            usefullSum.push_back(std::accumulate(useful.begin(), useful.end(), 0));
            allUseful.push_back(useful);
        }
    }

    if (allUseful.empty())
    {
#ifdef DRAW_PARTS
        cv::Mat image = cv::Mat::zeros(1000, 1000, CV_8UC3);
        cv::imshow("useful", image);
        cv::waitKey(1);
#endif
        return false;
    }

    std::size_t id = -1;
    id = std::max_element(usefullSum.begin(), usefullSum.end()) - usefullSum.begin();


    axis = allAxis.at(id);

    // TODO : should separate from ransac
    LeasrSquares(allUseful.at(id), k, b); // only one group
    this->useful.swap(allUseful.at(id));
    validPartNum = usefullSum.at(id);
#ifdef DRAW_PARTS
    DrawParts(*this, useful);
#endif

    return true;
}

float
escalator_map::Escalator::Distance2Line(const Point &point, const float k, const float b)
{
    float distance = -1;
    if (UNKNOWN == axis) return distance;

    float temp = sqrt(k * k + 1);

    if (X == axis)
    {
        distance = abs(k * point.location.x - point.location.y + b) / temp;
    }
    else if (Y == axis)
    {
        distance = abs(k * point.location.y - point.location.x + b) / temp;
    }

    return distance;
}

escalator_map::Axis
escalator_map::Escalator::GetLineFromPoint(const psl::Location &point1
                                           , const psl::Location &point2
                                           , float &k, float &b)
{
    // TODO : check the axis here
    psl::Location diff = point1 - point2;

    if (abs(diff.x) > abs(diff.y)) axis = X;
    else axis = Y;

    if (X == axis)
    {
        k = (point1.y - point2.y) /
            (point1.x - point2.x);
        b = point1.y - k * point1.x;
    }
    else if (Y == axis)
    {

        k = (point1.x - point2.x) /
            (point1.y - point2.y);
        b = point1.x - k * point1.y;
    }
    return axis;
}

bool escalator_map::Escalator::InvalidPair(const Point &point1, const Point &point2) const
{
    // distance between the pairs is less than edge * 1.414.

    auto diff = point1.location - point2.location;

    float maxDiff = MAX(abs(diff.x), abs(diff.y));
    float distance = point1.location.DistanceXY(point2.location);

    // TODO : error check

    return distance > maxDiff * SQRT_2;
}

escalator_map::Axis escalator_map::Escalator::VarianceAnalysis() const
{
    if (this->handrails.size() < 2)
    {
        return escalator_map::Axis::X;
    }
    else
    {
        escalator_map::Point initXY;
        initXY.location = psl::Location(0, 0, 0);

        auto sumXY = std::accumulate(std::begin(this->handrails), std::end(
                this->handrails), initXY, SumPointLocation);

        double meanX = sumXY.location.x / this->handrails.size();
        double meanY = sumXY.location.y / this->handrails.size();
        double accumX = 0.0;
        double accumY = 0.0;

        for (auto &simgleHandrail:this->handrails)
        {
            accumX += pow((simgleHandrail.location.x - meanX), 2);
            accumY += pow((simgleHandrail.location.y - meanY), 2);
        }
        double stdevX = sqrt(accumX / (this->handrails.size() - 1));
        double stdevY = sqrt(accumY / (this->handrails.size() - 1));

        if (stdevX >= stdevY)
        {
            return escalator_map::Axis::X;
        }
        else
        {
            return escalator_map::Axis::Y;
        }
    }
}

void escalator_map::Escalator::HandrailsPush(const escalator_map::Point &point)
{
    this->handrails.push_back(point);
    if (securityTime.find(point.classID) == securityTime.end())
    {
        securityTime[point.classID] = SECURITY_TIME_BASE;
    }

    groupNum++;
}

bool
escalator_map::Escalator::Close2Edge(const BoxInfo &box
                                     , const escalator_map::Point &point)
{
    double angleLeft = GetAngle(view.location.world, box.edgeLeft.world, point.location);
    double angleRight = GetAngle(view.location.world, box.edgeRight.world
                                 , point.location);

    LOG_CHECK_DEBUG(INFO) << "to edge angle: " << MIN(angleLeft, angleRight)
                          << " in view: "
                          << (MIN(angleLeft, angleRight) > MIN_VALID_RADAIN);

    if (MIN(angleLeft, angleRight) > MIN_VALID_RADAIN) return false;

    return true;
}

bool escalator_map::Escalator::HaveRegion() const
{
    return bindRegionID >= 0;
}

int escalator_map::Escalator::GetClassNum(const int &id) const
{
    int count = 0;
    for (const auto &h : handrails)
    {
        if (h.classID == id)
        {
            count++;
        }
    }

    return count;
}

bool escalator_map::Escalator::Read(ifstream &stream)
{
    if (not stream.is_open()) return false;

    // TODO : not write securityTime
    file_op::File::ReadItem(stream, k);
    file_op::File::ReadItem(stream, b);
    file_op::File::ReadItem(stream, handrailsUncompleteCount);
    file_op::File::ReadItem(stream, axis);
    file_op::File::ReadItem(stream, groupNum);
    file_op::File::ReadList(stream, useful);
    file_op::File::ReadItem(stream, bindRegionID);
    READ_LIST(stream, handrails);
    file_op::File::ReadItem(stream, id);
    file_op::File::ReadItem(stream, classID);

    return true;
}

bool escalator_map::Escalator::Write(ofstream &stream) const
{
    if (not stream.is_open()) return false;

    // TODO : not write securityTime
    file_op::File::WriteItem(stream, k);
    file_op::File::WriteItem(stream, b);
    file_op::File::WriteItem(stream, handrailsUncompleteCount);
    file_op::File::WriteItem(stream, axis);
    file_op::File::WriteItem(stream, groupNum);
    file_op::File::WriteList(stream, useful);
    file_op::File::WriteItem(stream, bindRegionID);
    WRITE_LIST(stream, handrails);
    file_op::File::WriteItem(stream, id);
    file_op::File::WriteItem(stream, classID);

    return true;
}

bool escalator_map::Point::Read(ifstream &stream)
{
    if (not stream.is_open()) return false;

    file_op::File::ReadItem(stream, weight);
    file_op::File::ReadItem(stream, distance2Line);
    file_op::File::ReadItem(stream, useful);
    file_op::File::ReadItem(stream, update);
    file_op::File::ReadItem(stream, failCheckoutCount);
    file_op::File::ReadItem(stream, classID);
    location.ReadBinary(stream);

    return true;
}

bool escalator_map::Point::Write(ofstream &stream) const
{
    if (not stream.is_open()) return false;

    file_op::File::WriteItem(stream, weight);
    file_op::File::WriteItem(stream, distance2Line);
    file_op::File::WriteItem(stream, useful);
    file_op::File::WriteItem(stream, update);
    file_op::File::WriteItem(stream, failCheckoutCount);
    file_op::File::WriteItem(stream, classID);
    location.WriteBinary(stream);

    return true;
}

escalator_map::Point
SumPointLocation(const escalator_map::Point &pX, const escalator_map::Point &pY)
{
    auto sum = pX;
    sum.location = pX.location + pY.location;
    return sum;
}
