/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include <mind_os/serialization/serialization.h>

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

namespace mind_os {
template<>
inline void serialize(BytesBuffer& buffer,const map_srvs::SceneData& data) {
    serialize(buffer, data.id);
    serialize(buffer, data.sceneId);
    serialize(buffer, data.dataPath);
    serialize(buffer, data.date);
    serialize(buffer, data.lastDate);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::SceneData& data) {
    deserialize(buffer, data.id);
    deserialize(buffer, data.sceneId);
    deserialize(buffer, data.dataPath);
    deserialize(buffer, data.date);
    deserialize(buffer, data.lastDate);
};

} /* namespace mind_os */