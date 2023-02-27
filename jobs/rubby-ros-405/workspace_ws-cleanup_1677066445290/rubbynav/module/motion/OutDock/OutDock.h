//
// Created by gby on 22-11-29.
//

#ifndef OUTDOCK_H_
#define OUTDOCK_H_

#include "BasicAction.h"
#include "Operation.h"

class OutDock: virtual public BasicAction, virtual public Operation
{
public:
    bool outDockInit(SensorInfo* sensor_, DataDown* datadown_);
    bool outDockRun(SensorInfo* sensor_, DataDown* datadown_, float vl_);
};

#endif