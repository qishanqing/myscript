#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>
#include <vector>

namespace tutorial {
namespace pub_sub {

struct InnerVector {
    std::vector<std::uint32_t> vector_data;
}; /* message InnerVector */

struct DataMsg {
    std::uint64_t stamp;
    double data;
    std::vector<InnerVector> inner_vector_data;
}; /* message DataMsg */

} /* namespace tutorial */
} /* namespace pub_sub */

namespace mind_os {
template<>
void serialize(const tutorial::pub_sub::InnerVector& data, BytesBuffer& buffer) {
    buffer << data.vector_data;
}; /* serialization of tutorial::pub_sub::InnerVector */

template<>
void deserialize(BytesBuffer& buffer, tutorial::pub_sub::InnerVector& data) {
    buffer >> data.vector_data;
}; /* de-serialization of tutorial::pub_sub::InnerVector */

template<>
void serialize(const tutorial::pub_sub::DataMsg& data, BytesBuffer& buffer) {
    buffer << data.stamp;
    serialize(data.data, buffer);
    serialize(data.inner_vector_data, buffer);
}; /* serialization of tutorial::pub_sub::DataMsg */

template<>
void deserialize(BytesBuffer& buffer, tutorial::pub_sub::DataMsg& data) {
    buffer >> data.stamp;
    deserialize(buffer, data.data);
    deserialize(buffer, data.inner_vector_data);
}; /* de-serialization of tutorial::pub_sub::DataMsg */

} /* namespace mind_os */


