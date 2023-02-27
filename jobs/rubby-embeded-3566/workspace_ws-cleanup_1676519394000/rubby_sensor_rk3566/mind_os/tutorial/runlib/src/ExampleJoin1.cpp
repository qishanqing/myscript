/*
 * ServerNode.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ubuntu
 */

#include <mind_os/runlib/runlib.h>
#include <glog/logging.h>

int main()
{
    mind_os::RunLib rl;
    std::string version = "v1";
    // will block here.
    rl.join(version);
    return 0;
}
