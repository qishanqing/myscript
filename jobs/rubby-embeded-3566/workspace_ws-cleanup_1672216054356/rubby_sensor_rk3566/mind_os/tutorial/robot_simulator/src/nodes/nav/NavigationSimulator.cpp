/*
 * NavigationSimulator.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>

#include "EncoderData.h"
#include "ImuData.h"
#include "LaserScan.h"
#include "SlamResult.h"
#include "OccupancyGrid.h"
#include "GetPoseSrv.h"
#include "StartNavSrv.h"

using namespace tutorial::slam;
using namespace tutorial::nav;

class NavigationSimulator : public mind_os::NodePlugin
{
    mind_os::Subscriber subPose;
    mind_os::Subscriber subMap;
    mind_os::util::LoopThread* getPoseThread;
    mind_os::ServiceClient scGetPose;
    mind_os::ServiceServer ssStartNav;

public:
    NavigationSimulator()
    {
        auto& nh = getPrivateNodeHandle();

        subPose = nh.subscribe<SlamResult>("/pose",
                std::bind(&NavigationSimulator::OnPose, this, std::placeholders::_1));

        subMap = nh.subscribe<OccupancyGrid>("/map",
                std::bind(&NavigationSimulator::OnMap, this, std::placeholders::_1));

        scGetPose = nh.serviceClient<GetPoseSrv>("/get_pose");

        ssStartNav = nh.advertiseService<StartNavSrv>("/start_nav",
                std::bind(&NavigationSimulator::OnStartNav, this, std::placeholders::_1, std::placeholders::_2));

        getPoseThread = new mind_os::util::LoopThread(25, [this](){
            GetPoseSrv srv;
            srv.request.timestamp = mind_os::util::now();
            if (scGetPose.call(srv))
            {
                LOG(INFO) << "success to get pose, time:" << srv.response.pose.header.stamp 
                    << ", latency: " << (srv.response.pose.header.stamp - srv.request.timestamp) << " us";
            }
            else
            {
                LOG(INFO) << "failed to get pose";
            }

            mind_os::main_thread::Event event([&](){
                auto req = std::make_shared<GetPoseSrv::Request>();
                auto resp = std::make_shared<GetPoseSrv::Response>();
                req->timestamp = mind_os::util::now();
                if (scGetPose.call(req, resp))
                {
                    LOG(INFO) << "success to get pose in main thread, time:" << resp->pose.header.stamp 
                        << ", latency: " << (resp->pose.header.stamp - req->timestamp) << " us";
                }
                else
                {
                    LOG(INFO) << "failed to get pose in main thread";
                }
            });

            LOG(INFO) << "wait for get pose in main thread.";
            mind_os::main_thread::wait(event);
            LOG(INFO) << "finished waiting for get pose in main thread.";
        });
    }

    void OnStartNav(mind_os::ConstPtr<StartNavSrv::Request>& request, mind_os::Ptr<StartNavSrv::Response>& response)
    {
        GetPoseSrv srv;
        srv.request.timestamp = mind_os::util::now();
        if (scGetPose.call(srv))
        {
            LOG(INFO) << "first success to get pose, time:" << srv.response.pose.header.stamp 
                << ", latency: " << (srv.response.pose.header.stamp - srv.request.timestamp) << " us";
        }
        else
        {
            LOG(INFO) << "first failed to get pose";
        }

        getPoseThread->start();
    }

    void OnPose(mind_os::ConstPtr<SlamResult>& msg)
    {
        auto lan = (mind_os::util::now() - msg->header.stamp);
        if (lan > 600)
        {
            LOG(INFO) << "Nav received pose, time:"  << msg->header.stamp << ", latency: " << lan << " us!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        }
        else
        {
            LOG(INFO) << "Nav received pose, time:"  << msg->header.stamp << ", latency: " << lan << " us";
        }
    };

    void OnMap(mind_os::ConstPtr<OccupancyGrid>& msg)
    {
        LOG(INFO) << "Nav received map, time:"  << msg->header.stamp 
            << ", latency: " << (mind_os::util::now() - msg->header.stamp)  << " us xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    };

    void onLoaded() override
    {
        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        getPoseThread->stop();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(NavigationSimulator)
