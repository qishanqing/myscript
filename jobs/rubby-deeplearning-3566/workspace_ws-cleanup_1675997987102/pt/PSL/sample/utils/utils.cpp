//
// Created by hao on 2021/6/15.
//

#include "utils.h"
#include <unistd.h>
#include <fstream>
#include <opencv2/core.hpp>

#define EXIST(file) (access((file).c_str(), 0) == 0)

const psl::Resolution RESOLUTION = psl::Resolution::RES_640X400;


uint64_t GetImageTime(const std::string imageFile)
{
    int startTime = imageFile.rfind('_');
    int endTime = imageFile.rfind('.');
    uint64_t time = std::stoul(imageFile.substr(startTime + 1, endTime - startTime));

    return time;
}

bool GetData(const std::string inputDir, std::vector<SyncDataFile>& dataset)
{
    std::string imagesTxt = inputDir + "/image.txt";
    std::string lidarTxt = inputDir + "/lidar.txt";
    std::string syncTxt = inputDir + "/sync.txt";
    const bool synced = not EXIST(imagesTxt);
    bool binocular = false;
    std::vector<std::string> imageNameList, lidarNameList;
    std::vector<SyncDataFile> fileList;

    if (synced)
    {
        if (not ReadSyncFile(syncTxt, fileList)) exit(0);
    }
    else
    {
        if (not ReadFile(imagesTxt, imageNameList)) exit(0);
        if (not ReadFile(lidarTxt, lidarNameList)) exit(0);
    }

    const size_t size =
            imageNameList.size() > 0 ? imageNameList.size() : fileList.size();

    for (size_t i = 0; i < size; ++i)
    {
        SyncDataFile item;

        if (synced)
        {
            item = fileList.at(i).SetPrefix(inputDir + "/");
            if (not EXIST(item.imageLeft))
            {
                binocular = true;
                item.AddCam01Path();
            }
        }
        else
        {
            item.imageLeft = inputDir + "/" + imageNameList.at(i);
            item.lidar = inputDir + "/" + lidarNameList[i];
            item.lidarPose = item.lidar;
            item.lidarPose = item.lidarPose.replace(item.lidar.find("lidar"), 5, "slam");
            item.imagePose = item.lidarPose;
        }

        dataset.push_back(item);
    }

    return binocular;
}

void ReadFilesFromDir(const std::string &path_to_dir
                      , std::vector<std::string> *image_name_list)
{
    DIR *dir;
    dir = opendir(path_to_dir.c_str());
    struct dirent *ent;
    // CHECK_NOTNULL(dir);
    while ((ent = readdir(dir)) != nullptr)
    {
        auto name = std::string(ent->d_name);
        // ignore "." ".."
        if (name.size() < 4)
        {
            continue;
        }
        auto suffix = name.substr(name.size() - 4, 4);
        if (suffix == ".png" || suffix == ".jpg")
        {
            // filter image
            image_name_list->emplace_back(name);
        }
    }

    closedir(dir);
}

bool ReadSyncFile(std::string srcFile, std::vector<SyncDataFile> &files)
{
    if (not access(srcFile.c_str(), 0) == 0)
    {
        ERROR_PRINT("no such File (" + srcFile + ")");
        return false;
    }

    std::ifstream fin(srcFile.c_str());

    if (!fin.is_open())
    {
        ERROR_PRINT("read file error (" + srcFile + ")");
        return false;
    }

    std::string s;
    SyncDataFile syncFile;

    do
    {
        fin >> syncFile.imageLeft >> syncFile.imagePose >> syncFile.lidar
            >> syncFile.lidarPose;
        files.push_back(syncFile);
    } while (fin.get() != EOF);

    if (files.size() > 1) files.pop_back();

    fin.close();

    return true;
}

bool ReadFile(std::string srcFile, std::vector<std::string> &image_files)
{
    if (not access(srcFile.c_str(), 0) == 0)
    {
        ERROR_PRINT("no such File (" + srcFile + ")");
        return false;
    }

    std::ifstream fin(srcFile.c_str());

    if (!fin.is_open())
    {
        ERROR_PRINT("read file error (" + srcFile + ")");
        return false;
    }

    std::string s;
    while (getline(fin, s))
    {
        image_files.push_back(s);
    }

    fin.close();

    return true;
}

void ReadArray(const YAML::Node &config, std::vector<float> &array)
{
    try
    {
        array = config.as<std::vector<float>>();
    }
    catch (...)
    {
        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it)
        {
            array.push_back((*it).as<float>());
        }
    }
}

