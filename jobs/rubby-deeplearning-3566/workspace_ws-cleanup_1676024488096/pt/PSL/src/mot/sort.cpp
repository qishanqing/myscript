//////////////////////////////////////////////////////////////////////
///  @file     sort.cpp
///  @brief    多目标跟踪算法——sort 实现
///  Details.
///
///  @author   孙昊
///  @version  1.0.0.0(版本号)
///  @date     2019.03.15
///
///  修订说明: 初始版本
//////////////////////////////////////////////////////////////////////
#include "sort.h"
#include "Hungarian.h"
#include "KalmanTracker.h"
#include <opencv2/video/tracking.hpp>
#include "src/utils/define.h"

using namespace std;

Sort::Sort()
        : MAX_AGE(1)
          , MIN_HISTS(3)
          , frameNum(0)
          , iouThreshold(0.3)
{
    KalmanTracker::kfCount = 0; // tracking id relies on this, so we have to reset it in each seq.
}

Sort::~Sort()
{

}

void Sort::SetMinID(const int id)
{
    KalmanTracker::kfCount = id;
}

void Sort::Update(const ShapeList &boxes, std::vector<TrackingBox<Rect2f>> &results, const int &id)
{
    SetMinID(id);
    if (0 == boxes.size())
    {
        return;
    }

    LOG_CHECK_DEBUG(INFO) << "track size: " << trackers.size();
    ++frameNum;
    if (0 == trackers.size())
    {// init
        TrackingBox<Rect2f> box;
        for (const auto &b : boxes)
        {
            trackers.push_back(KalmanTracker(b.data));
            box.region.Parse(trackers[trackers.size() - 1].GetState());
            box.id = trackers[trackers.size() - 1].id + 1;
            box.frame = frameNum;
            results.push_back(box);
        }
    }
    else
    {// update
        vector<ShapeType> boxesPredicted;
        Predict(boxesPredicted);
        Tracker(boxesPredicted, boxes, results);
//        Update(m_boxes, m_boxes);
    }
}

void Sort::Predict(ShapeList &boxes)
{
    boxes.clear();
    for (auto it = trackers.begin(); it != trackers.end();)
    {
        StateType pBox = it->Predict();

        if (pBox.data.x >= 0 && pBox.data.y >= 0)
        {
            boxes.push_back(ShapeType(pBox));
            ++it;
        }
        else
        {
            it = trackers.erase(it);
        }
    }
}

double Sort::IOU(cv::Rect_<float> test, cv::Rect_<float> groundtruth)
{
    double in = (test & groundtruth).area();
    double un = test.area() + groundtruth.area() - in;

    if (un < DBL_EPSILON) return 0;

    return static_cast<double>(in / un);
}

void Sort::GetIOUMatrix(const ShapeList &boxesLast
                        , const ShapeList &boxes
                        , std::vector<std::vector<double> > &iouMatrix)
{
    size_t numLast = boxesLast.size();
    size_t numCurrent = boxes.size();

    iouMatrix.resize(numLast, vector<double>(numCurrent, 0));
    for (unsigned int i = 0; i < numLast; i++) // compute iou matrix as a distance matrix
    {
        for (unsigned int j = 0; j < numCurrent; j++)
        {
            // use 1-iou because the hungarian algorithm computes a minimum-cost assignment.
            iouMatrix[i][j] = 1 - boxesLast[i].Similarity(dynamic_cast<const Shape*>(&boxes[j]));
        }
    }
}

