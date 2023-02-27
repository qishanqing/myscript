//
// Created by hao on 2021/6/9.
//

#ifndef DETECT_DEMO_IMAGE_TYPE_H
#define DETECT_DEMO_IMAGE_TYPE_H

#include <string>

struct ImageHead
{
    int width = -1;
    int height = -1;
    int channel = -1;
    long size = -1;

    ImageHead()
    {}

    ImageHead(const int w, const int h, const int c);

    void Set(const int w, const int h, const int c);

    std::string Print() const;

    bool operator==(const ImageHead &param) const;

    bool operator!=(const ImageHead &param) const;
};


#endif //DETECT_DEMO_IMAGE_TYPE_H
