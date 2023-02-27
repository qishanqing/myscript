//
// Created by donglijian on 11/8/22.
//
#include "aircrafthatch_detector.h"
#include "src/utils/define.h"

using namespace inner;

const int MAX_GRAD = 20;
const int EDGE_MARGIN = 10;
const int MAX_VALUE = 30;
const float MAX_DISTANCE = 10.5;
const int SMALL_OBJECT_LENGTH = 70;
const float MIN_DISTANCE_FILTER = 0.01;
const float GATE_EDGE_MIN_LENGTH = 0.2;
const float MAX_GATE = 2.9;
const float ANGLE_ERROR = 20.0 / 180 * M_PI;
const float MIN_GATE = 1;

#define CLIP(x, value) (x) > 0 ? MIN(value, (x)) : MAX(-(value), (x))

AircraftHatchDetector::AircraftHatchDetector():running(false)
{
}

AircraftHatchDetector::~AircraftHatchDetector()
{
}

void AircraftHatchDetector::Update(const Lidar &lidar)
{
    if (not running)
    {
        std::unique_lock<std::mutex> lock(lidarLock);
        lidarLastest = lidar;
    }
    LOG_CHECK_DEBUG(INFO) << "AircraftHatch lidar time: " << lidarLastest.pose.s_time;
}

// don't use this
void AircraftHatchDetector::Update(const DataGroup &item)
{
}

void AircraftHatchDetector::Polar2Cartesian(const swr::psl_sensor_types::LaserScan &lidarOrigin
                                            , std::vector<PointType> &lidarPoints)
{
    Timer timer;
    std::vector<float> xnums;
    std::vector<float> ynums;
    // TODO : check
    size_t pointCount = lidarOrigin.ranges.size();
    Eigen::ArrayXXd rangeWhole(pointCount, 2);//pt_num行，2列
    Eigen::ArrayXXd PointTypeCartesian(pointCount, 2);
    Eigen::ArrayXXd angleWhole(pointCount, 2);
    for (size_t i = 0; i < pointCount; i++)
    {
        auto angle =
                lidarOrigin.angle_min + (double) i * lidarOrigin.angle_increment;

        rangeWhole(i, 0) = (double) lidarOrigin.ranges[i];
        rangeWhole(i, 1) = (double) lidarOrigin.ranges[i];
        angleWhole(i, 0) = cos(
                lidarOrigin.angle_min + (double) i * lidarOrigin.angle_increment);
        angleWhole(i, 1) = sin(
                lidarOrigin.angle_min + (double) i * lidarOrigin.angle_increment);
    }
    PointTypeCartesian = rangeWhole * angleWhole; // x, y
    for (size_t i = 0; i < pointCount; i++)
    {
        auto temp_angle =
                lidarOrigin.angle_min + (double) i * lidarOrigin.angle_increment;

        double range_cutoff = 10;//假设有效距离10m
        const float range = lidarOrigin.ranges[i];
        if (range > lidarOrigin.range_min)
            lidarPoints.push_back(
                    PointType(PointTypeCartesian(i, 0), PointTypeCartesian(i, 1)));
        else
        {
            int k = 0;
        }
    }
//    std::cout << timer.TimingStr("to cartesian") << std::endl;
}

void AircraftHatchDetector::Gradient(const std::vector<float> &ranges, std::vector<float> &gradient)
{
    Timer timer;
    gradient.resize(ranges.size(), 0);
    const int gradStep = 3;
    const int gradWin = 0;

    for (size_t i = EDGE_MARGIN; i < ranges.size() - EDGE_MARGIN; ++i)
    {
        std::vector<float> windowCurrent{ranges.begin() + i - gradWin
                , ranges.begin() + i + gradWin + 1};
        std::vector<float> windowNext{ranges.begin() + i + 1
                , ranges.begin() + i + gradStep + 1};

        float median = Median(windowNext);
        float medianCurrent = Median(windowCurrent);

        gradient.at(i) = (median - medianCurrent);
        gradient.at(i) = CLIP(gradient.at(i), MAX_GRAD);
    }
//    std::cout << timer.TimingStr("Gradient") << std::endl;
}


