/*
 * ClientNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <thread>

#include "srv/TestSrv.h"

using namespace tutorial::srv_call;

class ClientNode : public mind_os::NodePlugin
{
    mind_os::util::LoopThread* ltCall;
    mind_os::ServiceClient scTest;
    mind_os::ServiceServer ssStartTest2;

public:
    ClientNode()
    {
        auto& nh = getPrivateNodeHandle();

        scTest = nh.serviceClient<TestSrv>("/test");
    }

    void OnTest2(mind_os::ConstPtr<TestSrv::Request>& request, mind_os::Ptr<TestSrv::Response>& response)
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
        LOG(INFO) << "test2: service called, param:" << request->param
            << ", lantency: " << mind_os::util::now() - request->stamp << " us.";
    }

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();
        ssStartTest2 = nh.advertiseService<TestSrv>("/test2", &ClientNode::OnTest2, this);

        ltCall = new mind_os::util::LoopThread(2, [this](){
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
                LOG(INFO) << "result of call /test:" << srv.response.success << ", result:" << srv.response.result;
            }
            else
            {
                LOG(INFO) << "failed to call /test.";
            }

            LOG(INFO) << "time cost:" << endStamp - srv.request.stamp << " us.";
        });
        ltCall->start();
        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        ltCall->stop();
        ssStartTest2.shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ClientNode)
