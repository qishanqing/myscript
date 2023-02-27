/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include "Scene.h"

namespace map_srvs {

struct SceneQuery
{
    SceneQuery() = default;

    struct Request
    {
        std::uint64_t time;
    } request;

    struct Response
    {
        std::vector<Scene> scenes;
        bool success;
    } response;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::SceneQuery::Request& data, BytesBuffer& buffer) {
//     buffer << data.sceneId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneQuery::Request& data) {
//     buffer >> data.sceneId;
// };

// template<>
// inline void serialize(const map_srvs::SceneQuery::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
//     serialize(data.scene, buffer);
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneQuery::Response& data) {
//     buffer >> data.success;
//     deserialize(buffer, data.scene);
// };

// } /* namespace mind_os */