bool GetCameraConfig(std::string file, psl::CameraMoudleParam &param)
{
    cv::FileStorage fileStream = cv::FileStorage(file, cv::FileStorage::READ);

    if (not fileStream.isOpened())
    {
        ERROR_PRINT("file not exist <" + file + ">");
        return false;
    }

    // TODO : the exception for lack option
    cv::Mat_<double> kl, dl, pl, rl;
    fileStream["Kl"] >> kl;
    fileStream["Dl"] >> dl;
    fileStream["Pl"] >> pl;
    fileStream["Rl"] >> rl;

    memcpy(param._left_camera[RESOLUTION]._K, kl.data
           , sizeof(param._left_camera[RESOLUTION]._K));
    memcpy(param._left_camera[RESOLUTION]._R, rl.data
           , sizeof(param._left_camera[RESOLUTION]._R));
    memcpy(param._left_camera[RESOLUTION]._P, pl.data
           , sizeof(param._left_camera[RESOLUTION]._P));
    memcpy(param._left_camera[RESOLUTION]._D, dl.data
           , sizeof(param._left_camera[RESOLUTION]._D));

    // TODO : the exception for lack option
    cv::Mat_<double> kr, dr, pr, rr;
    fileStream["Kr"] >> kr;
    fileStream["Dr"] >> dr;
    fileStream["Pr"] >> pr;
    fileStream["Rr"] >> rr;
    memcpy(param._right_camera[RESOLUTION]._K, kr.data
            , sizeof(param._right_camera[RESOLUTION]._K));
    memcpy(param._right_camera[RESOLUTION]._R, rr.data
            , sizeof(param._right_camera[RESOLUTION]._R));
    memcpy(param._right_camera[RESOLUTION]._P, pr.data
            , sizeof(param._right_camera[RESOLUTION]._P));
    memcpy(param._right_camera[RESOLUTION]._D, dr.data
            , sizeof(param._right_camera[RESOLUTION]._D));

    fileStream.release();

    return true;
}

bool GetLidar(std::string yamlFile, swr::psl_sensor_types::LaserScan &data, bool mode)
{
    try
    {
        YAML::Node config;

        if (not access(yamlFile.c_str(), 0) == 0)
        {
            std::cout << "file not exist <" + yamlFile + ">" << std::endl;
        }
        config = YAML::LoadFile(yamlFile);

        data.header.seq = config["Header"]["seq"].as<int>();
        data.header.stamp = config["Header"]["stamp"].as<unsigned long long>();

        data.angle_min = config["angle_min"].as<float>();
        data.angle_max = config["angle_max"].as<float>();
        data.angle_increment = config["angle_increment"].as<float>();

        data.scan_time = config["scan_time"].as<float>();
        data.range_min = config["range_min"].as<float>();
        data.range_max = config["range_max"].as<float>();

        // TODO : use try catch, not mode
        if (mode)
        {
            data.ranges = config["ranges"].as<std::vector<float>>();
            data.intensities = config["intensities"].as<std::vector<float>>();
        }
        else
        {
            ReadArray(config["ranges"], data.ranges);
            ReadArray(config["intensities"], data.intensities);
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool GetPose(std::string yamlFile, psl::SlamResult &pose)
{
    try
    {
        YAML::Node config;

        if (not access(yamlFile.c_str(), 0) == 0)
        {
            std::cout << "file not exist <" + yamlFile + ">" << std::endl;
        }
        config = YAML::LoadFile(yamlFile);

        pose.s_time = config["time"].as<unsigned long>();
        pose.s_state = config["state"].as<float>();

        try
        {
            pose.normal = config["normal"].as<bool>();
        }
        catch (YAML::TypedBadConversion<bool> &e)
        {
            pose.normal = true;
        }

        size_t size = 0;
        std::vector<double> position = config["position"].as<std::vector<double>>();
        size = position.size();
        for (size_t i = 0; i < size; ++i)
        {
            pose.s_position[i] = position[i];
        }

        std::vector<double> rotation = config["rotation"].as<std::vector<double>>();
        size = rotation.size();
        for (size_t i = 0; i < size; ++i)
        {
            pose.s_rotation[i] = rotation[i];
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}


std::string getCurrentExePath()
{
    char szPath[256] = {0};
    int ret = readlink("/proc/self/exe", szPath, sizeof(szPath) - 1);
    std::string path(szPath, ret);
    size_t last_ = path.find_last_of('/');
    if (std::string::npos != last_)
    {
        return path.substr(0, last_ + 1);
    }
    return path;
}

std::string getCurrentExeName()
{
    char szPath[256] = {0};
    int ret = readlink("/proc/self/exe", szPath, sizeof(szPath) - 1);
    std::string path(szPath, ret);
    size_t last_ = path.find_last_of('/');
    if (std::string::npos != last_)
    {
        return path.substr(last_ + 1);
    }
    return path;
}

void SyncDataFile::Print()
{
    std::cout << imageLeft << " " << imagePose << " "
              << lidar << " " << lidarPose << std::endl;
}
