/*
 * StopSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <vector>

namespace slam_srvs {

struct StopSlamSrv
{
    struct Request
    {
       std::uint64_t timestamp;
    } request;

    struct Response
    {
        bool success;
    } response;


};

}// namespace slam_srvs 