#pragma once

#include <cstdint>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

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