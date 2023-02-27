/*
 * StartNavSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once

#include <cstdint>

namespace still_srvs {

struct GetStillStateSrv
{
    GetStillStateSrv() {}

    struct Request
    {
        std::uint64_t timestamp;

    } request;

    struct Response
    {
        std::uint64_t finalStateTime;
        bool isStill;
    } response;

};

} //namespace still_srvs