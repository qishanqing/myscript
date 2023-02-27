//
// Created by donglijian on 11/23/21.
//
#include <gtest/gtest.h>
#include "src/utils/data_type.h"
#include "src/map/escalator_map.h"
//#include "src/map/escalator.h"

#define CENTER_POINT(a,b) {(a.x+b.x)/2, (a.y+b.y)/2, (a.z + b.z)/2}

const int w = 1000, h = 1000;
const int scale = 2; // the scale ground base on w and h
const int delta = 50;
int add = 10;
char message[200];
int range = w * scale / 100;
const int base = range;
const int step = delta; //100 / scale;


double randXY()
{
    return rand()%201;
}

void Convert(psl::Location &point, const float &base, const float step)
{
    point.x = (point.x + base) * step;
    point.y = (point.y + base) * step;
}

void TestDrawMap(const std::vector<psl::Location> srcLine
        , const std::vector<psl::Location> dstLine
        , const std::vector<psl::Location> &srcPoint
        , const std::vector<psl::Location> &dstPoint, cv::Mat &image)
{
    cv::Scalar colorBack(100, 100, 100);
    cv::Scalar colorAxis(0, 0, 200);
    cv::Scalar colorObject(50, 250, 50);

    // draw grid [-10, 10]
    for (int i = -range; i <= range; ++i)
    {
        int x = i + base;
        cv::line(image, cv::Point(int(x * step), 0)
                , cv::Point(int(x * step), h)
                , colorBack, 1);

        cv::line(image, cv::Point(0, int(x * step))
                , cv::Point(w, int(x * step))
                , colorBack, 1);

        cv::putText(image, std::to_string(i)
                , cv::Point(10, MAX(20, int(x *step)))
                , cv::FONT_HERSHEY_PLAIN, 1, colorBack);
        cv::putText(image, std::to_string(i), cv::Point(
                            MAX(20, int(x *step - 20)), w - 10)
                , cv::FONT_HERSHEY_PLAIN, 1, colorBack);
    }

    cv::line(image, cv::Point(int(srcLine[0].x), int(srcLine[0].y))
            , cv::Point(int(srcLine[1].x), int(srcLine[1].y))
            , colorAxis, 1);

    cv::putText(image, "srcLine"
                , cv::Point(int(srcLine[0].x + srcLine[1].x)/2, int(srcLine[0].y + srcLine[1].y)/2)
                , cv::FONT_HERSHEY_PLAIN, 1, colorAxis);

    for (auto point: srcPoint)
    {
        cv::circle(image, cv::Point(int(point.x), int(point.y)), 1
                , colorAxis, -1);
    }

    cv::line(image, cv::Point(int(dstLine[0].x), int(dstLine[0].y))
            , cv::Point(int(dstLine[1].x), int(dstLine[1].y))
            , colorObject, 1);

    cv::putText(image, "dstLine"
            , cv::Point(int(dstLine[0].x + dstLine[1].x)/2, int(dstLine[0].y + dstLine[1].y)/2)
            , cv::FONT_HERSHEY_PLAIN, 1, colorObject);

    for (auto point:dstPoint)
    {
        cv::circle(image, cv::Point(int(point.x), int(point.y)), 1
                   , colorObject, -1);
    }

    cv::imshow("map",image);
    cv::waitKey(0);
}

void GenerateEscalator(std::vector<psl::Location> &srcLine
                       , std::vector<psl::Location> &dstLine
                       , std::vector<psl::Location> &points)
{
    psl::Location p1, p2, p3, p4;
    p1 = {(-(randXY()/10.0)), (-(randXY()/10.0)), 0};
    p2 = {(-(randXY()/10.0)), (-(randXY()/10.0)), 0};
    double length = p1.Distance(p2);
    while (true)
    {
        p3 = {(-(randXY()/10.0)), (-(randXY()/10.0)), 0};
        double x = (-(randXY()/10.0));
        double y = sqrt(pow(length, 2) - pow(p3.x - x, 2));
        if (randXY() > 100)
        {
            y = y + p3.y;
        }
        else
        {
            y = p3.y - y;
        }
        if(y < 0 and y > -20)
        {
            p4.x = x;
            p4.y = y;
            break;
        }
    }

    while (points.size() < 4)
    {
        double x = (randXY() / 100.0) - 1;
        double y = randXY() / 100.0 - 1;
        if(MIN(abs(x),abs(y)) < 0.05)
            continue;

        if(randXY() > 100)
        {
            x += p1.x;
            y += p1.y;
        }
        else
        {
            x += p2.x;
            y += p2.y;
        }
        if (x > 0 or x < -20 or y > 0 or y < -20)
            continue;
        points.push_back({x, y, 0});
    }
    srcLine.push_back(p1);
    srcLine.push_back(p2);
    dstLine.push_back(p3);
    dstLine.push_back(p4);
}

void ModifyHandrial()
{
    std::vector<psl::Location> srcLine;
    std::vector<psl::Location> dstLine;
    std::vector<psl::Location> points;
    GenerateEscalator(srcLine, dstLine, points);

    EscalatorMap escalatorMap;
    escalator_map::Escalator escalator;
    DLObject newObject{};
    DLObject originalObject{};
    std::list<DLObject> origitnalDLObject;
    origitnalDLObject.push_back(originalObject);
    escalator.escalator = origitnalDLObject.begin();

    newObject.locationWorld.swap(dstLine);
    escalator.escalator->locationWorld.swap(srcLine);

    for (auto point:points)
    {
        escalator.handrails.emplace_back(point, 1);
    }

    float angle;
    auto &originObject = escalator.escalator;
    psl::Location src = originObject->locationWorld[1] - originObject->locationWorld[0];
    psl::Location dst = newObject.locationWorld[1] - newObject.locationWorld[0];

    escalatorMap.RotationAngle(src, dst, angle);

    Location startCenter;
    Location endCenter;
    startCenter.world = CENTER_POINT(originObject->locationWorld[0]
            ,originObject->locationWorld[1]);
    endCenter.world = CENTER_POINT(newObject.locationWorld[0]
            ,newObject.locationWorld[1]);

    Convert(originObject->locationWorld[0], base, step);
    Convert(originObject->locationWorld[1], base, step);
    Convert(newObject.locationWorld[0], base, step);
    Convert(newObject.locationWorld[1], base, step);

    cv::Mat image = cv::Mat::zeros(w, h, CV_8UC3);

    std::vector<psl::Location> srcPoint;
    std::vector<psl::Location> dstPoint;

    for (auto &point : escalator.handrails)
    {
        psl::Location pointOriginal = point.location;
        escalatorMap.UpdatePoint(startCenter, endCenter, angle, point.location);
        Convert(pointOriginal, base, step);
        Convert(point.location, base, step);
        srcPoint.push_back(pointOriginal);
        dstPoint.push_back(point.location);
    }
    TestDrawMap(originObject->locationWorld, newObject.locationWorld
                , srcPoint, dstPoint, image);
}

// case test
TEST(FunctorTest, Modify)
{
    srand((unsigned) time(NULL));
    for (int i = 0; i < 10; ++i)
    ModifyHandrial();
}