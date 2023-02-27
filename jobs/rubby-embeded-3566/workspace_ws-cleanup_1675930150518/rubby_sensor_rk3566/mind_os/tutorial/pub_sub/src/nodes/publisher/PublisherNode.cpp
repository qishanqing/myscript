#include <mind_os/mind_os.h>
#include "msg/DataMsg.h"
#include <glog/logging.h>
#include <iostream>
#include "A.h"
#include "B.h"

using namespace tutorial::pub_sub;

class PublisherNode : public mind_os::NodePlugin
{
    mind_os::util::LoopThread* ltPub;

    mind_os::Publisher pubData;

public:
    PublisherNode()  {}

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();

        pubData = nh.advertise<DataMsg>("/data");

        ltPub = new mind_os::util::LoopThread(1, [this](){
            static std::uint64_t data = 0;
            auto msg = std::make_shared<DataMsg>();
            msg->data = ++data;
            InnerVector iv1, iv2;
            for (int i = 0; i < 10; ++i)
            {
                msg->vector_data_1.push_back(data + i);
                iv1.vector_data.push_back(data + i);
                iv2.vector_data.push_back(data + i);
            }
            msg->vector_data_2.push_back(iv1.vector_data);
            msg->vector_data_2.push_back(iv2.vector_data);
            msg->vector_data_3.push_back(iv1);
            msg->vector_data_3.push_back(iv2);
            msg->stamp = mind_os::util::now();
            pubData.publish(msg);
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
