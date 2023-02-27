/*
 * OccupiedException.h
 *
 *  Created on: Aug 17, 2021
 *      Author: ubuntu
 */

#pragma once

#include "mind_os_exception.h"

namespace mind_os
{

class MemoryLeakException : public MindOSException
{
public:
    MemoryLeakException() = default;
    MemoryLeakException(std::string msg) {
        this->msg = msg;
    }
};

}