//associate detections to tracked object (both represented as bounding boxes)
void Sort::Tracker(const std::vector<Rect2f> &boxesLast, const ShapeList &boxesCurrent
                   , std::vector<TrackingBox<Rect2f>> &result)
{
    vector<vector<double>> iouMatrix;
    GetIOUMatrix(boxesLast, boxesCurrent, iouMatrix);
    if (iouMatrix.size() == 0)
    {
        set<unsigned int> fresh;
        vector<cv::Point_<size_t>> matchedPairs;
        for (unsigned int i = 0; i < boxesCurrent.size(); ++i)
        {
            fresh.insert(i);
        }
        Update(matchedPairs, fresh, boxesCurrent, result);
        return;
    }

    // the resulting assignment is [track(prediction) : detection], with len=preNum
    Hungarian hungAlgo;
    vector<int> assignment;
    hungAlgo.Solve(iouMatrix, assignment);

    // 这一段，在无符号和符号之间一直混着，人超级多的时候，会出问题；
    // all = matches + unmatched_detections(fresh) + unmatched_predictions(past)
    set<unsigned int> fresh, past, all, matched;
    vector<cv::Point_<size_t>> matchedPairs;
    size_t numLast = boxesLast.size();
    size_t numCurrent = boxesCurrent.size();

    if (numCurrent > numLast)
    {
        for (unsigned int n = 0; n < numCurrent; n++)
        {
            all.insert(n);
        }

        for (unsigned int i = 0; i < numLast; ++i)
        {
            if (-1 != assignment[i])
            {
                matched.insert(static_cast<unsigned int>(assignment[i]));
            }
            else
            {
                past.insert(i);
            }
        }

        set_difference(all.begin(), all.end(), matched.begin(), matched.end()
                       , insert_iterator<set<unsigned int>>(fresh, fresh.begin()));
    }
    else if (numCurrent < numLast) // there are unmatched trajectory/predictions
    {
        for (unsigned int i = 0; i < numLast; ++i)
            if (assignment[i] ==
                -1) // unassigned label will be set as -1 in the assignment algorithm
                past.insert(i);
    }


    // filter out matched with low IOU
    matchedPairs.clear();
    for (unsigned int i = 0; i < numLast; ++i)
    {
        if (assignment[i] == -1) // pass over invalid values
        {
            continue;
        }

        if (1 - iouMatrix[i][static_cast<unsigned int>(assignment[i])] < iouThreshold)
        {
            past.insert(i);
            fresh.insert(static_cast<unsigned int>(assignment[i]));
        }
        else
            matchedPairs.push_back(cv::Point(i, assignment[i]));
    }

    Update(matchedPairs, fresh, boxesCurrent, result);
}

void Sort::RemoveByIDs(const std::vector<int> ids)
{
    for (auto iter = trackers.begin(); iter != trackers.end();)
    {
        auto it = find(ids.begin(), ids.end(), (*iter).id + 1);
        if (it != ids.end())
        {
            trackers.erase(iter);
            continue;
        }
        iter++;
    }
}

void Sort::UpdateByIDs(const std::map<int, int> updateIDs)
{
    for (auto iter = updateIDs.begin(); iter != updateIDs.end(); iter++)
    {
        for (auto &track : trackers)
        {
            if (track.id + 1 == iter->first)
            {
                track.id = iter->second - 1;
                break;
            }
        }
    }
}

// update across frames/trackers
void Sort::Update(const std::vector<cv::Point_<size_t> > matchedPairs
                  , const std::set<unsigned int> fresh
                  , const ShapeList &boxes, std::vector<TrackingBox<Rect2f>> &result)
{
    // update matched trackers with assigned detections.
    // each prediction is corresponding to a tracker
    result.resize(boxes.size());
    for (const auto &p : matchedPairs)
    {
        trackers[p.x].Update(boxes[p.y].data);
        result[p.y].region.Parse(trackers[p.x].GetState());
        result[p.y].id = trackers[p.x].id + 1;
        result[p.y].frame = frameNum;
    }

    // create and initialise new trackers for unmatched detections
    for (auto n : fresh)
    {
        KalmanTracker tracker = KalmanTracker(boxes[n].data);
        trackers.push_back(tracker);
        result[n].region.Parse(trackers[trackers.size() - 1].GetState());
        result[n].id = trackers[trackers.size() - 1].id + 1;
        result[n].frame = frameNum;
    }

    // get trackers' output
//    result.clear();
    for (auto it = trackers.begin(); it != trackers.end();)
    {
        if (((*it).timeSinceUpdate < 1) &&
            ((*it).hitStreak >= MIN_HISTS || frameNum <= MIN_HISTS))
        {
            TrackingBox<Rect2f> res;
            res.region.Parse(it->GetState());
            res.id = (*it).id + 1;
            res.frame = frameNum;
//            result.push_back(res);
            ++it;
        }
        else
        {
            ++it;
        }

        // remove dead tracklet
        if (it != trackers.end() && (*it).timeSinceUpdate > MAX_AGE)
        {
            it = trackers.erase(it);
        }
    }
}
