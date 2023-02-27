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

namespace mind_os {
template<>
inline void serialize(const map_srvs::SceneQuery::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.time);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneQuery::Request& data) {
    deserialize(buffer,data.time);
};

template<>
inline void serialize(const map_srvs::SceneQuery::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.scenes);
    serialize(buffer,data.success);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneQuery::Response& data) {
    deserialize(buffer,data.scenes);
    deserialize(buffer,data.success);
};

} /* namespace mind_os */
