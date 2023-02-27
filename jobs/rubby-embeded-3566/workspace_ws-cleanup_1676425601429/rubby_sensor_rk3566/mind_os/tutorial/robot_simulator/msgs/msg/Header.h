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
void serialize(const tutorial::base::Header& data, BytesBuffer& buffer) {
    buffer << data.seq;
    buffer << data.stamp;
}; /* serialization of tutorial::base::Header */

template<>
void deserialize(BytesBuffer& buffer, tutorial::base::Header& data) {
    buffer >> data.seq;
    buffer >> data.stamp;
}; /* de-serialization of tutorial::base::Header */

} /* namespace mind_os */

