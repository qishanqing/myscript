#pragma once

#include <functional>
#include <condition_variable>

namespace mind_os {

namespace main_thread {

class Event
{
public:
    Event(std::function<void(void)> f) : f(f), finished(false) {}
    inline std::function<void(void)> get_f()
    {
        return f;
    }
private:
    std::condition_variable cv;
    std::mutex mtxOpt;
    std::mutex mtxNoti;
    std::function<void(void)> f;
    bool finished;

    friend class Processor;

    bool wait();
    void notify(bool processed = true);
};

using EventSP = std::shared_ptr<Event>;

}  // namespace main_thread

} /* mind_os */
