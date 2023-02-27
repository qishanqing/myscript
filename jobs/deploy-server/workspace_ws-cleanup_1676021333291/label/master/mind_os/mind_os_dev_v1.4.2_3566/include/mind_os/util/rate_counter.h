#pragma once

#include <cstdint>
#include <deque>
#include <mutex>
#include <atomic>
#include "time.h"

namespace mind_os {

namespace util {

class RateCounter {
public:
    RateCounter();
    ~RateCounter();

    /**
     * call it when a frame is published.
     */
    void tick();

    /**
     * get the rate
     */
    double get();

    /**
     * The time duration since from last ticking
     */
    Duration sinceLastTick();

private:
    std::deque<std::uint64_t> msgTimeQueue;
    std::mutex lockMutex;
    std::atomic_int64_t lastPrintTime;
    static const std::uint8_t maxTime = 2u; // s
};
}  // namespace util
}  // namespace mind_os
