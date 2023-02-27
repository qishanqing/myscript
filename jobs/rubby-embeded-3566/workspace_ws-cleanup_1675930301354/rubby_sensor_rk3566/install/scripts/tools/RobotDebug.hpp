#ifndef __ROBOT_DEBUG_HPP
#define __ROBOT_DEBUG_HPP

#include <errno.h>
#include <glog/logging.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>

#include <common.hpp>
#include <iomanip>
#include <iostream>

#define LOG_COLOR_RESET   "\033[0m"
#define LOG_COLOR_BLACK   "\033[30m"      /* Black */
#define LOG_COLOR_RED     "\033[31m"      /* Red */
#define LOG_COLOR_GREEN   "\033[32m"      /* Green */
#define LOG_COLOR_YELLOW  "\033[33m"      /* Yellow */
#define LOG_COLOR_BLUE    "\033[34m"      /* Blue */
#define LOG_COLOR_MAGENTA "\033[35m"      /* Magenta */
#define LOG_COLOR_CYAN    "\033[36m"      /* Cyan */
#define LOG_COLOR_WHITE   "\033[37m"      /* White */
#define LOG_COLOR_BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define LOG_COLOR_BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define LOG_COLOR_BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define LOG_COLOR_BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define LOG_COLOR_BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define LOG_COLOR_BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define LOG_COLOR_BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define LOG_COLOR_BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


enum LogLevelType {
  LOG_LEVEL_DEBUG = 0,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARN,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_FATAL,
};

enum LogSwitchType{
  LOG_OFF = 0,
  LOG_ON,
};



#ifdef SENSORS_LOG_EN
#define DEFAULT_LOG_SWITCH LOG_ON
#else
#define DEFAULT_LOG_SWITCH LOG_ON
#endif
#define DEFAULT_LOG_MIN_LEVE  LOG_LEVEL_DEBUG

class EmbLog {
 private:
  LogLevelType gLogMinLevel = LOG_LEVEL_DEBUG;
  LogSwitchType gLogSwitch = DEFAULT_LOG_SWITCH;

 public:
  EmbLog();
  ~EmbLog();
  static EmbLog *GetInstance(std::string configDir = "./config");
  void SetLogMinLevel(LogLevelType value);
  void SetLogSwitch(LogSwitchType value);
  LogLevelType GetLogMinLevel();
  LogSwitchType GetLogSwitch();
};

/*
    glog 打印，输出到文件， 有条件控制， 需要实现相关函数
所有 RBLOG_DEBUG 的打印在编译成release版本的时候会失效
*/
#define RBLOG_DEBUG if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_DEBUG >= EmbLog::GetInstance()->GetLogMinLevel()))  DLOG(INFO)<<"[sensors] " 
#define RBLOG_INFO  if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_INFO >= EmbLog::GetInstance()->GetLogMinLevel()))  LOG(INFO)<<"[sensors] " 
#define RBLOG_WARN  if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_WARN >= EmbLog::GetInstance()->GetLogMinLevel()))  LOG(WARNING)<<"[sensors] " 
#define RBLOG_ERROR if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_ERROR >= EmbLog::GetInstance()->GetLogMinLevel()))  LOG(ERROR)<<"[sensors] " 
#define RBLOG_FATAL if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_FATAL >= EmbLog::GetInstance()->GetLogMinLevel()))  LOG(FATAL)<<"[sensors] "
#define RBLOG_ENTRY(module)                                  \
  {                                                                \
    LOG(INFO) << "[sensors] ------------ Entry " << #module << " " \
              << __FUNCTION__ << " ------------" << std::endl;     \
  }
#define RBLOG_EXIT(module, status)                            \
  {                                                                 \
    LOG(INFO) << "[sensors] ------------ Exit " << #module << " "   \
              << __FUNCTION__ << ", " << #status << " ------------" \
              << std::endl;                                         \
  }

