#include <mind_os/mind_os.h>
#include "msg/DataMsg.h"
#include <glog/logging.h>
#include <iostream>
#include "A.h"
#include "B.h"
// #include "msg/Map.h"

using namespace tutorial::pub_sub;

class PublisherNode : public mind_os::NodePlugin
{
    mind_os::util::LoopThread* ltPub;

    mind_os::Publisher pubData;

public:
    PublisherNode()  {}

    void onLoaded() override
    {
        std::unordered_map<int, int> m, n;
        m.insert(std::make_pair(1, 2));
        m.insert(std::make_pair(2, 3));
        LOG(INFO) << "Test m size:" << m.size();
        mind_os::BytesBuffer buffer;
        mind_os::serialize(buffer, m);
        buffer.Reset();
        LOG(INFO) << "Test buffer size:" << buffer.Size();
        mind_os::deserialize(buffer, n);
        LOG(INFO) << "Test n size:" << n.size();
        LOG(INFO) << "Test:" << n[1];
        LOG(INFO) << "Test:" << n[2];

        int a[3] = {1, 2, 3};
        mind_os::BytesBuffer buf2;
        mind_os::serialize(buf2, a, 3);
        buf2.Reset();
        mind_os::deserialize(buf2, a, 3);
        LOG(INFO) << "Test a:" << a[0];
        LOG(INFO) << "Test a:" << a[1];
        LOG(INFO) << "Test a:" << a[2];

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
