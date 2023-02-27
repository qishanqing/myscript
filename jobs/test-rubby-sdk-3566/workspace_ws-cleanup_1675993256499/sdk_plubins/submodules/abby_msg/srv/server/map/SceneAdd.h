/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include "Scene.h"

namespace map_srvs {

struct SceneAdd
{
    SceneAdd() = default;

    struct Request
    {
        std::uint64_t timestamp;
    } request;

    struct Response
    {
        bool success;
        Scene scene;
    } response;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::SceneAdd::Request& data, BytesBuffer& buffer) {
//     buffer << data.timestamp;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneAdd::Request& data) {
//     buffer >> data.timestamp;
// };

// template<>
// inline void serialize(const map_srvs::SceneAdd::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
//     serialize(data.scene, buffer);
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneAdd::Response& data) {
//     buffer >> data.success;
//     deserialize(buffer, data.scene);
// };

// } /* namespace mind_os */
