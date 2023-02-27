#include "sector.h"
#include "src/utils/detector_param.h"
#include "src/utils/file.h"
#include "src/utils/gemotry.h"
#include "src/utils/define.h"

const float EDGE_DELTA = 0.3; // 30 cm
const float EDGE_DELTA_MAX = 0.5; // 50 cm
const float EDGE_SAFTY_DIATANCE = 0.3; // 50 cm
const float MIN_AREA_4_REMOVE = 0.04; // 2cm*cm
const float MAX_LENGTH = 6; // 6 m

const int MIN_VALID_ANGLE_4_EDGE = 7;
const double MIN_VALID_RADAIN_4_EDGE = MIN_VALID_ANGLE_4_EDGE * ANGLE_2_RADIAN;

Sector::Sector(const BoxInfo &box)
{
    isTarget = false;
    vertex = box.location.world;
    left = box.edgeLeft.world;
    right = box.edgeRight.world;
    leftOrg = left;
    rightOrg = right;
    left.z = 0;
    vertex.z = 0;
    right.z = 0;
    leftUpdate = false;
    rightUpdate = false;
    id = -1;
    boxID = -1;
    partCount = 0;

    leftLeft = left;
    rightRight = right;
    vertexOrigin = vertex;
    invalid = false;
}

Sector::Sector()
{
    leftUpdate = false;
    rightUpdate = false;
    isTarget = false;
    partCount = 0;
    invalid = false;
}

bool Sector::UpdateEdge(const BoxInfo &box, const BoxInfo &view)
{
    bool ret = false;
    ret = UpdateLeft(box, view);
    ret |= UpdateRight(box, view);
    return ret;
}

bool Sector::UpdateRight(const BoxInfo &box, const BoxInfo &view)
{
    auto &viewVertex = view.location.world;
    Segment rightOrg(this->vertex, this->right);
    Segment rightCurrent(viewVertex, box.edgeRight.world);

    // update left and right
    this->rightRight = rightCurrent.end;
    bool ret = GetCrossPoint4Line(rightOrg, rightCurrent, this->rightRight);
    LOG_CHECK_DEBUG(INFO) << "rr: " << this->rightRight.Print();

    if (not ret); // TODO : parallel

    if (this->rightRight.Distance(viewVertex) > MAX_LENGTH) return false;

    double angleEdge = GetAngle(viewVertex, this->vertex, this->right);
    rightCLose2Edge = angleEdge < MIN_VALID_RADAIN_4_EDGE;

    if (rightCLose2Edge)
    {
        if (rightCurrent.end.DistanceXY(this->vertex) >
            this->right.DistanceXY(this->vertex)
            and rightCurrent.end.DistanceXY(this->vertex) >
                rightCurrent.end.DistanceXY(viewVertex)
            and this->partCount <= box.locationSerials.size())
        {
            this->right = rightCurrent.end;
            return true; // TODO : too complex
        }

        rightCLose2Edge = false;
        return false;
    }

    double angleViewRight = GetAngle(viewVertex, view.edgeRight.world
                                     , box.edgeRight.world);
    double angleViewLeft = GetAngle(viewVertex, view.edgeLeft.world, box.edgeRight.world);
    bool close2View = MIN(angleViewLeft, angleViewRight) < MIN_VALID_RADAIN_4_VIEW;

    if (close2View) return false;

    double distanceVertexOrg = this->rightRight.Distance(this->vertex);
    if (distanceVertexOrg < MIN_DISTANCE_EDGE_2_VERTEX) return false;

    bool crossRight = SegmentCross(rightOrg, rightCurrent);
    double distanceEdgeCurrent = this->rightRight.Distance(rightCurrent.end);
    double distanceEdgeOrg = this->rightRight.Distance(rightOrg.end);
    double distanceLine = LineDistance(rightOrg.start, rightOrg.end
                                       , rightCurrent.start, rightCurrent.end);

    // expand
    bool directSame = Dot(this->vertex, this->right, this->rightRight) > 0;
    if ((not crossRight) and
        this->rightRight.Distance(this->vertex) > this->right.Distance(this->vertex)
        and directSame)
    {
        this->right = this->rightRight;
        this->rightUpdate = false;
        return false;
    }

    if (crossRight or distanceEdgeCurrent < EDGE_DELTA
        or (distanceLine < EDGE_DELTA and distanceEdgeCurrent < EDGE_DELTA_MAX))
    {
        if ((not crossRight) and rightUpdate and
            this->rightRight.Distance(this->right) > EDGE_SAFTY_DIATANCE)
        {
            return false;
        }

        // check intersection area too small
        Segment leftOrg(this->vertex, this->left);

        if (SegmentCross(leftOrg, rightCurrent))
        {
            GetCrossPoint4Line(leftOrg, rightCurrent, this->leftRight);
            float area = 0;
            std::vector<Point> removeRegion{this->vertex, this->rightRight
                                            , this->leftRight};

            ConvexPolygonArea(removeRegion, area);

            if (area < MIN_AREA_4_REMOVE) return false;
        }


        this->right = this->rightRight;
        rightUpdate = true;
    }
    else if (distanceEdgeOrg < EDGE_DELTA)
    {
        return false; // TODO : what
    }
    else return false; // TODO : what to do, do not check the other side?

    return true;
}

