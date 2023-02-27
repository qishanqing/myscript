#pragma once

#include <map>
// 目标检测接口
// Detect(const cv::Mat&, vectot<BoxInfo>&) 

// 距离推算接口
// GetInstancesInfo(const vectot<BoxInfo>&，const ImrPose pose, vector<Instance>&)
// GetInstancesInfo(const vectot<BoxInfo>&，const ImrPose pose, vector<Instance>&, imrobot::Obstacle &)

// 安全策略接口
// GetInstancesStatus(const ector<Instance>&, imrobot::Obstacle &)

// SDK接口
// Detector(const MoudleParam&, DetectorParam &)
// SetDetectorCallback(DetectorCallback);
// SetErrorCallback(ErrorCallback)


/* Input
struct MoudleParam

*/
struct CameraParameter 
{
    double TSC[16]; // 4X4 camera to imu
    int width;
    int height;

    // distortion_type:equidistant
    double focalLength[2];    // fx,fy
    double principalPoint[2]; // cx,cy
    double R[9];
    // Projection/camera matrix
    //     [fx'  0  cx' Tx]
    // P = [ 0  fy' cy' Ty]
    //     [ 0   0   1   0]
    double P[12];
    // Intrinsic camera matrix for the raw (distorted) images.
    //     [fx  0 cx]
    // K = [ 0 fy cy]
    //     [ 0  0  1]
    double K[9];
    // The distortion parameters, size depending on the distortion model.
    // For us, the 4 parameters are: (k1, k2, t1, t2).
    double D[4];
};
返回数据结构：
struct BoxInfo {
cv::Rect box;
float score;
float cx;
float cy;
// in some case, float type is needed
float width;
float height;

// class
className className;

// used for record its position
int index;
};
struct InstanceInfo {
// class
className className;

// instance
int instanceID;

// xyz
float location[3];
// camera coords
float locationCamera[3];

// time
float time;

// scale
float scale;

// used calc iou in image
cv::Rect box;

int _count;
float frontFacePoints[3][3];
float rearFacePoints[3][3];
bool visible;

int classCount = 0;

bool valid;

// calc from location instead of center of box
float cx;
float cy;
float depth;
};
Struct Region
{
// 状态
int status;
// 中心点
float center[3];
// 动静态判定半径
float radius;
// 静态计数
int staticCount;
// 无人统计
int noPersonCount;
// 有人统计
int havePerson;
};
struct Obstacle
{
int status;
int RegionSize;
std::vector<int> mapIndexes;
std::vector<Region> Regions;
}
Yaml结构：
deeplearning：
LOGShow: 0
ParamShow: 0
StatusShow: 0
Display: 0
radius: 10			# 动静态判定半径
diagcondation: 10	# 动静态判定帧率
distance: 3.0			# 危险区域判定距离
angle: 30.0			# 危险区域判定角度
clpath: /xxx/xxx/xxx/xxx.cl
modelpath: xxx.mnn.1.0.0
threshold: 0.5
imagesize: 256
version: 1.0.0