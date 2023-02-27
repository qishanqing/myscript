#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>
#include "msg/DataMsg.h"

using namespace tutorial::pub_sub;

class SubscriberNode : public mind_os::NodePlugin
{
    mind_os::Subscriber subData, subData2;
    mind_os::util::LoopThread* lp;

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

        LOG(INFO) << "vector_data_1:";
        LOG(INFO)  << "size:" << msg->vector_data_1.size();
        for (auto& v : msg->vector_data_1)
        {
            LOG(INFO)  << "  data:" << v;
        }

        LOG(INFO) << "vector_data_2:";
        LOG(INFO)  << "size:" << msg->vector_data_2.size();
        for (auto& v : msg->vector_data_2)
        {
            LOG(INFO)  << "    size: " << v.size();
            for (auto& d : v)
            {
                LOG(INFO)  << "    data:" << d;
            }
        }

        LOG(INFO) << "vector_data_3:";
        LOG(INFO)  << "size:" << msg->vector_data_3.size();
        for (auto& v : msg->vector_data_3)
        {
            LOG(INFO)  << "    size: " << v.vector_data.size();
            for (auto& d : v.vector_data)
            {
                LOG(INFO)  << "    data:" << d;
            }
        }
    };

    void OnDataMsg2(mind_os::ConstPtr<DataMsg>& msg)
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
    };

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();
        subData2 = nh.subscribe<DataMsg>("/data", &SubscriberNode::OnDataMsg2, this);
        subData = nh.subscribe<DataMsg>("/data", &SubscriberNode::OnDataMsg, this);
        // subData = nh.subscribe("/data", std::bind(&SubscriberNode::OnDataMsg, this, std::placeholders::_1));
        // subData = nh.subscribe("/data", [this](){});
        // subData = nh.subscribe<DataMsg>("/data", [this](mind_os::ConstPtr<DataMsg>& msg){});

        lp = new mind_os::util::LoopThread(0.5, [this](){
            static bool flag = true;
            if (flag)
            {
                subData.shutdown();
                LOG(INFO) << "shutdown";
                flag = false;
            }
            else
            {
                auto& nh = getPrivateNodeHandle();
                subData = nh.subscribe<DataMsg>("/data", &SubscriberNode::OnDataMsg, this);
                LOG(INFO) << "subscribe again";
                flag = true;
            }
        });
        // lp->start();

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        // lp->stop();
        subData.shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SubscriberNode)
