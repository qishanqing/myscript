//
// Created by hao on 2022/7/21.
//

#include <sstream>
#include "message.h"
#include "src/utils/define.h"

Message::Message()
{
    step = 0;
    lastTime = GET_TIMESTAMP;
    turnON = true;
}

Message::Message(const int step)
        : Message()
{
    this->step = step;
}

std::string Message::LogStr(const bool flag, const std::string message)
{
    std::stringstream stream("");

    if (flag)
    {
        if ((GET_TIMESTAMP - lastTime) > step or turnON)
        {
            stream << message;
        }

        turnON = (GET_TIMESTAMP - lastTime) > step;
    }
    else
    {
        lastTime = GET_TIMESTAMP;
    }
    std::string k = stream.str();

    return stream.str();
}

void Message::Log(const bool flag, const std::string message)
{
    LOG_CHECK_STATUS(INFO) << LogStr(flag, message);
}
