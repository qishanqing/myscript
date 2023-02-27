/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct SceneDefault
{
    enum MapEnum{
        NewMap,
        Default,
    };

    SceneDefault() = default;

    struct Request
    {
        std::uint64_t timestamp;
    } request;

    struct Response
    {
        MapEnum mapState;
        std::uint64_t sceneId;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::SceneDefault::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.timestamp);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDefault::Request& data) {
    deserialize(buffer,data.timestamp);
};

template<>
inline void serialize(const map_srvs::SceneDefault::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.mapState);
    serialize(buffer,data.sceneId);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDefault::Response& data) {
    deserialize(buffer,data.mapState);
    deserialize(buffer,data.sceneId);
};

} /* namespace mind_os */

