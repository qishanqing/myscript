/*
 * RosNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <ros/ros.h>
#include <glog/logging.h>

class RosNode : public mind_os::NodePlugin
{
    mind_os::Subscriber subPose;
    mind_os::Subscriber subMap;
    std::shared_ptr<ros::NodeHandle> nh;

public:
    RosNode()
    {
        auto argc = mind_os::main_thread::argc();
        auto argv = mind_os::main_thread::argv();
        ros::init(argc, argv, "ros_node", ros::init_options::NoSigintHandler);
        nh = std::make_shared<ros::NodeHandle>();

        LOG(INFO) << "success to start ros." << std::endl;
    }


    void onLoaded() override
    {
        std::thread([this](){
            ros::spin();
        }).detach();
        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        ros::shutdown();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(RosNode)
