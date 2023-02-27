#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>

#include "EncoderData.h"
#include "ImuData.h"
#include "LaserScan.h"
#include "SlamResult.h"
#include "GetPoseSrv.h"
#include "depends/Depends.h"
#include "OccupancyGrid.h"

using namespace tutorial::slam;
using namespace tutorial::sensor;

class SlamSimulator : public mind_os::NodePlugin
{
    mind_os::Subscriber subImu;
    mind_os::Subscriber subLaserScan;
    mind_os::Subscriber subEncoder;

    mind_os::util::LoopThread* slamResultThread;
    mind_os::util::LoopThread* mapThread;

    mind_os::Publisher pubSlamResult;
    mind_os::Publisher pubMap;

    mind_os::ServiceServer ssGetPose;

    //test
    std::uint64_t total;
    std::uint64_t lac;
    std::uint64_t highest;

public:
    SlamSimulator()
    {
        Depends d;
        d.TestDepends();
        total = 0;
        lac = 0;
        highest = 0;
        auto& nh = getPrivateNodeHandle();

//        subImu = nh.subscribe<ImuData>("/imu",
//                std::bind(&SlamSimulator::OnImu, this, std::placeholders::_1));

        subImu = nh.subscribe<ImuData>("/imu", &SlamSimulator::OnImu, this);

        subLaserScan = nh.subscribe<LaserScan>("/scan",
                std::bind(&SlamSimulator::OnLaserScan, this, std::placeholders::_1));

        subEncoder = nh.subscribe<EncoderData>("/encoder",
                std::bind(&SlamSimulator::OnEncoder, this, std::placeholders::_1));

        pubSlamResult = nh.advertise<SlamResult>("/pose");
        auto slamResult = std::make_shared<SlamResult>();

        slamResultThread = new mind_os::util::LoopThread(50, [this, slamResult](){
            slamResult->header.stamp = mind_os::util::now();
//            LOG(INFO) << "publish pose: " << slamResult->s_time;
            // pubSlamResult.publish(slamResult);
        });

        pubMap = nh.advertise<OccupancyGrid>("/map");
        auto map = std::make_shared<OccupancyGrid>();
        for (size_t j = 0; j < 100; ++j)
        {
            tutorial::base::Grid g;
            map->grid_data.push_back(g);
        }

        mapThread = new mind_os::util::LoopThread(2,
                [this, map](){
                    std::this_thread::sleep_for(std::chrono::milliseconds(123));
                    map->header.stamp = mind_os::util::now();
                    for (int i = 0; i < 10; ++i)
                    {
                        LOG(INFO) << "publish map: " << map->header.stamp;
                        pubMap.publish(map);
                        std::this_thread::yield();
                    }
                },
                [this, map](){
//                    LOG(INFO) << "Publish map!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                    map->header.stamp = mind_os::util::now();
                    pubMap.publish(map);
                }
        );

//        ssGetPose = nh.advertiseService<GetPoseSrv>("/get_pose",
//                std::bind(&SlamSimulator::OnGetPose, this, std::placeholders::_1, std::placeholders::_2));

        ssGetPose = nh.advertiseService<GetPoseSrv>("/get_pose", &SlamSimulator::OnGetPose, this);
    }

    void OnImu(mind_os::ConstPtr<ImuData>& msg)
    {
        ++total;

//        if (total > 1000)
//        {
//            subImu.Shutdown();
//        }

        int a = 0;
        if(getPrivateNodeHandle().getParam("test.v", a))
        {
            LOG(INFO) << "slam a:" << a;
        }
        else
        {
            LOG(INFO) << "slam a: failed.";
        }

        auto latency = (mind_os::util::now() - msg->header.stamp);
        if (latency > highest)
        {
            highest = latency;
        }
        if (latency < 100)
        {
            LOG(INFO)  << "Slam, received imu: " << msg->header.stamp << " latency: " << (latency) << " us";
//            std::cout << "Slam received imu, latency: " << (latency) << " us" << std::endl;
        }
        else
        {
            ++lac;
            LOG(WARNING) << "Slam, received imu: " << msg->header.stamp << ", latency: " << (latency) << " us!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
//            std::cout << "Slam received imu, latency: " << (latency) << " us!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        }
//        LOG(INFO) << "Subscriber, highest: " << highest
//                << " us, latency percent: " << (lac * 1.0 / total) * 100 << " %";
    };

    void OnLaserScan(mind_os::ConstPtr<LaserScan>& msg)
    {
        auto lan = (mind_os::util::now() - msg->header.stamp);
        if (lan > 1000)
        {
            LOG(INFO) << "received laser, latency: " << lan << " us!!!!!!!!!!!!!!!!!!!!!!!!!!!";
//            std::cout << "Slam received laser, latency: " << lan << " us!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        }
        else
        {
            LOG(INFO) << "received laser, latency: " << lan << " us";
//            std::cout << "Slam received laser, latency: " << lan << " us" << std::endl;
        }

    };

    void OnEncoder(mind_os::ConstPtr<EncoderData>& msg)
    {
        LOG(INFO) << "received encoder, latency: " << (mind_os::util::now() - msg->header.stamp) << " us";
//        std::cout << "Slam received encoder, latency: " << (mind_os::util::now() - msg->time) << " us" << std::endl;
    };

    void OnGetPose(mind_os::ConstPtr<GetPoseSrv::Request>& request, mind_os::Ptr<GetPoseSrv::Response>& response)
    {
        response->pose.header.stamp = mind_os::util::now();
        response->pose.s_rotation.push_back(1);
        response->pose.s_rotation.push_back(1);
        response->pose.s_rotation.push_back(1);
        response->pose.s_position.push_back(1);
        response->pose.s_position.push_back(1);
        response->pose.s_position.push_back(1);
        response->pose.s_state = 1;
    }

    void onLoaded() override
    {
        slamResultThread->start();
        mapThread->start();

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        slamResultThread->stop();
        mapThread->stop();

        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SlamSimulator)
