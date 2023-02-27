#pragma once

#include <functional>

namespace mind_os
{
class Subscriber {
    std::function<void(void)> funcShutdown;

public:
    Subscriber() : funcShutdown(nullptr) {}
    Subscriber(std::function<void(void)> f) : funcShutdown(f) {}
    void shutdown()
    {
        if (funcShutdown != nullptr)
        {
            funcShutdown();
        }
    }
};
}
