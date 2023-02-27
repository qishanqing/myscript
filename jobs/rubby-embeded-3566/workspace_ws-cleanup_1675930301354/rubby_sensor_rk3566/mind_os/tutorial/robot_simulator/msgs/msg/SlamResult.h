#pragma once

#include <mind_os/serialization/serialization.h>
#include <vector>
#include "Header.h"

namespace tutorial {
namespace slam {

struct SlamResult {
    tutorial::base::Header header;
    std::vector<float> s_rotation;
    std::vector<float> s_position;
    float s_state;
}; /* message SlamResult */

} /* namespace tutorial */
} /* namespace slam */

namespace mind_os {
template<>
void serialize(const tutorial::slam::SlamResult& data, BytesBuffer& buffer) {
    serialize(data.header, buffer);
    serialize(data.s_rotation, buffer);
    serialize(data.s_position, buffer);
    serialize(data.s_state, buffer);
}; /* serialization of tutorial::slam::SlamResult */

template<>
void deserialize(BytesBuffer& buffer, tutorial::slam::SlamResult& data) {
    deserialize(buffer, data.header);
    deserialize(buffer, data.s_rotation);
    deserialize(buffer, data.s_position);
    deserialize(buffer, data.s_state);
}; /* de-serialization of tutorial::slam::SlamResult */

} /* namespace mind_os */

