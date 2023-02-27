#include "save_dataset.h"
#include "src/utils/file.h"
#include "utils/timer.h"
#include "src/utils/utils.h"
#include "src/utils/define.h"

const std::string SYNC_FILE = "sync.txt";
const char SPLIT_FLAG = '_';
namespace save_dataset
{
const std::string CONFIG_FILE_NAME = "config.yaml";
const std::string IMAGE_SUFFIX = ".jpg";
const std::string LIDAR_SUFFIX = ".yaml";
}

SaveDataset::SaveDataset()
        : outputDir("")
          , batch(0)
{

}

SaveDataset::~SaveDataset()
{

}

bool
SaveDataset::Init(const std::string path, const psl::Resolution resolution)
{
    // TODO : new thread
    this->resolution = resolution;
    rootPath = path;

    InitBatch(path);
    outputDir = path + SPLIT_FLAG + std::to_string(batch);

    return true;
}

cv::FileStorage SaveDataset::GetFileStreamConfig()
{
    std::string configFile = outputDir + "/" + save_dataset::CONFIG_FILE_NAME;
    LOG_CHECK_DEBUG(INFO) << "write config to: " << configFile;
    file_op::File::MkdirFromFile(configFile);
    return GetFileStream(configFile);
}

cv::FileStorage SaveDataset::GetFileStream(const std::string file)
{
    fileStream = cv::FileStorage(file, cv::FileStorage::WRITE);

    // TODO : mkdir for every  minutes
    return fileStream;
}


