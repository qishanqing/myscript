//
// Created by lcy on 21-6-15.
//

#ifndef ROBOT_MATLAB_ASTAR_H
#define ROBOT_MATLAB_ASTAR_H


#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <algorithm>

#include "Common.h"
#include "aStar.h"

using namespace std;
using namespace cv;

#define POT_HIGH 1.0e10        // unassigned cell potential

namespace global_planner {
    class Index {
    public:
        Index(int a, float b) {
            i = a;
            cost = b;
        }

        int i;
        float cost;
    };

    struct greater1 {
        bool operator()(const Index &a, const Index &b) const {
            return a.cost > b.cost;
        }
    };
    const double EPSILON = 0.000001;
    struct Vector2d
    {
    public:
        Vector2d() = default;;

        Vector2d(double dx, double dy)
        {
            x = dx;
            y = dy;
        }

        // 矢量赋值
        void set(double dx, double dy)
        {
            x = dx;
            y = dy;
        }

        // 矢量相加
        Vector2d operator + (const Vector2d& v) const
        {
            return Vector2d(x + v.x, y + v.y);
        }

        // 矢量相减
        Vector2d operator - (const Vector2d& v) const
        {
            return Vector2d(x - v.x, y - v.y);
        }

        //矢量数乘
        Vector2d Scalar(double c) const
        {
            return Vector2d(c*x, c*y);
        }

        // 矢量点积
        double Dot(const Vector2d& v) const
        {
            return x * v.x + y * v.y;
        }

        //向量的模
        double Mod() const
        {
            return sqrt(x * x + y * y);
        }

        bool Equel(const Vector2d& v) const
        {
            return abs(x - v.x) < EPSILON && abs(y - v.y) < EPSILON;
        }

        double x{}, y{};
    };


    class NavPath:public aStar{
    public:
        NavPath() :
                lethal_cost_(250), neutral_cost_(50), factor_(3.0) {
        }
        virtual ~NavPath() = default;

        int add_cost(Mat &src);

        bool connect_p2p(Mat & map , Point start , Point target);
        bool nav(Mat & map , Point start , Point target , std::vector<Point>& plan);
        bool selectpose(Mat & src , std::vector<Point>& path);
        bool raster_Line(Point p_s , Point p_e , vector<Point> &list);

        bool calculatePotentials(unsigned char* costs, Point start, Point end, int cycles, float* potential);

        bool getPath(float* potential, Point start, Point end, std::vector<std::pair<int, int> >& path);

        void setSize(int nx, int ny) {
            nx_ = nx;
            ny_ = ny;
            ns_ = nx * ny;
        }

    protected:
        inline int toIndex(int x, int y) {
            return x + nx_ * y;
        }
        inline int getIndex(int x, int y) {
            return x + y * nx_;
        }

        int nx_{}, ny_{}, ns_{};
        unsigned char lethal_cost_, neutral_cost_;
        int cells_visited_{};
        float factor_;

    private:
        int addcost = 4;

        void add(unsigned char *costs, float *potential, float prev_potential, int next_i, int end_x, int end_y);

        std::vector<Index> queue_;

        float calculatePotential(float *potential, unsigned char cost, int n, float prev_potential);
        void outlineMap(unsigned char* costarr, int nx, int ny, unsigned char value);
        void RasterLine(std::pair<Vector2d, Vector2d> line, std::vector<Vector2d>& linePointList);
        void RasterLine_0(Point start , Point end , vector<Point>& linePointList);
        float* potential_array_{};
    };
}
#endif //ROBOT_MATLAB_ASTAR_H
