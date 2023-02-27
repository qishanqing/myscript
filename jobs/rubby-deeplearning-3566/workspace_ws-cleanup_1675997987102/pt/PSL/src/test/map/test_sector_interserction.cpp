//
// Created by donglijian on 11/29/21.
//

#include <gtest/gtest.h>
#include "src/utils/gemotry.h"
#include "src/utils/timer.h"

using sector = std::vector<psl::Location>;

const int W = 2000, H = 1200;
const double LINE_LENGTH = 200;
void DrawGrid(const cv::Mat &image, const int DELTA, const int ADD, cv::Scalar color)
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

void DrawSector(cv::Mat &image, const sector &points, const cv::Scalar scalar, bool triangle = false)
{
    if (points.size() != 3)
    {
        std::cout<<"sector points' number is wrong!"<<std::endl;
       return;
    }
    psl::Location p0 = points[0];
    psl::Location p1 = points[1];
    psl::Location p2 = points[2];

    cv::line(image, cv::Point(int(p0.x), int(p0.y))
            , cv::Point(int(p1.x), int(p1.y))
            , scalar, 1);

    cv::line(image, cv::Point(int(p0.x), int(p0.y))
            , cv::Point(int(p2.x), int(p2.y))
            , scalar, 1);
    if(triangle)
    {
        cv::line(image, cv::Point(int(p1.x), int(p1.y))
                , cv::Point(int(p2.x), int(p2.y))
                , scalar, 1);
    }
}

void GetCrossPointTest()
{
    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    psl::Location p1, p2, p3, p4, p5, p6;

    p1.x = rand()%1000;
    p1.y = rand()%1000;
    p2.x = rand()%1000;
    p2.y = rand()%1000;
    p3.x = rand()%1000;
    p3.y = rand()%1000;
    p4.x = rand()%1000;
    p4.y = rand()%1000;
    p5.x = rand()%1000;
    p5.y = rand()%1000;
    p6.x = rand()%1000;
    p6.y = rand()%1000;


    Segment l0 = Segment(p1, p2);
    Segment l1 = Segment(p1, p3);
    Segment l2 = Segment(p4, p5);
    Segment l3 = Segment(p4, p6);

    sector crossPoints;

    if(InView(p1, p2, p3, p4))
    {
        crossPoints.push_back(p4);
    }
    if(InView(p4, p5, p6, p1))
    {
        crossPoints.push_back(p1);
    }

    GetCrossPoint(l0, l2, crossPoints);
    GetCrossPoint(l0, l3, crossPoints);
    GetCrossPoint(l1, l2, crossPoints);
    GetCrossPoint(l1, l3, crossPoints);

    cv::Mat image = cv::Mat::zeros(H, W, CV_8UC3);

    cv::line(image, cv::Point(int(p1.x), int(p1.y))
            , cv::Point(int(p2.x), int(p2.y))
            , colorObject, 1);

    cv::line(image, cv::Point(int(p1.x), int(p1.y))
            , cv::Point(int(p3.x), int(p3.y))
            , colorObject, 1);

    cv::line(image, cv::Point(int(p4.x), int(p4.y))
            , cv::Point(int(p5.x), int(p5.y))
            , colorObject, 1);

    cv::line(image, cv::Point(int(p4.x), int(p4.y))
            , cv::Point(int(p6.x), int(p6.y))
            , colorObject, 1);

    for (auto point : crossPoints)
    {
        cv::circle(image, cv::Point(int(point.x), int(point.y)), 1
                   , colorAxis, 3);
    }

    std::cout<<crossPoints.size()<<std::endl;
    cv::imshow("GetCrossPointTest", image);
    cv::waitKey(0);
}

void DrawCenterPoint(cv::Mat image, const psl::Location centerPoint, const float area
                     , cv::Scalar colorBack = cv::Scalar (100, 100, 100))
{
    std::string printArea;
    if (area < 0)
    {
        printArea = "Point";
    }
    else if (abs(area - 1e+6) < 1e-6)
    {
        printArea = "MAX_INVIEW";
    }
    else if (abs(area - 2e+6) < 1e-6)
    {
        printArea = "MAX_OUTVIEW";
    }
    else
    {
        printArea =  std::to_string(area);
    }
    cv::putText(image, printArea, cv::Point(centerPoint.x ,centerPoint.y)
                , cv::FONT_HERSHEY_PLAIN, 1, colorBack);
}
void DrawCrossPoint(const sector &crossPoints, cv::Mat image)
{
    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    for (auto point : crossPoints)
    {
        cv::circle(image, cv::Point(int(point.x), int(point.y)), 1, colorAxis, 3);
    }
}

