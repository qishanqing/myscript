//
// Created by donglijian on 12/16/21.
//

#include <gtest/gtest.h>
#include "src/utils/gemotry.h"

const int W = 2000, H = 1200;

void DrawGridLine(const cv::Mat &image, const int DELTA, const int ADD, cv::Scalar color)
{
    for (int i = -ADD; i <= ADD; ++i)
    {
        int x = i + ADD;
        cv::line(image, cv::Point(int(x * DELTA), 0)
                , cv::Point(int(x * DELTA), H)
                , color, 1);

        cv::line(image, cv::Point(0, int(x * DELTA))
                , cv::Point(W, int(x * DELTA))
                , color, 1);

        cv::putText(image, std::to_string(int(x * DELTA)), cv::Point(10, MAX(20
                    , int(x *DELTA)))
                , cv::FONT_HERSHEY_PLAIN, 1, color);
        cv::putText(image, std::to_string(int(x * DELTA)), cv::Point(
                            MAX(20, int(x *DELTA - 20))
                            , W - 10)
                , cv::FONT_HERSHEY_PLAIN, 1, color);
    }
}


void GenerateSegment(std::vector<Segment> &segmentTests, const Segment &segment)
{
    //same one point
    Segment segmentTest = segment;
    segmentTests.push_back(segmentTest);

    //vertical line
    segmentTest.end.y = segmentTest.start.y;
    segmentTests.push_back(segmentTest);

    //horizontal line
    segmentTest = segment;
    segmentTest.end.x = segmentTest.start.x;
    segmentTests.push_back(segmentTest);

    //diagonal line
    segmentTest = segment;
    segmentTest.end.x = 2 * segmentTest.start.x - segmentTest.end.x;
    segmentTests.push_back(segmentTest);

    //diagonal line
    segmentTest = segment;
    segmentTest.end.x = 2 * segmentTest.end.x - segmentTest.start.x;
    segmentTests.push_back(segmentTest);

    // point in another line
    Segment segmentTest1;
    segmentTest1 = segment;
    segmentTest1.start.x = (segment.start.x + segment.end.x)/2;
    segmentTest1.start.y = (segment.start.y + segment.end.y)/2;
    segmentTests.push_back(segmentTest1);

    segmentTest = segmentTest1;
    segmentTest.end.y = segmentTest.start.y;
    segmentTests.push_back(segmentTest);

    segmentTest = segmentTest1;
    segmentTest.end.x = segmentTest.start.x;
    segmentTests.push_back(segmentTest);

    segmentTest = segmentTest1;
    segmentTest.end.x = 2 * segmentTest.start.x - segmentTest.end.x;
    segmentTests.push_back(segmentTest);

    segmentTest = segmentTest1;
    segmentTest.end.x = 2 * segmentTest.end.x - segmentTest.start.x;
    segmentTests.push_back(segmentTest);

    // other
    segmentTests.push_back(Segment({700,700,0},{600,600,0}));
    segmentTests.push_back(Segment({800,600,0},{400,500,0}));

    segmentTests.push_back(Segment({100,900,0},{400,600,0}));

    segmentTests.push_back(Segment({500,100,0},{500,900,0}));

}

void GenerateSegmentRandom(std::vector<Segment> &segmentTests, const Point &point)
{
    for (int i = 0; i < 15; ++i)
    {
        Point temp = Point(rand()%1000, rand()%1000,0);
        segmentTests.push_back(Segment({point, temp}));
    }
}

void MoveSegment(Segment &sectorTemp, psl::Location startPoint, const float scale)
{
    sectorTemp.start.x = int(sectorTemp.start.x * scale + startPoint.x);
    sectorTemp.start.y = int(sectorTemp.start.y * scale + startPoint.y);
    sectorTemp.end.x = int(sectorTemp.end.x * scale + startPoint.x);
    sectorTemp.end.y = int(sectorTemp.end.y * scale + startPoint.y);
}
void DrawSegment(cv::Mat &image, const Segment &segment, const cv::Scalar scalar, bool triangle = false)
{
    psl::Location p0 = segment.start;
    psl::Location p1 = segment.end;

    cv::line(image, cv::Point(int(p0.x), int(p0.y))
            , cv::Point(int(p1.x), int(p1.y))
            , scalar, 1);
}

