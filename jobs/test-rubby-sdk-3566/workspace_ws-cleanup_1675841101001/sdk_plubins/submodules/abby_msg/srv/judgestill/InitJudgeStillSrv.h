/*
 * StartSlamSrv.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once
#include <cstdint>
#include <vector>
namespace still_srvs {

struct InitJudgeStillSrv
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

} //namespace still_srvs 
