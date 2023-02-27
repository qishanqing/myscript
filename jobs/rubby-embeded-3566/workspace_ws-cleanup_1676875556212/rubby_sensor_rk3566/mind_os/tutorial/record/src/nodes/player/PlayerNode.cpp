#include <mind_os/mind_os.h>
#include "msg/DataMsg.h"
#include <glog/logging.h>
#include <iostream>

using namespace tutorial::record;

class PlayerNode : public mind_os::NodePlugin
{
    mind_os::Player* player;
    mind_os::Bag* bag;
public:
    PlayerNode()  {}

    void onLoaded() override
    {
        std::thread([this](){
            auto& nh = getPrivateNodeHandle();
            auto pathRun = mind_os::main_thread::rundir();
            auto pathBag = pathRun + "/a.bag";
            bag = new mind_os::Bag(pathBag);
            player = new mind_os::Player(nh, bag);
            player->advertise<DataMsg>("/data");
            player->advertise<DataMsg>("/data2");
            player->start([](){
                LOG(INFO) << "Finished.";
                mind_os::main_thread::shutdown();
            }, 1);
            std::this_thread::sleep_for(std::chrono::seconds(3));
            LOG(INFO) << "unadvertise data.";
            player->unadvertise("/data");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            LOG(INFO) << "advertise data.";
            player->advertise<DataMsg>("/data");
        }).detach();
        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        player->stop();
        player->unadvertise("/data2");
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(PlayerNode)