#pragma once

#include "Result.h"

namespace state_msgs {

struct ResultTest : public Result
{
    int value;
};

}

namespace mind_os {
template<>
inline void serialize(const state_msgs::ResultTest& data, BytesBuffer& buffer) {
    serialize(buffer, data.resultId);
    serialize(buffer, data.value);
};

template<>
inline void deserialize(BytesBuffer& buffer, state_msgs::ResultTest& data) {
    deserialize(buffer, data.resultId);
    deserialize(buffer, data.value);
};

} /* namespace mind_os */
