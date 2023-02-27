#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>
#include <vector>
#include "Header.h"

namespace tutorial {
namespace sensor {

struct ImuData {
    tutorial::base::Header header;
    std::uint32_t frame_id;
    std::vector<double> accel;
    std::vector<double> gyro;
}; /* message ImuData */

} /* namespace tutorial */
} /* namespace sensor */

namespace mind_os {
template<>
void serialize(const tutorial::sensor::ImuData& data, BytesBuffer& buffer) {
    serialize(data.header, buffer);
    buffer << data.frame_id;
    serializeVector(data.accel, buffer);
    serializeVector(data.gyro, buffer);
}; /* serialization of tutorial::sensor::ImuData */

template<>
void deserialize(BytesBuffer& buffer, tutorial::sensor::ImuData& data) {
    deserialize(buffer, data.header);
    buffer >> data.frame_id;
    deserializeVector(buffer, data.accel);
    deserializeVector(buffer, data.gyro);
}; /* de-serialization of tutorial::sensor::ImuData */

} /* namespace mind_os */

