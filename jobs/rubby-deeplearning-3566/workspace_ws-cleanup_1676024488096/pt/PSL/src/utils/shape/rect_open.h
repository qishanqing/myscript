//////////////////////////////////////////////////////////////////////
///  @file     rect_open.h
///  @brief    rectangle shape without one edge
///  Details.
///
///  @author   sunhao
///  @date     2021.07.28
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#ifndef SAMPLE_RECT_OPEN_H
#define SAMPLE_RECT_OPEN_H
#include "src/utils/shape/shape.h"


struct RectOpen : public Shape
{
    RectOpen();

    virtual ~RectOpen();

    double Similarity(const Shape* dst)  const;

    std::vector<float> Serialize() const override;

    void Parse(std::vector<float> item) override;
};


#endif //SAMPLE_RECT_OPEN_H
