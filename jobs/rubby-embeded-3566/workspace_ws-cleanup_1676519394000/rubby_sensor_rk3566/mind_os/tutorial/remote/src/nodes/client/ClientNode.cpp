#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>
#include "msg/DataMsg.h"
#include "srv/TestSrv.h"

using tutorial::pub_sub::DataMsg;
using tutorial::srv_call::TestSrv;

class ClientNode : public mind_os::NodePlugin
{
    mind_os::RemoteNodeHandle nhRemote;
    mind_os::Subscriber subRemoteData, subRemoteData2;
    mind_os::util::LoopThread* ltCall;
    mind_os::ServiceClient scTest;

public:
    ClientNode() 
    {
    }

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


    void onConnected(bool connected)
    {
        LOG(INFO) << "connected:" << connected;
        if (connected && !ltCall->isRunning()) {
            subRemoteData = nhRemote.subscribe<DataMsg>("/data", &ClientNode::OnDataMsg, this);
            subRemoteData2 = nhRemote.subscribe<DataMsg>("/data2", &ClientNode::OnDataMsg2, this);
            scTest = nhRemote.serviceClient<TestSrv>("/test");
            ltCall->start();
        }
        else if (!connected)
        {
            ltCall->stop();
            nhRemote.stop();
            subRemoteData.shutdown();
            subRemoteData2.shutdown();
            connectServer();
        }
    }

    void onTimeout()
    {
        LOG(INFO) << "Timeout to start client.";
        nhRemote.stop();
        connectServer();
    }

    void connectServer()
    {
        mind_os::RemoteSetting setting;
        setting.ip = "127.0.0.1";
        // setting.ip = "192.168.50.253";
        setting.port = 20001;
        setting.protocol = mind_os::RemoteSetting::TCP1V1;
        setting.retry = 5;
        setting.interval = 1000;
        auto rlt = nhRemote.startAsClient(setting, 
            std::bind(&ClientNode::onConnected, this, std::placeholders::_1),
            std::bind(&ClientNode::onTimeout, this)
            );
        if (!rlt)
        {
            LOG(INFO) << "Failed to start client.";
        }
    }

    void onLoaded() override
    {
        ltCall = new mind_os::util::LoopThread(2, [this](){
            static std::int32_t param = 0;
            TestSrv srv;
            srv.request.param = ++param;
            tutorial::srv_call::InnerVector iv1, iv2;
            static int data = 0;
            srv.response.result = 1111111;
            ++data;
            for (int i = 0; i < 100; ++i)
            {
                iv1.vector_data.push_back(data + i);
            }

            iv2 = iv1;
            srv.request.inner_vector_data.push_back(iv1);
            srv.request.inner_vector_data.push_back(iv2);
            srv.request.stamp = mind_os::util::now();
            
            auto&& rlt = scTest.call(srv);
            auto endStamp = mind_os::util::now();
            if (rlt)
            {
                LOG(INFO) << "result of call /test:" << srv.response.success << ", result:" << srv.response.result;
            }
            else
            {
                LOG(INFO) << "failed to call /test.";
            }

            LOG(INFO) << "time cost:" << endStamp - srv.request.stamp << " us.";
        });

        connectServer();

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ClientNode)
