/*
 * HMISimulator.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <thread>

#include "StartNavSrv.h"

using namespace tutorial::nav;

class HMISimulator : public mind_os::NodePlugin
{

    mind_os::ServiceClient scStartNav;

public:
    HMISimulator()
    {
        auto& nh = getPrivateNodeHandle();

        scStartNav = nh.serviceClient<StartNavSrv>("/start_nav");
    }

    void onLoaded() override
    {
        std::thread([this](){
            StartNavSrv srv;
            srv.request.mode = Mode::AUTO;
            scStartNav.call(srv);
        }).detach();
        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(HMISimulator)
