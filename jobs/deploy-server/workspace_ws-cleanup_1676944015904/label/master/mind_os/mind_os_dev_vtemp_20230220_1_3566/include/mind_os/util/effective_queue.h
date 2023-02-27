/*
 * EffectiveQueue.h
 *
 * The EffectiveQueue is a limited thread-safe queue ONLY for ONE producer and ONE customer without std::mutex.
 *
 *  Created on: Sep 10, 2021
 *      Author: ubuntu
 */

#pragma once

#include <cstddef>

namespace mind_os {
namespace util {

template <typename T>
class EffectiveQueue
{
public:
    EffectiveQueue()
    {
        head = new Element();
        tail = head;
    }

    T pop()
    {
        if (empty())
        {
            throw std::out_of_range("queue is empty");
        }

        auto e = head->next->value;
        auto preHead = head;
        head = head->next;
        delete preHead;
        return e;
    }

    bool empty()
    {
        return head == tail;
    }

    void push(T& e)
    {
        auto node = new Element();
        node->value = e;
        tail->next = node;
        tail = node;
    }

private:
    struct Element
    {
        T value;
        Element* next = nullptr;

        Element() {}
    };

    Element* head;
    Element* tail;
};

}
}
