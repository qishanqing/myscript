#pragma once

#include <cstdint>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

namespace nav_msgs
{

	struct MapInfo
	{
		int width;			//宽
		int height;			//高
		float resolving;	//分辨率
		cv::Point2f origin; //  原点(世界坐标系)
		cv::Mat map;		//地图
		int Id;   // 地图类别ID号
	};

	struct HistoryInfo
	{
		std::uint64_t TimeStamp;
		std::vector<MapInfo> Map; // 地图
	};
}