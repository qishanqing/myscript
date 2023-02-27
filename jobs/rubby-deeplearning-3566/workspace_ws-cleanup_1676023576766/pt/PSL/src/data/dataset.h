//////////////////////////////////////////////////////////////////////
///  @file     data.h
///  @brief    for data sync
///  Details.
///         data include image, lidar, pose, depth
///  @author   sunhao
///  @version
///  @date     2021.07.01
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_DATASET_H
#define DETECTOR_SAMPLE_DATASET_H

#include <list>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include "src/utils/data_type.h"
#include "src/utils/define.h"
#include "src/utils/config.h"
#include "observer_data.h"
#include "src/utils/define.h"
#include "src/error.h"

class SaveDataset;

typedef std::function<bool(const psl::Time time
                           , psl::SlamResult &res)> GetPoseCallback;

const int MAX_ITEM_COUNT = 50;
const psl::Time TIME_DIFF = UINT64_MAX;
const int TOLARENT_TIME_4_DATA = 200; // milliseconds // TODO : move to config


namespace dataset
{
struct StatusItem
{
    bool lack;
    std::string name;
    ErrorMessage *errorMessage;

    StatusItem();

    StatusItem(const std::string &name);

    bool Check(const bool empty);

    ErrorMessageCode ErrorCode(const std::string &name);
};

struct StatusData
{
    StatusItem image = StatusItem("image");
    StatusItem lidar = StatusItem("lidar");
    StatusItem depth = StatusItem("depth");
    StatusItem pose = StatusItem("pose");
    // TODO : why StatusItem pose("pose") error;
};

const std::string IMAGE = "image";
const std::string LIDAR = "lidar";
}

// TODO : add data
class DataSet
{
public:
    DataSet();

    ~DataSet();

    bool Init(const psl::CameraMoudleParam &moduleParam
              , const psl::Resolution resolution
              , const ConfigParam &configParam);

    void SetGetPoseByTime(GetPoseCallback getPoseFunc);

    void SetImage(const psl::Time &time, const cv::Mat &image);

    void SetLidar(const swr::sensor_types::LaserScan &lidar);

    void SetPose(const psl::SlamResult &pose);

    void Attach(ObserverData *observer);

    void Remove(ObserverData *observer);

    void Notify(); // sent data to observers

    void SetReceive(const bool flag);

private:
    /**
    * @brief sync all data by time
    * @param[in](member) poses lidars images;
    * @param[out](member) items;
    * @return void
    */

    bool GetPoseByTime(Lidar &lidar, Image &image);

    bool GetPoseByTime(const psl::Time time
                       , psl::SlamResult &pose);

    void PopPose(const psl::Time time);

    bool GetImageByTime(const psl::Time time, Image &image);

    bool GetLidar(swr::sensor_types::LaserScan &lidar);

    void Sync();

    void ThreadRun();

    void CleanHistory(const swr::sensor_types::LaserScan &current);

    bool CheckSatus();

    bool Waiting();

    template<class T>
    void RemoveUnusedData(std::queue<T> &dataList);

    template<class T>
    void RemoveUnusedData(std::list<T> &dataList);

    void Save(const DataGroup &item) const;

    template<class T>
    void GetCurrentData(const psl::Time stamp
                        , std::queue<T> &dataList, T &item);

    void ReadPara(const double *Kl, const double *Dl, const double *Rl, const double *Pl
                  , cv::Mat &remapXLeft_fisheye_H, cv::Mat &remapYLeft_fisheye_H);

    bool InitCamera(const psl::CameraMoudleParam &moduleParam);

private:
    GetPoseCallback getPoseByTime;
    std::list<psl::SlamResult> poses;
    std::list<swr::sensor_types::LaserScan> lidars;
//    std::list<swr::depth_types::DepthMatrix> depthes;
    std::list<Image> images;
    std::list<DataGroup> syncedData; // 已做完同步的数据
    std::list<ObserverData *> observers; // the users who need the syncedData
    std::thread threadNotify;
    bool working;
    std::mutex imageMutex, lidarMutex, poseMutex;
    SaveDataset *savedata;
    ConfigParam configParam;
    dataset::StatusData statusData;

    // camera
    psl::CameraMoudleParam cameraParam;
    // error
    ErrorMessage *errorMessage;
    bool notifyRunning;
};


template<class T>
void DataSet::RemoveUnusedData(std::queue<T> &dataList)
{
    int size = dataList.size();
    while (size-- > MAX_ITEM_COUNT)
    {
        dataList.pop();
    }
}

template<class T>
void DataSet::RemoveUnusedData(std::list<T> &dataList)
{
    int size = dataList.size();
    while (size-- > MAX_ITEM_COUNT)
    {
        dataList.pop_front();
    }
}

template<class T>
void DataSet::GetCurrentData(const psl::Time stamp
                             , std::queue<T> &dataList, T &item)
{
    do
    {
        psl::Time timediffTemp =
                ABS_DIFF(dataList.front().s_time, stamp);
        if (timediffTemp < TIME_DIFF)
        {
            item = dataList.front();
            dataList.pop();
        }
        else
        {
            break;
        }
    } while (not dataList.empty());
}

#endif //DETECTOR_SAMPLE_DATASET_H
