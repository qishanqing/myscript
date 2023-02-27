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
void serialize(const tutorial::slam::GetPoseSrv::Request& data, BytesBuffer& buffer) {
    buffer << data.timestamp;
}; /* serialization of tutorial::slam::GetPoseSrv Request */

template<>
void serialize(const tutorial::slam::GetPoseSrv::Response& data, BytesBuffer& buffer) {
    serialize(data.pose, buffer);
}; /* serialization of tutorial::slam::GetPoseSrv Response */

template<>
void deserialize(BytesBuffer& buffer, tutorial::slam::GetPoseSrv::Request& data) {
    buffer >> data.timestamp;
}; /* de-serialization of tutorial::slam::GetPoseSrv Request */
template<>
void deserialize(BytesBuffer& buffer, tutorial::slam::GetPoseSrv::Response& data) {
    deserialize(buffer, data.pose);
}; /* de-serialization of tutorial::slam::GetPoseSrv Response */

} /* namespace mind_os */

