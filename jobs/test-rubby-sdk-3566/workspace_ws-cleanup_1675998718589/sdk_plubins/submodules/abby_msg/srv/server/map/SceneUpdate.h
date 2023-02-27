/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include "Scene.h"

namespace map_srvs {

struct SceneUpdate
{
    SceneUpdate() = default;

    struct Request
    {
        Scene scene;
    } request;

    struct Response
    {
        bool success;
        Scene scene;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::SceneUpdate::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.scene);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneUpdate::Request& data) {
    deserialize(buffer, data.scene);
};

template<>
inline void serialize(const map_srvs::SceneUpdate::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.success);
    serialize(buffer,data.scene);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneUpdate::Response& data) {
    deserialize(buffer,data.success);
    deserialize(buffer,data.scene);
};

} /* namespace mind_os */