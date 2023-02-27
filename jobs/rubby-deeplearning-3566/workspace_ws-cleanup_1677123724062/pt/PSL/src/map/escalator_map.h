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
#include "psl/perception/instance.h"
#include "src/utils/detector_param.h"
#include "src/utils/data_type.h"
#include "src/utils/escalator_region.h"
#include "escalator.h"
#include "src/map/map.h"

class WeightEscalatorMap;

class EscalatorMap : public Map
{
public:
    EscalatorMap();

    ~EscalatorMap();

    void Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view
                , const psl::Time &time);

    void GetMap(psl::SenmaticMap &senmaticMap) const;

    cv::Mat Show(const BoxInfo &view, const std::vector<BoxInfo> &boxes, cv::Mat image);

    void Write(const cv::Mat image) const;

    bool Save(std::ofstream &ofstream) override;

    bool Load(std::ifstream &ifstream) override;

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
    EscalatorRegion regions; // TODO : only bind once
    WeightEscalatorMap *weightEscalatorMap;
    std::vector<std::vector<int>> bindCount; // for map update by region
};

#endif //DETECTOR_SAMPLE_ESCALATOR_H
