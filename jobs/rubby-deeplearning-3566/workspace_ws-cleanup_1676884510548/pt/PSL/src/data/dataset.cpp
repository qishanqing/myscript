//
// Created by hao on 2021/7/1.
//

#include "dataset.h"
#include "src/utils/utils.h"
#include "src/utils/define.h"
#include "src/data/save_dataset.h"
#include <sys/prctl.h>
#include "src/utils/de_distortion.h"

const int WAIT_4_DATA_TIME = 5; // milliseconds
const float EXP = 0.000001;
const float EXP_MAX = 65536;
const uint MIN_TIME_MARGIN = 5; // 5ms
const uint WAIT_FOR_POSE = 30; // 30ms

DataSet::DataSet()
        : working(false)
          , savedata(nullptr)
          , getPoseByTime(nullptr)
          , errorMessage(nullptr)
          , notifyRunning(true)
{
    errorMessage = ErrorMessage::GetInstance();
}

DataSet::~DataSet()
{
    working = false;
    notifyRunning = false;
    if (threadNotify.joinable())
    {
        threadNotify.join();
    }
    RELEASE(savedata);
}

void DataSet::Attach(ObserverData *observer)
{
    this->observers.push_back(observer);
}

void DataSet::Remove(ObserverData *observer)
{
    this->observers.remove(observer);
}

void DataSet::SetImage(const psl::Time &time, const cv::Mat &image)
{
    if (working)
    {
        std::unique_lock<std::mutex> lock(imageMutex);
        this->images.push_back(Image(time, image.clone()));
    }
}

void DataSet::SetLidar(const swr::sensor_types::LaserScan &lidar)
{
    if (working)
    {
        std::unique_lock<std::mutex> lock(lidarMutex);
        this->lidars.push_back(lidar);
    }
}

void DataSet::SetPose(const psl::SlamResult &pose)
{
    if (working)
    {
        std::unique_lock<std::mutex> lock(poseMutex);
        this->poses.push_back(pose);
    }
}

void DataSet::Notify()
{
    if (not working) return;

    // TODO : throw exception when no data for a long time
    if (syncedData.empty()) return;

    // for all observers
    for (auto it = observers.begin(); it != observers.end(); ++it)
    {
        (*it)->Update(syncedData.front());
    }
}

void DataSet::SetReceive(const bool flag = false)
{
    working = flag;

    // TODO : how to process savedata, more simple
    if (nullptr != savedata and flag) savedata->NextBatch();
}

void DataSet::Sync()
{
    Timer timerAll, timer;
    if (not working) return;
    // sync based on lidar tampstemp
    // TODO : add serials data empty except
    if (Waiting()) return;

    {
        std::unique_lock<std::mutex> lock(lidarMutex);
        RemoveUnusedData(lidars);
    }
    {
        std::unique_lock<std::mutex> lock(poseMutex);
        RemoveUnusedData(poses);
    }
    {
        std::unique_lock<std::mutex> lock(imageMutex);
        RemoveUnusedData(images);
    }

    Lidar currentLidar;
    static Image currentOrgImage;
    bool ret = false;

    ret = GetLidar(currentLidar.data);
    if (not ret)
    {
        LOG_CHECK_DEBUG(WARNING) << "lidar is empty.";
        SLEEP_MS(WAIT_4_DATA_TIME);
        return;
    }

    LOG_CHECK_TIME(INFO) << timer.TimingStr("lidar");
    // TODO : refactor pose check
    ret &= GetImageByTime(currentLidar.data.header.stamp, currentOrgImage);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("image");
    ret &= GetPoseByTime(currentLidar, currentOrgImage);
    LOG_CHECK_TIME(INFO) << timer.TimingStr("pose");
    if (ret) PopPose(MAX(currentLidar.pose.s_time, currentOrgImage.pose.s_time));
    else return;  // happen when start, you lack of pose and GetPoseByTime are alse not have pose

    long long timeMargin = ABS_DIFF(currentOrgImage.time, currentLidar.data.header.stamp);
    bool dataValid = timeMargin < configParam.timeMargin;

    if (dataValid)
    {
        syncedData.push_back(DataGroup(currentLidar, Image(), currentOrgImage));
        currentOrgImage.Reset();
        LOG_CHECK_DEBUG(INFO) << "time margin [true]: " << timeMargin;
    }
    else
    {
        LOG_CHECK_DEBUG(INFO) << "time margin [false]: " << timeMargin;
    }

    return;
}

