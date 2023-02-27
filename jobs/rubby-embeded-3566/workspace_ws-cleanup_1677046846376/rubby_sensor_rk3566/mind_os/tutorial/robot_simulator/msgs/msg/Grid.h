#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>

namespace tutorial {
namespace base {

struct Grid {
    float x;
    float y;
    std::int8_t value;
}; /* message Grid */

} /* namespace tutorial */
} /* namespace base */

namespace mind_os {
template<>
inline void serialize(const tutorial::base::Grid& data, BytesBuffer& buffer) {
    serialize(data.x, buffer);
    serialize(data.y, buffer);
    serialize(data.value, buffer);
}; /* serialization of tutorial::base::Grid */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::base::Grid& data) {
    deserialize(buffer, data.x);
    deserialize(buffer, data.y);
    deserialize(buffer, data.value);
}; /* de-serialization of tutorial::base::Grid */

} /* namespace mind_os */

