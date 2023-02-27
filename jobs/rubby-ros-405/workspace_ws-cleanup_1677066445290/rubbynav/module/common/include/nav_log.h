#ifndef NAV_LOG_H
#define NAV_LOG_H

#include <glog/logging.h>
#include "stime.h"

#define NAVLOG(TYPE) LOG(TYPE) << "[NAV] "
#define NAVLOG_EVERY_N(TYPE, N) LOG_EVERY_N(TYPE, N) << "[NAV] "
#define NAVLOG_EVERY_SEC(TYPE, time_sec, note)            \
    {                                                     \
        static double history_time = 0;                   \
        if (nav::Now() / 1.0e6 - history_time > time_sec) \
        {                                                 \
            NAVLOG(TYPE) << note;                         \
            history_time = nav::Now() / 1.0e6;            \
        }                                                 \
    }

#define NAVDLOG(TYPE) DLOG(TYPE) << "[NAV] "
#define NAVDLOG_EVERY_N(TYPE, N) DLOG_EVERY_N(TYPE, N) << "[NAV] "
#define NAVDLOG_EVERY_SEC(TYPE, time_sec, note)           \
    {                                                     \
        static double history_time = 0;                   \
        if (nav::Now() / 1.0e6 - history_time > time_sec) \
        {                                                 \
            NAVDLOG(TYPE) << note;                        \
            history_time = nav::Now() / 1.0e6;            \
        }                                                 \
    }

#define NAVLOG_LEVEL(TYPE, FLAG, note) \
    {                                  \
        if (FLAG)                      \
        {                              \
            NAVDLOG(TYPE) << note;     \
        }                              \
    }

#define NAVLOG_LEVEL_EVERY_N(TYPE, FLAG, N, note) \
    {                                             \
        if (FLAG)                                 \
        {                                         \
            NAVLOG_EVERY_N(TYPE, N) << note;      \
        }                                         \
    }

#define NAVLOG_LEVEL_EVERY_SEC(TYPE, FLAG, time_sec, note)    \
    {                                                         \
        if (FLAG)                                             \
        {                                                     \
            static double history_time = 0;                   \
            if (nav::Now() / 1.0e6 - history_time > time_sec) \
            {                                                 \
                NAVLOG(TYPE) << note;                         \
                history_time = nav::Now() / 1.0e6;            \
            }                                                 \
        }                                                     \
    }

// sample
// NAVLOG_LEVEL(INFO, clern_robot->mLogLevel >= 1, "test log 0");
// NAVLOG_LEVEL_EVERY_N(INFO, clern_robot->mLogLevel >= 1, 2, "test log 1");
// NAVLOG_LEVEL_EVERY_SEC(INFO, clern_robot->mLogLevel >= 1, 1.0e-8, "test log 2");
// if(clern_robot->mLogLevel){do something}
#endif // NAV_LOG_H
