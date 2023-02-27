#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>
#include "msg/DataMsg.h"

using namespace tutorial::pub_sub;

class SubscriberNode : public mind_os::NodePlugin
{
    mind_os::Subscriber subImu;

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

        LOG(INFO)  << "received vector size:" << msg->inner_vector_data.size();

        for (auto& v : msg->inner_vector_data)
        {
            LOG(INFO)  << "vector size: " << v.vector_data.size() << ", first elem: " << v.vector_data[0];
            // for (auto& d : v.vector_data)
            // {
            //     LOG(INFO)  << "data:" << d;
            // }
        }
    };

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();
        subImu = nh.subscribe<DataMsg>("/data", &SubscriberNode::OnDataMsg, this);
        // subImu = nh.subscribe("/data", std::bind(&SubscriberNode::OnDataMsg, this, std::placeholders::_1));
        // subImu = nh.subscribe("/data", [this](){});
        // subImu = nh.subscribe<DataMsg>("/data", [this](mind_os::ConstPtr<DataMsg>& msg){});

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        subImu.shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SubscriberNode)
