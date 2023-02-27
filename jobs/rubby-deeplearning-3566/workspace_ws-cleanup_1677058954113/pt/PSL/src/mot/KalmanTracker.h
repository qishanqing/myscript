///////////////////////////////////////////////////////////////////////////////
// KalmanTracker.h: KalmanTracker Class Declaration

#ifndef KALMAN_H
#define KALMAN_H 2

#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "src/utils/shape/rect.h"

using namespace std;

using StateType = Rect2f;


// This class represents the internel state of individual tracked objects observed as bounding box.
class KalmanTracker
{
public:
    KalmanTracker();

    KalmanTracker(StateType initRect);

    ~KalmanTracker();

    StateType Predict();

    void Update(StateType stateMat);

    std::vector<float> GetState();

private:
    void InitKf(StateType stateMat);

public:
    static int kfCount;

    int timeSinceUpdate;
    int hits;
    int hitStreak;
    int age;
    int id;

private:
    cv::KalmanFilter kf;
    cv::Mat measurement;
    std::vector<StateType> history;
};


#endif