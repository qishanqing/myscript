#include <mind_os/mind_os.h>
#include "msg/ErrorEvent.h"
#include <glog/logging.h>
#include <iostream>

using namespace tutorial::event;

#define RegisterEventTimeout(handle, id, timeout) \
        handle->addPublishTrigger<ErrorEvent>(name, \
            [](mind_os::ConstPtr<ErrorEvent>& msg) { \
                return msg->eventId == id; \
            }, \
            uint64_t(timeout), \
            [this]() { \
                ErrorEvent e = {id, false}; \
                pubData.publish(e); \
            }, \
            [](mind_os::ConstPtr<ErrorEvent>& msg) { \
                return !msg->on; \
            } \
        )

#define RegisterEventCondition(handle, id, condition) \
        handle->addPublishTrigger<ErrorEvent>(name, \
            [](mind_os::ConstPtr<ErrorEvent>& msg) { \
                return msg->eventId == id; \
            }, \
            condition, \
            [this]() { \
                ErrorEvent e = {id, false}; \
                pubData.publish(e); \
            }, \
            [](mind_os::ConstPtr<ErrorEvent>& msg) { \
                return !msg->on; \
            } \
        )

class PublisherNode : public mind_os::NodePlugin
{
    mind_os::util::LoopThread* ltPub;

    mind_os::Publisher pubData;

    mind_os::EventHandle* pEh;

public:
    PublisherNode()  {}

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();
        pEh = new mind_os::EventHandle(nh);

        std::string name = "/error";
        pubData = pEh->advertise<ErrorEvent>(name);


        auto flag = std::make_shared<int>();
        *flag = 0;
        RegisterEventTimeout(pEh, EventId::ERROR_2_1, 3 * 1000 * 1000);
        RegisterEventTimeout(pEh, EventId::ERROR_2_2, 1 * 1000 * 1000);
        RegisterEventCondition(pEh, EventId::ERROR_3_1, [flag](){
            return *flag == 1;
        });

        std::thread([this](){
            auto msg = std::make_shared<ErrorEvent>();
            msg->eventId = EventId::ERROR_1;
            msg->on = true;
            LOG(INFO) << "publish ERROR_1 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "publish ERROR_1 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "publish ERROR_1 " << msg->on;
            pubData.publish(msg);
        }).detach();

        std::thread([this](){
            auto msg = std::make_shared<ErrorEvent>();
            msg->eventId = EventId::ERROR_2_1;
            msg->on = true;
            LOG(INFO) << "publish ERROR_2_1 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "publish ERROR_2_1 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "publish ERROR_2_1 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // msg->on = false;
            // pubData.publish(msg);
        }).detach();

        std::thread([this](){
            auto msg = std::make_shared<ErrorEvent>();
            msg->eventId = EventId::ERROR_2_2;
            msg->on = true;
            LOG(INFO) << "publish ERROR_2_2 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "publish ERROR_2_2 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "publish ERROR_2_2 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            msg->on = false;
            LOG(INFO) << "publish ERROR_2_2 " << msg->on;
            pubData.publish(msg);
        }).detach();

        std::thread(std::bind([this](std::shared_ptr<int> flag){
            auto msg = std::make_shared<ErrorEvent>();
            msg->eventId = EventId::ERROR_3_1;
            msg->on = true;
            LOG(INFO) << "publish ERROR_3_1 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "publish ERROR_3_1 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "publish ERROR_3_1 " << msg->on;
            pubData.publish(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "set flag = 1 ";
            *flag = 1;
            pubData.publish(msg);
        }, flag)).detach();


        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        // ltPub->stop();
        delete pEh;

        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(PublisherNode)
