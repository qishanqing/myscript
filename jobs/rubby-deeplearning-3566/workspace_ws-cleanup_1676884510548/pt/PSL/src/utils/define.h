//////////////////////////////////////////////////////////////////////
///  @file     define.h
///  @brief    all the macro defines
///  @author   sunhao
///  @date     2021.07.02
//////////////////////////////////////////////////////////////////////

#ifndef DEEFINE_SAMPLE_DEFINE_H
#define DEEFINE_SAMPLE_DEFINE_H

#include <glog/logging.h>
#include <chrono>
#include "src/utils/log.h"

const int ONE_MINUTE = 60000;
const int ONE_SECOND = 1000;
const int TEN_SECOND = 10000;
const int TWO_MINUTE = 120000;
const int MAX_SHOW_TIME = 10000;

#define ABS_DIFF(a, b) ((a) > (b) ? (a) -(b) : (b) - (a))
#define SLEEP_MS(x) std::this_thread::sleep_for(std::chrono::milliseconds((x)))
#define GET_TIMESTAMP std::chrono::duration_cast<std::chrono::milliseconds>( \
    std::chrono::system_clock::now().time_since_epoch()).count()

#define ERROR_PRINT(x) std::cout << "" << (x) << "" << std::endl
#define WARNING_PRINT(x) std::cout << "" << (x) << "" << std::endl
#define INFO_PRINT(x) std::cout << "" << (x) << "" << std::endl
#define PRINTF(a) (std::cout << "" << (#a) << " = " << (a) << "" << std::endl)
#define RELEASE(p) do{if (p != nullptr) delete (p); (p) = nullptr;}while(0)
#define FREE(x) do{if (nullptr != (x)) {free((x)); (x) = nullptr;}}while(0)

#define FUNC_NAME __FUNCTION__

extern Log g_logConfig;

const std::string MODULE_NAME = "DeepLearning";

#define LOG_INNER(severity) LOG(severity) << "[" << MODULE_NAME << "] "
#define LOG_CHECK_END(severity) LogStream(g_logConfig.end, (google::severity), __FILE__, __LINE__).GetStream() \
    << "[" << MODULE_NAME << "] " \
    << "[" << __FUNCTION__ << "] "
#define LOG_CHECK_DEBUG(severity) LogStream(g_logConfig.debug, (google::severity), __FILE__, __LINE__).GetStream() \
    << "[" << MODULE_NAME << "] " \
    << "[" << __FUNCTION__ << "] "
#define LOG_CHECK_TIME(severity) LogStream(g_logConfig.time, (google::severity), __FILE__, __LINE__).GetStream() << "[" << MODULE_NAME << "] " \
    << "[" << __FUNCTION__ << "] "
#define LOG_CHECK_TEST(severity) LogStream(g_logConfig.test, (google::severity), __FILE__, __LINE__).GetStream() << "[" << MODULE_NAME << "] "
#define LOG_CHECK_STATUS(severity) LogStream(g_logConfig.status, (google::severity), __FILE__, __LINE__).GetStream() << "[" << MODULE_NAME << "] "
#define LOG_NO_CHECK(severity) LogStream(true, (google::severity), __FILE__, __LINE__).GetStream()<< "[" << MODULE_NAME << "] "

#define END() LOG_CHECK_END(INFO) << " leave"
#define CHECK_BEFOR try{
#define CHECK_AFTER } \
                    catch (...) \
                    { \
                    goto exit; \
                    } \

// TODO : instead the stat with status{flag, message, error_code}
// TODO : use CHECK define contain switch status of algorithm
#define CHECK_STATUS(stat, lbl, message)  do {\
    if( true != (stat) ) {                        \
        if ("" != (message)) \
            LOG_CHECK_STATUS(WARNING) << "[" << FUNC_NAME << "] " << (message);\
        goto lbl;\
    }\
} while(0)

#define LOG_CHECK_STATUS_ONCE(message) do {\
    static bool first = true;       \
    if (first) \
    { \
        LOG_CHECK_STATUS(INFO) << message; \
        first = false;                                        \
    } \
} while(0)

// TODO : print the log first
#define LOG_CHECK_STATUS_WITE_STEP(time, message) do {\
    static time_t lastErrorTime = 0;      \
    if (GET_TIMESTAMP - lastErrorTime > (time)) \
    { \
        LOG_CHECK_STATUS(WARNING) << (message); \
        lastErrorTime = GET_TIMESTAMP; \
    } \
} while(0)


#define CHECK_STATUS_WITH_STEP(stat, time, message)  do {\
    if( true != (stat) ) {                        \
        if ("" != (message))       { \
            std::stringstream stream; \
            stream << "[" << FUNC_NAME << "] " << (message); \
            LOG_CHECK_STATUS_WITE_STEP((time), stream.str()); \
        }\
    }\
} while(0)

#define CHECK_STATUS_GOTO_WITH_STEP(stat, lbl, time, message)  do { \
    CHECK_STATUS_WITH_STEP(stat, time, message);                                            \
    if( true != (stat) ) {                        \
        goto lbl;\
    }\
} while(0)


#define CHECK_STATUS_WITH_STEP_EXIT(stat, time, message) CHECK_STATUS_GOTO_WITH_STEP(stat, exit, time, message )
#define CHECK_STATUS_WITH_TIME_END(stat, time, message) do{ \
    static time_t start = GET_TIMESTAMP;                    \
    if (stat)                                               \
    {                                                       \
        start = GET_TIMESTAMP;                               \
    }                                                       \
    else if (GET_TIMESTAMP - start <= MAX_SHOW_TIME)\
    {                                                       \
        CHECK_STATUS_WITH_STEP(stat, time, message);        \
    }                                                       \
}while(0)

#define CHECK_STATUS_WITH_TIME_EXIT_END(stat, time, message) do{ \
    CHECK_STATUS_WITH_TIME_END(stat, time, message);               \
    if (true != (stat))                                            \
    {                                                              \
        goto exit;                                                  \
    }                                                                  \
}while(0)

#define CHECK_STATUS_EXIT(stat, message) CHECK_STATUS( stat, exit, message )

#define PRINT_STR(format, ...) printf(format, ##__VA_ARGS__)
#define ERROR_STR(format, ...) printf(format, ##__VA_ARGS__)

#ifdef DEBUG
#define ASSERT(x)                                            \
    {                                                            \
        int res = (x);                                           \
        if (!res) {                                              \
            ERROR_STR("Error for %s, %d\n", __FILE__, __LINE__); \
            assert(res);                                         \
        }                                                        \
    }
#else
#define ASSERT(x)                                            \
    {                                                            \
        int res = (x);                                           \
        if (!res) {                                              \
            ERROR_STR("Error for %s, %d\n", __FILE__, __LINE__); \
        }                                                        \
    }
#endif

#endif //DEEFINE_SAMPLE_DEFINE_H
