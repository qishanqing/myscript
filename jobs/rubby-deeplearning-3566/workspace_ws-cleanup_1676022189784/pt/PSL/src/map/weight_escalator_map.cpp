//
// Created by indemind on 2/21/22.
//

#include "weight_escalator_map.h"
#include "src/map/map.h"

using namespace map_type;

const cv::Scalar COLOR_ADD(1, 1, 1);

void WeightEscalatorMap::Update(const std::vector<BoxInfo> &boxes, const BoxInfo &view)
{
    if (image.empty())
    {
        image = cv::Mat::zeros(W, H, CV_8UC3);
    }
    auto machine = view.location.world + LOCATION_ADD;
    for (auto &b : boxes)
    {
        if (not b.IsEscalator()) continue;
        cv::Mat temp = cv::Mat::zeros(W, H, CV_8UC3);
        auto vertexNew = machine; // r.newVertex + addLocation;
        auto leftLeft = b.edgeLeft.world + LOCATION_ADD;
        auto rightRight = b.edgeRight.world + LOCATION_ADD;
        cv::fillConvexPoly(temp, std::vector<cv::Point>{
                                   cv::Point(int(vertexNew.x * DELTA), int(vertexNew.y * DELTA))
                                   , cv::Point(int(leftLeft.x * DELTA), int(leftLeft.y * DELTA))
                                   , cv::Point(int(rightRight.x * DELTA), int(rightRight.y * DELTA))}
                , COLOR_ADD);
        image += temp;
    }
}

void WeightEscalatorMap::GetImage(cv::Mat &image) const
{
    image = this->image.clone();
}