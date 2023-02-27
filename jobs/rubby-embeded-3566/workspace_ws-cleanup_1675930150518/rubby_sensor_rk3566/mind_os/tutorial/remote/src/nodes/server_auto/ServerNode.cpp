#include <mind_os/mind_os.h>
#include "msg/DataMsg.h"
#include <glog/logging.h>
#include <iostream>
#include "srv/TestSrv.h"

using tutorial::pub_sub::DataMsg;
using tutorial::srv_call::TestSrv;

class ServerNode : public mind_os::NodePlugin
{
    mind_os::RemoteNodeHandle* pNhRemote;
    mind_os::util::LoopThread* ltPub;
    mind_os::Publisher pubRemoteData, pubRemoteData2;
    mind_os::Publisher pubPrivateData, pubPrivateData2;
    mind_os::ServiceServer ssRemoteTest;
    mind_os::ServiceServer ssPrivateTest;

public:
    ServerNode()  {}

    void OnTest(mind_os::ConstPtr<TestSrv::Request>& request, mind_os::Ptr<TestSrv::Response>& response)
    {
        response->success = true;
        response->result = request->param;
        response->inner_vector_data = request->inner_vector_data;
        LOG(INFO) << "service called, param:" << request->param
            << ", time cost: " << ( mind_os::util::now() > request->stamp ? mind_os::util::now() - request->stamp : 0) << " us.";
    }

    void onLoaded() override
    {
        pNhRemote = new mind_os::RemoteNodeHandle(getPrivateNodeHandle());
        mind_os::RemoteSetting setting;
        setting.port = 20001;
        setting.protocol = mind_os::RemoteSetting::TCP1V1;
        setting.sizeOfCompressLimited = 0;
        auto rlt = pNhRemote->startAsServer(setting, [this](bool connected){
            LOG(INFO) << "connected:" << connected;
            if (connected)
            {
                pubRemoteData = pNhRemote->advertise<DataMsg>("/data");
                pubRemoteData2 = pNhRemote->advertise<DataMsg>("/data2");
            }
            else
            {
                pubRemoteData.shutdown();
                pubRemoteData2.shutdown();
            }
            
        });
        if (!rlt)
        {
            LOG(INFO) << "Failed to start server.";
            return;
        }

        auto& nhPrivate = getPrivateNodeHandle();
        pubPrivateData = nhPrivate.advertise<DataMsg>("/data");
        pubPrivateData2 = nhPrivate.advertise<DataMsg>("/data2");

        ltPub = new mind_os::util::LoopThread(5, [this](){
            static std::uint64_t data = 0;
            auto msg = std::make_shared<DataMsg>();
            msg->data = ++data;
            tutorial::pub_sub::InnerVector iv1, iv2;
            for (int i = 0; i < 10000; ++i)
            {
                iv1.vector_data.push_back(data + i);
                iv2.vector_data.push_back(data + i);
            }
            msg->inner_vector_data.push_back(iv1);
            msg->inner_vector_data.push_back(iv2);
            msg->stamp = mind_os::util::now();
            pubPrivateData.publish(msg);
            pubPrivateData2.publish(msg);
        });

        ltPub->start();

        ssPrivateTest = nhPrivate.advertiseService<TestSrv>("/test", &ServerNode::OnTest, this);
        ssRemoteTest = pNhRemote->advertiseService<TestSrv>("/test");

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        ltPub->stop();
        ssRemoteTest.shutdown();
        delete pNhRemote;
        delete ltPub;
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ServerNode)