void DrawCrossSegmentPoint(const std::vector<Point> &crossPoints, cv::Mat image)
{
    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);

    for (auto point : crossPoints)
    {
        cv::circle(image, cv::Point(int(point.x), int(point.y)), 1, colorAxis, 3);
    }
}

void LineCrossTestOne()
{
    Point point;
    GetCrossPoint4Segment(Segment({1800,200,0},{1640,40,0}), Segment({1800,200,0},{1827,227,0}), point);
    std::cout<<"one wrong cross point: "<<point.Print()<<std::endl;
    return;
}

void PointsInLine(const Segment &line, const int interval, const std::string testFunc)
{
    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);
    cv::Mat image = cv::Mat::zeros(H/2, W/2, CV_8UC3);

    cv::line(image,cv::Point(line.start.x,line.start.y), cv::Point(line.end.x,line.end.y),colorBack);

    for (int i = 0; i < W; i += interval)
    {
        for (int j = 0; j < H; j += interval)
        {
            if (testFunc == "OnSegment")
            {
                if (OnSegment(line.start, line.end, Point(i,j,0)))
                {
                    cv::circle(image, cv::Point(i, j), 1, colorAxis, 3);
                }
                else
                {
                    cv::circle(image, cv::Point(i, j), 1, colorObject, 3);
                }
            }
            else if (testFunc == "OnLine")
            {
                if (OnLine(line.start, line.end, Point(i,j,0)))
                {
                    cv::circle(image, cv::Point(i, j), 1, colorAxis, 3);
                }
                else
                {
                    cv::circle(image, cv::Point(i, j), 1, colorObject, 3);
                }
            }
            else if (testFunc == "InLine")
            {
                if (InLine(Point(i,j,0), line))
                {
                    cv::circle(image, cv::Point(i, j), 1, colorAxis, 3);
                }
                else
                {
                    cv::circle(image, cv::Point(i, j), 1, colorObject, 3);
                }
            }
            else
            {
                std::cout<<"wrong function name return!\n";
                return;
            }
        }
    }

    cv::imshow(testFunc,image);
    cv::waitKey(0);
}

bool LineCrossTestRandom(const bool randFlag)
{
    const int DELTA = 50, ADD = 1600;
    const int DELTA1 = 400, ADD1 = 4;

    std::vector<Point> crossPoints;
    Segment segmentOriginal;
    std::vector<Segment> segmentTests;

    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    cv::Mat image = cv::Mat::zeros(H, W, CV_8UC3);

    DrawGridLine(image, DELTA, ADD, cv::Scalar(100, 100, 100));
    DrawGridLine(image, DELTA1, ADD1, cv::Scalar(255, 255, 255));

    psl::Location p1, p2;
    p1.x = 500;
    p1.y = 500;
    p2.x = 100;
    p2.y = 100;

    segmentOriginal = Segment(p1,p2);

    if (not randFlag)
    {
        GenerateSegment(segmentTests, segmentOriginal);
    }
    else
    {
        GenerateSegmentRandom(segmentTests, segmentOriginal.start);
    }

    std::vector<psl::Location> startPoints;
    std::vector<Segment> segmentOriginals;
    for (int i = 0; i < segmentTests.size(); ++i)
    {
        int row = i % 5;
        int col = i / 5;

        segmentOriginals.push_back(segmentOriginal);
        startPoints.push_back(psl::Location(DELTA1*row, DELTA1 * col, 0));

        MoveSegment(segmentOriginals[i], startPoints[i], DELTA1/ 1000.0);
        MoveSegment(segmentTests[i], startPoints[i], DELTA1/ 1000.0);

        DrawSegment(image, segmentOriginals[i], cv::Scalar(255, 255, 255), true);
        DrawSegment(image, segmentTests[i], cv::Scalar(50, 250, 50), true);

        Point crossPoint;
        GetCrossPoint4Segment(segmentOriginals[i], segmentTests[i], crossPoint);
        crossPoints.push_back(crossPoint);

        for(auto point: crossPoints)
        {
            if (OnLine(segmentOriginals[i].start, segmentOriginals[i].end, point))
            {
                std::cout<<"Line: "<< i <<" cross point in segmentOriginals";
            }
            else
            {
                std::cout<<"Line: "<< i <<" cross point: "<<point.Print()<<" not in segmentOriginals\n";
                std::cout<<"line1: start: "<<segmentOriginals[i].start.Print()<<", end: "
                         <<segmentOriginals[i].end.Print()<<". line2: start: "<<segmentTests[i].start.Print()
                         <<", end: "<<segmentTests[i].end.Print()<<std::endl;
                return false;
            }

            if (OnLine(segmentTests[i].start, segmentTests[i].end, point))
            {
                std::cout<<"Line: "<< i <<" cross point in segmentTests\n";
            }
            else
            {
                std::cout<<"Line: "<< i <<" cross point: "<<point.Print()<<" not in segmentTests\n";
                std::cout<<"line1: start: "<<segmentOriginals[i].start.Print()<<", end: "
                         <<segmentOriginals[i].end.Print()<<". line2: start: "<<segmentTests[i].start.Print()
                         <<", end: "<<segmentTests[i].end.Print()<<std::endl;
                return false;
            }
        }

        DrawCrossSegmentPoint(crossPoints, image);

        crossPoints.clear();
    }
    cv::resize(image, image, cv::Size(int(W * 0.8) , int(H * 0.8)));

    cv::imshow("GetCrossPointTestMulti",image);
    cv::waitKey(0);
    return true;
}

