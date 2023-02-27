//
// Created by lcy on 21-6-10.
//

#ifndef ROBOT_MATLAB_INDEMINDMAP_H
#define ROBOT_MATLAB_INDEMINDMAP_H

#include <iostream>
#include <config.h>
#include "Common.h"
#include "RWlock.h"
#include "RegionSegmentation.h"
#include "PointWithArea.h"

#if 1
const double EPSILON = 0.000001;
struct Vector2d
{
public:
    Vector2d() = default;

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

    double x = 0.0, y = 0.0;
};
class PlanningMapManager:public RegionSegmentation,PointWithArea{
public:
    PlanningMapManager(){
        range_removedepth = s_config.readParam<int>("depthIgnoreSize");
    };
    ~PlanningMapManager() = default;;

    void cleanagain();

    int setexplormap(Mat &src , Mat & dst);

    void map_dilate(Mat & src , uint8_t type , int num);//扩张
    void map_erode(Mat & src , uint8_t type , int num);//收缩
    void set_FollowMap(Point2f p);

    void fill_pathmap(Point p1 , Point p2);
    void fill_tof(SensorInfo & dataup);
    void fill_appmap(SensorInfo & dataup);
    void fill_appmap_pose(SensorInfo & dataup);
    void fill_appmap_cutpose(SensorInfo & dataup);
    void fill_appmap_weight(SensorInfo & dataup);
	void set_range_removedepth(int range);
    void fill_appmap_source(SensorInfo & dataup , DataDown * datadown);

    bool fill_perception_delimit(const PerceptionDelimit & info);
    bool fill_perception_obs(const perception & info , Point p_cur);

    void fill_limit(Lines & lines);//划分区线
    void fill_opened(Rectangles & rectangles);//划分区区域
    void fill_forbiden(Lines & lines , Rectangles & rectangles);//划虚拟墙 划禁区

    void fill_cover(Point p , BoxInfo box , int num);
    void fill_cover_uncover(Point p , BoxInfo box , int num);
    void fill_cover_delimit(Point p , const Mat & mark , BoxInfo box , int num);
    void fill_cover_delimit_uncover(Point p , const Mat & mark , BoxInfo box , int num);
    void fill_cover_rectangle(Point p , BoxInfo box , int type);
    void fill_cover_delimit_rectangle(Point p , const Mat & mark , BoxInfo box , int type);
    void fill_sensorobs(Point2f p);
    void fill_cliff(Point2f p , float dir , int flg);
    void fill_ir(PoseStamped p , float dir , int flg , float l);
    void fill_navobs(Point2f p);

    void fill_explor_map(Mat & src , Point p , int num , uint8_t setcell);

    bool raster_Line(Point p_s , Point p_e , vector<Point> &list);
    void RasterLine(std::pair<Vector2d, Vector2d> line, std::vector<Vector2d>& linePointList);

    bool RasterLine_0(Point start , Point end , vector<Point>& linePointList);
    bool flg_firstfill = true;
private:
    int radius_obs = 4;
	int range_removedepth = 0;

};
#endif
#endif //ROBOT_MATLAB_INDEMINDMAP_H