bool SaveDataset::WriteConfig()
{
    // TODO : only for 640*400, fix for other resolution
    cv::FileStorage fs = GetFileStreamConfig();

    auto citerLeft =
            param._left_camera.find(resolution);
    if (citerLeft == param._left_camera.cend())
    {
        LOG_CHECK_DEBUG(ERROR)
            << "fail to find the RES_640X400 from the left camera of model param.";
        // TODO : auto print the resolution ( RES_640X400)
        fs.release();
        return false;
    }

    auto citerRight =
            param._right_camera.find(resolution);
    if (citerRight == param._right_camera.cend())
    {
        LOG_CHECK_DEBUG(ERROR)
            << "fail to find the RES_640X400 from the right camera of model param.";
        // TODO : auto print the resolution ( RES_640X400)
        fs.release();
        return false;
    }

    const psl::CameraParam &leftCamera = citerLeft->second;
    const psl::CameraParam &rightCamera = citerRight->second;

    cv::Mat kl = cv::Mat(3, 3, CV_64FC1, (void *) leftCamera._K);
    cv::Mat kr = cv::Mat(3, 3, CV_64FC1, (void *) rightCamera._K);
    cv::Mat dl = cv::Mat(4, 1, CV_64FC1, (void *) leftCamera._D);
    cv::Mat dr = cv::Mat(4, 1, CV_64FC1, (void *) rightCamera._D);
    cv::Mat rl = cv::Mat(3, 3, CV_64FC1, (void *) leftCamera._R);
    cv::Mat rr = cv::Mat(3, 3, CV_64FC1, (void *) rightCamera._R);
    cv::Mat pl = cv::Mat(3, 4, CV_64FC1, (void *) leftCamera._P);
    cv::Mat pr = cv::Mat(3, 4, CV_64FC1, (void *) rightCamera._P);
    cv::Mat leftTSC(4, 4, CV_64FC1, (void *) leftCamera._TSC);
    cv::Mat rightTSC(4, 4, CV_64FC1, (void *) rightCamera._TSC);

    fs << "cameras"
       << "[";
    // left camera params
    fs << "{:"
       << "T_SC"
       << "[:" << leftCamera._TSC[0] << leftCamera._TSC[1] << leftCamera._TSC[2]
       << leftCamera._TSC[3] << leftCamera._TSC[4] << leftCamera._TSC[5]
       << leftCamera._TSC[6] << leftCamera._TSC[7] << leftCamera._TSC[8]
       << leftCamera._TSC[9] << leftCamera._TSC[10] << leftCamera._TSC[11]
       << leftCamera._TSC[12] << leftCamera._TSC[13] << leftCamera._TSC[14]
       << leftCamera._TSC[15] << "]";
    fs << "image_dimension"
       << "[:" << 640 << 400 << "]";
    fs << "distortion_coefficients"
       << "[:" << leftCamera._D[0] << leftCamera._D[1] << leftCamera._D[2]
       << leftCamera._D[3] << "]";
    fs << "distortion_type"
       << "equidistant";
    fs << "focal_length"
       << "[:" << leftCamera._focal_length[0] << leftCamera._focal_length[1]
       << "]";
    fs << "principal_point"
       << "[:" << leftCamera._principal_point[0]
       << leftCamera._principal_point[1] << "]";
    fs << "}";
    // right camera params
    fs << "{:"
       << "T_SC"
       << "[:" << rightCamera._TSC[0] << rightCamera._TSC[1]
       << rightCamera._TSC[2] << rightCamera._TSC[3] << rightCamera._TSC[4]
       << rightCamera._TSC[5] << rightCamera._TSC[6] << rightCamera._TSC[7]
       << rightCamera._TSC[8] << rightCamera._TSC[9] << rightCamera._TSC[10]
       << rightCamera._TSC[11] << rightCamera._TSC[12] << rightCamera._TSC[13]
       << rightCamera._TSC[14] << rightCamera._TSC[15] << "]";
    fs << "image_dimension"
       << "[:" << 640 << 400 << "]";
    fs << "distortion_coefficients"
       << "[:" << rightCamera._D[0] << rightCamera._D[1] << rightCamera._D[2]
       << rightCamera._D[3] << "]";
    fs << "distortion_type"
       << "equidistant";
    fs << "focal_length"
       << "[:" << rightCamera._focal_length[0] << rightCamera._focal_length[1]
       << "]";
    fs << "principal_point"
       << "[:" << rightCamera._principal_point[0]
       << rightCamera._principal_point[1] << "]";
    fs << "}";
    fs << "]";

    int cameraRate = 25;
    double sigmaAbsoluteTranslation = 0.0;
    double sigmaAbsoluteOrientation = 0.0;
    double sigmaCRrelativeTranslation = 0.0;
    double sigmaCRrelativeOrientation = 0.0;
    double timestampTolerance = 0.005;
    fs << "camera_params"
       << "{"
       << "camera_rate" << cameraRate << "sigma_absolute_translation"
       << sigmaAbsoluteTranslation << "sigma_absolute_orientation"
       << sigmaAbsoluteOrientation << "sigma_c_relative_translation"
       << sigmaCRrelativeTranslation << "sigma_c_relative_orientation"
       << sigmaCRrelativeOrientation << "timestampTolerance"
       << timestampTolerance << "}";

    fs << "Rl" << rl << "Rr" << rr << "Pl" << pl << "Pr"
       << pr;
    fs << "Kl" << kl << "Kr" << kr << "Dl" << dl << "Dr"
       << dr;

    double aMax = 176.0;
    double gMax = 30.0;
    double sigmaGC = 0.12000000000000000e+00;
    double sigmaAC = 9.0000000000000002e-03;
    double sigmaBg = 1.000000000000000e-01;
    double sigmaBa = 1.000000000000000e-03;
    double sigmaGwC = 4.0000000000000002e-05;
    double sigmaAwC = 4.0000000000000003e-05;
    double tau = 3600.;
    double g = 9.8019967;
    cv::Vec3d a0(0, 0, 0);
    int imuRate = 200;

    fs << "imu_params"
       << "{"
       << "a_max" << aMax << "g_max" << gMax << "sigma_g_c" << sigmaGC
       << "sigma_a_c" << sigmaAC << "sigma_bg" << sigmaBg << "sigma_ba"
       << sigmaBa << "sigma_gw_c" << sigmaGwC << "sigma_aw_c" << sigmaAwC
       << "tau" << tau << "g" << g << "a0" << a0 << "imu_rate" << imuRate
       << "T_BS"
       << "[:" << 1. << 0. << 0. << 0. << 0. << 1. << 0. << 0. << 0. << 0. << 1.
       << 0. << 0. << 0. << 0. << 1. << "]"
       << "}";

    fs << "useDriver"
       << "true";

    std::string strPublishLandmarks = "true";
    std::string strPublishImuPropagatedState = "true";
    std::string strTrackedBodyFrame = "B";
    std::string strVelocitiesFrame = "Wc";
    fs << "publishing_options"
       << "{"
       << "publish_rate" << 200 << "publishLandmarks" << strPublishLandmarks
       << "landmarkQualityThreshold" << 1.0e-2 << "maximumLandmarkQuality"
       << 0.05 << "maxPathLength" << 20 << "publishImuPropagatedState"
       << strPublishImuPropagatedState << "T_Wc_W"
       << "[:" << 1. << 0. << 0. << 0. << 0. << 1. << 0. << 0. << 0. << 0. << 1.
       << 0. << 0. << 0. << 0. << 1. << "]"
       << "trackedBodyFrame" << strTrackedBodyFrame << "velocitiesFrame"
       << strVelocitiesFrame << "}";

    fs.release();

    return true;
}

