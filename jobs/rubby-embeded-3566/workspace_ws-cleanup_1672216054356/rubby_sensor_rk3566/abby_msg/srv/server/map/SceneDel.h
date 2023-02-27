/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

// #include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct SceneDel
{
    SceneDel() = default;

    struct Request
    {
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
// inline void serialize(const map_srvs::SceneDel::Request& data, BytesBuffer& buffer) {
//     buffer << data.sceneId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDel::Request& data) {
//     buffer >> data.sceneId;
// };

// template<>
// inline void serialize(const map_srvs::SceneDel::Response& data, BytesBuffer& buffer) {
//     buffer << data.success;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneDel::Response& data) {
//     buffer >> data.success;
// };

// } /* namespace mind_os */