bool AircraftHatchDetector::WaveSegment(const std::vector<float> &ranges, const int start, const int end
                                        , const std::vector<float> &gradient, std::vector<LidarSegment> &segments)
{
    size_t size = ranges.size();
    if ((end - start) < 7) return false;
    bool runing = false;
    float gradThreshold = 2;
    const float gradThresholdSmooth = 2;
    int minID = std::min_element(ranges.begin() + start, ranges.begin() + end) -
                ranges.begin();
    float minValue = *(ranges.begin() + minID);
    int left = start, right = end - 1;

    // check left
    for (int i = minID - 1; i >= start; --i)
    {
        const auto &value = ranges.at(i);
        const auto &grad = gradient.at(i);

        if (abs(grad) > gradThresholdSmooth)
        {
            left = i + 1;
            break;
        }
    }

    // check right
    for (int i = minID + 1; i < end; ++i)
    {
        const auto &value = ranges.at(i);
        const auto &grad = gradient.at(i);

        if (abs(grad) > gradThresholdSmooth)
        {

            right = i - 1;
            break;
        }
    }
//    std::cout<< "grade: " << gradient.at(left) << ", " << gradient.at(right)  << std::endl;

    bool retLeft = WaveSegment(ranges, start, left, gradient, segments);
    bool retRight = WaveSegment(ranges, right + 1, end, gradient, segments);

    if ((right - left) > 3)
    {
        float head = *std::max_element(ranges.begin() + left,
                                       ranges.begin() + MIN(left + 10, right));
        float tail = *std::max_element(ranges.begin() + MAX(left, right - 10),
                                       ranges.begin() + right);

        segments.push_back(
                LidarSegment(left, right, LidarSegment::NORMAL, head, tail));
    }
}

float AircraftHatchDetector::Median(const std::vector<float> &ranges)
{
    if (ranges.size() < 3) return ranges.at(0);

    std::vector<float> sorted = ranges;
    std::sort(sorted.begin(), sorted.end(), [](float a, float b)
    { return a < b; });
//    *std::min_element(windowCurrent.begin(), windowCurrent.end());

    int id = (sorted.size() - 1) >> 1;
    float value = sorted.at(id);

    return value;
}

bool AircraftHatchDetector::Detect(std::vector<BoxInfo> &boxes, DataGroup &item)
{
    {
        std::unique_lock<std::mutex> lock(lidarLock);

        if (Empty(lidarLastest.data)) return false;

        lidarCurrent = lidarLastest;
        lidarLastest = Lidar();
    }

    item.lidar = lidarCurrent;

    this->ReSet();

    bool ret = this->Detect();

    for (const auto &box : this->boxes)
    {
        boxes.push_back(box);
    }
    return ret;
}

bool AircraftHatchDetector::Detect(const Lidar &lidar, std::vector<BoxInfo> &boxes)
{
    lidarCurrent = lidar;

    if (Empty(lidarCurrent.data)) return false;

    this->ReSet();

    bool ret = this->Detect();

    for (const auto &box : this->boxes)
    {
        boxes.push_back(box);
    }
    return ret;
}

bool AircraftHatchDetector::Detect()
{
    running = true;
    std::vector<float> gradient;
    std::vector<psl::Location> aircraftHatch;
    std::vector<LidarSegment> segments;
    std::vector<float> gradientSmooth;
    Timer timer;

    if (Empty(lidarCurrent.data))
    {
        running = false;
        return false;
    }

    Polar2Cartesian(lidarCurrent.data, points);

    Gradient(lidarCurrent.data.ranges, gradient);

    GradientSmooth(lidarCurrent.data.ranges, gradientSmooth);

    WaveSegment(lidarCurrent.data.ranges, 0, lidarCurrent.data.ranges.size(), gradient, segments);

    std::sort(segments.begin(), segments.end(), [](LidarSegment &a, LidarSegment &b)
    { return a.start < b.start; });

    std::vector<PointType> pointsClean = points;
    std::vector<PointType> rotateBox = GetConvex(lidarCurrent.data.ranges, pointsClean, segments);
    rotateBox = Dilation(0.1, rotateBox);
    convex.SetConvex(rotateBox);
    std::vector<std::pair<int, int>> gates{};
//    timer.Timing("convex", true);

    if (not convex.Invalid())
        gates = CheckGate(lidarCurrent.data.ranges, points, gradient, segments, convex, true);
//    timer.Timing("gate", true);

    GetGate(gates, points);

    running = false;
    return true;
}