void InViewTest()
{
    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    psl::Location p1;
    psl::Location p2;
    psl::Location p3;

    p1.x = 0;
    p1.y = 0;
    p2.x = 500;
    p2.y = 500;
    p3.x = 1000;
    p3.y = 0;

    cv::Mat image = cv::Mat::zeros(H, W, CV_8UC3);

    cv::line(image, cv::Point(int(p1.x), int(p1.y))
            , cv::Point(int(p2.x), int(p2.y))
            , colorObject, 1);

    cv::line(image, cv::Point(int(p1.x), int(p1.y))
            , cv::Point(int(p3.x), int(p3.y))
            , colorObject, 1);

    for (int i = 0; i < 10; ++i)
    {
        psl::Location point;
        if (i == 1)
        {
            point.x = 100;
            point.y = 100;
        } else if (i == 2)
        {
            point.x = 500;
            point.y = 500;
        }else if (i == 3)
        {
            point.x = 0;
            point.y = 0;
        }else if (i == 4)
        {
            point.x = 1000;
            point.y = 0;
        }
        else if (i == 4)
        {
            point.x = 550;
            point.y = 550;
        }
        else
        {
            point.x = rand()%1000;
            point.y = rand()%1000;
        }

        // point = p1 or p2 or p3 return true
        if (InView(p1,p2,p3,point))
        {
            cv::circle(image, cv::Point(int(point.x), int(point.y)), 1
                    , colorObject, 5);
            cv::putText(image, std::to_string(point.x)
                    , cv::Point(point.x,point.y)
                    , cv::FONT_HERSHEY_PLAIN, 1, colorAxis);
            std::cout<<"x:"<<point.x<<" , y: "<<point.y<<std::endl;
        }
        else
        {
            cv::circle(image, cv::Point(int(point.x), int(point.y)), 1
                    , colorAxis, 5);
            cv::putText(image, std::to_string(point.x)
                    , cv::Point(point.x,point.y)
                    , cv::FONT_HERSHEY_PLAIN, 1, colorAxis);
            std::cout<<"x:"<<point.x<<" , y: "<<point.y<<std::endl;

        }
    }
    cv::imshow("InViewTest", image);
    cv::waitKey(0);
}

void MoveSector(sector &sectorTemp, psl::Location startPoint, const float scale)
{
    for (auto &point:sectorTemp)
    {
        point.x = int(point.x * scale + startPoint.x);
        point.y = int(point.y * scale + startPoint.y);
    }
}

