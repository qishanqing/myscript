/*
 * recorder.h
 *
 *  Created on: Aug 29, 2022
 *      Author: ubuntu
 */

#pragma once

#include "../node_handle.h"
#include "bag.h"
#include <atomic>
#include <mutex>

namespace mind_os
{

class Recorder : public NoCopy
{
public:
    Recorder(NodeHandle& nh, Bag* bag);

    template<typename MSG>
    void subscribe(std::string topic)
    {
        std::lock_guard<std::mutex> guard(mtxSubs);
        if(subs.find(topic) == subs.end())
        {
            auto&& sub = nh.subscribe<MSG>(topic, [this, topic](ConstPtr<MSG>& msg){
                if (running)
                {
                    BytesBuffer buf;
                    serialize<MSG>(*msg, buf);
                    record(topic, buf);
                }
            });

            subs.insert(std::make_pair(topic, sub));
        }
    }

    void unsubscribe(std::string topic);
    bool start();
    bool stop();
    bool pause();
    bool resume();
private:
    NodeHandle& nh;
    Bag* bag;
    bool running;
    bool started;
    std::uint64_t timeStart;
    std::mutex mtxSubs;
    std::mutex mtxOpt;
    std::unordered_map<std::string, Subscriber> subs;

    void record(const std::string& topic, BytesBuffer& buf);
};

}

