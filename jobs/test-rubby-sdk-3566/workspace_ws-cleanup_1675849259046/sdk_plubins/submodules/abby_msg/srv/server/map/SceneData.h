/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

// #include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct SceneData
{
    std::uint64_t   id;
    std::uint64_t   sceneId;
    std::string     dataPath;
    std::uint64_t   date;
    std::uint64_t   lastDate;
}; 

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::SceneData& data, BytesBuffer& buffer) {
//     buffer << data.id;
//     buffer << data.sceneId;
//     buffer << data.dataPath;
//     buffer << data.date;
//     buffer << data.lastDate;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::SceneData& data) {
//     buffer >> data.id;
//     buffer >> data.sceneId;
//     buffer >> data.dataPath;
//     buffer >> data.date;
//     buffer >> data.lastDate;
// };

// } /* namespace mind_os */