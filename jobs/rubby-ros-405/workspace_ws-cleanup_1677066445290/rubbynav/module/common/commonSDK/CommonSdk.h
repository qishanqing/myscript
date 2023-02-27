//
// Created by lcy on 22-6-7.
//

#ifndef RUBBYDECISION_COMMONSDK_H
#define RUBBYDECISION_COMMONSDK_H

#include <iostream>
#include "Common.h"
#include "Map.h"

class CommonSdk{
public:
    CommonSdk(){
        s_map.init();
    };
    ~CommonSdk() = default;

    void standBy(){}
    void voiceAnnounce(){}
    void Destory();
private:

};

#endif //RUBBYDECISION_COMMONSDK_H
