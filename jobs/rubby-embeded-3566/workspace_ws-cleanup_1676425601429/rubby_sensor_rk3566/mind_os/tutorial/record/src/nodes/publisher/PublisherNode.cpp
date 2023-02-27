#include <mind_os/mind_os.h>
#include "msg/DataMsg.h"
#include <glog/logging.h>
#include <iostream>

using namespace tutorial::record;

class PublisherNode : public mind_os::NodePlugin
{
    mind_os::util::LoopThread* ltPub;

    mind_os::Publisher pubData, pubData2;

public:
    PublisherNode()  {}

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();

        pubData = nh.advertise<DataMsg>("/data");
        pubData2 = nh.advertise<DataMsg>("/data2");

        ltPub = new mind_os::util::LoopThread(5, [this](){
            static std::uint64_t data = 0;
            auto msg = std::make_shared<DataMsg>();
            msg->stamp = mind_os::util::now();
            msg->data = ++data;
            InnerVector iv1, iv2;
            for (int i = 0; i < 10000; ++i)
            {
                iv1.vector_data.push_back(data + i);
                iv2.vector_data.push_back(data + i);
            }
            msg->inner_vector_data.push_back(iv1);
            msg->inner_vector_data.push_back(iv2);
            pubData.publish(msg);
            LOG(INFO) << "publish data: " << msg->data;
            pubData2.publish(msg);
            LOG(INFO) << "publish data2: " << msg->data;
        });

        ltPub->start();

        LOG(INFO) << "Start.";
    }   

    void onUnloaded() override
    {
        ltPub->stop();

        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(PublisherNode)