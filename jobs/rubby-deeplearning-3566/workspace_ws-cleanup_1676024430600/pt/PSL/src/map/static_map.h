//////////////////////////////////////////////////////////////////////
///  @file     static_map.h
///  @brief    static object map
///  Details.
///
///  @author   sunhao
///  @date     2022-08-01 11:42:51
//////////////////////////////////////////////////////////////////////

#ifndef SAMPLE_STATIC_MAP_H
#define SAMPLE_STATIC_MAP_H

#include <vector>
#include <list>
#include "psl/perception/instance.h"
#include "src/utils/detector_param.h"
#include "src/utils/data_type.h"
#include "src/utils/escalator_region.h"
#include "escalator.h"
#include "src/map/map.h"
#include "src/mot/sort.h"
#include "src/map/desk.h"

class StaticMap : public Map
{
    using cv_rect_world = cv::Rect2f;

public:
    StaticMap();

    ~StaticMap();

    void Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view
                , const psl::Time &time);

    cv::Mat Show(const BoxInfo &view, const std::vector<BoxInfo> &boxes, cv::Mat image);

    bool Save(std::ofstream &ofstream) override;

    bool Load(std::ifstream &ifstream) override;

    void GetMap(psl::SenmaticMap &senmaticMap) const;

    void Clear();

    void RotationAllWorld(const float angle);

    void SetDistanceValidStatus(BoxInfo view);

