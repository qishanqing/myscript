//
// Created by hao on 2021/6/9.
//

#include "image_type.h"
#include <sstream>

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

std::string ImageHead::Print() const
{
    std::stringstream stream;

    stream << "[" << width << ", " << height << ", " << channel << "]";
    return stream.str();
}

bool ImageHead::operator!=(const ImageHead &param) const
{
    return not (this->operator==(param));
}

bool ImageHead::operator==(const ImageHead &param) const
{
    return (this->width == param.width)
           and (this->height == param.height)
           and (this->channel == param.channel);
}
