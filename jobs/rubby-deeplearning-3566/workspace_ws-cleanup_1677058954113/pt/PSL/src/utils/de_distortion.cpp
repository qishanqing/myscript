#include "de_distortion.h"
#include "src/utils/config.h"

psl::CameraMoudleParam DeDistortion::moduleParam = {};
cv::Mat DeDistortion::remapXLeftFisheye = cv::Mat();
cv::Mat DeDistortion::remapYLeftFisheye = cv::Mat();
cv::Mat DeDistortion::remapXRightFisheye = cv::Mat();
cv::Mat DeDistortion::remapYRightFisheye = cv::Mat();
bool DeDistortion::flag = false;

bool DeDistortion::ReadPara(const psl::CameraParam &cameraParam
              , cv::Mat& remapX, cv::Mat& remapY)
{
    cv::Mat K, P, R, D;
    K = cv::Mat(3, 3, CV_64FC1, (unsigned char *) cameraParam._K);
    D = cv::Mat(4, 1, CV_64FC1, (unsigned char *) cameraParam._D);
//    R = cv::Mat::eye(3, 3, CV_64FC1);
     R = cv::Mat(3, 3, CV_64FC1, (unsigned char *) cameraParam._R);
    P = cv::Mat(3, 4, CV_64FC1, (unsigned char *) cameraParam._P);

    remapX.create(cv::Size(WIDTH, HEIGHT), CV_32FC1);
    remapY.create(cv::Size(WIDTH, HEIGHT), CV_32FC1);
    cv::fisheye::initUndistortRectifyMap(K, D, R, P.rowRange(0, 3).colRange(0, 3)
                                         , cv::Size(WIDTH, HEIGHT), CV_32F
                                         , remapX, remapY);
}

bool DeDistortion::ReadParamLeft(const psl::CameraMoudleParam &moduleParam)
{
    ReadPara(moduleParam._left_camera.at(RESOLUTION), remapXLeftFisheye, remapYLeftFisheye);
    flag = true;

    return true;
}

bool DeDistortion::ReadParamRight(const psl::CameraMoudleParam &moduleParam)
{
    ReadPara(moduleParam._right_camera.at(RESOLUTION), remapXRightFisheye, remapYRightFisheye);
    flag = true;

    return true;
}

bool DeDistortion::Remap(const CameraType type, cv::Mat &image)
{
    if (!flag) return false;

    cv::setNumThreads(0);
    cv::Mat remap = image;
    cv::Mat rgb;

    if (LEFT == type)
    {
        cv::remap(image, remap, remapXLeftFisheye, remapYLeftFisheye
                  , cv::INTER_LINEAR);
    }
    else
    {
        cv::remap(image, remap, remapXRightFisheye, remapYRightFisheye
                  , cv::INTER_LINEAR);
    }
    if (1 == remap.channels())
    {
        std::vector<cv::Mat> grayGroup(3, remap);
        cv::merge(grayGroup, rgb);
    }
    else if (3 == remap.channels())
    {
        rgb = remap;
    }
    else
    {
        return false;
    }

    image = rgb;

    return true;
}
