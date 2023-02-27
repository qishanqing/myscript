#pragma once

// #include <mind_os/serialization/serialization.h>

namespace state_msgs {

struct Result
{
    std::uint64_t resultId;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const state_msgs::Result& data, BytesBuffer& buffer) {
//     buffer << data.resultId;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, state_msgs::Result& data) {
//     buffer >> data.resultId;
// };

// } /* namespace mind_os */
