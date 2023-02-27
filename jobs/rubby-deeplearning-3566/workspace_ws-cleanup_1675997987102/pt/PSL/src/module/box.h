//
// Created by hao on 2021/6/9.
//

#ifndef DETECT_DEMO_BOX_H
#define DETECT_DEMO_BOX_H

// TODO : use the same rect to common/boxinfo/rect
struct Box
{
    float x, y, w, h, prob;

    Box(){}
    Box(float x, float y, float w, float h);

    static float Overlap(float x1, float w1, float x2, float w2);

    static float Intersection(Box a, Box b);

    static float Union(Box a, Box b);

    static float IoU(Box a, Box b);

    static int NMSComparator(const void *pa, const void *pb);

    static void
    NMSSort(Box *boxes, float **probs, int total_in, int classes, float thresh);

};


#endif //DETECT_DEMO_BOX_H
