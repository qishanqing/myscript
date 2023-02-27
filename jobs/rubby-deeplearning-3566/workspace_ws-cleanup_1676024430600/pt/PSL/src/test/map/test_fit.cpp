//////////////////////////////////////////////////////////////////////
///  @file     test_instance.cpp
///  @brief    unit test for InstanceManager
///  Details.
///  @author   sunhao
///  @date     2021.07.26
///
///  revision statement:
//////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include "sample/utils/utils.h"
#include "types.h"
#include "src/test/utils.h"
#include "src/instance/manager.h"

#define private public
#define protected public

#include "src/map/escalator.h"
#include "src/utils/define.h"

//#define RAND_FLOAT rand() / double(RAND_MAX)
#define RAND_FLOAT(a, b) rand() / float(RAND_MAX) * ((b) - (a)) + (a)

extern InputParam inputParam;

const int DELTA = 50, ADD = 10;
const int W = 1000, H = 1000;
const cv::Scalar COLOR_BACK(100, 100, 100);
const psl::Location addLocation(ADD, ADD, ADD);

void DrawParts(const escalator_map::Escalator &escalator, const cv::Mat &image)
{
    const size_t size = escalator.handrails.size();

    // draw line [-10, 10]
    cv::Scalar color(255, 100, 100);
    cv::Scalar colorR(100, 255, 100);

    for (size_t i = 0; i < size; ++i)
    {
        auto &point = escalator.handrails.at(i);
        // draw view
        auto newPoint = point.location + addLocation;
        if (point.useful)
        {
            cv::circle(image, cv::Point(int(newPoint.x * DELTA), int(newPoint.y * DELTA))
                       , 3, color, 2);
        }
        else
        {
            cv::circle(image, cv::Point(int(newPoint.x * DELTA), int(newPoint.y * DELTA))
                       , 3, colorR, 2);
        }
    }
}

int Int(float a)
{
    return (a + ADD) * DELTA;
}

// TODO : const image
void DrawLine(const escalator_map::Escalator &escalator, const cv::Mat &image
              , const cv::Scalar &color, const std::string message)
{
    const float left = -10, right = 10;
    float y4Left = (left - escalator.b) / escalator.k;
    float y4Right = (right - escalator.b) / escalator.k;
    float x4zero = left * escalator.k + escalator.b;

    if (escalator_map::X == escalator.axis)
    {
        y4Left = left * escalator.k + escalator.b;
        y4Right = right * escalator.k + escalator.b;
        x4zero = (left - escalator.b) / escalator.k;
    }

    cv::line(image, cv::Point(Int(left), Int(y4Left))
             , cv::Point(Int(right), Int(y4Right))
             , color, 1);
    cv::putText(image, message, cv::Point(Int(x4zero) + 20, 30)
                , cv::FONT_HERSHEY_PLAIN, 1, color);
}

void DrawEdge(const escalator_map::Escalator &escalator, const cv::Mat &image)
{

}

void DrawGrid(const cv::Mat &image)
{
    for (int i = -ADD; i <= ADD; ++i)
    {
        int x = i + ADD;
        cv::line(image, cv::Point(int(x * DELTA), 0)
                 , cv::Point(int(x * DELTA), H)
                 , COLOR_BACK, 1);

        cv::line(image, cv::Point(0, int(x * DELTA))
                 , cv::Point(W, int(x * DELTA))
                 , COLOR_BACK, 1);

        cv::putText(image, std::to_string(i * 100), cv::Point(10, MAX(20
                                                                      , int(x *DELTA)))
                    , cv::FONT_HERSHEY_PLAIN, 1, COLOR_BACK);
        cv::putText(image, std::to_string(i * 100), cv::Point(
                MAX(20, int(x *DELTA - 20))
                , W - 10)
                    , cv::FONT_HERSHEY_PLAIN, 1, COLOR_BACK);

    }
}

