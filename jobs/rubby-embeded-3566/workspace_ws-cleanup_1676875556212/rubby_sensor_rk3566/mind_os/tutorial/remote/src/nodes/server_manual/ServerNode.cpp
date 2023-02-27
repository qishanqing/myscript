#include <mind_os/mind_os.h>
#include "msg/DataMsg.h"
#include <glog/logging.h>
#include <iostream>
#include "srv/TestSrv.h"

using tutorial::pub_sub::DataMsg;
using tutorial::srv_call::TestSrv;

class ServerNode : public mind_os::NodePlugin
{
    mind_os::RemoteNodeHandle nhRemote;
    mind_os::util::LoopThread* ltPub;
    mind_os::Publisher pubRemoteData, pubRemoteData2;
    mind_os::Publisher pubPrivateData, pubPrivateData2;
    mind_os::ServiceServer ssStartNav;
    mind_os::Subscriber subData, subData2;

public:
    ServerNode()  {}

    void OnTest(mind_os::ConstPtr<TestSrv::Request>& request, mind_os::Ptr<TestSrv::Response>& response)
    {
        response->success = true;
        response->result = request->param;
        // for (auto& v : request->inner_vector_data)
        // {
        //     LOG(INFO)  << "received inner vector:";
        //     for (auto& d : v.vector_data)
        //     {
        //         LOG(INFO)  << "data:" << d;
        //     }
        // }
        response->inner_vector_data = request->inner_vector_data;
        LOG(INFO) << "service called, param:" << request->param
            << ", time cost: " << ( mind_os::util::now() > request->stamp ? mind_os::util::now() - request->stamp : 0) << " us.";
    }

    void OnDataMsg(mind_os::ConstPtr<DataMsg>& msg)
    {
        LOG(INFO)  << "try to dispatch data to remote: " << msg->data;
        pubRemoteData.publish(msg);
    };

    void OnDataMsg2(mind_os::ConstPtr<DataMsg>& msg)
    {
        LOG(INFO)  << "try to dispatch data2 to remote: " << msg->data;
        pubRemoteData2.publish(msg);
    };

    void onLoaded() override
    {
        mind_os::RemoteSetting setting;
        setting.port = 20001;
        setting.protocol = mind_os::RemoteSetting::TCP1V1;
        setting.sizeOfCompressLimited = 0;
        auto rlt = nhRemote.startAsServer(setting, [this](bool connected){
            LOG(INFO) << "connected:" << connected;
            if (connected)
            {
                auto& nhPrivate = getPrivateNodeHandle();
                subData = nhPrivate.subscribe<DataMsg>("/data", &ServerNode::OnDataMsg, this);
                subData2 = nhPrivate.subscribe<DataMsg>("/data2", &ServerNode::OnDataMsg2, this);
            }
            else
            {
                subData.shutdown();
                subData2.shutdown();
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

        pubRemoteData = nhRemote.advertise<DataMsg>("/data");
        pubRemoteData2 = nhRemote.advertise<DataMsg>("/data2");
        

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

        ssStartNav = nhRemote.advertiseService<TestSrv>("/test", &ServerNode::OnTest, this);

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        ltPub->stop();
        ssStartNav.shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ServerNode)
