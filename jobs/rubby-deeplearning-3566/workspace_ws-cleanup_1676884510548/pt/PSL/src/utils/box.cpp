//
// Created by hao on 2022/5/23.
//

#include "box.h"
#include "src/utils/utils.h"

void box::NMS(std::vector<BoxInfo> &boxes, float threshold)
{
    int N = boxes.size();
    std::vector<int> labels(N, -1);
    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            cv::Rect pre_box = boxes[i].box;
            cv::Rect cur_box = boxes[j].box;
            float iou_ = IoU(pre_box, cur_box);
            if (iou_ > threshold)
            {
                labels[j] = 0;
            }
        }
    }

    std::vector<BoxInfo> boxesTemp;

    for (int i = 0; i < N; ++i)
    {
        if (labels[i] == -1)
            boxesTemp.push_back(boxes[i]);
    }
    boxes.swap(boxesTemp);
}