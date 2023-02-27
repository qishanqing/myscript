//////////////////////////////////////////////////////////////////////
///  @file     map.h
///  @brief    abstract map for objects manager
///  Details.
///
///  @author   sunhao
///  @date     2022-08-01 11:52:08
//////////////////////////////////////////////////////////////////////

#ifndef SAMPLE_MAP_H
#define SAMPLE_MAP_H
#include <vector>
#include <list>
#include "psl/perception/instance.h"
#include "src/utils/detector_param.h"
#include "src/utils/data_type.h"
#include "src/utils/escalator_region.h"
#include "src/utils/color.h"
#include "escalator.h"

//#define MAX_MAP

using DLObject = psl::Object;
using DLObjectLink = std::list<DLObject>::iterator;

namespace map_type
{
#ifdef MAX_MAP
const int W = 1000, H = 1000;
const int SCALE = 2; // the scale ground base on w and h
#else
const int W = 500, H = 500;
const int SCALE = 1; // the scale ground base on w and h
#endif
const int DELTA = 50 / SCALE, ADD = 10 / SCALE;
const int RANGE = W * SCALE / 100;
const int STEP = DELTA; //100 / scale;
const int BASE = RANGE;
const psl::Location LOCATION_ADD(BASE, BASE, BASE);
}

class Map
{
public:
    Map();

    virtual ~Map() {}

    static cv::Mat GetDisplayImage();

    virtual void Update(const std::vector <BoxInfo> &boxes, const BoxInfo &view
                , const psl::Time &time);

    void GetMap(psl::SenmaticMap &senmaticMap) const;

    virtual cv::Mat Show(const BoxInfo &view, const std::vector <BoxInfo> &boxes, cv::Mat image) = 0;

    virtual bool Save(std::ofstream &ofstream);

    virtual bool Load(std::ifstream &ifstream);

    void SetParam(const psl::DetectorParam &detectParam);

protected:
    virtual void Fresh(const std::vector<BoxInfo> &boxes) = 0;

    virtual DLObject Box2Object(const BoxInfo &box);

//    virtual escalator_map::Escalator Box2Escalator(const BoxInfo &box);

protected:
    static long lastID;
    static Color colors;

    psl::DetectorParam detectParam;
    std::vector<escalator_map::Escalator> mapInner;
    std::list<DLObject> mapInter;
    bool mapUpdate;
    psl::Time time;
    BoxInfo view;

};


#endif //SAMPLE_MAP_H
