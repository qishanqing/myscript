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
    // will block here.
    rl.join(argc, args, version);
    return 0;
}
