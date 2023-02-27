/*
 * no_copy.h
 *
 *  Created on: Aug 31, 2022
 *      Author: ubuntu
 */

#pragma once

namespace mind_os
{

class NoCopy {
public:
    NoCopy(const NoCopy&) = delete;
    NoCopy(const NoCopy&&) = delete;
    void operator=(const NoCopy&) = delete;
    void operator=(const NoCopy&&) = delete;

protected:
    NoCopy() = default;
    virtual ~NoCopy() = default;
};

}
