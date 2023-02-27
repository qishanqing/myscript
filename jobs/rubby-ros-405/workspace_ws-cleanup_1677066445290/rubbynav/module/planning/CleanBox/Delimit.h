//
// Created by lcy on 22-8-11.
//

#ifndef HSMCONTROLCORE_DELIMIT_H
#define HSMCONTROLCORE_DELIMIT_H

#include <opencv2/opencv.hpp>
#include <iostream>

#include "Common.h"
#include  "config.h"
#include "Map.h"

using namespace cv;
using namespace std;

class Delimit{
public:
    Delimit(){

    }
    ~Delimit(){

    }
    int DelimitCleanInit();
private:

};

#endif //HSMCONTROLCORE_DELIMIT_H
