//
// Created by hao on 2021/6/9.
//

#include "image_type.h"

ImageHead::ImageHead(const int w, const int h, const int c)
{
    this->Set(w, h, c);
}

void ImageHead::Set(const int w, const int h, const int c)
{
    width = w;
    height = h;
    channel = c;
    size = w * h * c;
}
