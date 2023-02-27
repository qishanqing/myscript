#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>

#include "msg/DataMsg.h"

using namespace tutorial::record;

class SubscriberNode : public mind_os::NodePlugin
{
    mind_os::Subscriber subImu;
    mind_os::Subscriber subImu2;

public:
    SubscriberNode() {}

    void OnDataMsg(mind_os::ConstPtr<DataMsg>& msg)
    {
        auto latency = (mind_os::util::now() - msg->stamp);
        if (latency < 100)
        {
            LOG(INFO)  << "received data: " << msg->data << " latency: " << (latency) << " us";
        }
        else
        {
            LOG(WARNING) << "received data: " << msg->data << ", latency: " << (latency) << " us!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        }

        // for (auto& v : msg->inner_vector_data)
        // {
        //     LOG(INFO)  << "received inner vector:";
        //     for (auto& d : v.vector_data)
        //     {
        //         LOG(INFO)  << "data:" << d;
        //     }
        // }
    };

    void OnDataMsg2(mind_os::ConstPtr<DataMsg>& msg)
    {
        auto latency = (mind_os::util::now() - msg->stamp);
        if (latency < 100)
        {
            LOG(INFO)  << "received data2: " << msg->data << " latency: " << (latency) << " us";
        }
        else
        {
            LOG(WARNING) << "received data2: " << msg->data << ", latency: " << (latency) << " us!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        }

        // for (auto& v : msg->inner_vector_data)
        // {
        //     LOG(INFO)  << "received inner vector:";
        //     for (auto& d : v.vector_data)
        //     {
        //         LOG(INFO)  << "data2:" << d;
        //     }
        // }
    };

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();
        subImu = nh.subscribe<DataMsg>("/data", &SubscriberNode::OnDataMsg, this);
        subImu2 = nh.subscribe<DataMsg>("/data2", &SubscriberNode::OnDataMsg2, this);
        // subImu = nh.subscribe("/data", std::bind(&SubscriberNode::OnDataMsg, this, std::placeholders::_1));
        // subImu = nh.subscribe("/data", [this](){});
        // subImu = nh.subscribe<DataMsg>("/data", [this](mind_os::ConstPtr<DataMsg>& msg){});

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        subImu.shutdown();
        subImu2.shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SubscriberNode)
