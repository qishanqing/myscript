#pragma once

#include <mind_os/serialization/serialization.h>
#include <cstdint>
#include "../msg/SlamResult.h"

namespace tutorial {
namespace slam {

struct GetPoseSrv2 {
    struct Request {
        std::uint64_t timestamp;
    } request; /* struct Request */
    struct Response {
        SlamResult pose;
    } response; /* struct Response */
}; /* service GetPoseSrv2 */

} /* namespace tutorial */
} /* namespace slam */

namespace mind_os {
template<>
void serialize(const tutorial::slam::GetPoseSrv2::Request& data, BytesBuffer& buffer) {
    buffer << data.timestamp;
}; /* serialization of tutorial::slam::GetPoseSrv2 Request */

template<>
void serialize(const tutorial::slam::GetPoseSrv2::Response& data, BytesBuffer& buffer) {
    serialize(data.pose, buffer);
}; /* serialization of tutorial::slam::GetPoseSrv2 Response */

template<>
void deserialize(BytesBuffer& buffer, tutorial::slam::GetPoseSrv2::Request& data) {
    buffer >> data.timestamp;
}; /* de-serialization of tutorial::slam::GetPoseSrv2 Request */
template<>
void deserialize(BytesBuffer& buffer, tutorial::slam::GetPoseSrv2::Response& data) {
    deserialize(buffer, data.pose);
}; /* de-serialization of tutorial::slam::GetPoseSrv2 Response */

} /* namespace mind_os */

