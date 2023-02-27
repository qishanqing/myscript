//
// Created by hao on 2021/7/27.
//

#ifndef DETECTOR_SAMPLE_TYPES_H
#define DETECTOR_SAMPLE_TYPES_H
#include <vector>
#include "perception/instance.h"
#include "perception/map.h"

struct Map
{
    //status
    int status;
    // 中心点
    psl::Location centerPoint;
    // 半径
    float radius;
    // 静态计数
    int staticCount;
    // no object count
    int noPersonCount;
    int havePerson;

    Map()
    {}

    Map(const psl::Location &location, float ridus)
    {
        this->centerPoint = location;
        this->status = 1;
        this->staticCount = 0;
        this->noPersonCount = 0;
        this->radius = ridus;
        this->havePerson = 1;
    }

};

struct Obstacle
{
    int status;
    int mapInfoSize;
    int mapInfoPreSize;
    std::vector<int> mapIndexes;
    std::vector<Map> maps;

    void ToObstacle(psl::Obstacle &obstacle)
    {
        for (const auto &m : this->maps)
        {
            obstacle.maps.push_back(psl::Map(m.status, m.centerPoint, m.radius));
        }
        obstacle.status = this->status;
    }
};

#endif //DETECTOR_SAMPLE_TYPES_H
