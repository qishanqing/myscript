/*
 * OccupiedException.h
 *
 *  Created on: Aug 17, 2021
 *      Author: ubuntu
 */

#ifndef MIND_OS_EXCEPTION_OCCUPIEDEXCEPTION_H_
#define MIND_OS_EXCEPTION_OCCUPIEDEXCEPTION_H_

#include <exception>

namespace mind_os
{

class OccupiedException : public std::exception
{};

}


#endif /* MIND_OS_EXCEPTION_OCCUPIEDEXCEPTION_H_ */
