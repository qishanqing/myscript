//////////////////////////////////////////////////////////////////////
///  @file     escalator.h
///  @brief    manage escalator map
///  Details.
///
///  @author   sunhao
///  @date     2021.08.11
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_ESCALATOR_MAP_H
#define DETECTOR_SAMPLE_ESCALATOR_MAP_H

#include <vector>
#include <list>
#include "perception/instance.h"
#include "src/utils/detector_param.h"
#include "src/utils/data_type.h"
#include "src/utils/escalator_region.h"
#include "escalator.h"

using DLObject = psl::Object;
using DLObjectLink = std::list<DLObject>::iterator;

class WeightEscalatorMap;

namespace escalator_map
{
const int W = 1000, H = 1000;
const int SCALE = 2; // the scale ground base on w and h
const int DELTA = 50 / SCALE, ADD = 10 / SCALE;
const int RANGE = W * SCALE / 100;
const int STEP = DELTA; //100 / scale;
const int BASE = RANGE;
const psl::Location LOCATION_ADD(BASE, BASE, BASE);
}

class EscalatorMap
{
public:
    EscalatorMap();

    ~EscalatorMap();

    void Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view
                , const psl::Time &time);

    void GetMap(psl::SenmaticMap &senmaticMap) const;

    cv::Mat Show(const BoxInfo &view, const std::vector<BoxInfo> &boxes);

    void Write(const cv::Mat image) const;

    bool Save(const std::string &file) const;

    bool Load(const std::string &file);

    void Clear();

    // modify the position after slam mldify
    void Modify(psl::SenmaticMap &newMap);

    void UpdatePoint(const Location &startCenter, const Location &endCenter
                     , const float &angle, psl::Location &srcPoint);

    void RotationAngle(const psl::Location &srcLine
                       , const psl::Location &dstLine
                       , float &rotationAngle);

    static void DrawObject(const BoxInfo &view, const std::vector<BoxInfo> &boxes
                           , const std::vector<Sector> &regions, const cv::Mat& image);

private:

    void Reset();

    void Fresh(const std::vector<BoxInfo> &boxes);

    void Remove(const BoxInfo &view);

    void Merge();

    void Delete();

    bool UpdateLine(const BoxInfo &box, escalator_map::Escalator &object);

    bool SameByLine(const BoxInfo &box, const escalator_map::Escalator &object) const;

    bool SameByPoint(const BoxInfo &box, const escalator_map::Escalator &object) const;

    bool SameByLine(const escalator_map::Escalator &object1
                    , const escalator_map::Escalator &object2) const;

    void WarpWithRegion();

    DLObject Box2Object(const BoxInfo &box);

    void Add(const BoxInfo &box);

    escalator_map::Escalator Box2Escalator(const BoxInfo &box);

    void ModifyHandrial(escalator_map::Escalator &escalator, const DLObject &newObject);

    void RemoveWithRegion();

    void UpdateHandrial(escalator_map::Escalator &escalator, const DLObject &newObject);

    void BindRegion();

    void RemoveMapNoRegion();

    void RemoveMapFromOccluder();

    void RemovePartsBeyondRegion();

    bool EscalatorLonger(const std::vector<psl::Location> &locationWorld1,
                         const std::vector<psl::Location> &locationWorld2);
private:
    std::vector<escalator_map::Escalator> escalatorMapInner;
    std::list<DLObject> escalatorMap;
    bool mapUpdate;
    long lastID;
    psl::Time time;
    std::vector<cv::Scalar> colorDefault;
    BoxInfo view;
    EscalatorRegion regions; // TODO : only bind once
    WeightEscalatorMap *weightEscalatorMap;
    std::vector<std::vector<int>> bindCount; // for map update by region
};

#endif //DETECTOR_SAMPLE_ESCALATOR_H
