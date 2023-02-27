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

struct CostmapGetId
{
    CostmapGetId() = default;

    struct Request
    {
        std::uint64_t mapId;
        std::uint64_t sceneId;
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
// inline void serialize(const map_srvs::CostmapGetId::Request& data, BytesBuffer& buffer) {
//     buffer << data.sceneId;
//     buffer << data.mapId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::CostmapGetId::Request& data) {
//     buffer >> data.sceneId;
//     buffer >> data.mapId;
// };

// template<>
// inline void serialize(const map_srvs::CostmapGetId::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
//     serialize(data.grid, buffer);
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::CostmapGetId::Response& data) {
//     buffer >> data.success;
//     deserialize(buffer, data.grid);
// };

// } /* namespace mind_os */


