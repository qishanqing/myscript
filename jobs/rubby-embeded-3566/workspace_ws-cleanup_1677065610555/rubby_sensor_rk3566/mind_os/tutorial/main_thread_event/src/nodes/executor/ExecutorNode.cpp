/*
 * ExecutorNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/mind_os.h>
#include <glog/logging.h>

class ExecutorNode : public mind_os::NodePlugin
{

    mind_os::util::LoopThread* ltExecutor;
public:
    ExecutorNode() = default;

    void onLoaded() override
    {
        LOG(INFO) << "rundir: " << mind_os::main_thread::rundir();
        ltExecutor = new mind_os::util::LoopThread(1, [this](){
            mind_os::main_thread::Event event([&](){
                LOG(INFO) << "output log in main-thread.";
            });
            LOG(INFO) << "-------------------wait------------------";
            LOG(INFO) << "wait event in sub-thread.";
            mind_os::main_thread::wait(event);
            LOG(INFO) << "finished waiting event in sub-thread.";

            LOG(INFO) << "-------------------send------------------";
            LOG(INFO) << "send event in sub-thread.";
            mind_os::main_thread::send(event);
            LOG(INFO) << "finished sending event in sub-thread.";
        });
        mind_os::main_thread::loaded([this](){
            LOG(INFO) << "Loaded.";
            ltExecutor->start();
        });
        LOG(INFO) << "Start.";
    }

    void onUnloaded() override
    {
        ltExecutor->stop();
        LOG(INFO) << "Stop.";
    }
};

REGISTER_PLUGIN(ExecutorNode)
