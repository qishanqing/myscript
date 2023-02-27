/*
 * RosSubscriberNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <sensor_msgs/LaserScan.h>
#include <ros/ros.h>
#include <glog/logging.h>

class RosSubscriberNode : public mind_os::NodePlugin
{
    std::shared_ptr<ros::NodeHandle> nh;

    ros::Subscriber subScan;

public:
    RosSubscriberNode()
    {
        nh = std::make_shared<ros::NodeHandle>();
    }


    void onLoaded() override
    {
        subScan = nh->subscribe<sensor_msgs::LaserScan>("/scan", 1000,
                [this](const sensor_msgs::LaserScan::ConstPtr& msg){
            LOG(INFO) << "received laser frame.";
        });
    }

    void onUnloaded() override
    {
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(RosSubscriberNode)