void AircraftHatchDetector::Clear()
{
    std::vector<BoxInfo> boxes;
}

void AircraftHatchDetector::GradientSmooth(const std::vector<float> &ranges, std::vector<float> &gradient)
{
    Timer timer;
    gradient.resize(ranges.size(), MAX_GRAD);
    const int gradStep = 5;
    const int gradWin = 0;
    int left = -1;
    float lastPoint = 0;
    std::deque<float> lastPoints;

    for (size_t i = EDGE_MARGIN; i < ranges.size() - EDGE_MARGIN; ++i)
    {
        float current = ranges.at(i);

//        if (IsINF(current)) continue;
        if (IsINF(ranges, i - 3, i) and IsINF(ranges, i, i + 3)) continue;
        lastPoint = current;
        lastPoints.push_back(current);
        if (lastPoints.size() > 5) lastPoints.pop_front();
        gradient.at(i) = CLIP(current - lastPoints.front(), MAX_GRAD);
    }

//    std::cout << timer.TimingStr("Gradient") << std::endl;
}

bool AircraftHatchDetector::IsINF(const float value)
{
    return ((abs(value - 0)) < 0.00001 or abs(value) > MAX_VALUE);
}

bool AircraftHatchDetector::IsINF(const std::vector<float> &ranges, const int start, const int end)
{
    bool inf = false;

    for (size_t i = MAX(0, start); i < MIN(ranges.size(), end); ++i)
    {
        if (IsINF(ranges.at(i)))
        {
            inf = true;
            break;
        }
    }

    return inf;
}


std::vector<PointType>
AircraftHatchDetector::GetConvex(const std::vector<float> &ranges, std::vector<PointType> &pointsClean
        , const std::vector<LidarSegment> &segments)
{
    float minID = std::min_element(ranges.begin(), ranges.end()) - ranges.begin();
    float minRange = ranges.at(minID);
    PointType minPoint = PointType(0, 0);// points.at(minID);
    const size_t size = ranges.size();

    std::vector<float> rangesClean;

    for (int i = 0; i < size; ++i)
    {
        if ((not IsINF(pointsClean.at(i).x)) and ranges.at(i) < MAX_DISTANCE)
        {
            rangesClean.push_back(ranges.at(i));
        }
        else
        {
            pointsClean.at(i) = minPoint;
            rangesClean.push_back(minRange);
        }
    }

    std::vector<cv::Point2f> rotateBox;

    std::vector<int> index = SortByIndex(rangesClean, true);
    int count = 0;

    for (int i = 0; i < pointsClean.size(); ++i)
    {
        if (count < segments.size())
        {
            const auto &seg = segments.at(count);
            if (i > seg.start)
            {
                i = seg.end - 2;
                count++;
                continue;
            }
        }

        pointsClean.at(i) = minPoint;
        rangesClean.at(i) = minRange;
    }
    count = 0;

    while (1)
    {
//        rotateBox = GrahamScan(pointsClean);
//        rotateBox = Dilation(0.1, rotateBox);

//        cv::Mat image = cv::Mat::zeros(imagerows, 1536, CV_8UC3);
//        cv::Mat imageWave = cv::Mat::zeros(imagerows, 1536, CV_8UC3);

//        showLidar(image, pointsClean);
//        DrawConvex(image, convex);
//        showWave(imageWave, rangesClean);
//        multipleImage(std::vector<cv::Mat>{image, imageWave}, image, 1);
//        cv::imshow("img", image);
//        cv::waitKey(0);
        if (count >= segments.size()) break;

        for (; count < segments.size(); ++count)
        {
            if (segments.at(count).Length() > SMALL_OBJECT_LENGTH) continue;

            const auto &seg = segments.at(count);
            int start = MIN(segments.at(count).start, segments.at(MAX(0, count - 1)).end);
            int end = MAX(segments.at(count).end, segments.at(
                    MIN(segments.size() - 1, count + 1)).start);

            for (int i = start; i < end; ++i)
            {
                pointsClean.at(i) = minPoint;
                rangesClean.at(i) = minRange;
            }

            break;
        }

        count++;

    }

    rotateBox = GrahamScan(pointsClean);

    return rotateBox;
}

