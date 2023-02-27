#pragma once

#include <cstdint>
#include <opencv2/highgui/highgui.hpp>

#include <mind_os/serialization/serialization.h>
#include "../common/base/CvMat.h"
#include "../common/base/CvPoint2f.h"
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

namespace mind_os
{
	template <>
	inline void serialize(const nav_msgs::OptimizeMapMsg& data, BytesBuffer& buffer){
		serialize(data.TimeStamp, buffer);
		serialize(data.width, buffer);
		serialize(data.height, buffer);
		serialize(data.resolving, buffer);
		serialize(data.origin, buffer);
		serialize(data.map, buffer);
		serialize(data.origin_real, buffer);
		serialize(data.width_real, buffer);
		serialize(data.height_real, buffer);
	}

	template <>
	inline void deserialize(BytesBuffer& buffer, nav_msgs::OptimizeMapMsg& data)
	{
		deserialize(buffer, data.TimeStamp);
		deserialize(buffer, data.width);
		deserialize(buffer, data.height);
		deserialize(buffer, data.resolving);
		deserialize(buffer, data.origin);
		deserialize(buffer, data.map);
		deserialize(buffer, data.origin_real);
		deserialize(buffer, data.width_real);
		deserialize(buffer, data.height_real);
	}
}