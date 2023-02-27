/*
 * ServiceServer.h
 *
 *  Created on: Aug 13, 2021
 *      Author: ubuntu
 */

#ifndef INCLUDE_MIND_OS_SERVICESERVER_H_
#define INCLUDE_MIND_OS_SERVICESERVER_H_

#include <functional>

namespace mind_os
{

class ServiceServer
{
    std::function<void(void)> funcShutdown;

public:
    ServiceServer() : funcShutdown(nullptr) {}
    ServiceServer(std::function<void(void)> funcShutdown) : funcShutdown(funcShutdown) {}
    void shutdown()
    {
        if (funcShutdown != nullptr)
        {
            funcShutdown();
        }
    }

};

}


#endif /* INCLUDE_MIND_OS_SERVICESERVER_H_ */
