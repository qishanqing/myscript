/*
 * Time.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */
#pragma once

#include <cstdint>

namespace mind_os {

namespace util {

struct Duration {
    std::int64_t sec;   // sencords
    std::int64_t usec;  // microseconds

    inline std::int64_t milliseconds() {
        return sec * 1000 + usec / 1000;
    }

    inline std::int64_t microseconds() {
        return sec * 1000000 + usec;
    }
};

std::uint64_t now();

}  // namespace util

}