void LineCrossTest(const bool getGetCrossPoint = false)
{
    const int DELTA = 50, ADD = 1600;
    const int DELTA1 = 400, ADD1 = 4;

    std::vector<Point> crossPoints;
    Segment segmentOriginal;
    std::vector<Segment> segmentTests;

    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    cv::Mat image = cv::Mat::zeros(H, W, CV_8UC3);

    DrawGridLine(image, DELTA, ADD, cv::Scalar(100, 100, 100));
    DrawGridLine(image, DELTA1, ADD1, cv::Scalar(255, 255, 255));

    psl::Location p1, p2;
    p1.x = 500;
    p1.y = 500;
    p2.x = 100;
    p2.y = 100;

    segmentOriginal = Segment(p1,p2);

    GenerateSegment(segmentTests, segmentOriginal);

    std::vector<psl::Location> startPoints;
    std::vector<Segment> segmentOriginals;
    for (int i = 0; i < segmentTests.size(); ++i)
    {
        int row = i % 5;
        int col = i / 5;

        segmentOriginals.push_back(segmentOriginal);
        startPoints.push_back(psl::Location(DELTA1*row, DELTA1 * col, 0));

        MoveSegment(segmentOriginals[i], startPoints[i], DELTA1/ 1000.0);
        MoveSegment(segmentTests[i], startPoints[i], DELTA1/ 1000.0);

        DrawSegment(image, segmentOriginals[i], cv::Scalar(255, 255, 255), true);
        DrawSegment(image, segmentTests[i], cv::Scalar(50, 250, 50), true);

        if (getGetCrossPoint)
        {
            GetCrossPoint(segmentOriginals[i], segmentTests[i], crossPoints);
        }
        else
        {
            Point crossPoint;
            GetCrossPoint4Segment(segmentOriginals[i], segmentTests[i], crossPoint);
            crossPoints.push_back(crossPoint);
        }

        DrawCrossSegmentPoint(crossPoints, image);

        crossPoints.clear();
    }
    cv::resize(image, image, cv::Size(int(W * 0.8) , int(H * 0.8)));

    if (getGetCrossPoint)
    {
        cv::imshow("GetCrossPoint",image);
        cv::waitKey(0);
    }
    else
    {
        cv::imshow("GetCrossPoint4Segment",image);
        cv::waitKey(0);
    }
}

TEST(FunctorTest, LineCrossAll)
{
    LineCrossTest();
    LineCrossTest(true);
}

// only for GetCrossPoint4Segment func
TEST(FunctorTest, LineCrossRandom)
{
    srand(time(NULL));
    for (int i = 0; i < 100; ++i)
    {
        if (not LineCrossTestRandom(true))
        {
            FAIL();
            break;
        }
    }
}

// one wrong cross point (nan,nan)
TEST(FunctorTest, LineCrossOne)
{
    LineCrossTestOne();
}

TEST(FunctorTest, PointOnLineTest)
{
    std::string funcOnSegment = "OnSegment";
    std::string funcOnLine = "OnLine";
    std::string funcInLine = "InLine";
    PointsInLine(Segment({100,100,0},{500,500,0}), 10, funcOnSegment);
    PointsInLine(Segment({100,100,0},{500,500,0}), 10, funcOnLine);
    PointsInLine(Segment({100,100,0},{500,500,0}), 10, funcInLine);
}