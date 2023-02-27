#pragma once

#include "main_thread_c.h"

namespace mind_os {

namespace main_thread {

template <typename T>
struct Object
{
public:
    std::shared_ptr<T> operator()()
    {
        T* object;
        T** pObj = &object;
        std::shared_ptr<T> event(new Event([pObj]()
        {
            *pObj = new T();
        }));
        wait(event);
        return std::shared_ptr<T>(object);
    }
};

}

} /* mind_os */
