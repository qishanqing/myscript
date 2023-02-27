//
// Created by lcy on 21-6-10.
//

#ifndef ROBOT_MATLAB_SINGLETON_H
#define ROBOT_MATLAB_SINGLETON_H

#if 1
#include <iostream>
#include <memory>
#include <mutex>
template <typename T>
class Singleton {
    public :
    template <typename... Args>
    static T& GetInstance(Args&&... args) {
        static std::once_flag s_flag;
        std::call_once(s_flag,[&]() {
            instance_.reset(new T(std::forward<Args>(args)...));
        });
        return *instance_;
    }
    ~Singleton() = default;
    private:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    private:
    static std::unique_ptr<T> instance_;
};
template <class T> std::unique_ptr<T> Singleton<T>::instance_;
#endif


#endif //ROBOT_MATLAB_SINGLETON_H
