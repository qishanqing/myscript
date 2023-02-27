#include "RobotDebug.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <yaml-cpp/yaml.h>

#include <fstream>
static EmbLog *logobj = nullptr;
EmbLog::EmbLog() {}
EmbLog::~EmbLog() {}
EmbLog *EmbLog::GetInstance(std::string configDir) {
  if (logobj == nullptr) {
    std::string configFile = "";
    logobj = new EmbLog();
    if (access(configDir.c_str(), F_OK) != 0) {
      if (mkdir(configDir.c_str(), 0755) == -1) {
        RBLOG_ERROR << "EmbSensorMonitor mkdir config dir: " << configDir << " failed"
                    << std::endl;
      }
    }
    configFile = configDir + "/emb_log.yaml";
    if (access(configFile.c_str(), F_OK) != 0) {
      YAML::Node config;
      config["Debug"]["LogEnable"] = "true";
      config["Debug"]["LogMinLevel"] = "1";
      std::ofstream out;
      out.open(configFile.c_str(), std::ios_base::out);
      out << config;
      out.close();
    }
    YAML::Node config = YAML::LoadFile(configFile.c_str());
    logobj->SetLogMinLevel((LogLevelType)config["Debug"]["LogMinLevel"].as<int>());
    if (config["Debug"]["LogEnable"].as<bool>()) {
      logobj->SetLogSwitch(LOG_ON);
    } else {
      logobj->SetLogSwitch(LOG_OFF);
    }
  }
  return logobj;
}
void EmbLog::SetLogMinLevel(LogLevelType value) { gLogMinLevel = value; }
void EmbLog::SetLogSwitch(LogSwitchType value) { gLogSwitch = value; }
LogLevelType EmbLog::GetLogMinLevel() { return gLogMinLevel; }
LogSwitchType EmbLog::GetLogSwitch() { return gLogSwitch; }
