//
// Created by hao on 2021/6/9.
//

#ifndef DETECT_DEMO_IMAGE_TYPE_H
#define DETECT_DEMO_IMAGE_TYPE_H


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
};


#endif //DETECT_DEMO_IMAGE_TYPE_H
