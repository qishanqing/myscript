#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>
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
inline void serialize(const tutorial::sensor::ImuData& data, BytesBuffer& buffer) {
    serialize(data.header, buffer);
    serialize(data.frame_id, buffer);
    serialize(data.accel, buffer);
    serialize(data.gyro, buffer);
}; /* serialization of tutorial::sensor::ImuData */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::sensor::ImuData& data) {
    deserialize(buffer, data.header);
    deserialize(buffer, data.frame_id);
    deserialize(buffer, data.accel);
    deserialize(buffer, data.gyro);
}; /* de-serialization of tutorial::sensor::ImuData */

} /* namespace mind_os */