void DataSet::SetGetPoseByTime(GetPoseCallback getPoseFunc)
{
    getPoseByTime = getPoseFunc;
}

void DataSet::ThreadRun()
{
    prctl(PR_SET_NAME, "dl_dataset");
    LOG(INFO) << "[" << MODULE_NAME << "] " << "dataset start.";

    while (true)
    {
        if (working)
        {
            try
            {
                Sync();

                if (syncedData.empty())
                {
                    SLEEP_MS(WAIT_4_DATA_TIME);
                    continue;
                }

                Save(syncedData.front());
                Notify();
                syncedData.pop_front();
            }
            catch (...)
            {
                LOG_CHECK_STATUS(ERROR) << "error when sync data.";
            }
        }
        else
        {
            SLEEP_MS(WAIT_4_DATA_TIME);
        }
    }
}

void DataSet::CleanHistory(const swr::sensor_types::LaserScan &current)
{
    while (not lidars.empty())
    {
        if (lidars.front().header.stamp != current.header.stamp)
        {
            lidars.pop_front();
        }
        else
        {
            lidars.pop_front();
            break;
        }
    }

    END();
}

bool DataSet::Init(const psl::CameraMoudleParam &moduleParam
                   , const psl::Resolution resolution
                   , const ConfigParam &configParam)
{
    threadNotify = std::thread([this]()
                               { this->ThreadRun(); });

    this->configParam = configParam;
    this->cameraParam = moduleParam;

    if (not InitCamera(moduleParam))
    {
        return false;
    }

    if (configParam.dataset.save) // TODO : check for path(type)
    {
        savedata = new SaveDataset();
        LOG_CHECK_DEBUG(INFO) << "init dataset save.";
        if (not savedata->Init(configParam.dataset.path, moduleParam, resolution))
        {
            LOG_CHECK_DEBUG(WARNING) << "dataset save dir create failed." << " <"
                                     << configParam.dataset.path << ">.";
        }
    }

    LOG_CHECK_DEBUG(INFO) << "success init.";

    return true;
}

void DataSet::Save(const DataGroup &item) const
{
    if (nullptr == savedata) return;
    savedata->WriteData(item.orgImage, item.lidar);
}

bool DataSet::CheckSatus()
{
    bool ret = true;

    {
        std::unique_lock<std::mutex> lock(imageMutex);
        ret &= statusData.image.Check(images.empty());
    }
    if (nullptr == getPoseByTime)
    {
        std::unique_lock<std::mutex> lock(poseMutex);
        ret &= statusData.pose.Check(poses.empty());
    }
    {
        std::unique_lock<std::mutex> lock(lidarMutex);
        ret &= statusData.lidar.Check(lidars.empty());
    }

    return ret;
}

dataset::StatusItem::StatusItem(const std::string &name)
{
    lack = true;
    this->name = name;
    errorMessage = ErrorMessage::GetInstance();
}

dataset::StatusItem::StatusItem()
{
    lack = true;
    name = "";
}

bool DataSet::InitCamera(const psl::CameraMoudleParam &moduleParam)
{
    if (!DeDistortion::ReadParamLeft(moduleParam))
    {
        return false;
    }

    LOG_CHECK_DEBUG(INFO) << "success init.";
    return true;
}

bool DataSet::GetPoseByTime(Lidar &lidar, Image &image)
{
    LOG_CHECK_DEBUG(INFO) << "get pose.";

    bool ret = GetPoseByTime(lidar.data.header.stamp, lidar.pose);
    ret &= GetPoseByTime(image.time, image.pose);

    return ret;
}

