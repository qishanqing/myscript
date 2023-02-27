#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>
#include "msg/ErrorEvent.h"

using namespace tutorial::event;

class SubscriberNode : public mind_os::NodePlugin
{
    mind_os::Subscriber sub;

public:
    SubscriberNode() {}

    void OnErrorEvent(mind_os::ConstPtr<ErrorEvent>& msg)
    {
        switch (msg->eventId)
        {
            case ERROR_1:
                LOG(INFO) << "received ERROR_1, state: " << msg->on;
                break;
            case ERROR_2_1:
                LOG(INFO) << "received ERROR_2_1, state: " << msg->on;
                break;
            case ERROR_2_2:
                LOG(INFO) << "received ERROR_2_2, state: " << msg->on;
                break;
            case ERROR_3_1:
                LOG(INFO) << "received ERROR_3_1, state: " << msg->on;
                break;
            case ERROR_3_2:
                LOG(INFO) << "received ERROR_3_2, state: " << msg->on;
                break;
        }
        
    };

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();
        sub = nh.subscribe<ErrorEvent>("/error", &SubscriberNode::OnErrorEvent, this);

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        sub.shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SubscriberNode)
