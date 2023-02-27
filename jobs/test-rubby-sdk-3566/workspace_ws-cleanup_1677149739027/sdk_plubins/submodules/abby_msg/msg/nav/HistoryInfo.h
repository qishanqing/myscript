#pragma once

#include <cstdint>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include <mind_os/serialization/serialization.h>
#include "../common/base/CvMat.h"
#include "../common/base/CvPoint2f.h"
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

namespace mind_os
{
	template<>
    inline void serialize(const nav_msgs::MapInfo& data, BytesBuffer& buffer)
    {
        serialize(data.width, buffer);
		serialize(data.height, buffer);
		serialize(data.resolving, buffer);
		serialize(buffer, data.origin);
		serialize(data.map, buffer);
		serialize(data.Id, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_msgs::MapInfo& data)
    {
        deserialize(buffer, data.width);
		deserialize(buffer, data.height);
		deserialize(buffer, data.resolving);
		deserialize(buffer, data.origin);
		deserialize(buffer, data.map);
		deserialize(buffer, data.Id);
    }

	template<>
    inline void serialize(const nav_msgs::HistoryInfo& data, BytesBuffer& buffer)
    {
        serialize(data.TimeStamp, buffer);
		serialize(data.Map, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_msgs::HistoryInfo& data)
    {
        deserialize(buffer, data.TimeStamp);
		deserialize(buffer, data.Map);
    }
}