/*
 * ServerNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/runlib/runlib.h>
#include <glog/logging.h>

int main(int argc, char* args[])
{
    mind_os::RunLib rl;
    std::string version = "v1";
    // after detach, main thread event does not work.
    rl.detach(argc, args, version);
    return 0;
}
