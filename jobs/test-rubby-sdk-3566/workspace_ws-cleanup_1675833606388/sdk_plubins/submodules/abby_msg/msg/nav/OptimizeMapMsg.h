#pragma once

#include <cstdint>
#include <opencv2/highgui/highgui.hpp>

namespace nav_msgs {

struct OptimizeMapMsg
    {
		std::uint64_t TimeStamp;

		int width;//宽
		int height;//高
		float resolving;//分辨率
		cv::Point2f origin;// 原点
		cv::Mat map;//地图
		cv::Point2f origin_real;  //世界坐标系下最大包络小端角点
		int width_real;   // 世界坐标系下最大包络(宽栅格数)
		int height_real;  // 世界坐标系下最大包络(高栅格数)

		enum Cell
		{	
			Free = 0,   /*空闲*/
			Obs = 100,  /*障碍物*/
			Unknow = 255   /*未知*/
		};

		cv::Point2f MapToWorld(cv::Point m)   /*栅格坐标转世界坐标*/
		{
			return cv::Point2f((origin.x + (m.x + 0.5f) * resolving),(origin.y + (m.y + 0.5f) * resolving));
		}
		cv::Point WorldToMap(cv::Point2f w)   /*世界坐标转栅格坐标*/
		{
			return cv::Point((int)((w.x - origin.x) / resolving),(int)((w.y - origin.y) / resolving));
		}
    };

}