#pragma once

#include<vector>

namespace psl
{
struct Map
{
    //status
    int status;
    // 中心点
    Location centerPoint;
    // 半径
    float radius;

    Map();

    Map(const int status, const Location &point, const float radius)
    {
        this->status = status;
        this->centerPoint = point;
        this->radius = radius;
    }

};

struct Obstacle
{
    int status;
    std::vector<Map> maps;
};
}