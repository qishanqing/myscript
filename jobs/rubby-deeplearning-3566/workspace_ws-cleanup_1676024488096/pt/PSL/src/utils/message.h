//
// Created by hao on 2022/7/21.
//

#ifndef SAMPLE_MESSAGE_H
#define SAMPLE_MESSAGE_H

#include <string>

class Message
{
public:
    Message();

    Message(const int step);

    std::string LogStr(const bool flag, const std::string message);

    void Log(const bool flag, const std::string message);

private:
    int step;
    time_t lastTime;
    bool turnON;
};


#endif //SAMPLE_MESSAGE_H
