/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

// #include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct SceneDataDel
{
    SceneDataDel() = default;

    struct Request
    {
        std::uint64_t sceneDataId;
    } request;

    struct Response
    {
        bool success;
    } response;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::SceneDataDel::Request& data, BytesBuffer& buffer) {
//     buffer << data.sceneDataId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDataDel::Request& data) {
//     buffer >> data.sceneDataId;
// };

// template<>
// inline void serialize(const map_srvs::SceneDataDel::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDataDel::Response& data) {
//     buffer >> data.success;
// };

// } /* namespace mind_os */
