/*
 * ServerNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>

#include "srv/TestSrv.h"

using namespace tutorial::srv_call;

class ServerNode : public mind_os::NodePlugin
{
    mind_os::ServiceServer ssStartTest;
    mind_os::ServiceClient scTest;

public:
    ServerNode() {}

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
        LOG(INFO) << "test service called, param:" << request->param
            << ", lantency: " << mind_os::util::now() - request->stamp << " us.";

        // call test2
        static std::int32_t param = 0;
        TestSrv srv;
        srv.request.param = ++param;
        InnerVector iv1, iv2;
        static int data = 0;
        srv.response.result = 1111111;
        ++data;
        // for (int i = 0; i < 10000; ++i)
        // {
        //     iv1.vector_data.push_back(data + i);
        // }

        iv2 = iv1;
        srv.request.inner_vector_data.push_back(iv1);
        srv.request.inner_vector_data.push_back(iv2);
        srv.request.stamp = mind_os::util::now();
        auto&& rlt = scTest.call(srv);
        auto endStamp = mind_os::util::now();
        if (rlt)
        {
            LOG(INFO) << "result of call /test2:" << srv.response.success << ", result:" << srv.response.result;
        }
        else
        {
            LOG(INFO) << "failed to call /test2.";
        }

        LOG(INFO) << "time cost:" << endStamp - srv.request.stamp << " us.";
    }

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();
        scTest = nh.serviceClient<TestSrv>("/test2");

        ssStartTest = nh.advertiseService<TestSrv>("/test", &ServerNode::OnTest, this);
        // ssStartTest = nh.advertiseService<TestSrv>("/test", 
        //     [this](mind_os::ConstPtr<TestSrv::Request>& request, mind_os::Ptr<TestSrv::Response>& response){});
        // ssStartTest = nh.advertiseService<TestSrv>("/test",
        //         std::bind(&ServerNode::OnTest, this, std::placeholders::_1, std::placeholders::_2));

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        ssStartTest.shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ServerNode)