bool DataSet::GetPoseByTime(const psl::Time time
                            , psl::SlamResult &pose)
{
    // TODO : what the ret mean for, determine what to do after
    bool ret = false;
    psl::Time timediffTempLast = TIME_DIFF;

    if (nullptr == getPoseByTime)
    {
        ret = false;
        LOG_CHECK_STATUS_WITE_STEP(ONE_MINUTE, "getPoseByTime is null.");
    }
    else
    {
        // TODO : when get pose error
        ret = getPoseByTime(time, pose);
        if (not ret)
        {
            SLEEP_MS(WAIT_FOR_POSE);
            ret = getPoseByTime(time, pose);
        }
        CHECK_STATUS_WITH_TIME_END(ret, TWO_MINUTE, "slam getPoseByTime error.");
    }

    if (not ret)
    {
        // TODO : pose delay
        std::unique_lock<std::mutex> lock(poseMutex);
        // TODO : get interpolation by time
        // TODO : use the latest pose in this->poses
        for (auto &p : this->poses)
        {
            psl::Time timediff = ABS_DIFF(p.s_time, time);

            if (timediff < timediffTempLast)
            {
                pose = p;
                timediffTempLast = timediff;
            }
            else
            {
                ret = true;  // TODO : use pose interpret
                break;
            }
        }
    }

    if (timediffTempLast < MIN_TIME_MARGIN) ret = true;

    return ret;
}

void DataSet::PopPose(const psl::Time time)
{
    std::unique_lock<std::mutex> lock(poseMutex);
    while (not poses.empty())
    {
        if (poses.front().s_time < time)
        {
            poses.pop_front();
        }
        else
        {
            break;
        }
    }
}

bool DataSet::GetImageByTime(const psl::Time time, Image &lastImage)
{
    LOG_CHECK_DEBUG(INFO) << "get image.";
    // TODO : if image is empty, should to emit error status;

    psl::Time timediffTempLast = TIME_DIFF;

    if ((not lastImage.data.empty())
        and ABS_DIFF(lastImage.time, time) < configParam.timeMargin)
    {
        // use the last image
    }
    else
    {
        std::unique_lock<std::mutex> lock(imageMutex);
        if (images.empty()) return false;

        while (not images.empty())
        {
            psl::Time timediff = ABS_DIFF(images.front().time, time);
            if (timediff < timediffTempLast)
            {
                lastImage = images.front();
                images.pop_front();
                timediffTempLast = timediff;
            }
            else break;
        }
    }

    return true;
}

bool DataSet::Waiting()
{
    if (CheckSatus())
    {
        return false;
    }
    else
    {
        SLEEP_MS(WAIT_4_DATA_TIME);
        return true;
    }
}

bool DataSet::GetLidar(swr::sensor_types::LaserScan &lidar)
{
    std::unique_lock<std::mutex> lock(lidarMutex);
    if (this->lidars.empty()) return false;
    lidar = lidars.back();
    CleanHistory(lidar);

    return true;
}

ErrorMessageCode dataset::StatusItem::ErrorCode(const std::string &name)
{
    //TODO add other lack image exception
    if (name == IMAGE) return ErrorMessageCode::DL_LACK_IMAGE;
    else if (name == LIDAR) return ErrorMessageCode::DL_LACK_LIDAR;
}

bool dataset::StatusItem::Check(const bool empty)
{
    static time_t lastDataTime = GET_TIMESTAMP;
    if (not empty)
    {
        lastDataTime = GET_TIMESTAMP;
    }

    // print log per 1s in 5s, when lack of data than 100ms
    bool lack = (GET_TIMESTAMP - lastDataTime > TOLARENT_TIME_4_DATA) and empty;
    CHECK_STATUS_WITH_TIME_END(not lack, TEN_SECOND, "lack of " + name + " data.");

    //TODO deal with pose Report and Cancel exception
    if (lack and (name == dataset::IMAGE or name == dataset::LIDAR))
    {
        errorMessage->Push(ErrorCode(name), "lack of " + name + " data");
    }

    if (lack != this->lack and not lack)
    {
        LOG_CHECK_STATUS(INFO) << "receive " << name << " data.";
        if (name == dataset::IMAGE or name == dataset::LIDAR)
        {
            errorMessage->Pop(ErrorCode(name), "receive " + name + " data");
        }
    }

    this->lack = lack;

    return not empty;
}
