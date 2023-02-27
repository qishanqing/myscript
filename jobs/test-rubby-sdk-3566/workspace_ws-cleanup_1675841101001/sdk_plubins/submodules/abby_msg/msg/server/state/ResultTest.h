#pragma once

#include "Result.h"

namespace state_msgs {

struct ResultTest : public Result
{
    int value;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const state_msgs::ResultTest& data, BytesBuffer& buffer) {
//     buffer << data.resultId;
//     buffer << data.value;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, state_msgs::ResultTest& data) {
//     buffer >> data.resultId;
//     buffer >> data.value;
// };

// } /* namespace mind_os */
