///////////////////////////////////////////////////////////////////////////////
// KalmanTracker.cpp: KalmanTracker Class Implementation Declaration

#include "KalmanTracker.h"


int KalmanTracker::kfCount = 0;


// initialize Kalman filter
void KalmanTracker::InitKf(StateType stateMat)
{
    int stateNum = 7;
    int measureNum = 4;
    kf = cv::KalmanFilter(stateNum, measureNum, 0);

    measurement = cv::Mat::zeros(measureNum, 1, CV_32F);

    kf.transitionMatrix = (cv::Mat_<float>(stateNum, stateNum) <<
                                                               1, 0, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 0, 1, 0,
            0, 0, 1, 0, 0, 0, 1,
            0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 1);

    setIdentity(kf.measurementMatrix);
    setIdentity(kf.processNoiseCov, cv::Scalar::all(1e-2));
    setIdentity(kf.measurementNoiseCov, cv::Scalar::all(1e-1));
    setIdentity(kf.errorCovPost, cv::Scalar::all(1));

    // initialize state vector with bounding box in [cx,cy,s,r] style
    std::vector<float> data = stateMat.Serialize();
    kf.statePost.at<float>(0, 0) = data.at(0);
    kf.statePost.at<float>(1, 0) = data.at(1);
    kf.statePost.at<float>(2, 0) = data.at(2);
    kf.statePost.at<float>(3, 0) = data.at(3);
}


// Predict the estimated bounding box.
StateType KalmanTracker::Predict()
{
    // predict
    cv::Mat p = kf.predict();

    age += 1;

    if (timeSinceUpdate > 0)
        hitStreak = 0;
    timeSinceUpdate += 1;

    StateType predictBox;
    predictBox.Parse(std::vector<float>{p.at<float>(0, 0), p.at<float>(1, 0)
                                        , p.at<float>(2, 0), p.at<float>(3, 0)});

    history.push_back(predictBox);
    return history.back();
}


// Update the state vector with observed bounding box.
void KalmanTracker::Update(StateType stateMat)
{
    timeSinceUpdate = 0;
    history.clear();
    hits += 1;
    hitStreak += 1;

    // measurement
    std::vector<float> data = stateMat.Serialize();
    measurement.at<float>(0, 0) = data.at(0);
    measurement.at<float>(1, 0) = data.at(1);
    measurement.at<float>(2, 0) = data.at(2);
    measurement.at<float>(3, 0) = data.at(3);

    // update
    kf.correct(measurement);
}


// Return the current state vector
std::vector<float> KalmanTracker::GetState()
{
    cv::Mat s = kf.statePost;

    return std::vector<float>{s.at<float>(0, 0), s.at<float>(1, 0)
                              , s.at<float>(2, 0), s.at<float>(3, 0)};
}

KalmanTracker::KalmanTracker()
{
    InitKf(StateType());
    timeSinceUpdate = 0;
    hits = 0;
    hitStreak = 0;
    age = 0;
    id = kfCount;
    //kfCount++;
}

KalmanTracker::KalmanTracker(StateType initRect)
{
    InitKf(initRect);
    timeSinceUpdate = 0;
    hits = 0;
    hitStreak = 0;
    age = 0;
    id = kfCount;
    kfCount++;
}

KalmanTracker::~KalmanTracker()
{
    history.clear();
}

/*
// --------------------------------------------------------------------
// Kalman Filter Demonstrating, a 2-d ball demo
// --------------------------------------------------------------------

const int winHeight = 600;
const int winWidth = 800;

Point mousePosition = Point(winWidth >> 1, winHeight >> 1);

// mouse event callback
void mouseEvent(int event, int x, int y, int flags, void *param)
{
	if (event == CV_EVENT_MOUSEMOVE) {
		mousePosition = Point(x, y);
	}
}

void TestKF();

void main()
{
	TestKF();
}


void TestKF()
{
	int stateNum = 4;
	int measureNum = 2;
	KalmanFilter kf = KalmanFilter(stateNum, measureNum, 0);

	// initialization
	Mat processNoise(stateNum, 1, CV_32F);
	Mat measurement = Mat::zeros(measureNum, 1, CV_32F);

	kf.transitionMatrix = *(Mat_<float>(stateNum, stateNum) <<
		1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1);

	setIdentity(kf.measurementMatrix);
	setIdentity(kf.processNoiseCov, Scalar::all(1e-2));
	setIdentity(kf.measurementNoiseCov, Scalar::all(1e-1));
	setIdentity(kf.errorCovPost, Scalar::all(1));

	randn(kf.statePost, Scalar::all(0), Scalar::all(winHeight));

	namedWindow("Kalman");
	setMouseCallback("Kalman", mouseEvent);
	Mat img(winHeight, winWidth, CV_8UC3);

	while (1)
	{
		// predict
		Mat prediction = kf.predict();
		Point predictPt = Point(prediction.at<float>(0, 0), prediction.at<float>(1, 0));

		// generate measurement
		Point statePt = mousePosition;
		measurement.at<float>(0, 0) = statePt.x;
		measurement.at<float>(1, 0) = statePt.y;

		// update
		kf.correct(measurement);

		// visualization
		img.setTo(Scalar(255, 255, 255));
		circle(img, predictPt, 8, CV_RGB(0, 255, 0), -1); // predicted point as green
		circle(img, statePt, 8, CV_RGB(255, 0, 0), -1); // current position as red

		imshow("Kalman", img);
		char code = (char)waitKey(100);
		if (code == 27 || code == 'q' || code == 'Q')
			break;
	}
	destroyWindow("Kalman");
}
*/
