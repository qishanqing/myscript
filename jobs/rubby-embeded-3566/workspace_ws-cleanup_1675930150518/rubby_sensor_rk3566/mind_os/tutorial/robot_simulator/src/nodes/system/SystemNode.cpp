/*
 * HMISimulator.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <glog/logging.h>

class SystemNode : public mind_os::NodePlugin
{

public:
    SystemNode()
    {
    }

    void onLoaded() override
    {
        // FLAGS_minloglevel = 2;
        mind_os::main_thread::loaded([this](){
            LOG(INFO) << "set start.";
            auto& nh = getPrivateNodeHandle();
            LOG(INFO) << "set finished: " << nh.setParam("test.v", 1);
        });
        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SystemNode)
