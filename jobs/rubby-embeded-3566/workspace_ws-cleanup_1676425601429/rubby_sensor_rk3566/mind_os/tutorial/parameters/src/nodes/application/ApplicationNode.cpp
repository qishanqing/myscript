/*
 * ApplicationNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>

#include "srv/TestSrv.h"

class ApplicationNode : public mind_os::NodePlugin
{
    mind_os::ServiceServer ssStartNav;
    mind_os::util::LoopThread* ltCall;

public:
    ApplicationNode() = default;

    void onLoaded() override
    {
        ltCall = new mind_os::util::LoopThread(2, [this](){
            auto& nh = getPrivateNodeHandle();
            
            auto start_time = mind_os::util::now();
            {
                // get string
                std::string path;
                auto&& rlt = nh.getParam("setting.config.path", path);
                if (rlt)
                {
                    LOG(INFO) << "success to get path: " << path;
                }
                else
                {
                    LOG(INFO) << "failed to get path.";
                }
            }
            {
                // get int
                int int_value;
                auto&& rlt = nh.getParam("setting.config.int_value", int_value);
                if (rlt)
                {
                    LOG(INFO) << "success to get int_value: " << int_value;
                }
                else
                {
                    LOG(INFO) << "failed to get int_value.";
                }
            }
            {
                // set double
                double double_value;
                auto&& rlt = nh.getParam("setting.config.double_value", double_value);
                if (rlt)
                {
                    LOG(INFO) << "success to get double_value: " << double_value;
                }
                else
                {
                    LOG(INFO) << "failed to get double_value.";
                }
            }

            LOG(INFO) << "time cost:" << mind_os::util::now() - start_time << " us.";
        });
        ltCall->start();

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ApplicationNode)
