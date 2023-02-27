//
// Created by donglijian on 2/15/22.
//

#include "log.h"
#include "src/utils/define.h"

LogMessage logMessageStashed;

static std::string GetLocalTime()
{
    struct tm *currentTime;
    char timeStr[200];
    char timeString[200];
    struct timeval tv;

    gettimeofday(&tv, NULL);
    currentTime = localtime(&(tv.tv_sec));
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", currentTime);
    snprintf(timeString, sizeof(timeString), "%s.%06ld ", timeStr, tv.tv_usec);

    return timeString;
}

LogStream::~LogStream()
{
    if (flag)
    {
        google::LogMessage(file.c_str(), line, level).stream() << GetMessage();
    }

    stream << std::endl;
    logMessageStashed.Expand(head.str() + GetMessage());
}

std::ostream &LogStream::GetStream()
{
    head << GetLocalTime() << syscall(SYS_gettid) << " " << (FILENAME(file.c_str()))
         << ":" << line << "]";

    return stream;
}

std::string LogStream::GetMessage() const
{
    return stream.str();
}

LogMessage::LogMessage()
{
    message.push("");
}

void LogMessage::NewLine()
{
    std::unique_lock<std::mutex> lock(logMutex);
    RemoveUnused();
    message.push("");
}

void LogMessage::Pop()
{
    std::unique_lock<std::mutex> lock(logMutex);
    if (not message.empty())
        message.pop();
}

void LogMessage::RemoveUnused()
{
    while (message.size() > LOG_STASH_ITEM_NUM)
    {
        message.pop();
    }
}

void LogMessage::Logging()
{
    std::unique_lock<std::mutex> lock(logMutex);
    if (message.empty()) return;
    if (message.size() == 1 && message.front().empty()) return;

    LOG_INNER(INFO) << "log pop.";

    int messageCount = message.size();

    while (message.size() > 0)
    {
        LOG_INNER(INFO) << "last frame " << messageCount-- << std::endl
                        << message.front();
        message.pop();
    }

    LOG_INNER(INFO) << "log pop end.";
    message.push(""); // prepare for next stash
}

void LogMessage::Expand(const std::string info)
{
    std::unique_lock<std::mutex> lock(logMutex);
    message.back() += info;
}

LogMessage::~LogMessage()
{
    LOG(INFO) << "release log message.";
    this->Logging();
}
