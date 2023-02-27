#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>
#include <vector>

namespace tutorial {
namespace record {

struct InnerVector {
    std::vector<std::uint32_t> vector_data;
}; /* message InnerVector */

struct DataMsg {
    std::uint64_t stamp;
    double data;
    std::vector<InnerVector> inner_vector_data;
}; /* message DataMsg */

} /* namespace tutorial */
} /* namespace record */

namespace mind_os {
template<>
void serialize(const tutorial::record::InnerVector& data, BytesBuffer& buffer) {
    buffer << data.vector_data;
}; /* serialization of tutorial::record::InnerVector */

template<>
void deserialize(BytesBuffer& buffer, tutorial::record::InnerVector& data) {
    buffer >> data.vector_data;
}; /* de-serialization of tutorial::record::InnerVector */

template<>
void serialize(const tutorial::record::DataMsg& data, BytesBuffer& buffer) {
    buffer << data.stamp;
    serialize(data.data, buffer);
    serialize(data.inner_vector_data, buffer);
}; /* serialization of tutorial::record::DataMsg */

template<>
void deserialize(BytesBuffer& buffer, tutorial::record::DataMsg& data) {
    buffer >> data.stamp;
    deserialize(buffer, data.data);
    deserialize(buffer, data.inner_vector_data);
}; /* de-serialization of tutorial::record::DataMsg */

} /* namespace mind_os */

