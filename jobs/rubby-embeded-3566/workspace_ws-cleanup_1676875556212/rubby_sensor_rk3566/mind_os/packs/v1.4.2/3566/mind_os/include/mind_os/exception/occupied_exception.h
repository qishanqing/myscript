/*
 * OccupiedException.h
 *
 *  Created on: Aug 17, 2021
 *      Author: ubuntu
 */

#ifndef MIND_OS_EXCEPTION_OCCUPIEDEXCEPTION_H_
#define MIND_OS_EXCEPTION_OCCUPIEDEXCEPTION_H_

#include "mind_os_exception.h"

namespace mind_os
{

class OccupiedException : public MindOSException
{
public:
    OccupiedException() = default;
    OccupiedException(std::string msg) {
        this->msg = msg;
    }
};

}


#endif /* MIND_OS_EXCEPTION_OCCUPIEDEXCEPTION_H_ */
