#include <mind_os/mind_os.h>
#include <glog/logging.h>
#include <iostream>

#include "msg/DataMsg.h"

using namespace tutorial::record;

class RecorderNode : public mind_os::NodePlugin
{
    mind_os::Recorder* recorder;
    mind_os::Bag* bag;
public:
    RecorderNode() {}

    void onLoaded() override
    {
        auto& nh = getPrivateNodeHandle();
        auto pathRun = mind_os::main_thread::rundir();
        auto pathBag = pathRun + "/a.bag";
        bag = new mind_os::Bag(pathBag);
        recorder = new mind_os::Recorder(getPrivateNodeHandle(), bag);
        recorder->subscribe<DataMsg>("/data");
        recorder->subscribe<DataMsg>("/data2");
        recorder->start();

        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        recorder->stop();
        recorder->unsubscribe("/data");
        delete recorder;
        delete bag;
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(RecorderNode)
