//////////////////////////////////////////////////////////////////////
///  @file     test_yolov3.cpp
///  @brief    unit test for YoloV3
///  Details.
///  @author   sunhao
///  @date     2021.10.25
//////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include "src/module/yolov3.h"
#include "sample/utils/utils.h"
#include "src/test/module/init_yolov3.h"

const int NET_OUTPUT = 95823; //指针指向的地址开辟内存小于额定内存(95824)会崩溃(实际情况下再小一些才会崩溃)

void Tensor2BoxTest(float *predictions, std::unique_ptr<ModelType>  yolov3)
{
    yolov3->Tensor2Box(predictions);
}


TEST(YoloV3Test, Tensor2Box2)
{
    for (int i = 0; i < 3000; ++i)
    {
        std::unique_ptr<ModelType> detector;
        detector = InitYoloV3();
        int size_file = NET_OUTPUT;
        float *probs = (float *) calloc(size_file, sizeof(float ));
        srand(time(NULL));

        for (int j = 0; j < size_file; ++j)
        {
            float a = rand() % (1000) / 1000.0;
            a = (a - 0.5) * 20;
            probs[j] = a;
        }

        Tensor2BoxTest(probs,std::move(detector));
        free(probs);
    }
}