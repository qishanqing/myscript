//
// Created by hao on 2021/7/27.
//

#ifndef DETECTOR_SAMPLE_STATUS_H
#define DETECTOR_SAMPLE_STATUS_H
#include <string>

enum ErrorCode {SUCCESS = 0, MISS_FILE, MISS_OPTION};

struct Status
{
    ErrorCode code = SUCCESS;
    std::string message = "";

    Status(){}

    Status(const ErrorCode code, const std::string& message)
    {
        this->code = code;
        this->message = message;
    }
};

#endif //DETECTOR_SAMPLE_STATUS_H
