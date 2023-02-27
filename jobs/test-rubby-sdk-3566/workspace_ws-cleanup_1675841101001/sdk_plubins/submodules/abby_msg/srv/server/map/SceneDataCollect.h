/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

// #include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct SceneDataCollect
{
    SceneDataCollect() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        bool success;
    } response;
};
}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::SceneDataCollect::Request& data, BytesBuffer& buffer) {
//     buffer << data.timestamp;
//     buffer << data.sceneId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDataCollect::Request& data) {
//     buffer >> data.timestamp;
//     buffer >> data.sceneId;
// };

// template<>
// inline void serialize(const map_srvs::SceneDataCollect::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDataCollect::Response& data) {
//     buffer >> data.success;
// };

// } /* namespace mind_os */
