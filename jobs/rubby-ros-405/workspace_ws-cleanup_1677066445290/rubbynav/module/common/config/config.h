//
// Created by gby on 22-08-02.
//

#ifndef CONFIG_H_
#define CONFIG_H_

#include <opencv2/core/core.hpp>
#include <string>

#include "Singleton.h"
#include "Common.h"

class config
{
private:
	std::string configPath;

public:
	std::string getConfigPath()
	{
#ifdef RUBBYSIMULATION
		configPath = "../src/modules/common/config/config.yaml";
#endif

		return configPath;
	}

	void setConfigPath(const std::string &path_)
	{
		configPath = path_;
		LOGCOMMON(INFO) << "configPath: " << configPath;
	}

	template <typename T>
	T readParam(const std::string &configName)
	{
		T result;
		cv::FileStorage FILE_READ(getConfigPath(), cv::FileStorage::READ);
		if (!FILE_READ.isOpened())
		{
			LOGCOMMON(INFO) << "[ERROR] could not open configuration file";
		}
		else
		{
			FILE_READ[configName] >> result;
		}
		return result;
	}
};

#define s_config Singleton<config>::GetInstance()

#endif