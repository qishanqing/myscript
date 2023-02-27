#pragma once

#include <mutex>
#include <condition_variable>
#include <glog/logging.h>

namespace mind_os {
namespace util {

class Notifier
{
    std::condition_variable cv;
    std::mutex mtxNoti;
    bool notified = false;
    bool interrputed = false;
public:

    bool wait_for(std::chrono::milliseconds time);

    bool wait();

    void notify_one();

    void interrupt();
};
}
}