std::vector<PointType> AircraftHatchDetector::GrahamScan(std::vector<PointType> location)
{
    std::vector<PointType> locationConvex;
    int top = 2;
    int index = 0;

    for (int i = 1; i < location.size(); ++i)//选出Y坐标最小的点，若Y坐标相等，选择X坐标小的点
    {
        if (location[i].y < location[index].y ||
            ((fabs(location[i].y - location[index].y) < 1e-6)
             && location[i].x < location[index].x))
        {
            index = i;
        }
    }

    auto temp = location[0];
    location[0] = location[index];
    location[index] = temp;

    locationConvex.push_back(location[0]);
    //按极角排序
    sort(location.begin() + 1, location.end(), [location, this](const PointType &p1, const PointType &p2)
    {
        double temp = CrossProduct(location[0], p1, p2);
        if (fabs(temp) < 1e-6)//极角相等按照距离从小到大排序
        {
            return Dis(location[0], p1) > Dis(location[0], p2);
        }
        else
        {
            return temp > 0;
        }
    });

    for (int i = 1; i < location.size(); ++i)
    {
        if (Distance(location[i], location[i - 1]) < MIN_DISTANCE_FILTER)
        {
            location.erase(location.begin() + i);
            --i;
        }
    }

    locationConvex.push_back(location[1]);
    locationConvex.push_back(location[2]);

    for (int i = 3; i < location.size(); ++i)
    {
        while (top > 0 &&
               CrossProduct(locationConvex[top - 1], location[i], locationConvex[top]) >= 0)
        {
            --top;
            locationConvex.pop_back();
        }
        locationConvex.push_back(location[i]);
        ++top;
    }

//    std::reverse(locationConvex.begin(), locationConvex.end());

    return locationConvex;
}

