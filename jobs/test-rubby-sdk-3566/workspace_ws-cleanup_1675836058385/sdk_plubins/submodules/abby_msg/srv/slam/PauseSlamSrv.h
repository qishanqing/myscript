/*
 * PauseSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>

namespace slam_srvs {

struct PauseSlamSrv
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

} // namespace slam_srvs