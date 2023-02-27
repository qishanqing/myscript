#pragma once

#include <mutex>
#include <condition_variable>
#include <sys/time.h>

namespace mind_os {

namespace util {

class Rate
{
public:
    enum ReturnCode {
        SLEEP_RTC_OK,
        SLEEP_RTC_NOT_SLEEP,
        SLEEP_RTC_INTERRUPTED,
    };
    Rate(double rate);
    Rate(Rate&& rate);
    void interrupt();
    ReturnCode sleep();

private:
    std::uint64_t base_time;
    std::uint64_t counter;
    struct timeval tvStart, tvEnd;

    std::mutex mtxCv;
    std::condition_variable cv;
};

}
}