void GenerateSector(std::vector<sector> &sectorTests)
{
    sector sectorTest;
    //test1
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,700,0));
    sectorTest.push_back(psl::Location(400,400,0));
    sectorTest.push_back(psl::Location(600,400,0));
    sectorTests.push_back(sectorTest);

    //test 2
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,400,0));
    sectorTest.push_back(psl::Location(400,300,0));
    sectorTest.push_back(psl::Location(600,300,0));
    sectorTests.push_back(sectorTest);

    //test 3
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,200,0));
    sectorTest.push_back(psl::Location(700,400,0));
    sectorTest.push_back(psl::Location(200,500,0));
    sectorTests.push_back(sectorTest);

    //test 4
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,300,0));
    sectorTest.push_back(psl::Location(0,100,0));
    sectorTest.push_back(psl::Location(1000,100,0));
    sectorTests.push_back(sectorTest);

    //test 5
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,350,0));
    sectorTest.push_back(psl::Location(400,300,0));
    sectorTest.push_back(psl::Location(600,250,0));
    sectorTests.push_back(sectorTest);


    //test 6
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,450,0));
    sectorTest.push_back(psl::Location(700,500,0));
    sectorTest.push_back(psl::Location(700,900,0));
    sectorTests.push_back(sectorTest);

    //test 7
    sectorTest.clear();
    sectorTest.push_back(psl::Location(300,400,0));
    sectorTest.push_back(psl::Location(700,450,0));
    sectorTest.push_back(psl::Location(700,175,0));
    sectorTests.push_back(sectorTest);

    //test 8
    sectorTest.clear();
    sectorTest.push_back(psl::Location(800,300,0));
    sectorTest.push_back(psl::Location(100,300,0));
    sectorTest.push_back(psl::Location(100,400,0));
    sectorTests.push_back(sectorTest);

    //test 9
    sectorTest.clear();
    sectorTest.push_back(psl::Location(300,800,0));
    sectorTest.push_back(psl::Location(200,1000,0));
    sectorTest.push_back(psl::Location(400,1000,0));
    sectorTests.push_back(sectorTest);


    //test 10
    sectorTest.clear();
    sectorTest.push_back(psl::Location(734,700,0));
    sectorTest.push_back(psl::Location(634,400,0));
    sectorTest.push_back(psl::Location(834,400,0));
    sectorTests.push_back(sectorTest);


    //test 11
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,500,0));
    sectorTest.push_back(psl::Location(600,400,0));
    sectorTest.push_back(psl::Location(834,400,0));
    sectorTests.push_back(sectorTest);

    //test 12
    sectorTest.clear();
    sectorTest.push_back(psl::Location(100,100,0));
    sectorTest.push_back(psl::Location(500,500,0));
    sectorTest.push_back(psl::Location(900,100,0));
    sectorTests.push_back(sectorTest);

    //test 13
    sectorTest.clear();
    sectorTest.push_back(psl::Location(50,50,0));
    sectorTest.push_back(psl::Location(600,600,0));
    sectorTest.push_back(psl::Location(1000,0,0));
    sectorTests.push_back(sectorTest);

    //test 14
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,100,0));
    sectorTest.push_back(psl::Location(700,400,0));
    sectorTest.push_back(psl::Location(200,200,0));
    sectorTests.push_back(sectorTest);

    //test 15
    sectorTest.clear();
    sectorTest.push_back(psl::Location(500,10,0));
    sectorTest.push_back(psl::Location(700,400,0));
    sectorTest.push_back(psl::Location(200,500,0));
    sectorTests.push_back(sectorTest);
}

void TestOneCross()
{
    const int DELTA = 50, ADD = 1600;

    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    cv::Mat image = cv::Mat::zeros(H, W, CV_8UC3);

    DrawGrid(image, DELTA, ADD, cv::Scalar(100, 100, 100));
    sector sectorOriginal;
    sector sectorTest;

    sector crossPoints;
    float area = 0;
    sectorOriginal.push_back(psl::Location(500,500,0));
    sectorOriginal.push_back(psl::Location(200,800,0));
    sectorOriginal.push_back(psl::Location(300,850,0));
    sectorTest.push_back(psl::Location(670,650,0));
    sectorTest.push_back(psl::Location(250,750,0));
    sectorTest.push_back(psl::Location(250,850,0));

    DrawSector(image, sectorOriginal, cv::Scalar(255, 255, 255));
    DrawSector(image, sectorTest, cv::Scalar(50, 250, 50));

    GetCrossPointTestMulti(sectorOriginal, sectorTest, area, crossPoints);
    DrawCrossPoint(crossPoints, image);
    DrawCenterPoint(image, sectorOriginal[0], area, cv::Scalar(0,0,200));
    cv::imshow("GetCrossPointTestMulti1",image);
    cv::waitKey(0);
}

