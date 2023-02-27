//
// Created by gby on 22-7-11.
//

#ifndef RUBBYDECISION_LOG_H
#define RUBBYDECISION_LOG_H

#include <boost/filesystem.hpp>
#include <iostream>
#include <ctime>
#include <string>
#include <chrono>
#include <vector>
#include "dirent.h"
#include "Singleton.h"
#include <mind_os/mind_os.h>
#include "Common.h"
#define BOOST_FILESYSTEM_NO_DEPRECATED

enum Type
{
  LOG_HSM = 0,     /*状态机*/
  LOG_PLANNING,    /*规划*/
  LOG_SEARCH,      /*搜索*/
  LOG_BOW,         /*弓字形*/
  LOG_NAV,         /*导航*/
  LOG_BOX,         /*区域*/
  LOG_MOTION       /*运动控制*/
};

class LOGNAV
{
public:
	void startWorkInit(const char* logAddress_);     /*开始工作初始化：从状态机运行开始，创建至第三级目录*/
  void storageTypeInit(const int type_);
  void checkLogDate(const char* path_, std::string& fileAddress_, std::string& fileName_, std::fstream& FILE_);
  std::string logNotes();    /*注释接口，格式：时:分:秒:毫秒 --- */
  std::string pathTwo_;     /*获得string年月日*/
  std::string pathFive_;     /*五级目录地址*/
  std::string getYMD();       /*获得年月日*/   
  std::string getHMS();       /*获得时分秒*/

private:
  void checkLogDirNum(const std::string logAddress_);    /*保证log目录不超过7个*/

  std::string getMil();          /*获得毫秒，保留3位*/
  std::string getMic();         /*获得微秒，保留3位*/


private:
  std::string pathThree_;   /*三级目录地址*/

};

#define s_log Singleton<LOGNAV>::GetInstance()

#endif