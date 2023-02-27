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
        auto& nh = getPrivateNodeHandle();
        nh.setParam("test.v", 1);
        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SystemNode)
