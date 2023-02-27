//
// Created by hao on 2021/7/26.
//

#include "utils.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

const int WIDTH = 640;
const int HEIGHT = 400;

void GetAnnotations(const std::string &file, std::vector<BoxInfo> &boxes
                    , std::vector<int> &distances)
{
    if (not access(file.c_str(), 0) == 0)
    {
        std::cout << "file not exist <" + file + ">" << std::endl;
    }

    FILE *fin = fopen(file.c_str(), "r");

    int label, dist;
    float xmin, ymin, xmax, ymax;

    while (EOF != fscanf(fin, "%d %f %f %f %f %d", &label, &xmin, &ymin, &xmax, &ymax
                         , &dist))
    {
        BoxInfo box;

        box.FromCenter(int(xmin * WIDTH), int(ymin * HEIGHT), int(
                xmax * WIDTH), int(ymax * HEIGHT));
        box.classID = (ClassID)(label-1);

        boxes.emplace_back(box);
        distances.emplace_back(dist);
    }

    fclose(fin);
}

void FindReplace(std::string &str, const std::string orig, const std::string rep)
{
    size_t pos = str.find(orig);

    if (std::string::npos != pos)
    {
        str.replace(pos, orig.size(), rep);
        return;
    }
}