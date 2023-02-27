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
    else
    {
        stream << std::endl;
        logMessageStashed.Expand(GetMessage());
    }
}

std::ostream &LogStream::GetStream()
{
    if (!flag)
    {
        stream << GetLocalTime() << syscall(SYS_gettid) << " " << (filename(file.c_str()))
               << ":" << line << "]";
    }
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
    if (not message.back().empty()) message.push("");

    LOG_INNER(INFO) << "log pop.";

    int i = message.size();

    while (message.size() > 1)
    {
        LOG_INNER(INFO) << "last frame " << --i << std::endl << message.front();
        message.pop();
    }

    LOG_INNER(INFO) << "log pop end.";
}

void LogMessage::Expand(const std::string info)
{
    std::unique_lock<std::mutex> lock(logMutex);
    message.back() += info;
}
