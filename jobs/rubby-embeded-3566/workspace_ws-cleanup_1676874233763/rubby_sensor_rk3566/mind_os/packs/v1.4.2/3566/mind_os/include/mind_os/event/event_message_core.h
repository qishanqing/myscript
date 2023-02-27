#pragma once

#include <functional>
#include <any>

#include "../subscriber.h"
#include "../publisher.h"
#include "../const_ptr.h"
#include "event_type.h"

namespace mind_os
{
class EventMessageCore
{
public:
    virtual ~EventMessageCore() {}

    virtual Publisher advertise(std::function<void(std::any&)> funcPub, std::string& name) = 0;

    virtual void addPublishTrigger(std::string name,
            std::function<bool(std::any&)> funcFilter,
            std::uint64_t usTimeout,
            std::function<void()> funcOnTimeout,
            std::function<bool(std::any&)> funcEndingCondition) = 0;

    virtual void addPublishTrigger(std::string& name,
            std::function<bool(std::any&)> funcFilter,
            std::function<bool()> funcTriggeredCondition,
            std::function<void()> funcTriggeredAction,
            std::function<bool(std::any&)> funcEndingCondition) = 0;
};

}
