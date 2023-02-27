//
// Created by hao on 2021/7/2.
//

#ifndef DETECTOR_SAMPLE_OBSERVER_DATA_H
#define DETECTOR_SAMPLE_OBSERVER_DATA_H

#include "src/utils/data_type.h"

class ObserverData
{
public:
    ObserverData()
    {}

    virtual ~ObserverData(){}

    virtual void Update(const DataGroup &item) = 0;
};

#endif //DETECTOR_SAMPLE_OBSERVER_DATA_H
