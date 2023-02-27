#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>
#include "Header.h"

namespace tutorial {
namespace sensor {

struct EncoderData {
    tutorial::base::Header header;
    std::int64_t encoderL;
    std::int64_t encoderR;
}; /* message EncoderData */

} /* namespace tutorial */
} /* namespace sensor */

namespace mind_os {
template<>
inline void serialize(const tutorial::sensor::EncoderData& data, BytesBuffer& buffer) {
    serialize(data.header, buffer);
    serialize(data.encoderL, buffer);
    serialize(data.encoderR, buffer);
}; /* serialization of tutorial::sensor::EncoderData */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::sensor::EncoderData& data) {
    deserialize(buffer, data.header);
    deserialize(buffer, data.encoderL);
    deserialize(buffer, data.encoderR);
}; /* de-serialization of tutorial::sensor::EncoderData */

} /* namespace mind_os */

