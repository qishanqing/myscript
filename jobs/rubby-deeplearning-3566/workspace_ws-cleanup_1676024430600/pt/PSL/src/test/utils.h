//
// Created by hao on 2021/7/26.
//

#ifndef DETECTOR_TEST_UTILS_H
#define DETECTOR_TEST_UTILS_H

#include <string>
#include <vector>
#include "src/utils/detector_param.h"

void GetAnnotations(const std::string &file, std::vector<BoxInfo> &boxes
                    , std::vector<int> &distances);

void FindReplace(std::string &str, const std::string orig, const std::string rep);

#endif //DETECTOR_TEST_UTILS_H