void TestAllCross()
{
    const int DELTA = 50, ADD = 1600;
    const int DELTA1 = 400, ADD1 = 4;

    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    cv::Mat image = cv::Mat::zeros(H, W, CV_8UC3);

    DrawGrid(image, DELTA, ADD, cv::Scalar(100, 100, 100));
    DrawGrid(image, DELTA1, ADD1, cv::Scalar(255, 255, 255));

    sector crossPoints;
    float area = 0;
    double TriangleAreaTime = 0;

    sector sectorOriginal;
    std::vector<sector> sectorTests;
    GenerateSector(sectorTests);

    psl::Location p1, p2, p3;
    p1.x = 500;
    p1.y = 500;
    p2.x = 100;
    p2.y = 100;
    p3.x = 900;
    p3.y = 100;
    sectorOriginal.push_back(p1);
    sectorOriginal.push_back(p2);
    sectorOriginal.push_back(p3);

    std::vector<psl::Location> startPoints;
    std::vector<sector> sectorOriginals;
    for (int i = 0; i < sectorTests.size(); ++i)
    {
        int row = i % 5;
        int col = i / 5;
        std::string triangleIndex = "sector: " + std::to_string(i);

        sectorOriginals.push_back(sectorOriginal);
        startPoints.push_back(psl::Location(DELTA1*row, DELTA1 * col, 0));
        MoveSector(sectorOriginals[i], startPoints[i], DELTA1/ 1000.0);
        MoveSector(sectorTests[i], startPoints[i], DELTA1/ 1000.0);

        DrawSector(image, sectorOriginals[i], cv::Scalar(255, 255, 255));
        DrawSector(image, sectorTests[i], cv::Scalar(50, 250, 50));

        Timer timeCost;
        GetCrossPointTestMulti(sectorOriginals[i], sectorTests[i], area, crossPoints);
        TriangleAreaTime += timeCost.Timing(triangleIndex, true);

        DrawCrossPoint(crossPoints, image);
        DrawCenterPoint(image, sectorTests[i][0], -1);
        DrawCenterPoint(image, sectorOriginals[i][0], -1);

        psl::Location areaShow;
        areaShow.x =  sectorOriginals[i][0].x + 50;
        areaShow.y =  sectorOriginals[i][0].y + 50;
        DrawCenterPoint(image, areaShow, area, cv::Scalar(0,0,200));
        crossPoints.clear();
    }
    cv::resize(image, image, cv::Size(int(W * 0.8) , int(H * 0.8)));

    std::cout<<"Sectors' numbers:  "<<sectorTests.size()<<std::endl;
    std::cout<<"Sectors' area cal total time: "<<TriangleAreaTime<<std::endl;
    std::cout<<"Average sectors' area time: "<<TriangleAreaTime / sectorTests.size()<<std::endl;

    cv::imshow("GetCrossPointTestMulti",image);
    cv::waitKey(0);
}

bool GetTriangleAreaTimeFunc(const std::vector<Point> &sectorOriginal
        , const std::vector<Point> &sectorTest
        , float &area, std::vector<Point> &crossPoints
        , const double &lineLength)
{
    Timer  GetTriangleAreaTime, GetTriangleAreaTimeAll;
    if (sectorOriginal.size() != 3 or sectorTest.size() != 3)
    {
        std::cout<<"Triangle with wrong points"<<std::endl;
        return false;
    }
    GetTriangleAreaTime.Timing("if (sectorOriginal.size() != 3 or sectorTest.size() != 3)",true);
    std::vector<Point> sectorExtend;
    ExtendSector(sectorOriginal, sectorExtend, lineLength);
    GetTriangleAreaTime.Timing("sectorExtend", true);

    CrossPointInTriangleLine(sectorExtend, sectorTest, crossPoints);
    GetTriangleAreaTime.Timing("CrossPointInTriangleLine", true);

    CrossPointInTriangle(sectorExtend, sectorTest, crossPoints);
    GetTriangleAreaTime.Timing("CrossPointInTriangle", true);

    DeleteRepeatPoint(crossPoints);
    GetTriangleAreaTime.Timing("DeleteRepeatPoint", true);

    if (crossPoints.size() < 3)
    {
        area = 0;
        return true;
    }

    ConvexPolygonArea(crossPoints, area);
    GetTriangleAreaTime.Timing("ConvexPolygonArea", true);
    GetTriangleAreaTimeAll.Timing("GetTriangleAreaTimeAll",true);
}

