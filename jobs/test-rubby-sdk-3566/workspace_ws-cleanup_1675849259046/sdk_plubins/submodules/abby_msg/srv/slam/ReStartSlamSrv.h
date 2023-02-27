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

struct ReStartSlamSrv
{
    struct Request
    {
        std::string map_path;
        std::uint64_t sceneId;
        std::uint64_t mapId;
        std::uint64_t timestamp;
    } request;

    struct Response
    {
        bool success;
    } response;

};

} //namespace slam_srvs 