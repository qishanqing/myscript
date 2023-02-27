#pragma once

#include "rate.h"
#include <thread>
#include <atomic>
#include <functional>

namespace mind_os {

namespace util {

class LoopThread
{
public:
    enum {
        POLICY_DEFAULT  = SCHED_OTHER,
        POLICY_FIFO     = SCHED_FIFO,
        POLICY_RR       = SCHED_RR,
        POLICY_NONE = SCHED_OTHER + SCHED_FIFO + SCHED_RR
    };


    LoopThread(double hz, std::function<void(void)> run);

    LoopThread(double hz, std::function<void(void)> init, std::function<void(void)> run);

    LoopThread(double hz,
        int policy, int priority, std::function<void(void)> run);

    LoopThread(double hz, 
        int policy, int priority, std::function<void(void)> init, std::function<void(void)> run);

    ~LoopThread();
    

    double getHz() const;

    void start();

    bool stop(bool blocked = true);

    bool isRunning();

private:
    double hz;
    std::atomic_bool running;
    std::thread t;
    int policy = POLICY_NONE;
    int priority;
    Rate *rate;
    std::function<void(void)> init;
    std::function<void(void)> run;
};

}
}