void TestTriangle(const bool timeFlag = false)
{
    const int DELTA = 50, ADD = 1600;
    const int DELTA1 = 400, ADD1 = 4;

    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    cv::Mat image = cv::Mat::zeros(H, W, CV_8UC3);

    DrawGrid(image, DELTA, ADD, cv::Scalar(100, 100, 100));
    DrawGrid(image, DELTA1, ADD1, cv::Scalar(255, 255, 255));

    sector crossPoints;
    float area = 0;
    double TriangleAreaTime = 0;

    sector sectorOriginal;
    std::vector<sector> sectorTests;
    GenerateSector(sectorTests);

    psl::Location p1, p2, p3;
    p1.x = 500;
    p1.y = 500;
    p2.x = 100;
    p2.y = 100;
    p3.x = 900;
    p3.y = 100;
    sectorOriginal.push_back(p1);
    sectorOriginal.push_back(p2);
    sectorOriginal.push_back(p3);

    std::vector<psl::Location> startPoints;
    std::vector<sector> sectorOriginals;
    for (int i = 0; i < sectorTests.size(); ++i)
    {
        int row = i % 5;
        int col = i / 5;
        std::string triangleIndex = "triangle: " + std::to_string(i);

        sectorOriginals.push_back(sectorOriginal);
        startPoints.push_back(psl::Location(DELTA1*row, DELTA1 * col, 0));
        MoveSector(sectorOriginals[i], startPoints[i], DELTA1/ 1000.0);
        MoveSector(sectorTests[i], startPoints[i], DELTA1/ 1000.0);

        std::vector<Point> sectorExtend;
        ExtendSector(sectorOriginals[i], sectorExtend, LINE_LENGTH);

        DrawSector(image, sectorExtend, cv::Scalar(255, 255, 255), true);
        DrawSector(image, sectorTests[i], cv::Scalar(50, 250, 50), true);
        Timer timeCost;

        if (timeFlag)
        {
            GetTriangleAreaTimeFunc(sectorOriginals[i], sectorTests[i], area, crossPoints, LINE_LENGTH);
        }
        else
        {
            GetTriangleArea(sectorOriginals[i], sectorTests[i], area, crossPoints, LINE_LENGTH);

        }

        TriangleAreaTime += timeCost.Timing(triangleIndex, true);

        DrawCrossPoint(crossPoints, image);
        DrawCenterPoint(image, sectorTests[i][0], -1);
        DrawCenterPoint(image, sectorOriginals[i][0], -1);

        psl::Location areaShow;
        areaShow.x =  sectorOriginals[i][0].x + 50;
        areaShow.y =  sectorOriginals[i][0].y + 50;
        DrawCenterPoint(image, areaShow, area, cv::Scalar(0,0,LINE_LENGTH));
        crossPoints.clear();
    }
    cv::resize(image, image, cv::Size(int(W * 0.8) , int(H * 0.8)));

    std::cout<<"Triangles' numbers:  "<<sectorTests.size()<<std::endl;
    std::cout<<"Triangles' area cal total time: "<<TriangleAreaTime<<std::endl;
    std::cout<<"Average triangles' area time: "<<TriangleAreaTime / sectorTests.size()<<std::endl;

    cv::imshow("GetCrossPointTestMulti",image);
    cv::waitKey(0);
}

void TestOneCrossTriangle(const bool timeFlag = false)
{
    const int DELTA = 50, ADD = 1600;

    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    cv::Mat image = cv::Mat::zeros(H, W, CV_8UC3);

    DrawGrid(image, DELTA, ADD, cv::Scalar(100, 100, 100));
    sector sectorOriginal;
    sector sectorTest;

    sector crossPoints;
    float area = 0;
    sectorOriginal.push_back(psl::Location(500,500,0));
    sectorOriginal.push_back(psl::Location(100,100,0));
    sectorOriginal.push_back(psl::Location(900,100,0));
    sectorTest.push_back(psl::Location(50,50,0));
    sectorTest.push_back(psl::Location(600,600,0));
    sectorTest.push_back(psl::Location(1000,0,0));

    sector temp;

    ExtendSector(sectorOriginal, temp, LINE_LENGTH);
    sectorOriginal.swap(temp);
    DrawSector(image, sectorOriginal, cv::Scalar(255, 255, 255), true);
    DrawSector(image, sectorTest, cv::Scalar(50, 250, 50), true);

    if (timeFlag)
    {
        GetTriangleAreaTimeFunc(sectorOriginal, sectorTest, area, crossPoints, LINE_LENGTH);
    }
    else
    {
        GetTriangleArea(sectorOriginal, sectorTest, area, crossPoints, LINE_LENGTH);
    }

    DrawCrossPoint(crossPoints, image);
    DrawCenterPoint(image, sectorOriginal[0], area, cv::Scalar(0,0,LINE_LENGTH));
    cv::imshow("GetCrossPointTestMulti1",image);
    cv::waitKey(0);
}

TEST(FunctorTest, InViewTest)
{
    InViewTest();
}

TEST(FunctorTest, TrangleCrossTest)
{
    TestOneCrossTriangle();
    TestTriangle();
}

TEST(FunctorTest, TrangleCrossTestTime)
{
    TestOneCrossTriangle(true);
    TestTriangle(true);
}

TEST(FunctorTest, SectorCrossTest)
{
    TestAllCross();
}
