#pragma once

#include <string>

namespace mind_os
{
    void stringToNumber(const std::string& str, auto& data)
    {
        std::istringstream iss(str);
        iss >> data;
    }
}
