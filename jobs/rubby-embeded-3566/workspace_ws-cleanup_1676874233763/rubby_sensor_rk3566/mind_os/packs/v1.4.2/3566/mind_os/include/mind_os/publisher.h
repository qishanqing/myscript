/*
 * Publisher.h
 *
 *  Created on: Aug 13, 2021
 *      Author: ubuntu
 */

#ifndef INCLUDE_MIND_OS_PUBLISHER_H_
#define INCLUDE_MIND_OS_PUBLISHER_H_

#include <functional>
#include <memory>
#include <any>
#include "const_ptr.h"

namespace mind_os
{

class Publisher
{
    std::function<void(std::any&)> funcPub;
    std::function<void()> funcShutdown;
    friend class RemoteNodeHandle;

public:
    Publisher() {}
    Publisher(std::function<void(std::any&)> funcPub) : funcPub(funcPub) {}
    Publisher(std::function<void(std::any&)> funcPub,
            std::function<void()> funcShutdown) : funcPub(funcPub), funcShutdown(funcShutdown)  {}

    template <typename T>
    void publish(T& msg)
    {
        auto msgDuplicated = std::make_shared<T>();
        *msgDuplicated = msg;
        ConstPtr<T> constMsg = msgDuplicated;
        std::any msgCasted = constMsg;
        funcPub(msgCasted);
    }

    template <typename T>
    void publish(std::shared_ptr<T>& msg)
    {
        ConstPtr<T> constMsg = msg;
        std::any msgCasted = constMsg;
        funcPub(msgCasted);
    }

    template <typename T>
    void publish(const std::shared_ptr<T>& msg)
    {
        ConstPtr<T> constMsg = msg;
        std::any msgCasted = constMsg;
        funcPub(msgCasted);
    }

    template <typename T>
    void publish(const std::shared_ptr<const T>& msg)
    {
        ConstPtr<T> constMsg = msg;
        std::any msgCasted = constMsg;
        funcPub(msgCasted);
    }

    void publish(std::any& msg)
    {
        funcPub(msg);
    }

    void shutdown()
    {
        if (funcShutdown) {
            funcShutdown();
        }
    }
};

}


#endif /* INCLUDE_MIND_OS_PUBLISHER_H_ */
