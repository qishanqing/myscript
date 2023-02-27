#pragma once

#include "event.h"
#include <yaml-cpp/yaml.h>

namespace mind_os {

namespace main_thread {

extern void wait(Event& event);
extern void send(Event& event);
extern int argc();
extern char** argv();
extern std::string rundir();
extern void loaded(std::function<void()>);
extern YAML::Node qos();

extern void shutdown();

}

} /* mind_os */
