/*
 * TypeInconsistentException.h
 *
 *  Created on: Aug 17, 2021
 *      Author: ubuntu
 */

#ifndef MIND_OS_EXCEPTION_TYPEINCONSISTENTEXCEPTION_H_
#define MIND_OS_EXCEPTION_TYPEINCONSISTENTEXCEPTION_H_


#include "mind_os_exception.h"

namespace mind_os
{

class TypeInconsistentException : public MindOSException
{
public:
    TypeInconsistentException() = default;
    TypeInconsistentException(std::string msg) {
        this->msg = msg;
    }
};

}


#endif /* MIND_OS_EXCEPTION_TYPEINCONSISTENTEXCEPTION_H_ */
