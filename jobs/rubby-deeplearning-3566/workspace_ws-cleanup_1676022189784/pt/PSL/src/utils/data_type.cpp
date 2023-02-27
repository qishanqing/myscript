#include "data_type.h"
#include "src/utils/define.h"
#include "src/utils/timer.h"
#include "src/utils/file.h"

bool Empty(const swr::psl_sensor_types::LaserScan &data)
{
    return 0 == data.header.stamp || data.ranges.empty() || data.intensities.empty();
}

Data4SaveInfo::Data4SaveInfo(const std::string path, const bool checkObject)
{
    this->checkObject = checkObject;
    this->path = path;
    this->save = (not path.empty());
}

bool DataGroup::Empty() const
{
//    return 0 == time;
//    return ::Empty(this->lidar);
    return this->orgImage.left.empty();
}

DataGroup::DataGroup()
{

}

DataGroup::DataGroup(const Lidar &lidar, const Image &image, const Image &orgImage)
{
    this->lidar = lidar;
    this->image = image;
    this->orgImage = orgImage;
}

bool DataGroup::Remap()
{
    Timer timer;
    image = orgImage;

    if (orgImage.left.empty()) return false;
    if (not DeDistortion::Remap(LEFT, image.left)) return false;

//    if (orgImage.right.empty())
//    {
//        LOG_CHECK_DEBUG(INFO) << "right image is empty";
//    }
//    else
//    {
//        if (not DeDistortion::Remap(RIGHT, image.right))
//        {
//            LOG_CHECK_DEBUG(INFO) << "Fail to remap right image!";
//        }
//    }

    LOG_CHECK_TIME(INFO) << timer.TimingStr();

    return true;
}

psl::SlamResult DataGroup::GetPose() const
{
    return lidar.pose;
}

Image &Image::Clone()
{
    if (not this->left.empty())
    {
        cv::Mat temp = this->left.clone();;
        this->left = temp;
    }

    if (not this->right.empty())
    {
        cv::Mat temp = this->right.clone();;
        this->right = temp;
    }

    return *this;
}

Image::Image()
{
    this->Reset();
}

Image::Image(psl::Time time, cv::Mat data)
{
    this->time = time;
    this->left = data;
}

Image::Image(psl::Time time,const cv::Mat &imageLeft,const cv::Mat &imageRight)
{
    this->time = time;
    this->left = imageLeft;
    this->right = imageRight;
}

void Image::Reset()
{
    this->time = 0;
    this->left = cv::Mat();
    this->right = cv::Mat();
}

Image &Image::operator=(const Image &image)
{
    this->time = image.time;
    this->left = image.left; //.clone();
    this->right = image.right; //.clone();
    this->pose = image.pose;

    return *this;
}

Lidar::Lidar()
{

}

Lidar::Lidar(const swr::psl_sensor_types::LaserScan data
             , const psl::SlamResult pose)
{
    this->pose = pose;
    this->data = data;
}
