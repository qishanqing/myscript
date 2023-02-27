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
void serialize(const tutorial::sensor::EncoderData& data, BytesBuffer& buffer) {
    serialize(data.header, buffer);
    buffer << data.encoderL;
    buffer << data.encoderR;
}; /* serialization of tutorial::sensor::EncoderData */

template<>
void deserialize(BytesBuffer& buffer, tutorial::sensor::EncoderData& data) {
    deserialize(buffer, data.header);
    buffer >> data.encoderL;
    buffer >> data.encoderR;
}; /* de-serialization of tutorial::sensor::EncoderData */

} /* namespace mind_os */

