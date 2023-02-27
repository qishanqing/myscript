/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include <mind_os/serialization/serialization.h>

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
    bool            valid;
};

}

namespace mind_os {
template<>
inline void serialize(BytesBuffer& buffer,const map_srvs::Scene& data) {
    serialize(buffer,data.id);
    serialize(buffer,data.alias);
    serialize(buffer,data.useCount);
    serialize(buffer,data.sceneImage);
    serialize(buffer,data.sceneThumbnail);
    serialize(buffer,data.date);
    serialize(buffer,data.lastDate);
    serialize(buffer,data.valid);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::Scene& data) {
    deserialize(buffer,data.id);
    deserialize(buffer,data.alias);
    deserialize(buffer,data.useCount);
    deserialize(buffer,data.sceneImage);
    deserialize(buffer,data.sceneThumbnail);
    deserialize(buffer,data.date);
    deserialize(buffer,data.lastDate);
    deserialize(buffer,data.valid);
};

} /* namespace mind_os */
