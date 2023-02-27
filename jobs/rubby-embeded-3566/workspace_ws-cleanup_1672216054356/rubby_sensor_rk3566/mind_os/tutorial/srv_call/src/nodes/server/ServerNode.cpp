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
    mind_os::ServiceServer ssStartNav;

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
        LOG(INFO) << "service called, param:" << request->param
            << ", lantency: " << mind_os::util::now() - request->stamp << " us.";
    }

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();

        ssStartNav = nh.advertiseService<TestSrv>("/test", &ServerNode::OnTest, this);
        // ssStartNav = nh.advertiseService<TestSrv>("/test", 
        //     [this](mind_os::ConstPtr<TestSrv::Request>& request, mind_os::Ptr<TestSrv::Response>& response){});
        // ssStartNav = nh.advertiseService<TestSrv>("/test",
        //         std::bind(&ServerNode::OnTest, this, std::placeholders::_1, std::placeholders::_2));

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        ssStartNav.shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ServerNode)
