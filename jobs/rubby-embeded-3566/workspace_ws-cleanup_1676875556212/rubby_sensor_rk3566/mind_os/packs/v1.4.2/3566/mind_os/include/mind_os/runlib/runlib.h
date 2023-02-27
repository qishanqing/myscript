#pragma once

#include <string>

namespace mind_os
{

class RunLib
{
public:
    int run(int argc, char** argv);

    // same as run();
    int execute(int argc, char** argv);

    // in detach mode, main thread event will be abandoned
    int detach(std::string kernelVersion); // v1, v2
    int detach(int argc, char** argv, std::string kernelVersion); // v1, v2

    int join(std::string kernelVersion); // v1, v2
    int join(int argc, char** argv, std::string kernelVersion); // v1, v2
};

}
