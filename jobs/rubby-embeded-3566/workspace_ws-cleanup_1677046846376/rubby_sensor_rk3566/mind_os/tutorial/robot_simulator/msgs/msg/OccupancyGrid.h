#pragma once

#include <mind_os/serialization/serialization.h>
#include "Header.h"
#include "Grid.h"

namespace tutorial {
namespace slam {

struct OccupancyGrid {
    tutorial::base::Header header;
    int width;
    int height;
    double origin_x;
    double origin_y;
    float resolution;
    std::vector<int8_t> mdata;
    std::vector<base::Grid> grid_data;
}; /* message OccupancyGrid */

} /* namespace tutorial */
} /* namespace slam */

namespace mind_os {
template<>
inline void serialize(const tutorial::slam::OccupancyGrid& data, BytesBuffer& buffer) {
    serialize(data.header, buffer);
    serialize(data.width, buffer);
    serialize(data.height, buffer);
    serialize(data.origin_x, buffer);
    serialize(data.origin_y, buffer);
    serialize(data.resolution, buffer);
    serialize(data.mdata, buffer);
    serialize(data.grid_data, buffer);
}; /* serialization of tutorial::slam::OccupancyGrid */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::slam::OccupancyGrid& data) {
    deserialize(buffer, data.header);
    deserialize(buffer, data.width);
    deserialize(buffer, data.height);
    deserialize(buffer, data.origin_x);
    deserialize(buffer, data.origin_y);
    deserialize(buffer, data.resolution);
    deserialize(buffer, data.mdata);
    deserialize(buffer, data.grid_data);
}; /* de-serialization of tutorial::slam::OccupancyGrid */

} /* namespace mind_os */

