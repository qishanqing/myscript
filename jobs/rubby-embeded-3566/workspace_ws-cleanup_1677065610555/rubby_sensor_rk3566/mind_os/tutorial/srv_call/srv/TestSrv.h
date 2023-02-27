#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>
#include <vector>

namespace tutorial {
namespace srv_call {

struct InnerVector {
    std::vector<std::uint32_t> vector_data;
}; /* message InnerVector */

struct TestSrv {
    struct Request {
        std::uint64_t stamp;
        std::int32_t param;
        std::vector<InnerVector> inner_vector_data;
    } request; /* struct Request */
    struct Response {
        std::int32_t result;
        bool success;
        std::vector<InnerVector> inner_vector_data;
    } response; /* struct Response */
}; /* service TestSrv */

} /* namespace tutorial */
} /* namespace srv_call */

namespace mind_os {
template<>
void serialize(const tutorial::srv_call::InnerVector& data, BytesBuffer& buffer) {
    buffer << data.vector_data;
}; /* serialization of tutorial::srv_call::InnerVector */

template<>
void deserialize(BytesBuffer& buffer, tutorial::srv_call::InnerVector& data) {
    buffer >> data.vector_data;
}; /* de-serialization of tutorial::srv_call::InnerVector */

template<>
void serialize(const tutorial::srv_call::TestSrv::Request& data, BytesBuffer& buffer) {
    buffer << data.stamp;
    buffer << data.param;
    serialize(data.inner_vector_data, buffer);
}; /* serialization of tutorial::srv_call::TestSrv Request */

template<>
void serialize(const tutorial::srv_call::TestSrv::Response& data, BytesBuffer& buffer) {
    buffer << data.result;
    serialize(data.success, buffer);
    serialize(data.inner_vector_data, buffer);
}; /* serialization of tutorial::srv_call::TestSrv Response */

template<>
void deserialize(BytesBuffer& buffer, tutorial::srv_call::TestSrv::Request& data) {
    buffer >> data.stamp;
    buffer >> data.param;
    deserialize(buffer, data.inner_vector_data);
}; /* de-serialization of tutorial::srv_call::TestSrv Request */
template<>
void deserialize(BytesBuffer& buffer, tutorial::srv_call::TestSrv::Response& data) {
    buffer >> data.result;
    deserialize(buffer, data.success);
    deserialize(buffer, data.inner_vector_data);
}; /* de-serialization of tutorial::srv_call::TestSrv Response */

} /* namespace mind_os */

