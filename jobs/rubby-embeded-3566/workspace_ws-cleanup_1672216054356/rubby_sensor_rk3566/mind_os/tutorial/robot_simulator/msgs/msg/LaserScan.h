#pragma once

#include <mind_os/serialization/serialization.h>
#include <vector>
#include "Header.h"

namespace tutorial {
namespace sensor {

struct LaserScan {
    tutorial::base::Header header;
    float angle_min;
    float angle_max;
    float angle_increment;
    float time_increment;
    float scan_time;
    float range_min;
    float range_max;
    std::vector<float> ranges;
    std::vector<float> intensities;
}; /* message LaserScan */

} /* namespace tutorial */
} /* namespace sensor */

namespace mind_os {
template<>
void serialize(const tutorial::sensor::LaserScan& data, BytesBuffer& buffer) {
    serialize(data.header, buffer);
    serialize(data.angle_min, buffer);
    serialize(data.angle_max, buffer);
    serialize(data.angle_increment, buffer);
    serialize(data.time_increment, buffer);
    serialize(data.scan_time, buffer);
    serialize(data.range_min, buffer);
    serialize(data.range_max, buffer);
    serializeVector(data.ranges, buffer);
    serializeVector(data.intensities, buffer);
}; /* serialization of tutorial::sensor::LaserScan */

template<>
void deserialize(BytesBuffer& buffer, tutorial::sensor::LaserScan& data) {
    deserialize(buffer, data.header);
    deserialize(buffer, data.angle_min);
    deserialize(buffer, data.angle_max);
    deserialize(buffer, data.angle_increment);
    deserialize(buffer, data.time_increment);
    deserialize(buffer, data.scan_time);
    deserialize(buffer, data.range_min);
    deserialize(buffer, data.range_max);
    deserializeVector(buffer, data.ranges);
    deserializeVector(buffer, data.intensities);
}; /* de-serialization of tutorial::sensor::LaserScan */

} /* namespace mind_os */

