
/*
 * RotateSlamPose.h
 */

#pragma once

#include "../../msg/slam/SlamResult.h"
#include <mind_os/serialization/serialization.h>

namespace slam_srvs
{
struct RotateSlamPoseSrv
{
    struct Request
    {
        std::uint64_t timestamp;
	//Two =[q, position],机器人在世界系下的位姿, q表示旋转时其||q||=1
	float q[4]; //w, x, y, z
	float position[3]; // x, y,z
    } request;
    struct Response
    {
	float q[4]; //w, x, y, z
	float position[3]; // x, y,z
        bool success;
    } response;
};

}//namespace slam_srvs

namespace mind_os
{
    template<>
    inline void serialize(const slam_srvs::RotateSlamPoseSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
	serialize(data.q, 4, buffer);
	serialize(data.position, 3, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::RotateSlamPoseSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
	deserialize(buffer, data.q, 4);
	deserialize(buffer, data.position, 3);
    }

    template<>
    inline void serialize(const slam_srvs::RotateSlamPoseSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.q, 4, buffer);
	serialize(data.position, 3, buffer);
	serialize(data.success, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::RotateSlamPoseSrv::Response& data)
    {
        deserialize(buffer, data.q, 4);
	deserialize(buffer, data.position, 3);
	deserialize(buffer, data.success);
    }
}
