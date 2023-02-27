//
// Created by hao on 2021/6/15.
//

#include "src/test/validate/validate_map.h"
#include "src/utils/file.h"

extern InputParam inputParam;

void GetDetectResult()
{
    std::unique_ptr<ModelType> detector;
    detector = InitYoloV3();

    std::vector<std::string> imageList;
    std::vector<std::vector<box_prob>> boxes_all;
    std::vector<std::vector<box_label>> truth_all;

    std::string imagesTxt = inputParam.imagePath;

    if (file_op::File::Exist(imagesTxt))
    {
        std::cout << "start to run :" << imagesTxt << std::endl;
    }
    else
    {
        std::cout<<"test list file: "<< imagesTxt
                 <<" not exist ,continue next list file "<<std::endl;
        return;
    }

    ReadFile(imagesTxt, imageList);
    for (int i = 0; i < imageList.size(); ++i)
    {
        cv::Mat image = cv::imread(imageList.at(i));
        std::vector<BoxInfo> boxes;
        detector->Detect(image, boxes);
        std::vector<box_prob> boxesTemp;

        for (auto box: boxes)
        {
            // TODO use original Box
            box_prob boxTemp;
            boxTemp.b = Box{box.cx / image.cols, box.cy / image.rows, box.width / image.cols,
                            box.height / image.rows};
            boxTemp.p = box.score;
            boxTemp.class_id = box.classID;
            boxesTemp.push_back(boxTemp);
        }
        boxes_all.push_back(boxesTemp);

        char labelpath[4096];
        replace_image_to_label(imageList.at(i).c_str(), labelpath);

        int nums_labels = 0;
        std::vector<box_label> truth;
        read_boxes(labelpath, &nums_labels, &truth);

        int count1 = 0;
        for (int i = 0; i < nums_labels; ++i)
        {
//            LabelResizeByPadding(truth[i], image.cols, image.rows, cv::Size(416,416));
            truth[count1].track_id = count1 + 0;
            truth[count1].id = truth[i].id;
            truth[count1].x = truth[i].x;
            truth[count1].y = truth[i].y;
            truth[count1].h = truth[i].h;
            truth[count1].w = truth[i].w;
            truth[count1].left = truth[i].x - truth[i].w / 2;
            truth[count1].right = truth[i].x + truth[i].w / 2;
            truth[count1].top = truth[i].y - truth[i].h / 2;
            truth[count1].bottom = truth[i].y + truth[i].h / 2;

            ++count1;
        }
        truth_all.push_back(truth);
    }

    validate_detector_map(boxes_all, truth_all, 0.5, 0.5, 0, imagesTxt, imageList);
}

// case test
TEST(FunctorTest, Init)
{
    GetDetectResult();

    EXPECT_EQ(1, 1) << "error in location calculate";
}