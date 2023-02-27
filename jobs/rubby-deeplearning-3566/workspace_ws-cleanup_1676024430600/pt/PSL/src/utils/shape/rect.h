//////////////////////////////////////////////////////////////////////
///  @file     rect.h
///  @brief    rectangle shape
///  Details.
///
///  @author   sunhao
///  @date     2021.07.28
///
///  revision statement:
//////////////////////////////////////////////////////////////////////
#ifndef SAMPLE_RECT_H
#define SAMPLE_RECT_H

#include "src/utils/shape/shape.h"
#include <opencv2/highgui.hpp>

template<typename T>
struct Rect_ : public Shape
{
    cv::Rect_<T> data;

    Rect_<T>()
    {

    }

    Rect_<T>(T x, T y, T width, T height)
    {
        data = cv::Rect_<T>(x, y, width, height);
    }

    Rect_<T>(cv::Rect_<T> rect)
    {
        data = rect;
    }

    virtual ~Rect_()
    {}

    double Similarity(const Shape *dst) const
    {
        const cv::Rect_<T> current = (dynamic_cast<const Rect_<T> *>(dst))->data;
        double in = (this->data & current).area();
        double un = this->data.area() + current.area() - in;

        if (un < DBL_EPSILON) return 0;

        return static_cast<double>(in / un);
    }

    std::vector<float> Serialize() const
    {
        return std::vector<float>{
                data.x
                , data.y
                , data.x + data.width
                , data.y + data.height
        };
    }

    void ToRect(T cx, T cy, T s, T r)
    {
        T w = sqrt(s * r);
        T h = s / w;
        T x = (cx - w / 2);
        T y = (cy - h / 2);

        if (x < 0 && cx > 0)
            x = 0;
        if (y < 0 && cy > 0)
            y = 0;

        data = cv::Rect_<T>(x, y, w, h);
    }

    void Parse(std::vector<float> item)
    {
        this->data = cv::Rect_<T>(item.at(0), item.at(1), item.at(2) - item.at(0),
                item.at(3) - item.at(1));
    }
};

using Rect = Rect_<int>;

using Rect2f = Rect_<float>;

#endif //SAMPLE_RECT_H
