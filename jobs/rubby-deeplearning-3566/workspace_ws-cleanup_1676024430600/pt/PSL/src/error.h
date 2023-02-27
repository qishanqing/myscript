//////////////////////////////////////////////////////////////////////
///  @file     error.h
///  @brief    error exception message upload
///  Details.
///
///  @author   donglijian
///  @version  1.6.32
///  @date     2021.10.26
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_ERROR_H
#define DETECTOR_SAMPLE_ERROR_H

#include "perception.h"
#include <mutex>
#include "src/utils/define.h"
#include "psl/error/ErrorMessage.h"


using ErrorMessageCode = psl::ErrorCode;
using ErrorMessageExternal = psl::ErrorMessage;
using ErrorState = psl::ErrorState;

class ErrorMessage
{
private:
    ErrorMessage(const ErrorMessage &) = delete;

    ErrorMessage()
    {};

    void operator=(const ErrorMessage &) = delete;

    class AutoRelease
    {
    public:
        AutoRelease()
        {}

        ~AutoRelease()
        {
            if (errorMessage != nullptr)
            {
                LOG_CHECK_STATUS(INFO) << "release errorMessage.";
                delete errorMessage;
            }
        }
    };

public:
    static ErrorMessage *GetInstance();

    void Push(const ErrorMessageCode &errorCode, std::string message);

    void Pop(const ErrorMessageCode &errorCode, std::string message);

    void Init(const robot_detector::ErrorMessageCallback);

private:
    robot_detector::ErrorMessageCallback errorMessageCallback = nullptr;
    std::vector<ErrorMessageCode> errorCodes;
    std::mutex mutexErrorMessage;

protected:
    static ErrorMessage *errorMessage;
    static AutoRelease autoRelease;
};

#endif //DETECTOR_SAMPLE_ERROR_H
