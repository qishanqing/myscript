#pragma once

#include <mind_os/serialization/serialization.h>

namespace tutorial {
namespace nav {

enum Mode {
    MANNUAL = 0x00,
    AUTO = 0x01,
}; /* enum Mode */

struct StartNavSrv {
    struct Request {
        Mode mode;
    } request; /* struct Request */
    struct Response {
        bool success;
    } response; /* struct Response */
}; /* service StartNavSrv */

} /* namespace tutorial */
} /* namespace nav */

namespace mind_os {
template<>
inline void serialize(const tutorial::nav::StartNavSrv::Request& data, BytesBuffer& buffer) {
    serialize(data.mode, buffer);
}; /* serialization of tutorial::nav::StartNavSrv Request */

template<>
inline void serialize(const tutorial::nav::StartNavSrv::Response& data, BytesBuffer& buffer) {
    serialize(data.success, buffer);
}; /* serialization of tutorial::nav::StartNavSrv Response */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::nav::StartNavSrv::Request& data) {
    deserialize(buffer, data.mode);
}; /* de-serialization of tutorial::nav::StartNavSrv Request */
template<>
inline void deserialize(BytesBuffer& buffer, tutorial::nav::StartNavSrv::Response& data) {
    deserialize(buffer, data.success);
}; /* de-serialization of tutorial::nav::StartNavSrv Response */

} /* namespace mind_os */

