//
// Created by hao on 2021/6/9.
//

#include "box.h"
#include <cstdlib>


struct BoxSortable{
    int index;
    int classId;
    float **probs;
};

float Box::Overlap(float x1, float w1, float x2, float w2)
{
    float l1 = x1 - w1 / 2;
    float l2 = x2 - w2 / 2;
    float left = l1 > l2 ? l1 : l2;
    float r1 = x1 + w1 / 2;
    float r2 = x2 + w2 / 2;
    float right = r1 < r2 ? r1 : r2;
    return right - left;
}

float Box::Intersection(Box a, Box b)
{
    float area = 0;
    float w = Overlap(a.x, a.w, b.x, b.w);
    float h = Overlap(a.y, a.h, b.y, b.h);
    if (w < 0 || h < 0)
        return 0;
    area = w * h;
    return area;
}

float Box::Union(Box a, Box b)
{
    float i = Intersection(a, b);
    float u = a.w * a.h + b.w * b.h - i;
    return u;
}

float Box::IoU(Box a, Box b)
{
    return Intersection(a, b) / Union(a, b);
}

int Box::NMSComparator(const void *pa, const void *pb)
{
    BoxSortable a = *(BoxSortable *) pa;
    BoxSortable b = *(BoxSortable *) pb;
    float diff = a.probs[a.index][b.classId] - b.probs[b.index][b.classId]; // TODO : class id
    if (diff < 0) return 1;
    else if (diff > 0) return -1;
    return 0;
}

void Box::NMSSort(Box *boxes, float **probs, int total_in, int classes, float thresh)
{
    int i, j, k;
    BoxSortable *s = (BoxSortable *) calloc(total_in, sizeof(BoxSortable));

    int total = 0;
    for (i = 0; i < total_in; ++i)
    {
        if (boxes[i].prob > 0)
        {
            s[total].index = i;
            s[total].classId = 0;
            s[total].probs = probs;
            total++;
        }
    }

    for (k = 0; k < classes; ++k)
    {
        for (i = 0; i < total; ++i)
        {
            s[i].classId = k;
        }
        qsort(s, total, sizeof(BoxSortable), NMSComparator);

        for (i = 0; i < total; ++i)
        {
            if (probs[s[i].index][k] == 0)
                continue;
            for (j = i + 1; j < total; ++j)
            {
                Box b = boxes[s[j].index];
                if (probs[s[j].index][k] > 0)
                {
                    if (IoU(boxes[s[i].index], b) > thresh)
                    {
                        probs[s[j].index][k] = 0;
                    }
                }
            }
        }
    }
    free(s);
}

Box::Box(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}
