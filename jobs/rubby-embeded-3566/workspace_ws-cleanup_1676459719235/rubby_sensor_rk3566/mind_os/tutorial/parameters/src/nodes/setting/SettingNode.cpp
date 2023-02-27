/*
 * SettingNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <glog/logging.h>

class SettingNode : public mind_os::NodePlugin
{
    mind_os::util::LoopThread* ltCall;
    mind_os::ServiceClient scTest;

public:
    SettingNode() = default;

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();

        std::string path = "/root/config";
        nh.setParam("setting.config.path", path);
        LOG(INFO) << "set setting.config.path to " << path;

        int int_value = 123;
        nh.setParam("setting.config.int_value", int_value);
        LOG(INFO) << "set setting.config.int_value to " << int_value;

        double double_value = 321.0;
        nh.setParam("setting.config.double_value", double_value);
        LOG(INFO) << "set setting.config.double_value to " << double_value;

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SettingNode)