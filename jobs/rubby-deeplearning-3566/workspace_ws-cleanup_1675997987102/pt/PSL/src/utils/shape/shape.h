//////////////////////////////////////////////////////////////////////
///  @file     shape.h
///  @brief    abstract class for shapes, like rect, circle, sector etc.
///  Details.
///
///  @author   sunhao
///  @date     2021.07.28
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#ifndef SAMPLE_SHAPE_H
#define SAMPLE_SHAPE_H
#include <vector>

struct Shape
{
    Shape() {}

    virtual ~Shape() {}

    virtual double Similarity(const Shape* dst)  const = 0;

    virtual std::vector<float> Serialize() const = 0;

    virtual void Parse(std::vector<float> item) = 0;
};


#endif //SAMPLE_SHAPE_H
