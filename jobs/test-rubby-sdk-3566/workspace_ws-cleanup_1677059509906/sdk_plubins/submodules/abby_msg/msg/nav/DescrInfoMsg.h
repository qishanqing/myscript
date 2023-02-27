#pragma once

#include <cstdint>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include <mind_os/serialization/serialization.h>
#include "../common/base/CvMat.h"
namespace nav_msgs {

struct DescrInfo
    {
        std::uint64_t TimeStamp;
		std::vector< cv::Mat > map;  // 规划地图
		float posePile[3] = {1e6, 1e6, 1e6}; // x , y, yaw  充电桩位姿
		std::vector< std::vector<uint8_t> > forbiddenZone;  // 虚拟禁区
		std::vector< std::vector<uint8_t> > forbiddenLine;  // 虚拟墙
		std::vector< std::vector<uint8_t> > precincts ;  // 清扫划区
		bool isSweepingContinue = false;   // 是否断点续扫，默认false
    };

}

namespace mind_os
{
	template<>
	inline void serialize(const nav_msgs::DescrInfo& data, BytesBuffer& buffer)
	{
		serialize(data.TimeStamp, buffer);
		serialize(data.map, buffer);
		serialize(data.posePile, 3,  buffer);
		serialize(data.forbiddenZone, buffer);
		serialize(data.forbiddenLine, buffer);
		serialize(data.precincts, buffer);
		serialize(data.isSweepingContinue, buffer);
	}

    template<>
    inline void deserialize(BytesBuffer& buffer, nav_msgs::DescrInfo& data)
    {
        deserialize(buffer, data.TimeStamp);
		deserialize(buffer, data.map);
		deserialize(buffer, data.posePile, 3);
		deserialize(buffer, data.forbiddenZone);
		deserialize(buffer, data.forbiddenLine);
		deserialize(buffer, data.precincts);
		deserialize(buffer, data.isSweepingContinue);
    }	
}