bool Sector::UpdateLeft(const BoxInfo &box, const BoxInfo &view)
{
    const auto &viewVertex = view.location.world;
    Segment leftOrg(this->vertex, this->left);
    Segment leftCurrent(viewVertex, box.edgeLeft.world); // TODO : use view or b.location

    // update left and right
    this->leftLeft = leftCurrent.end;

    bool ret = GetCrossPoint4Line(leftOrg, leftCurrent, this->leftLeft);
    if (not ret);// TODO : parallel
    LOG_CHECK_DEBUG(INFO) << "ll: " << this->leftLeft.Print();

    if (this->leftLeft.Distance(viewVertex) > MAX_LENGTH) return false;

    double angleEdge = GetAngle(viewVertex, this->vertex, this->left);
    leftClose2Edge = angleEdge < MIN_VALID_RADAIN_4_EDGE;

    // TODO : which case work for
    if (leftClose2Edge)
    {
        if (leftCurrent.end.DistanceXY(this->vertex) > this->left.DistanceXY(this->vertex)
            and leftCurrent.end.DistanceXY(this->vertex) >
                leftCurrent.end.DistanceXY(viewVertex)
            and this->partCount <= box.locationSerials.size())
        {
            this->left = leftCurrent.end;
            return true; // TODO : too complex
        }

        leftClose2Edge = false;
        return false; // TODO : move to below, after expand
    }

    double angleViewLeft = GetAngle(viewVertex, view.edgeLeft.world, box.edgeLeft.world);
    double angleViewRight = GetAngle(viewVertex, view.edgeRight.world
                                     , box.edgeLeft.world);
    bool close2View = MIN(angleViewLeft, angleViewRight) < MIN_VALID_RADAIN_4_VIEW;

    if (close2View) return false;

    double distanceVertexOrg = this->leftLeft.Distance(this->vertex);
    if (distanceVertexOrg < MIN_DISTANCE_EDGE_2_VERTEX) return false;

    bool crossLeft = SegmentCross(leftOrg, leftCurrent);
    double distanceEdgeCurrent = this->leftLeft.Distance(leftCurrent.end);
    double distanceEdgeOrg = this->leftLeft.Distance(leftOrg.end);
    double distanceLine = LineDistance(leftOrg.start
                                       , leftOrg.end // TODO : may be not exist
                                       , leftCurrent.start, leftCurrent.end);

    // expand
    bool directSame = Dot(this->vertex, this->left, this->leftLeft) > 0;
    if ((not crossLeft) and
        this->leftLeft.Distance(this->vertex) > this->left.Distance(this->vertex)
        and directSame)
    {
        this->left = this->leftLeft;
        this->leftUpdate = false;
        return false;
    }

    //  decrease
    if (crossLeft or distanceEdgeCurrent < EDGE_DELTA
        or (distanceLine < EDGE_DELTA and distanceEdgeCurrent < EDGE_DELTA_MAX))
    {
        // TODO : what is this
        if (not crossLeft and leftUpdate and this->leftLeft.Distance(this->left) >
                                             EDGE_SAFTY_DIATANCE) // TODO : should be inner
        {
            return false;
        }

        // check intersection area too small
        Segment rightOrg(this->vertex, this->right);

        if (SegmentCross(rightOrg, leftCurrent))
        {
            GetCrossPoint4Line(rightOrg, leftCurrent, this->rightLeft);
            float area = 0;
            std::vector<Point> removeRegion{this->vertex, this->leftLeft
                                            , this->rightLeft};

            ConvexPolygonArea(removeRegion, area);

            if (area < MIN_AREA_4_REMOVE) return false;
        }

        this->left = this->leftLeft; // TODO : check the axis, and get the min/max x/y
        leftUpdate = true;
    }
    else if (distanceEdgeOrg < EDGE_DELTA)
    {
        return false; // TODO : what
    }
    else return false; // TODO : what to do, do not check the other side?

    return true;
}

