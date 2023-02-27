/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include "SceneData.h"

namespace map_srvs {

struct SceneDataQuery
{
    SceneDataQuery() = default;

    struct Request
    {
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        SceneData data;
        bool success;
    } response;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::SceneDataQuery::Request& data, BytesBuffer& buffer) {
//     buffer << data.sceneId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDataQuery::Request& data) {
//     buffer >> data.sceneId;
// };

// template<>
// inline void serialize(const map_srvs::SceneDataQuery::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
//     serialize(data.data, buffer);
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDataQuery::Response& data) {
//     buffer >> data.success;
//     deserialize(buffer, data.data);
// };

// } /* namespace mind_os */