double AircraftHatchDetector::CrossProduct(PointType p0, PointType p1, PointType p2)
{
    return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

double AircraftHatchDetector::Dis(PointType p1, PointType p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

float AircraftHatchDetector::Distance(const PointType &p1, const PointType &p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

std::vector<PointType> AircraftHatchDetector::Dilation(const double radius, const std::vector<PointType> &location)
{
    const size_t size = location.size();
    std::vector<PointType> locationExtend;
    PointType center = Centroid(location);
    std::vector<double> allK(size, 0);
    std::vector<double> allB(size, 0);


    for (int i = 0; i < size; i++)
    {
        const auto &loc = location.at(i);
        int sign = (center.x > loc.x) ? -1 : 1;

        const PointType &l0 = location.at((i + size - 1) % size);
        const PointType &l1 = location.at(i);
        const PointType &l2 = location.at((i + 1) % size);
        const PointType &l3 = location.at((i + 2) % size);

        PointType fromCenter(0, 0);

        float kCenter = (loc.y - center.y) / (loc.x - center.x);
        float bCenter = loc.y - kCenter * loc.x;
        fromCenter.x = loc.x + sign * radius / sqrt(kCenter * kCenter + 1);
        fromCenter.y = kCenter * fromCenter.x + bCenter;

        locationExtend.push_back(fromCenter);

//        float convexNext = Cross(l1, l2, l3);
//        float convexPre = Cross(l0, l1, l2);

        double k = (l2.y - l1.y) / (l2.x - l1.x + 0.000001);
        float angle = atan(-1 / k);
        float extendX = cos(angle) * radius;
        float extendY = sin(angle) * radius;
        for (auto &k : {-1, 1})
        {
            sign = k;
            PointType fromLineStart(0, 0);

            fromLineStart.x = l1.x + sign * extendX;
            fromLineStart.y = l1.y + sign * extendY;


            PointType fromLineEnd(0, 0);

            fromLineEnd.x = l2.x + sign * extendX;
            fromLineEnd.y = l2.y + sign * extendY;

            locationExtend.push_back(fromLineStart);
            locationExtend.push_back(fromLineEnd);
        }
    }

    locationExtend = GrahamScan(locationExtend);

    return locationExtend;
}

PointType AircraftHatchDetector::Centroid(const std::vector<PointType> &vertexes)
{
    double area = 0.0;
    double cx = 0.0;
    double cy = 0.0;

    //取多边形第一个节点为剖分点
    PointType v0;
    bool init = false;

    for (int i = 1; i < vertexes.size() - 1; i++)
    {
        if (IsINF(vertexes.at(i).x) or IsINF(vertexes.at(i + 1).x))
            continue;
        if (not init)
        {
            v0 = vertexes[i];
            init = true;
        }
        //三角形两边向量
        PointType v1 = PointType(vertexes[i].x - v0.x, vertexes[i].y - v0.y);
        PointType v2 = PointType(vertexes[i + 1].x - v0.x, vertexes[i + 1].y - v0.y);

        double triangleArea = (v1.x * v2.y - v2.x * v1.y) / 2;
        area += triangleArea;

        double triangleCx = 0 + v1.x + v2.x;
        double triangleCy = 0 + v1.y + v2.y;

        //加权面积
        cx += triangleArea * triangleCx;
        cy += triangleArea * triangleCy;
    }

    cx = cx / area / 3 + v0.x;
    cy = cy / area / 3 + v0.y;

    return PointType(cx, cy);
}

Convex::Convex()
{}

Convex::Convex(std::vector<PointType> &rotateBox)
{
    this->SetConvex(rotateBox);
}

void Convex::SetConvex(vector<PointType> &rotateBox)
{
    this->rotateBox.swap(rotateBox);
    minRect.resize(4);
    minRectInfo = cv::minAreaRect(this->rotateBox);
    minRectInfo.points(minRect.data());
    ratio = MAX(minRectInfo.size.width, minRectInfo.size.height) /
            MIN(minRectInfo.size.width, minRectInfo.size.height);
    axisAngle = GetAxis(this->rotateBox);
}

float Convex::GetAxis(const std::vector<PointType> &points)
{
    Timer timer;
    const float MAX_ANGLE = M_PI;
    std::vector<float> lengthX;
    const float step = 0.01;

    for (float i = 0; i < MAX_ANGLE; i += step)
    {
        std::vector<PointType> pointRotate = Rotate(points, i);
        float len = 0;
        for (const auto &p : pointRotate)
        {
            len += abs(p.x);
        }

        lengthX.push_back(len);
    }

    int maxID = std::max_element(lengthX.begin(), lengthX.end()) - lengthX.begin();
    float angle = step * maxID;

    timer.Timing("get angle", false);
    return angle;
}

bool Convex::Invalid() const
{
    return (MAX(minRectInfo.size.height, minRectInfo.size.width) < MIN_OBJ_LENGTH
    or ratio < MIN_OBJ_RATION);
}

std::vector<PointType> Convex::Rotate(const std::vector<PointType> &points, const float theta)
{
    std::vector<PointType> result = points;

    for (auto &p : result)
    {
        float x = p.x * cos(theta) + p.y * sin(theta);
        float y = p.y * cos(theta) - p.x * sin(theta);
        p.x = x;
        p.y = y;
    }

    return result;
}

std::vector<std::pair<int, int>>
AircraftHatchDetector::CheckGate(const std::vector<float> &ranges, const std::vector<PointType> &points
                                 , const std::vector<float> &gradient, std::vector<LidarSegment> &segment
                                 , const Convex &convex, const bool axisSame)
{
    size_t size = ranges.size();
    const int step = 0; // limit in GetConvex
    std::vector<std::pair<int, int>> results{};
    float targetAngle = convex.axisAngle;
    if (not axisSame) targetAngle = targetAngle - M_PI / 2;

    for (int i = 0; i < segment.size(); ++i)
    {
        const auto &segPre = segment.at(i);

        if (IsINF(points.at(segPre.start + step).x)) continue;
        if (IsINF(points.at(segPre.end - step).x)) continue;
        const float distanceSegPre = Distance(points.at(segPre.start + step), points.at(
                segPre.end - step));

        if (distanceSegPre < GATE_EDGE_MIN_LENGTH) continue;

        const int left = segPre.end - step;
        int right = left;

        for (int j = i + 1; j < segment.size(); ++j)
        {
            const auto &segNext = segment.at(j);
            const float distanceSegNext = Distance(points.at(segNext.start + step), points.at(
                    segNext.end - step));

            if (distanceSegNext < GATE_EDGE_MIN_LENGTH) continue;
            right = segment.at(j).start + step;

            const PointType leftPoint = points.at(left);
            const PointType rightPoint = points.at(right);

            float margin = Distance(leftPoint, rightPoint);
            if (margin < MIN_GATE or margin > MAX_GATE) continue;

            float angle = atan((rightPoint.y - leftPoint.y) / (rightPoint.x - leftPoint.x));
            if (abs(angle - targetAngle) > ANGLE_ERROR) continue;

            results.push_back(std::make_pair(left, right));
            i = j;
            break;
        }
    }

    PRINTF(results.size());
    return results;
}

void AircraftHatchDetector::GetGate(const std::vector<std::pair<int, int>> &gates
                                     , const std::vector<PointType> &points)
{
    for (const auto &g : gates)
    {
        BoxInfo box;
        box.classID = 4;
        box.className = "aircraft_hatch";
        box.edgeLeft.machine = psl::Location(points.at(g.first).x, points.at(g.first).y,0);
        box.edgeRight.machine = psl::Location(points.at(g.second).x, points.at(g.second).y,0);
        boxes.push_back(box);
    }
}

void AircraftHatchDetector::ReSet()
{
    this->boxes.clear();
    this->points.clear();
    this->convex = Convex();
}

cv::Mat AircraftHatchDetector::Show(const ConfigParam &configParam, cv::Mat image)
{
    std::vector<PointType> points;
    points = this->points;
    Convex convex = this->convex;
    std::vector<PointType> rotateBox = convex.rotateBox;

    Lidar2World(points,configParam);
    Lidar2World(rotateBox,configParam);
    convex.SetConvex(rotateBox);
    ShowLidar(image, points);
    DrawConvex(image, convex);
    return image;
}

void
AircraftHatchDetector::ShowLidar(cv::Mat &image, std::vector<PointType> points)
{
    cv::Mat rotateImg;
    cv::Scalar pointcolor(255, 0, 0);
    cv::Scalar linescolor(255, 255, 255);

    for (size_t i = 0; i < points.size(); ++i)
    {
        PointType PointScale = Scale(points.at(i));
        cv::circle(image, cv::Point(PointScale.x, PointScale.y), 1, pointcolor, -1);
    }

    //draw view
    PointType view(0, 0);
    for (int i = 0; i < points.size(); ++i)
    {
        auto first = points.at(i);
        if (first.x > MAX_VALUE) continue;
        first = Scale(first);
        cv::line(image, Scale(view), cv::Point(first.x, first.y)
                , linescolor);
        break;
    }
    for (int i = points.size() - 1; i > 0; --i)
    {
        auto last = points.at(i);
        if (IsINF(last.x)) continue;
        last = Scale(last);
        cv::line(image, Scale(view), cv::Point(last.x, last.y)
                , linescolor);
        break;
    }
}

PointType AircraftHatchDetector::Scale(const PointType &points)
{
    PointType PointScale;
    PointScale.x = (points.x + 10) * 25;
    PointScale.y = (points.y + 10) * 25;

    return PointScale;
}

void AircraftHatchDetector::DrawConvex(cv::Mat &image, const Convex &convex)
{
    const size_t size = convex.rotateBox.size();
    PointType maxStart, maxEnd;
    float maxDistance = 0;

    for (int i = 0; i < size; i++)
    {
        const auto &start = convex.rotateBox.at(i);
        const auto &end = convex.rotateBox.at((i + 1) % size);
        float distance = Distance(start, end);
        if (maxDistance < distance)
        {
            maxDistance = distance;
            maxStart = start;
            maxEnd = end;
        }

        cv::Scalar color(0, 0, 255);
        cv::line(image, Scale(start), Scale(end), color, 2);
    }

    // convex min rect
    for (int i = 0; i < convex.minRect.size(); i++)
    {
        cv::Scalar color = cv::Scalar(0, 255, 0);

        if (convex.Invalid()) color = cv::Scalar(0, 100, 100);

        cv::line(image, Scale(convex.minRect.at(i))
                , Scale(convex.minRect.at((i + 1) % convex.minRect.size())), color
                , 2);
    }
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2)
           << "H: " << convex.minRectInfo.size.height << " W: " << convex.minRectInfo.size.width
           << " R: "
           << convex.ratio;
    cv::putText(image, stream.str(), cv::Point(80, 80)
            , cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 0), 1);

    // center
    PointType centroid = Centroid(convex.rotateBox);
    cv::circle(image, Scale(centroid), 3, cv::Scalar(255, 100, 255), -1);

    cv::circle(image, Scale(maxStart), 6, cv::Scalar(0, 0, 255), -1);
    cv::circle(image, Scale(maxEnd), 6, cv::Scalar(0, 0, 255), -1);

    // draw angle

    float k = tan(convex.axisAngle);
    float b = centroid.y - k * centroid.x;
    PointType startAxis = convex.minRect[0], endAxis = convex.minRect[0];

    for (const auto &p : convex.minRect)
    {
        startAxis.x = MIN(p.x, startAxis.x);
        endAxis.x = MAX(p.x, startAxis.x);
    }

    startAxis.y = k * startAxis.x + b;
    endAxis.y = k * endAxis.x + b;
    cv::line(image, Scale(startAxis), Scale(endAxis), cv::Scalar(0, 140, 255), 2);

    for (int i = 0; i < size; i++)
    {
        const auto &start = convex.rotateBox.at(i);
        cv::Scalar color(0, 0, 255);
        switch (i)
        {
            case 0:
                color = cv::Scalar(255, 255, 255);
                cv::circle(image, Scale(start), 3, color, -1);
                break;
            case 1:
                color = cv::Scalar(0, 255, 255);
                cv::circle(image, Scale(start), 3, color, -1);
                break;
            case 2:
                color = cv::Scalar(255, 0, 255);
                cv::circle(image, Scale(start), 3, color, -1);
                break;
            case 3:
                color = cv::Scalar(100, 100, 255);
                cv::circle(image, Scale(start), 3, color, -1);
                break;
            default:
                break;
        }
    }
}

void AircraftHatchDetector::Lidar2World(std::vector<PointType> &points, const ConfigParam &configParam)
{
    for (auto &point : points)
    {
        psl::Location locationWorld;
        psl::Location locationMachine = psl::Location(point.y, point.x, 0);
        Lidar2Machine(configParam, locationMachine);
        Machine2World(locationMachine, locationWorld, lidarCurrent.pose);
        point = PointType(locationWorld.x, locationWorld.y);
    }
}