private:
    void Fresh(const std::vector<BoxInfo> &boxes);

    void Add(const BoxInfo &box);

    void Reset();

    bool UpdateLocation(const BoxInfo &box);

    DLObject DeskBox2Object(const BoxInfo &box);

    void Remove();

    void
    Tracking(const std::vector<BoxInfo> &boxes, vector<TrackingBox<ShapeType>> &rectResult
             , Sort &tracker);

    void TrackingWorld(const std::vector<BoxInfo> &boxes, const psl::Location &view
                       , std::vector<BoxInfo> &trackingResult);

    void Box2WorldRect(const BoxInfo &box, cv::Rect &rect, const psl::Location &view);

    void GetParallelLinesCrossPoints(const BoxInfo &box, psl::Location view
                                     , psl::Location &p1, psl::Location &p2);

    void Trapezoid2Rect(const psl::Location &leftCrossPoint
                        , const psl::Location &rightCrossPoint
                        , const psl::Location &edgeLeft, const psl::Location &edgeRight
                        , cv::Rect &rect) const;


    void Trapezoid2Rect(const psl::Location &leftCrossPoint
                        , const psl::Location &rightCrossPoint
                        , const psl::Location &edgeLeft, const psl::Location &edgeRight
                        , cv_rect_world &rect) const;

    void FreshDeskMap(const std::vector<BoxInfo> &boxes);

    void
    UpdateMapByBox(const BoxInfo &box, const int index, const std::vector<int> &status);

    void BindByMaxIOU(std::vector<std::vector<float>> &ious, std::vector<int> &binds);

    template<typename T>
    vector<size_t> SortIndexes(vector<T> &v)
    {
        vector<size_t> idx(v.size());
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2)
        { return v[i1] < v[i2]; });//< is ascending; > is descending
        return idx;
    }

    void AddDeskMap(const BoxInfo &box, const std::vector<int> &status);

    void
    FilterDeskBox(const std::vector<BoxInfo> &boxes, std::vector<BoxInfo> &boxesDesk);

    bool Close2Boundary(const BoxInfo &box);

    void SetPointsStatus(const std::vector<BoxInfo> &boxes
                         , std::vector<std::vector<int>> &statuses);

    void
    GetMatchedByTrack(const std::vector<BoxInfo> &boxes, std::vector<int> &matchedTrack);

    void RemoveByIDs(const std::vector<int> ids);

    void SetIouByMatchedTrack(std::vector<std::vector<float>> &ious
                              , const std::vector<int> &matchedTrack);

    void UpdateByIDs(const std::map<int, int> updateIDs);

    void FreshSmallBox(const std::vector<BoxInfo> &boxes);

    void FilterSmallObject(const std::vector<BoxInfo> &boxes
                           , std::vector<BoxInfo> &boxesSmallObject);

    void UpdateSmallBox(const BoxInfo &box, const int bind);

    void BoxesSmallObjectsIOU(const vector<BoxInfo> &boxes
                              , std::vector<std::vector<float>> &ious);

    void AddSmallBox(const BoxInfo &box);

    DLObject SmallBox2Object(const BoxInfo &box);

    void BoxView4CrossPoints(const BoxInfo &box, const psl::Location &view
                             , std::vector<Point> &points);

    void FilterBoxes(const std::vector<BoxInfo> &boxes
                     , std::vector<BoxInfo> &boxesSmallObject);

    void RemoveSmallObject(const std::vector<BoxInfo> &boxes);

    void UpdateSmallBoxByBinds(const std::vector<BoxInfo> boxesSmallObject
                               , const std::vector<int> &binds);

    void BindNearest2Points(std::vector<std::vector<float >> &distances
                            , std::vector<int> &BindResult);

    void UpdatePoint(const psl::Location &locationSrc, psl::Location &locationDst) const;

    void BoxLocationsObjectDistance(const std::vector<psl::Location> &boxLocations
                                    , const psl::Object &object
                                    , std::vector<std::vector<float >> &distances);

    void Bind(std::vector<int> &binds, std::vector<BoxInfo> &boxesDesk
              , std::vector<int> &matchedTrack);

    void BoxesObjectsIOU(const vector<BoxInfo> &boxesDesk
                         , std::vector<std::vector<float>> &ious);

    void Tracking(std::vector<BoxInfo> &boxes, Sort &tracker);

    void
    UpdateDeskMap(const std::vector<int> &binds, const std::vector<BoxInfo> &boxesDesk
                  , const std::vector<std::vector<int>> &statuses
                  , const std::vector<int> &matchedTrack);

    bool Write(std::ofstream &stream) const;

    bool WriteMap(std::ofstream &stream, const std::vector<DLObject>& objects) const;

    bool Read(std::ifstream &stream);

    bool ReadMap(std::ifstream &stream, std::vector<DLObject>& objects);

    void BoxExtendSingle4(const BoxInfo &box, std::vector<Point> &points);

    int PointsValid(const std::vector<int> &status);

    void PointsExtendSingle4(const Point &left, const Point &right, const float &extend
                             , std::vector<Point> &points);

    void GetDeskMap4Rects(std::vector<cv::Rect> &objectRects);

    DLObject DeskBox4Object(const BoxInfo &box);

    void View2PointsAxisDistance(const vector<Point> &points, float &distanceAxisMin
                                 , float &distanceAxisMax);

    void View2PointAxis(const Point &location, float &distance);

    bool ViewClosePoints(const vector<Point> &points);

    bool DeskBoxInvalid(const BoxInfo &box);

    void SetLastID(const vector<TrackingBox<ShapeType>> &rectResult);

    void Delete(const int index);

    void SmallRectExtend(const float extendPoint, cv_rect_world &rect) const;

    void LineExtendVerticalBisector(const BoxInfo &view, const Point &p1, const Point &p2
                                    , std::vector<Point> &points
                                    , const float extend) const;

    void PointOnVerticalBisector(const BoxInfo &view, const Segment &line
                                 , const float extend, std::vector<Point> &points) const;

    void ViewKMiddleVerticalBisector(const float k, const float b, const float extend
                                     , const Point &middlePoint, const Point &viewPoint
                                     , Point &dst) const;

    void GetCrossPoint(const float k1, const float k2, const float p1
                       , const float p2, Point &crossPoint) const;

    void
    SetPointOrders(const Segment &line, const Point viewPoint, const Point &startPoint
                   , const Point &endPoint, std::vector<Point> &points) const;

    bool ViewClosePointsRect(const vector<Point> &points);

    void LineExtendParallel(const BoxInfo &view, const Segment &line
                            , std::vector<Point> &points, const float extend) const;

    void BoxExtendVerticalBisectorRect(const BoxInfo &box, cv::Rect &rect);

    void BoxExtendVerticalBisectorRect(const BoxInfo &box, cv_rect_world &rect);

    void BoxExtendVerticalBisector(const std::vector<BoxInfo> boxes
                                   , std::vector<cv::Rect> &rects);

    void ReduceWeight(const std::vector<BoxInfo> &boxes, const int &index);

    void Location2Points(const std::vector<psl::Location> &locations
                         , std::vector<cv::Point> &points) const;

    void StoreLocation(const int id, const BoxInfo &box);

    template<class T>
    void Extend(const T radius, cv::Rect_<T> &rect) const
    {
        rect -= cv::Point_<T>(radius, radius);
        rect += cv::Size_<T>(radius + radius, radius + radius);
    }

    void DrawPoint(cv::Mat image) const;

    void DrawSmallObject(cv::Mat image) const;

    void DrawDesk(cv::Mat image) const;

    void RotationPoint(const float angle, psl::Location &location);

    void RotationObject(const float angle, std::vector<DLObject> &objects);

    void UpdateSmallBoxPoint(const BoxInfo &box, const int bind);

    void Point2SegmentDistance(const Segment &l, const psl::Location location, float &distance);

    void CompareBinds(const std::vector<int> &queenPlace
                      , const std::vector<std::vector<float>> &iouMap
                      , std::vector<int> &indexes);

    bool IsValidCompare(int row, int col
                        , const std::vector<std::vector<int>> &indexMap
                        , std::vector<int> &queenPlace);

    void EightQueen(int row,const std::vector<std::vector<int>> &indexMap
                    ,const std::vector<std::vector<float>> &iouMap, std::vector<int> &queenPlace
                    , std::vector<int> &indexes);

    void BindByIOUEightQueen(const std::vector<std::vector<float>> &iouMap
                             , std::vector<int> &indexes);

    bool GetObjectsDistance(psl::Object &object, psl::Location &p);

private:
    Sort tracker;
    Sort trackerWorld;
    Sort trackerDesk;

    psl::SenmaticMap staticObjectMap; // data flow: add, delete, update
    std::vector<char> smallObjectUpdate;
    std::vector<std::vector<int>> pointStatus;
    std::vector<int> objectWeights;

    dl_map::Desk desk;
    std::vector<DLObject> smallObjectMapInner;

    float indexesIouSum;
    int indexesBindsNumbers;
};


#endif //SAMPLE_STATIC_MAP_H
