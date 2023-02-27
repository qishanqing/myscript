#ifndef TIME_H
#define TIME_H
#include <cstdint>
#pragma once

#include <sys/time.h>

namespace nav
{
static std::uint64_t Now()
{
    struct timeval tvt;
    gettimeofday(&tvt, 0);
    return static_cast<std::uint64_t>(tvt.tv_sec * 1000000 + tvt.tv_usec);
}
} // namespace nav

#endif // TIME_H
