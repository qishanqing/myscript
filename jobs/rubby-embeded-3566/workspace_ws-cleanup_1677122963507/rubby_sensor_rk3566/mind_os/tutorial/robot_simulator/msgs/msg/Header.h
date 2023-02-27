#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>

namespace tutorial {
namespace base {

struct Header {
    std::uint32_t seq;
    std::uint64_t stamp;
}; /* message Header */

} /* namespace tutorial */
} /* namespace base */

namespace mind_os {
template<>
inline void serialize(const tutorial::base::Header& data, BytesBuffer& buffer) {
    serialize(data.seq, buffer);
    serialize(data.stamp, buffer);
}; /* serialization of tutorial::base::Header */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::base::Header& data) {
    deserialize(buffer, data.seq);
    deserialize(buffer, data.stamp);
}; /* de-serialization of tutorial::base::Header */

} /* namespace mind_os */

