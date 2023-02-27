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

public:
    ClientNode()
    {
        auto& nh = getPrivateNodeHandle();

        scTest = nh.serviceClient<TestSrv>("/test");
    }

    void onLoaded() override
    {
        ltCall = new mind_os::util::LoopThread(2, [this](){
            static std::int32_t param = 0;
            TestSrv srv;
            srv.request.param = ++param;
            InnerVector iv1, iv2;
            static int data = 0;
            srv.response.result = 1111111;
            ++data;
            for (int i = 0; i < 10000; ++i)
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
                // for (auto& v : srv.response.inner_vector_data)
                // {
                //     LOG(INFO)  << "result inner vector:";
                //     for (auto& d : v.vector_data)
                //     {
                //         LOG(INFO)  << "data:" << d;
                //     }
                // }
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
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ClientNode)
