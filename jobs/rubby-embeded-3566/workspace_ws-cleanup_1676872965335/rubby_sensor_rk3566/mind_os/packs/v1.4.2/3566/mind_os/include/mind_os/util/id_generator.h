#pragma once

#include <mutex>

namespace mind_os {

namespace util {

template <typename T>
class IDGenerator
{
    T id;
    std::mutex mtx;
public:
    T next()
    {
        std::lock_guard<std::mutex> guard(mtx);
        return id++;
    }
};

}

}