void Show(escalator_map::Escalator &escalator)
{
    cv::Mat image = cv::Mat::zeros(W, H, CV_8UC3);
    DrawGrid(image);
    cv::putText(image, "size: " + std::to_string(escalator.handrails.size()), cv::Point(
            400, 50), cv::FONT_HERSHEY_PLAIN, 2.5, cv::Scalar(255, 255, 255));

    escalator.Fit();
    DrawLine(escalator, image, cv::Scalar(100, 255, 100), "y->x");
    DrawEdge(escalator, image);
    DrawParts(escalator, image);

    // reset
    for (auto &h : escalator.handrails)
    {
        h.update = false;
        h.useful = false;
    }
    escalator.axis = escalator_map::Y;
    escalator.Fit();
    DrawLine(escalator, image, cv::Scalar(100, 100, 255), "x->y");
    DrawEdge(escalator, image);

    cv::imshow("fit", image);
    cv::waitKey(0);
}

void FitCheck()
{
    // case 1
//    const int row = 5, col = 3;
//    float array[row][col] = {0.19, 5.22, 0
//                             , -1.49, 2.05, 1
//                             , -0.78, 2.08, 2
//                             , -1.49, 2.05, 1
//                             , -0.78, 2.08, 2};

    // case2
//    const int row = 16, col = 3;
//    float array[row][col] = {0.02, 5.22, 0
//                             , -1.46, 2.21, 1
//                             , -0.05, 5.24, 0
//                             , -1.47, 2.09, 1
//                             , -1.46, 2.08, 1
//                             , -1.45, 2.07, 1
//                             , -0.78, 2.08, 2
//                             , -1.49, 2.05, 1
//                             , -0.78, 2.08, 2
//                             , -1.49, 2.05, 1
//                             , -0.75, 2.08, 2
//                             , -1.49, 2.05, 1
//                             , -0.76, 2.05, 2
//                             , -0.76, 2.04, 2
//                             , -0.76, 2.05, 2
//                             , -0.76, 2.05, 2};

    // case3
//    const int row = 9, col = 3;
//    float array[row][col] = {0.09, 5.23, 0
//                             , -1.41, 2.13, 1
//                             , -0.72, 2.03, 2
//                             , -1.4, 2.12, 1
//                             , -0.73, 2.04, 2
//                             , -1.43, 2.05, 1
//                             , -0.71, 2.03, 2
//                             , -1.46, 2.07, 1
//                             , -0.75, 2.06, 2};

    // case4 noise
//    const int row = 15, col = 3;
//    float array[row][col] = {-2.72, 2.22, 0
//                             , -2.32, 2.48, 1
//                             , -2.37, 2.49, 1
//                             , -2.31, 2.49, 2
//                             , -2.31, 2.46, 2
//                             , -2.34, 2.53, 2
//                             , -2.31, 2.49, 2
//                             , -2.30, 2.52, 2
//                             , -2.76, 2.21, 0
//                             , -2.87, 2.14, 0
//                             , -2.72, 2.21, 0
//                             , -2.68, 2.22, 0
//                             , -2.68, 2.22, 0
//                             , -2.74, 2.19, 0
//                             , -3.50, 3.62, 3};

// case5: not in one line, a noise point
//    const int row = 15, col = 3;
//    float array[row][col] = {-5.02, 0.37, 0
//                             , -5.02, 0.37, 0
//                             , -5.02, 0.37, 0
//                             , -5.01, 0.37, 0
//                             , -5.03, 0.77, 1
//                             , -5.01, 0.37, 0
//                             , -5.03, 0.77, 1
//                             , -5.00, 0.37, 0
//                             , -5.03, 0.48, 1
//                             , -5.01, 0.37, 0
//                             , -5.03, 0.48, 1
//                             , -5.01, 0.51, 1
//                             , -5.03, 0.48, 1
//                             , -5.03, 0.77, 1
//                             , -7.08, -0.12, 2};

// case6: not in one line, a noise point
//    const int row = 13, col = 3;
//    float array[row][col] = {-1.40, 1.96, 1
//                             , -1.59, 1.95, 0
//                             , -1.45, 1.97, 1
//                             , -1.47, 1.96, 0
//                             , -1.47, 1.97, 1
//                             , -1.45, 1.98, 0
//                             , -1.46, 1.98, 0
//                             , -1.46, 1.98, 1
//                             , -1.23, 1.97, 0
//                             , -1.59, 1.95, 1
//                             , -1.23, 1.97, 1
//                             , -2.27, 4.72, 2
//                             , -1.60, 2.46, 4};

//    // case7: less point
//    const int row = 2, col = 3;
//    float array[row][col] = {-4.13, 8.46, 1
//                             , -3.28, 8.56, 2};

//    // case8:
//    const int row = 14, col = 3;
//    float array[row][col] = {-0.53, 8.37, 22
//                             , -0.37, 8.43, 24
//                             ,-0.36, 8.35, 24
//                             ,-0.32,8.36,24
//                             ,-1.31,8.41,31
//                             ,-0.31,8.37,24
//                             ,-1.31,8.38,31
//                             ,-1.31,8.38,31
//                             ,-1.38,8.37,31
//                             ,-1.35,8.44,31
//                             ,-1.34,8.37,31
//                             ,-1.34,8.36,31
//                             ,-0.34, 8.39,24
//                             ,-0.40,8.33,24};

//    // case9:
//    const int row = 36, col = 3;
//    float array[row][col] = {-4.86,8.04,58
//                             ,-3.89,8.11,34
//                             ,-3.90,8.09,34
//                             ,-3.91,8.10,34
//                             ,-3.91,8.09,34
//                             ,-3.91,8.09,34
//                             ,-3.90,8.09,34
//                             ,-3.85,8.01,34
//                             ,-4.86,7.96,58
//                             ,-4.87,7.95,58
//                             ,-4.88,7.95,58
//                             ,-4.87,7.95,58
//                             ,-4.87,7.95,58
//                             ,-4.88,7.96,58
//                             ,-5.62,7.97,174
//                             ,-5.63,7.94,174
//                             ,-5.63,7.94,174
//                             ,-5.66,7.94,174
//                             ,-5.66,7.95,174
//                             ,-5.67,7.94,174
//                             ,-5.66,7.94,174
//                             ,-3.70,7.76,34
//                             ,-3.68,7.72,34
//                             ,-3.55,7.50,34
//                             ,-3.86,7.76,34
//                             ,-5.61,7.66,174
//                             ,-4.95,7.09,320
//                             ,-4.99,7.64,356
//                             ,-5.01,7.10,320
//                             ,-4.99,7.08,320
//                             ,-5.00,7.40,320
//                             ,-5.01,7.08,320
//                             ,-4.97,7.04,320
//                             ,-4.87, 8.58,368
//                             ,-5.15,9.53,370
//                             ,-5.38,9.53,370};

    // case9:
    const int row = 2, col = 3;
    float array[row][col] = {-3.93, 8.04, 59
                             , -4.01, 6.15, 58};

    escalator_map::Escalator escalator;
    std::list<DLObject> escalatorMap;
    DLObject escalatorOut;
    escalatorOut.locationWorld.push_back(psl::Location());
    escalatorMap.push_back(escalatorOut);
    escalator.escalator = escalatorMap.begin();

    for (int i = 0; i < row; ++i)
    {
        auto point = escalator_map::Point(psl::Location(array[i][0], array[i][1], 0)
                                          , array[i][2]);
        escalator.handrails.push_back(point);
    }

    for (int i = 0; i < 100; ++i)
    {
        Show(escalator);
        auto point1 = escalator_map::Point(
                psl::Location(array[2][0] + RAND_FLOAT(-0.2f, 0.2f),
                        array[2][1] + RAND_FLOAT(-0.3f, 0.3f), 0), array[2][2]);
        auto point2 = escalator_map::Point(
                psl::Location(array[1][0] + RAND_FLOAT(-0.2f, 0.2f),
                        array[1][1] + RAND_FLOAT(-0.3f, 0.3f), 0), array[1][2]);
        escalator.handrails.push_back(point1);
        escalator.handrails.push_back(point2);
        // reset
        for (auto &h : escalator.handrails)
        {
            h.update = false;
            h.useful = false;
        }
    }
}

// case test
TEST(Escalator, FitShow)
{
    FitCheck();
}