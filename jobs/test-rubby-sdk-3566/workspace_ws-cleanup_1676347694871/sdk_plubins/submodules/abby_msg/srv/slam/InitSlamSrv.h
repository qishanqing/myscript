/*
 * StartSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <vector>
#include <mind_os/serialization/serialization.h>
namespace slam_srvs {

struct InitSlamSrv
{

    struct Request
    {
        std::uint64_t timestamp;
        std::string log_path;
        std::string calibration_config_path;

    } request;

    struct Response
    {
        bool success;
    } response;


};

} //namespace slam_srvs 

namespace  mind_os
{
    template<>
    inline void serialize(const slam_srvs::InitSlamSrv::Request& data, BytesBuffer& buffer)
    {
        serialize(data.timestamp, buffer);
        serialize(data.log_path, buffer);
        serialize(data.calibration_config_path, buffer);
    }
    
    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::InitSlamSrv::Request& data)
    {
        deserialize(buffer, data.timestamp);
        deserialize(buffer, data.log_path);
        deserialize(buffer, data.calibration_config_path);
    }

    template<>
    inline void serialize(const slam_srvs::InitSlamSrv::Response& data, BytesBuffer& buffer)
    {
        serialize(data.success, buffer);
    }

    template<>
    inline void deserialize(BytesBuffer& buffer, slam_srvs::InitSlamSrv::Response& data)
    {
        deserialize(buffer, data.success);
    }
}
