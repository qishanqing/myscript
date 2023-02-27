//////////////////////////////////////////////////////////////////////
///  @file     escalator_region.h
///  @brief    some process about region of escalator in world coordinate
///  @author   sunhao
///  @date     2021.11.26
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_ESCALATOR_REGION_H
#define DETECTOR_SAMPLE_ESCALATOR_REGION_H

#include <vector>
#include "src/utils/data_type.h"
#include "src/utils/gemotry.h"

const int DEFALUT_INDEX = -1;

struct BindResult
{
    int id = DEFALUT_INDEX;
    bool valid = false;

    BindResult();

    BindResult(int id);

    bool Valid() const;

    bool operator==(const BindResult &b1) const;
};

struct EscalatorRegion
{
    std::vector<Sector> regions;

    EscalatorRegion();

    EscalatorRegion(const std::vector<BoxInfo> &boxes, const BoxInfo &view);

    void Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view);

    bool Empty() const;

    void SetRegion(const std::vector<BoxInfo> &boxes, const BoxInfo &view);

    bool SetRegion(const BoxInfo &box, const BoxInfo &view);

    void Bind(const std::vector<BoxInfo> &boxes, const BoxInfo &view
              , const bool useInvalid
              , std::vector<BindResult> &index);


    bool Read(std::ifstream &stream);

    bool Write(std::ofstream &stream) const;
};


#endif //DETECTOR_SAMPLE_ESCALATOR_REGION_H
