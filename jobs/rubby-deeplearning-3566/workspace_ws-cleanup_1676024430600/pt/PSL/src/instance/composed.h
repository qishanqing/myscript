//////////////////////////////////////////////////////////////////////
///  @file     composed.h
///  @brief    get the distance for an body composed with multi part
///  @author   sunhao
///  @date     2021.07.27
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_COMPOSED_H
#define DETECTOR_SAMPLE_COMPOSED_H

#include "src/instance/instance.h"

namespace composed
{
struct Body
{
    BoxInfo whole;
    std::vector<BoxInfo> parts;

    Body();

    explicit Body(const BoxInfo &box);

    bool Contain(const BoxInfo &box) const;

    double IoU(const BoxInfo &box) const;

    bool Intersection(const BoxInfo &box) const;

    void SortLeft();

    psl::Location Average() const;

    std::vector<Location> PartLocation() const;

    void UpdateParts2Whole();
};
}

class Composed : public Instance
{
public:
    Composed();

    void GetDistance(const Lidar &lidar, DetectResult &boxes);

private:
    void PrepareBody(const std::vector<BoxInfo> &boxes);

    void GetWhole(const std::vector<BoxInfo> &boxes);

    void GetPartial(const std::vector<BoxInfo> &boxes, std::vector<int> &orphan);

    void GroupInSideView();

    bool LeasrSquares(const std::vector<Location> &locations, float &k, float &b);

    bool Fit(const std::vector<Location> &locations, composed::Body &body);

    void MergeBoxes(std::vector<BoxInfo> &boxes) const;

    void UpdateParts2Whole();

    void AlignmentWhole();

private:
    std::vector<composed::Body> bodys;
};


#endif //DETECTOR_SAMPLE_COMPOSED_H
