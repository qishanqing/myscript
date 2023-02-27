/*
 * ErrorMessage.h
 *
 *  Created on: Sep 22, 2021
 *      Author: ubuntu
 */

#pragma once

#include "base/Time.h"
#include "ErrorCode.h"

namespace psl
{
enum ErrorState
{
    ON,
    OFF
};

struct ErrorMessage
{
    ErrorMessage() = default;

    ErrorMessage(psl::Time stamp, psl::ErrorCode code, psl::ErrorState state)
            : stamp(stamp), code(code), state(state)
    {}

    psl::Time stamp;
    ErrorCode code;
    ErrorState state;
};
}
