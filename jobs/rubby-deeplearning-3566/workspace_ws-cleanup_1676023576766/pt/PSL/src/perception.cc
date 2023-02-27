#include "perception.h"
#include "perception_inner.h"

robot_detector::Detector* robot_detector::Create(const psl::CameraMoudleParam &moduleParam)
{
    return static_cast<Detector*>(new DetectorInner(moduleParam));
}

void robot_detector::Destroy(robot_detector::Detector *detectorPtr)
{
    if (detectorPtr != nullptr)
    {
        delete detectorPtr;
        detectorPtr = nullptr;
    }
}

Detector *robot_detector::Create()
{
    return static_cast<Detector*>(new DetectorInner());
}