bool
SaveDataset::WriteData(const Image &image, const Lidar &lidar)
{
    bool rightImageExist = false;
    if (!image.right.empty())
    {
        rightImageExist = true;
    }
    // TODO : if data is empty 【all right】
    std::string date = "", second = "";
    std::string time = GetTimestamp(date, second);
    std::string root = outputDir + "/" + date + "/" + second + "_";
    std::string imageFile =
            root + std::to_string(image.time) + save_dataset::IMAGE_SUFFIX;
    std::string rootLeft = outputDir + "/" + date + "/cam0/" + second + "_";
    std::string rootRight = outputDir + "/" + date + "/cam1/" + second + "_";


    std::string imageLeftFile = "";
    std::string imageRightFile = "";

    if (rightImageExist)
    {
        imageLeftFile = rootLeft + std::to_string(image.time) + save_dataset::IMAGE_SUFFIX;
        imageRightFile = rootRight + std::to_string(image.time) + save_dataset::IMAGE_SUFFIX;
    }

    std::string imagePoseFile =
            root + std::to_string(image.time) + "_slam" + save_dataset::LIDAR_SUFFIX;
    std::string lidarFile = root + std::to_string(lidar.data.header.stamp) + "_lidar" +
                            save_dataset::LIDAR_SUFFIX;
    std::string lidarPoseFile = root + std::to_string(lidar.data.header.stamp) + "_slam" +
                                save_dataset::LIDAR_SUFFIX;

    // write image
    if (rightImageExist)
    {
        file_op::File::MkdirFromFile(imageLeftFile);
        cv::imwrite(imageLeftFile, image.left);

        file_op::File::MkdirFromFile(imageRightFile);
        cv::imwrite(imageRightFile, image.right);
    }
    else
    {
        file_op::File::MkdirFromFile(imageFile);
        cv::imwrite(imageFile, image.left);
    }

    // write lidar
    cv::FileStorage stream = GetFileStream(lidarFile);
    file_op::File::MkdirFromFile(lidarFile);

    const swr::psl_sensor_types::LaserScan lidarData = lidar.data;

    stream << "Header" << "{"
           << "seq" << std::to_string(lidarData.header.seq)
           << "stamp" << std::to_string(lidarData.header.stamp) << "}"
           << "angle_increment" << lidarData.angle_increment
           << "angle_max" << lidarData.angle_max
           << "angle_min" << lidarData.angle_min
           << "intensities" << lidarData.intensities
           << "range_max" << lidarData.range_max
           << "range_min" << lidarData.range_min
           << "ranges" << lidarData.ranges
           << "scan_time" << lidarData.scan_time
           << "time_increment" << lidarData.time_increment;

    stream.release();

    WritePose(lidar.pose, lidar.data.header.stamp, lidarPoseFile);
    WritePose(image.pose, image.time, imagePoseFile);

    // write sync file
    std::string syncFile = outputDir + "/" + SYNC_FILE;
    file_op::File::MkdirFromFile(syncFile);

    std::ofstream syncStream(syncFile, std::ios_base::out | std::ios_base::app);
    const auto rootLen = outputDir.length() + 1;
    syncStream << imageFile.substr(rootLen, imageFile.length() - rootLen) << " "
               << imagePoseFile.substr(rootLen, imagePoseFile.length() - rootLen) << " "
               << lidarFile.substr(rootLen, lidarFile.length() - rootLen) << " "
               << lidarPoseFile.substr(rootLen, lidarPoseFile.length() - rootLen)
               << std::endl;

    syncStream.close();

    return true;
}

bool SaveDataset::WritePose(const psl::SlamResult &pose
                            , const psl::Time time
                            , const std::string fileName)
{
    // write slam (pose, )
    cv::FileStorage stream = GetFileStream(fileName);
    file_op::File::MkdirFromFile(fileName);

    stream << "time" << std::to_string(time)
            << "state" << std::to_string(pose.s_state)
            << "normal" << std::to_string(pose.normal)
           << "rotation" << std::vector<float>(pose.s_rotation, pose.s_rotation + 4)
           << "position" << std::vector<float>(pose.s_position, pose.s_position + 3);

    stream.release();

    return true;
}

void SaveDataset::NextBatch()
{
    outputDir = rootPath + SPLIT_FLAG + std::to_string(batch++);
    WriteConfig();
}

void SaveDataset::InitBatch(const std::string path)
{
    std::string dirPath = file_op::File::Dir(path);
    std::vector<std::string> dirs;
    if (not file_op::File::Walk(dirPath, file_op::DIRECTORY, dirs))
    {
        batch = 0;
        return;
    }

    std::vector<int> id;

    std::string basename = file_op::File::Basename(path);
    // get number
    for (const auto& d : dirs)
    {
        auto it = d.find(basename + SPLIT_FLAG);
        if (it != std::string::npos)
        {
            it = d.rfind(SPLIT_FLAG);
            id.push_back(std::stoi(d.substr(it + 1, d.length() - it)));
        }
    }

    if (id.empty())
    {
        batch = 0;
    }
    else
    {
        batch = *std::max_element(id.begin(), id.end()) + 1;
    }
}

bool SaveDataset::InitCamera(const psl::CameraMoudleParam &moduleParam)
{
    this->param = moduleParam;

    return true;
}
