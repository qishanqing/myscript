#include "config.h"
#include <yaml-cpp/yaml.h>

#include "src/utils/file.h"
#include "src/utils/utils.h"

extern Log g_logConfig;

const int TIME_MARGIN_DEFAULT = 15; // 15ms
const float RADIAN_2_ANGLE = 180 / M_PI;

Status LoadConfig(const std::string &configFile, ConfigParam &configParam)
{
    // TODO : when lack of option, how to continue (now is block)
    // TODO : check the option elegant
    if (not file_op::File::Exist(configFile))
    {
        return Status(MISS_FILE, "config file missed <" + configFile + ">");
    }

    YAML::Node configYaml = YAML::LoadFile(configFile);

    try
    {
        YAML::Node configLog = configYaml["deeplearning"]["log"];

        configParam.log.status = configLog["status"].as<int>();
        configParam.log.end = configLog["end"].as<int>();
        configParam.log.debug = configLog["debug"].as<int>();
        configParam.log.test = configLog["test"].as<int>();
        configParam.log.time = configLog["time"].as<int>();
        configParam.display = ConfigParam::Display(
                configYaml["deeplearning"]["display"].as<int>());

        // TODO : use one struct with configParam
        g_logConfig.end = configParam.log.end;
        g_logConfig.debug = configParam.log.debug;
        g_logConfig.time = configParam.log.time;
        g_logConfig.test = configParam.log.test;
        g_logConfig.status = configParam.log.status;
    }
    catch (const std::exception &e)
    {
        return Status(MISS_OPTION, "config option <log/... or display> missed");
    }

    try
    {
        configParam.radius = configYaml["deeplearning"]["radius"].as<float>();
    }
    catch (const std::exception &e)
    {
        return Status(MISS_OPTION, "config option <radius> missed");
    }

    try
    {
        configParam.timeMargin = configYaml["deeplearning"]["timeMargin"].as<int>();
    }
    catch (const std::exception &e)
    {
        configParam.timeMargin = TIME_MARGIN_DEFAULT;
        return Status(MISS_OPTION, "config option <timeMargin> missed");
    }

    configParam.timeMargin *= 1000; // ms to us

    try
    {
        configParam.distance = configYaml["deeplearning"]["distance"].as<float>();
    }
    catch (const std::exception &e)
    {
        return Status(MISS_OPTION, "config option <distance> missed");
    }
    try
    {
        configParam.nFrames = configYaml["deeplearning"]["diagconditon"].as<int>();
    }
    catch (const std::exception &e)
    {
        return Status(MISS_OPTION, "config option <diagconditon> missed");
    }
    try
    {
        configParam.angle = configYaml["deeplearning"]["angle"].as<int>();
    }
    catch (const std::exception &e)
    {
        return Status(MISS_OPTION, "config option <angle> missed");
    }

    try
    {
        // TODO : use node-dl replace many index
        configParam.threshold = configYaml["deeplearning"]["threshold"].as<float>();
    }
    catch (const std::exception &e)
    {
        return Status(MISS_OPTION, "config option <threshold> missed");
    }

    try
    {
        configParam.structure.poseLidar2Machine = configYaml["deeplearning"]["structure"]["pose2Machine"].as<std::vector<float >>();
        configParam.structure.leftcamera2lidar = configYaml["deeplearning"]["structure"]["leftcamera2lidar"].as<std::vector<float >>();
        configParam.structure.rightcamera2lidar = configYaml["deeplearning"]["structure"]["rightcamera2lidar"].as<std::vector<float >>();
        configParam.structure.lidarDirection = configYaml["deeplearning"]["structure"]["lidarDirection"].as<bool>();
        configParam.structure.lidarAngle = configYaml["deeplearning"]["structure"]["lidarAngle"].as<float>();
        configParam.structure.cameraHeight = configYaml["deeplearning"]["structure"]["cameraHeight"].as<float>();
    }
    catch (const std::exception &e)
    {
        return Status(MISS_OPTION
                      , "config option <\nstructure/\n\tpose2Machine\n\tleftcamera2lidar\n\trightcamera2lidar"
                        "\n\tlidarDirection\n\tlidarAngle\n\tcameraHeight\n\t> missed");
    }

    try
    {
        configParam.structure.cameraAngle = configYaml["deeplearning"]["structure"]["cameraAngle"].as<float>();
    }
    catch (const std::exception &e)
    {
        configParam.structure.cameraAngle = 10;
    }
    configParam.structure.cameraAngle = configParam.structure.cameraAngle / RADIAN_2_ANGLE;

    // TODO : when the private config option is null, how to handle the error status
    try
    {
        std::string path = configYaml["deeplearning"]["saveData"]["Path"].as<std::string>();
        bool checkObject = configYaml["deeplearning"]["saveData"]["checkObject"].as<bool>();
        configParam.dataset = Data4SaveInfo(path, checkObject);
    }
    catch (...)
    {
        // nothing
    }

    return Status();
}
