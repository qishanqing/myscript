/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include "../../../msg/slam/OccupancyGrid.h"
// #include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct CostmapGetUrl
{
    CostmapGetUrl() = default;

    struct Request
    {
        std::string url;
    } request;

    struct Response
    {
        bool success;
        slam_msgs::OccupancyGrid grid;
    } response;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::CostmapGetUrl::Request& data, BytesBuffer& buffer) {
//     buffer << data.url;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::CostmapGetUrl::Request& data) {
//     buffer >> data.url;
// };

// template<>
// inline void serialize(const map_srvs::CostmapGetUrl::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
//     serialize(data.grid, buffer);
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::CostmapGetUrl::Response& data) {
//     buffer >> data.success;
//     deserialize(buffer, data.grid);
// };

// } /* namespace mind_os */


