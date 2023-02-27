/*
 * event_guard.h
 *
 *  Created on: Sep 14, 2022
 *      Author: ubuntu
 */

#pragma once

#include "../node_handle.h"
#include "event_message_core.h"
#include "../util/time.h"

namespace mind_os
{

class EventHandle {

private:
    EventHandle() = default;
public:
    EventHandle(NodeHandle* pNh, double hz = 100);
    EventHandle(NodeHandle& nh, double hz = 100);

    template <typename EVENT>
    Publisher advertise(std::string name)
    {
        auto pub = pNh->advertise<EVENT>(name);
        auto funcPub = std::bind([](Publisher pub, std::any&msg) {
            pub.publish(std::any_cast<ConstPtr<EVENT>>(msg));
        }, pub, std::placeholders::_1);
        return impl->advertise(funcPub, name);
    }

    template <typename EVENT>
    void addPublishTrigger(std::string name,
            std::function<bool(ConstPtr<EVENT>&)> funcFilter,
            std::uint64_t usTimeout,
            std::function<void()> funcOnTimeout,
            std::function<bool(ConstPtr<EVENT>&)> funcEndingCondition)
    {
        auto packedFuncPackedFilter = [funcFilter](std::any& msg) {
            return funcFilter(std::any_cast<ConstPtr<EVENT>>(msg));
        };

        if (funcEndingCondition)
        {
            auto packedFuncEndingCondition = [funcEndingCondition](std::any& msg) {
                return funcEndingCondition(std::any_cast<ConstPtr<EVENT>>(msg));
            };
            impl->addPublishTrigger(name, packedFuncPackedFilter,
                    usTimeout, funcOnTimeout, packedFuncEndingCondition);
        }
        else
        {
            impl->addPublishTrigger(name, packedFuncPackedFilter,
                    usTimeout, funcOnTimeout, nullptr);
        }
    }

    template <typename EVENT>
    void addPublishTrigger(std::string name,
            std::function<bool(ConstPtr<EVENT>&)> funcFilter,
            std::function<bool()> funcTriggeredCondition,
            std::function<void()> funcTriggeredAction,
            std::function<bool(ConstPtr<EVENT>&)> funcEndingCondition = nullptr)
    {
        auto packedFuncPackedFilter = [funcFilter](std::any& msg) {
            return funcFilter(std::any_cast<ConstPtr<EVENT>>(msg));
        };

        if (funcEndingCondition)
        {
            auto packedFuncEndingCondition = [funcEndingCondition](std::any& msg) {
                return funcEndingCondition(std::any_cast<ConstPtr<EVENT>>(msg));
            };
            impl->addPublishTrigger(name, packedFuncPackedFilter,
                    funcTriggeredCondition, funcTriggeredAction, packedFuncEndingCondition);
        }
        else
        {
            impl->addPublishTrigger(name, packedFuncPackedFilter,
                    funcTriggeredCondition, funcTriggeredAction, nullptr);
        }
    }


private:
    NodeHandle* pNh = nullptr;
    std::shared_ptr<EventMessageCore> impl;
};

}
