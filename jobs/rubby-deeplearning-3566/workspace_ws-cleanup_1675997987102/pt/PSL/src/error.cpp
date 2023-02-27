//
// Created by indemind on 10/26/21.
//

#include "error.h"
#include "src/utils/utils.h"
#include "src/utils/define.h"

ErrorMessage *ErrorMessage::errorMessage = new ErrorMessage();
ErrorMessage::AutoRelease ErrorMessage::autoRelease;

void ErrorMessage::Push(const ErrorMessageCode &errorCode, std::string message)
{
    if (errorMessageCallback == nullptr) return;

    std::lock_guard<std::mutex> lock(mutexErrorMessage);
    std::vector<ErrorMessageCode>::iterator iter = find(errorCodes.begin()
                                                        , errorCodes.end(), errorCode);

    if (iter == errorCodes.end())
    {
        ErrorMessageExternal error;
        error.state = ErrorState::ON;
        error.stamp = GetTimestamp();
        error.code = errorCode;
        errorMessageCallback(error);

        errorCodes.push_back(errorCode);
        LOG_CHECK_STATUS(INFO) << "Report exception " << message
                               << ", Code: " << errorCode;
    }

    return;
}

void ErrorMessage::Pop(const ErrorMessageCode &errorCode, std::string message)
{
    if (errorMessageCallback == nullptr) return;

    std::lock_guard<std::mutex> lock(mutexErrorMessage);
    if (errorCodes.empty()) return;

    std::vector<ErrorMessageCode>::iterator iter = find(errorCodes.begin()
                                                        , errorCodes.end(), errorCode);

    if (iter != errorCodes.end())
    {
        ErrorMessageExternal error;
        error.state = ErrorState::OFF;
        error.stamp = GetTimestamp();
        error.code = errorCode;
        errorCodes.erase(iter);
        errorMessageCallback(error);
        LOG_CHECK_STATUS(INFO) << "Cancel exception " << message
                               << ", Code: " << errorCode;
    }

    return;
}

void ErrorMessage::Init(const robot_detector::ErrorMessageCallback errorMessageCallback)
{
    if (this->errorMessageCallback == nullptr)
    {
        this->errorMessageCallback = errorMessageCallback;
    }

    return;
}

ErrorMessage *ErrorMessage::GetInstance()
{
    return errorMessage;
}