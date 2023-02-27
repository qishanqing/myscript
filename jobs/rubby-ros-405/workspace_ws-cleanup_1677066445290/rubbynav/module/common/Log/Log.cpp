//
// Created by gby on 22-7-11.
//

#include "Log.h"

void removeDir(std::string path_)
{
	boost::filesystem::remove_all(path_);
}

void LOGNAV::checkLogDirNum(const std::string rootPath_)
{
	std::vector<std::string> dirName_;
	std::vector<int> dirNameInt_;
	DIR* dir_;
	struct dirent* ent;
	dir_ = opendir(rootPath_.c_str());
	if(dir_ != nullptr)
	{
		while(((ent = readdir(dir_)) != nullptr))
		{
			if (ent->d_type == DT_DIR)
			{
				std::string szFileName = ent->d_name;

				if (strcmp(".", szFileName.c_str())==0 || strcmp("..", szFileName.c_str())==0)
					continue;
				dirName_.push_back(szFileName);
			}
		}
		closedir(dir_);
	}

	for(int i = 0; i < dirName_.size(); i++)
	{
		dirNameInt_.push_back(std::stoi(dirName_[i]));
	}

	std::sort(dirNameInt_.begin(), dirNameInt_.end(), [](int x, int y){return x > y;});

	int count = dirNameInt_.size();
	while(count > 3)
	{
		std::string dirNa;
		dirNa = rootPath_ + std::to_string(dirNameInt_[count-1]);
		dirNameInt_.pop_back();
		count--;
		removeDir(dirNa);
	}

}

std::string LOGNAV::getYMD()
{
	std::string res;
	time_t curTime;
	time(&curTime);
	struct tm* nowTime = std::localtime(&curTime);

	std::string m_year = std::to_string(1900 + nowTime->tm_year);
	std::string m_month = std::to_string(1 + nowTime->tm_mon);
	std::string m_day = std::to_string(nowTime->tm_mday);

	if((1 + nowTime->tm_mon) < 10)
	{
		m_month = "0" + m_month;
	}

	if((nowTime->tm_mday) < 10)
	{
		m_day = "0" + m_day;
	}

	res = m_year + m_month +  m_day;


	return res;
}

std::string LOGNAV::getHMS()
{
	std::string res;
	time_t curTime;
	time(&curTime);
	struct tm* nowTime = std::localtime(&curTime);

	std::string m_hour = std::to_string(nowTime->tm_hour);
	std::string m_minute = std::to_string(nowTime->tm_min);
	std::string m_second = std::to_string(nowTime->tm_sec);

	if((nowTime->tm_hour) < 10)
	{
		m_hour = "0" + m_hour;
	}

	if((nowTime->tm_min) < 10)
	{
		m_minute = "0" + m_minute;
	}

	if((nowTime->tm_sec) < 10)
	{
		m_second = "0" + m_second;
	}

	res = m_hour + m_minute +  m_second;

	return res;
}

std::string LOGNAV::getMil()
{
	std::string res;
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	std::chrono::milliseconds ms;
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	res = std::to_string(ms.count());

	if(ms.count() < 10)
	{
		res = "00" + res;
	}

	if(ms.count() >= 10 && ms.count() < 100)
	{
		res = "0" + res;
	}
	return res;
}

std::string LOGNAV::getMic()
{
	std::string res;
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	std::chrono::microseconds cs;
	cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
	res = std::to_string(cs.count()%1000);

	if(cs.count()%1000 < 10)
	{
		res = "00" + res ;
	}

	if(cs.count()%1000 >= 10 && cs.count()%1000 < 100)
	{
		res = "0" + res;
	}
	return res;
}

void LOGNAV::startWorkInit(const char* logAddress_)
{
	if(logAddress_ != nullptr)
	{
		std::string rootPath_ = logAddress_;
		rootPath_ = (rootPath_[rootPath_.size() - 1] == '/') ? rootPath_ : rootPath_ + "/";
		checkLogDirNum(rootPath_);
		
		pathTwo_  = getYMD();
		
		// std::string path = rootPath_ + pathTwo_ + "/" + getHMS();
		// if(!boost::filesystem::is_directory(path))
		// {
		// 	boost::filesystem::create_directories(path);
		// }
		
		std::string path = rootPath_ + pathTwo_ + "/" + getHMS();
		if(!mind_os::util::FilePath::isDirExisted(path.c_str()))
		{
			if(mind_os::util::FilePath::createDirR(path.c_str()))
			{
				LOGCOMMON(INFO) << "StartWorkInit create dir success." << path;
			}
			else
			{
				LOGCOMMON(INFO) << "StartWorkInit create dir failed." << path;
			}
		}
		pathThree_ = path;
	}
}

void LOGNAV::storageTypeInit(const int type_)
{
	std::string prePath_ = LOGNAV::pathThree_;

	std::string hms_ = LOGNAV::getHMS();
	std::string mil_ = LOGNAV::getMil();
	std::string mic_ = LOGNAV::getMic();

	std::string mode_;
	switch (type_)
	{
	case LOG_HSM:
		mode_ = "hsm";
		break;

	case LOG_PLANNING:
		mode_ = "planning";
		break;

	case LOG_SEARCH:
		mode_ = "search";
		break;

	case LOG_BOW:
		mode_ = "bow";
		break;

	case LOG_NAV:
		mode_ = "nav";
		break;

	case LOG_BOX:
		mode_ = "box";
		break;

	case LOG_MOTION:
		mode_ = "motion";
		break;

	default:
		break;
	}
	
	// std::string path = prePath_ + "/" + mode_ + "/" + hms_ + mil_ + mic_ + "/";

	// if(!boost::filesystem::is_directory(path))
	// {
	// 	boost::filesystem::create_directories(path);
	// }

	std::string path = prePath_ + "/" + mode_ + "/" + hms_ + mil_ + mic_;
	if(!mind_os::util::FilePath::isDirExisted(path.c_str()))
	{
		if(mind_os::util::FilePath::createDirR(path.c_str()))
		{
			LOGCOMMON(INFO) << "StorageTypeInit create dir success." << path;
		}
		else
		{
			LOGCOMMON(INFO) << "StorageTypeInit create dir failed." << path;
		}
	}
	path += "/";
	pathFive_ = path;
}

std::string LOGNAV::logNotes()
{
	std::string res;

	std::string HMS = getHMS();
	int HMS_I = std::stoi(HMS);

	int H_I = HMS_I/10000;
	std::string H_S = (H_I < 10) ? ("0" + std::to_string(H_I)) : (std::to_string(H_I));

	int M_I = HMS_I/100%100;
	std::string M_S = (M_I < 10) ? ("0" + std::to_string(M_I)) : (std::to_string(M_I));

	int S_I = HMS_I%100;
	std::string S_S = (S_I < 10) ? ("0" + std::to_string(S_I)) : (std::to_string(S_I));

	std::string Mil_S = getMil();

	res = H_S + ":" + M_S + ":" + S_S + ":" + Mil_S + " --- ";
	return res;
}

void LOGNAV::checkLogDate(const char* path_, std::string& fileAddress_, std::string& fileName_, std::fstream& FILE_)
{
	std::string date = getYMD();
    if(std::stoi(date) > std::stoi(pathTwo_))
    {
        FILE_.close();
        startWorkInit(path_);
        storageTypeInit(LOG_HSM);
        fileAddress_ = pathFive_;
        fileName_ = getHMS() + ".txt";
        FILE_.open(fileAddress_ + fileName_, std::fstream::out);
    }
}