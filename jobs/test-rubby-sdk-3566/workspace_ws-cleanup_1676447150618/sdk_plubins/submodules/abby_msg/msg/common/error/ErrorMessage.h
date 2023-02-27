/*
 * EulerAngle.h
 *
 *  Created on: Feb 23, 2021
 *      Author: ubuntu
 */
#pragma once

#include "../base/Time.h"
#include "ErrorCode.h"
// #include <mind_os/serialization/serialization.h>

namespace error_msgs {

enum ErrorState
{
    ON,
    OFF
};

struct ErrorMessage
{
    ErrorMessage() = default;
    ErrorMessage(base_msgs::Time stamp, ErrorCode code, ErrorState state)
        : stamp(stamp), code(code), state(state) {}
    base_msgs::Time stamp;
    ErrorCode code;
    ErrorState state;
};
}

// namespace mind_os {
// template<>
// inline void serialize(const error_msgs::ErrorMessage& data, BytesBuffer& buffer) {
//     buffer << data.stamp;
//     buffer << data.code;
//     buffer << data.state;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, error_msgs::ErrorMessage& data) {
//     buffer >> data.stamp;
//     buffer >> data.code;
//     buffer >> data.state;
// };

// } /* namespace mind_os */


