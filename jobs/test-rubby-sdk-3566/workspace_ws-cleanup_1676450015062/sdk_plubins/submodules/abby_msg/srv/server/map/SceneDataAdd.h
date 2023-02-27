/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include "SceneData.h"

namespace map_srvs {

struct SceneDataAdd
{
    SceneDataAdd() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        bool success;
        SceneData data;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::SceneDataAdd::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.timestamp);
    serialize(buffer,data.sceneId);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDataAdd::Request& data) {
    deserialize(buffer,data.timestamp);
    deserialize(buffer,data.sceneId);
};

template<>
inline void serialize(const map_srvs::SceneDataAdd::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.success);
    serialize(buffer,data.data);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDataAdd::Response& data) {
    deserialize(buffer, data.success);
    deserialize(buffer, data.data);
};

} /* namespace mind_os */
