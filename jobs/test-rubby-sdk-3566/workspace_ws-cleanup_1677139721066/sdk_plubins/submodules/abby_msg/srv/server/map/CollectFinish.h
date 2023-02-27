#pragma once

#include "SceneData.h"

namespace map_srvs {

struct CollectFinish
{
    SceneData data; 
    bool success;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::CollectFinish& data, BytesBuffer& buffer) {
    serialize(buffer, data.data);
    serialize(buffer, data.success);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::CollectFinish& data) {
    deserialize(buffer, data.data);
    deserialize(buffer, data.success);
};

} /* namespace mind_os */
