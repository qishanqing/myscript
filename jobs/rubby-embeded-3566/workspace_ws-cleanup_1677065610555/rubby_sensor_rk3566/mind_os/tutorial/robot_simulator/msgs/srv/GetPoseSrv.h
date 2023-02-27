#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>
#include "../msg/SlamResult.h"

namespace tutorial {
namespace slam {

struct GetPoseSrv {
    struct Request {
        std::uint64_t timestamp;
    } request; /* struct Request */
    struct Response {
        SlamResult pose;
    } response; /* struct Response */
}; /* service GetPoseSrv */

} /* namespace tutorial */
} /* namespace slam */

namespace mind_os {
template<>
inline void serialize(const tutorial::slam::GetPoseSrv::Request& data, BytesBuffer& buffer) {
    serialize(data.timestamp, buffer);
}; /* serialization of tutorial::slam::GetPoseSrv Request */

template<>
inline void serialize(const tutorial::slam::GetPoseSrv::Response& data, BytesBuffer& buffer) {
    serialize(data.pose, buffer);
}; /* serialization of tutorial::slam::GetPoseSrv Response */

template<>
inline void deserialize(BytesBuffer& buffer, tutorial::slam::GetPoseSrv::Request& data) {
    deserialize(buffer, data.timestamp);
}; /* de-serialization of tutorial::slam::GetPoseSrv Request */
template<>
inline void deserialize(BytesBuffer& buffer, tutorial::slam::GetPoseSrv::Response& data) {
    deserialize(buffer, data.pose);
}; /* de-serialization of tutorial::slam::GetPoseSrv Response */

} /* namespace mind_os */

