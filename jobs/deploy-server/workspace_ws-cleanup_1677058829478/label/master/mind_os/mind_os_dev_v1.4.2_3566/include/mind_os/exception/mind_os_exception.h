/*
 * mind_os_exception.h
 *
 *  Created on: Oct 21, 2022
 *      Author: ubuntu
 */

#pragma once

#include <string>
#include <exception>

namespace mind_os
{

class MindOSException : public std::exception
{
public:
    MindOSException() = default;
    MindOSException(std::string msg) : msg(msg) {}
    virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override
    {
        return msg.c_str();
    }
protected:
    std::string msg;
};

}
