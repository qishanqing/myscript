/*
 * ConstPtr.h
 *
 *  Created on: Nov 30, 2021
 *      Author: ubuntu
 */

#pragma once

#include <memory>

namespace mind_os
{

template <typename T>
using ConstPtr = const std::shared_ptr<const T>;

template <typename T>
using Ptr = const std::shared_ptr<T>;

}
