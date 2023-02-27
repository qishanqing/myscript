/*
 * StartSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <vector>
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
