#pragma once

#include "../node_handle.h"
#include <thread>

namespace mind_os
{

class NodePlugin
{
public:
    NodePlugin();
    virtual ~NodePlugin();
    virtual void onLoaded() = 0;
    virtual void onUnloaded() = 0;

protected:
    NodeHandle& getPrivateNodeHandle();

private:
    NodeHandle nh;
    std::thread _t;
};

}

#define REGISTER_PLUGIN(X) \
extern "C" mind_os::NodePlugin* Create() \
{ \
    return new X(); \
}

#define REGISTER_PLUGIN_CLASS(X) REGISTER_PLUGIN(X)
