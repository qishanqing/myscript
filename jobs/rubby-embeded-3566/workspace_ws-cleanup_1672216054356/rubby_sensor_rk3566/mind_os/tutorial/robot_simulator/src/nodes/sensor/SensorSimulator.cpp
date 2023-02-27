#include <mind_os/mind_os.h>
#include "EncoderData.h"
#include "ImuData.h"
#include "LaserScan.h"
#include <glog/logging.h>
#include <iostream>

using namespace tutorial::sensor;

class SensorSimulator : public mind_os::NodePlugin
{
    mind_os::util::LoopThread* imuThread;
    mind_os::util::LoopThread* laserThread;
    mind_os::util::LoopThread* encoderThread;

    mind_os::Publisher pubImu;
    mind_os::Publisher pubLaserScan;
    mind_os::Publisher pubEncoder;

    bool running = true;

public:
    SensorSimulator() // @suppress("Class members should be properly initialized")
    {
        auto& nh = getPrivateNodeHandle();

        pubImu = nh.advertise<ImuData>("/imu");

        imuThread = new mind_os::util::LoopThread(500, [this](){
            static int a = 1;
            auto imuMsg = std::make_shared<ImuData>();
            static auto lastTime = mind_os::util::now();
            imuMsg->header.stamp = mind_os::util::now();
            lastTime = imuMsg->header.stamp;
            pubImu.publish(imuMsg);
        });

        pubLaserScan = nh.advertise<LaserScan>("/scan");

        auto laserMsg = std::make_shared<LaserScan>();
        for (size_t i = 0; i < 1536; ++i)
        {
            laserMsg->ranges.push_back(i);
        }
        for (size_t j = 0; j < 1536; ++j)
        {
            laserMsg->intensities.push_back(j);
        }

        laserThread = new mind_os::util::LoopThread(25, [this, laserMsg](){
            laserMsg->header.stamp = mind_os::util::now();
            pubLaserScan.publish(laserMsg);
        });

        pubEncoder = nh.advertise<EncoderData>("/encoder");
        auto encoderMsg = std::make_shared<EncoderData>();

        encoderThread = new mind_os::util::LoopThread(50, [this, encoderMsg](){
            encoderMsg->header.stamp = mind_os::util::now();
            pubEncoder.publish(encoderMsg);
        });
    }

    void onLoaded() override
    {
        imuThread->start();
        laserThread->start();
        encoderThread->start();

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        imuThread->stop();
        laserThread->stop();
        encoderThread->stop();
        running = false;

        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(SensorSimulator)
