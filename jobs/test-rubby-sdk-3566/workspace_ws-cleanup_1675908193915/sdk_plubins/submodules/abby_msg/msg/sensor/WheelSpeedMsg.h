#pragma once

#include <cstddef>
#include <cstdint>

namespace sensor_msgs {

struct WheelSpeedMsg
{
	uint32_t frameId; // 帧号
	float leftSpeed;      // 左轮速度
	float rightSpeed;     // 右轮速度
	std::uint64_t timeStamp;
};
}