void Sector::UpdateVertex(const BoxInfo &box, const Point &view
                          , const bool leftUpdate, const bool rightUpdate)
{
    if (InView(this->vertex, this->left, this->right, view))
    {
        // TODO : should add by intersection
        if (leftUpdate and rightUpdate)
        {
            SetVertex(view);
        }
        else if (leftUpdate)
        {
            Point vertex;
            GetCrossPoint4Line(Segment(this->vertex, this->rightOrg)
                               , Segment(view, this->leftLeft), vertex);
            if (InLine(vertex, Segment(this->vertex, this->rightOrg)))
                SetVertex(vertex);
        }
        else if (rightUpdate)
        {
            Point vertex;
            GetCrossPoint4Line(Segment(this->vertex, this->leftOrg)
                               , Segment(view, this->rightRight), vertex);
            if (InLine(vertex, Segment(this->vertex, this->leftOrg)))
                SetVertex(vertex);
        }
        else
        {

        }

        return;
    }

    if (leftClose2Edge or rightCLose2Edge)
    {
        this->vertex = view;
    }

    Segment leftOrg(this->vertex, this->left);
    Segment rightOrg(this->vertex, this->right);
    Segment leftCurrent(view, box.edgeLeft.world); // TODO : use view or b.location
    Segment rightCurrent(view, box.edgeRight.world);

    bool ret = false;
    ret = GetCrossPoint4Line(leftOrg, rightCurrent, this->leftRight);
    LOG_CHECK_DEBUG(INFO) << "lr: " << leftRight.Print();

    if (not ret) int k = 0; // TODO : parallel
    ret = GetCrossPoint4Line(rightOrg, leftCurrent, this->rightLeft);
    if (not ret) int k = 0; // TODO : parallel

    this->rightLeft.z = this->rightRight.z;
    this->leftRight.z = this->leftLeft.z;

    if (rightUpdate)
    {
        if (SegmentCross(leftOrg, rightCurrent)) //  and not leftUpdate)
        {
            if ((this->rightLeft.Distance(this->vertex) >
                 this->rightRight.Distance(this->vertex))
                and this->vertex.Distance(this->right) > MIN_DISTANCE_EDGE_2_VERTEX)
            {
                SetVertex(this->leftRight);
            }
            else
            {
                // nothing
            }
        }
    }
    // TODO : should/must be else
    if (leftUpdate)
    {
        if (SegmentCross(rightOrg, leftCurrent))  //  and not rightUpdate)
        {
            if (this->leftRight.Distance(this->vertex) >
                this->leftLeft.Distance(this->vertex)
                and this->vertex.Distance(this->left) > MIN_DISTANCE_EDGE_2_VERTEX)
            {
                SetVertex(this->rightLeft);
            }
            else
            {
                // nothing
            }
        }
    }

    LOG_CHECK_DEBUG(INFO) << "rl: " << this->rightLeft.Print();
    LOG_CHECK_DEBUG(INFO) << "view: " << view.Print();
}

void Sector::Reset()
{
    invalid = false;
    isTarget = false;
    leftOrg = left;
    rightOrg = right;
    vertexOrigin = vertex;
    boxID = -1;
    Point defaultPoint;
    leftRight = defaultPoint;
    rightLeft = defaultPoint;
    leftLeft = defaultPoint;
    rightRight = defaultPoint;
    leftClose2Edge = false;
    rightCLose2Edge = false; // TODO : may be use temp
}

Sector::Sector(const Point &vertex, const Point &left, const Point &right)
{
    isTarget = false;
    this->vertex = vertex;
    this->left = left;
    this->right = right;
    leftOrg = left;
    rightOrg = right;
    this->left.z = 0;
    this->vertex.z = 0;
    this->right.z = 0;
    leftUpdate = false;
    rightUpdate = false;
    id = -1;
    boxID = -1;
    partCount = 0;

    leftLeft = left;
    rightRight = right;
    vertexOrigin = vertex;
    invalid = false;
}

void Sector::SetVertex(const Point &point)
{
    if (point.Distance(this->right) > MIN_DISTANCE_EDGE_2_VERTEX
        and point.Distance(this->left) > MIN_DISTANCE_EDGE_2_VERTEX)
    {
        this->vertex = point;
    }

    this->vertex.z = 0;
}

std::vector<Point> Sector::GetPoint() const
{
    std::vector<Point> points;

    points.push_back(this->vertex);
    points.push_back(this->left);
    points.push_back(this->right);

    return points;
}

bool Sector::Read(std::ifstream &stream)
{
    if (not stream.is_open()) return false;

    vertex.ReadBinary(stream);
    left.ReadBinary(stream);
    right.ReadBinary(stream);
    leftOrg.ReadBinary(stream);
    rightOrg.ReadBinary(stream);

    leftLeft.ReadBinary(stream);
    rightRight.ReadBinary(stream);
    vertexOrigin.ReadBinary(stream);
    leftRight.ReadBinary(stream);
    rightLeft.ReadBinary(stream);

    file_op::File::ReadItem(stream, id);
    file_op::File::ReadItem(stream, partCount);

    return true;
}

bool Sector::Write(std::ofstream &stream) const
{
    if (not stream.is_open()) return false;

    vertex.WriteBinary(stream);
    left.WriteBinary(stream);
    right.WriteBinary(stream);
    leftOrg.WriteBinary(stream);
    rightOrg.WriteBinary(stream);

    leftLeft.WriteBinary(stream);
    rightRight.WriteBinary(stream);
    vertexOrigin.WriteBinary(stream);
    leftRight.WriteBinary(stream);
    rightLeft.WriteBinary(stream);

    file_op::File::WriteItem(stream, id);
    file_op::File::WriteItem(stream, partCount);

    return true;
}
