/*
 * player.h
 *
 *  Created on: Aug 29, 2022
 *      Author: ubuntu
 */

#pragma once

#include "../node_handle.h"
#include "bag.h"
#include "../util/thread.h"

namespace mind_os
{

class Player : util::Thread, NoCopy
{
public:
    Player(NodeHandle& nh, Bag* bag);

    template <typename T>
    void advertise(std::string topic)
    {
        auto pub = nh.advertise<T>(topic);

        auto func = std::bind([](BytesBuffer& buf, Publisher pub){
            auto msg = std::make_shared<T>();
            deserialize<T>(buf, *msg);
            pub.publish(msg);
        }, std::placeholders::_1, pub);

        std::lock_guard<std::mutex> guard(mtxPubs);
        pubs.insert(std::make_pair(topic, func));
    }

    void unadvertise(std::string toipic);
    bool start(std::function<void()> finished = nullptr, double mpPlay = 1.0);
    bool stop();
    bool pause();
    bool resume();

protected:
    void run() override;

private:
    Bag* bag;
    bool running;
    std::atomic_bool started;
    NodeHandle& nh;
    std::mutex mtxPubs;
    std::unordered_map<std::string, std::function<void(BytesBuffer&)>> pubs;

    std::uint64_t startTime;
    double mpPlay;
    std::function<void()> finished;
};

}



