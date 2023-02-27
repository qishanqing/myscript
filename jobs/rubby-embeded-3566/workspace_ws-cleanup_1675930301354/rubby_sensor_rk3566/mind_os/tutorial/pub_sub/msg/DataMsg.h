#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>

namespace tutorial {
namespace pub_sub {

struct InnerVector {
    std::vector<std::uint32_t> vector_data;
}; /* message InnerVector */

struct DataMsg {
    std::uint64_t stamp;
    double data;
    std::vector<std::uint32_t> vector_data_1;
    std::vector<std::vector<std::uint32_t>> vector_data_2;
    std::vector<InnerVector> vector_data_3;
}; /* message DataMsg */

} /* namespace tutorial */
} /* namespace pub_sub */

namespace mind_os {
template<>
inline void serialize(const tutorial::pub_sub::InnerVector& data, BytesBuffer& buffer) {
    serialize(data.vector_data, buffer);
}; /* serialization of tutorial::pub_sub::InnerVector */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::pub_sub::InnerVector& data) {
    deserialize(buffer, data.vector_data);
}; /* de-serialization of tutorial::pub_sub::InnerVector */

template<>
inline void serialize(const tutorial::pub_sub::DataMsg& data, BytesBuffer& buffer) {
    serialize(data.stamp, buffer);
    serialize(data.data, buffer);
    serialize(data.vector_data_1, buffer);
    serialize(data.vector_data_2, buffer);
    serialize(data.vector_data_3, buffer); 
}; /* serialization of tutorial::pub_sub::DataMsg */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::pub_sub::DataMsg& data) {
    deserialize(buffer, data.stamp);
    deserialize(buffer, data.data);
    deserialize(buffer, data.vector_data_1);
    deserialize(buffer, data.vector_data_2);
    deserialize(buffer, data.vector_data_3);
}; /* de-serialization of tutorial::pub_sub::DataMsg */

} /* namespace mind_os */

