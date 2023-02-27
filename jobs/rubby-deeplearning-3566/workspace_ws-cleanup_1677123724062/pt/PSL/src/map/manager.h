//////////////////////////////////////////////////////////////////////
///  @file     manager.h
///  @brief    map manager
///  Details.
///
///  @author   sunhao
///  @date     2022-08-03 11:10:51
//////////////////////////////////////////////////////////////////////
#ifndef SAMPLE_MANAGER_H
#define SAMPLE_MANAGER_H
#include <vector>
#include "src/map/static_map.h"
#include "src/map/escalator_map.h"
#include "src/map/aircraft_hatch.h"

class MapManager
{
public:
    MapManager();

    ~MapManager();

    void SetParam(const psl::DetectorParam &detectParam);

    void Update(std::vector<BoxInfo> &boxes, const BoxInfo &view
                , const psl::Time &time);

    bool Save(const std::string &file);

    bool Load(const std::string &file);

    void Clear();

    void Modify(psl::SenmaticMap &newMap);

    void GetMap(psl::SenmaticMap &senmaticMap) const;

    cv::Mat Show(const BoxInfo &view, const std::vector<BoxInfo> &boxes);

    bool RotateMap(const float angle);

    void GetDistanceValidStatus(BoxInfo view);

private:
    StaticMap staticMap;
    EscalatorMap escalatorMap;
    AircraftHatch aircraftHatchMap;
    std::ifstream ifstream;
    std::ofstream ofstream;
};


#endif //SAMPLE_MANAGER_H
