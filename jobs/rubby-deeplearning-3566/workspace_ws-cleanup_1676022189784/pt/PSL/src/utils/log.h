//////////////////////////////////////////////////////////////////////
///  @file     log.h
///  @brief   defines for log
///  @author   sunhao
///  @date     2021.07.05
//////////////////////////////////////////////////////////////////////

#ifndef LOG_SAMPLE_DATA_TYPE_H
#define LOG_SAMPLE_DATA_TYPE_H

#include <iostream>
#include <glog/logging.h>
#include<sys/time.h>
#include <queue>
#include <sys/syscall.h>
#include <mutex>

#define FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x

const int LOG_STASH_ITEM_NUM = 3;

struct LogMessage
{
    std::queue<std::string> message;
    std::mutex logMutex;

    LogMessage();

    ~LogMessage();

    void NewLine();

    void Pop();

    void Logging();

    void RemoveUnused();

    void Expand(const std::string info);
};

extern LogMessage logMessageStashed;

struct Log
{
    int end; // for function end (debug)
    int debug; // for debug program
    int time; // for timing
    int test; // for test(time, distance, mAP)
    int status; // for release log
};


class LogStream
{
public:
    LogStream(const bool flag, const int level, const char *file, const int line)
            : flag(flag), level(level), file(file), line(line)
    {}

    ~LogStream();

    std::string GetMessage() const;

    std::ostream &GetStream();

private:
    std::stringstream stream;
    std::stringstream head;
    bool flag;
    int level;
    std::string file;
    int line;
};


#endif //LOG_SAMPLE_DATA_TYPE_H