#define RBLOG_EXIT_SUCCESS(module)                          \
  {                                                               \
    LOG(INFO) << "[sensors] ------------ Exit " << #module << " " \
              << __FUNCTION__ << ", "                             \
              << "success"                                         \
              << " ------------" << std::endl;                    \
  }
#define RBLOG_EXIT_FAILD(module)                            \
  {                                                               \
    LOG(INFO) << "[sensors] ------------ Exit " << #module << " " \
              << __FUNCTION__ << ", "                             \
              << "failed"                                         \
              << " ------------" << std::endl;                    \
  }

//非Glog打印有条件控制， 不会失效， 不会输出到文件
#define RobotDEBUG(format, ...) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_DEBUG >= EmbLog::GetInstance()->GetLogMinLevel()))  fprintf(stdout, "%lu [%s:%d/%s] [sensors] " format "\n", GetSystimeMs(), basename((char *)__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__);}
#define RobotINFO(format, ...) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_INFO >= EmbLog::GetInstance()->GetLogMinLevel()))  fprintf(stdout, "%lu [%s:%d/%s] [sensors] " LOG_COLOR_BOLDGREEN format LOG_COLOR_RESET "\n", GetSystimeMs(), basename((char *)__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__);}
#define RobotWARN(format, ...) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_WARN >= EmbLog::GetInstance()->GetLogMinLevel()))  fprintf(stdout, "%lu [%s:%d/%s] [sensors] " LOG_COLOR_BOLDYELLOW format LOG_COLOR_RESET "\n", GetSystimeMs(), basename((char *)__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__);}
#define RobotERR(format, ...) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_ERROR >= EmbLog::GetInstance()->GetLogMinLevel()))  fprintf(stderr, "%lu [%s:%d/%s] [sensors] " LOG_COLOR_BOLDRED format LOG_COLOR_RESET "\n", GetSystimeMs(), basename((char *)__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__);}
#define RobotENTER() {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_INFO >= EmbLog::GetInstance()->GetLogMinLevel()))   fprintf(stderr, "%lu [%s:%d] [sensors] " LOG_COLOR_GREEN "ENTER %s" LOG_COLOR_RESET "\n", GetSystimeMs(), basename((char *)__FILE__), __LINE__, __FUNCTION__);}
#define RobotEXIT(code) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_INFO >= EmbLog::GetInstance()->GetLogMinLevel()))   fprintf(stderr, "%lu [%s:%d] [sensors] EXIT %s (%d)\n", GetSystimeMs(), basename((char *)__FILE__), __LINE__, __FUNCTION__, code);}
#define RobotEXIT_VOID() {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_INFO >= EmbLog::GetInstance()->GetLogMinLevel()))   fprintf(stderr, "%lu [%s:%d] [sensors] EXIT %s\n", GetSystimeMs(), basename((char *)__FILE__), __LINE__, __FUNCTION__);}


//非Glog打印有条件控制， 不会失效， 不会输出到文件
#define RobotDebugNoWrap(format, ...) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_DEBUG >= EmbLog::GetInstance()->GetLogMinLevel()))  fprintf(stdout, format, ##__VA_ARGS__);}
#define RobotInfoNoWrap(format, ...) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_INFO >= EmbLog::GetInstance()->GetLogMinLevel()))  fprintf(stdout, LOG_COLOR_BOLDGREEN format LOG_COLOR_RESET, ##__VA_ARGS__);}
#define RobotWarnNoWrap(format, ...) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_WARN >= EmbLog::GetInstance()->GetLogMinLevel()))  fprintf(stdout, LOG_COLOR_BOLDYELLOW format LOG_COLOR_RESET,##__VA_ARGS__);}
#define RobotErrNoWrap(format, ...) {if(EmbLog::GetInstance()->GetLogSwitch() && (LOG_LEVEL_ERROR >= EmbLog::GetInstance()->GetLogMinLevel()))  fprintf(stderr, LOG_COLOR_BOLDRED format LOG_COLOR_RESET, ##__VA_ARGS__);}





#endif
