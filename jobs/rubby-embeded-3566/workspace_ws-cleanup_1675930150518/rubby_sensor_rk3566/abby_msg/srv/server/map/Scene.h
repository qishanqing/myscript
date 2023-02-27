/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

// #include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct Scene
{
    std::uint64_t   id;
    std::string     alias;
    std::uint32_t   useCount;
    std::string     sceneImage;
    std::string     sceneThumbnail;
    std::uint64_t   date;
    std::uint64_t   lastDate;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_srvs::Scene& data, BytesBuffer& buffer) {
//     buffer << data.id;
//     buffer << data.alias;
//     buffer << data.useCount;
//     buffer << data.sceneImage;
//     buffer << data.sceneThumbnail;
//     buffer << data.date;
//     buffer << data.lastDate;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_srvs::Scene& data) {
//     buffer >> data.id;
//     buffer >> data.alias;
//     buffer >> data.useCount;
//     buffer >> data.sceneImage;
//     buffer >> data.sceneThumbnail;
//     buffer >> data.date;
//     buffer >> data.lastDate;
// };

// } /* namespace mind_os */
