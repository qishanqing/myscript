//
// Created by hao on 2021/7/5.
//

#ifndef DETECTOR_PROPERTY_SAMPLE_DETECTOR_H
#define DETECTOR_PROPERTY_SAMPLE_DETECTOR_H

#include <string>
#include <vector>

using ClassID = int;

//TODO : remove
static const std::vector<std::string> voc_names = {"aeroplane", "bicycle", "bird", "boat"
                                                   , "bottle", "bus", "car", "cat"
                                                   , "chair", "cow", "diningtable", "dog"
                                                   , "horse", "motorbike"
                                                   , "person", "pottedplant", "sheep"
                                                   , "sofa"
                                                   , "train", "tvmonitor"};
static const std::vector<std::string> COCO_NAMES = {"person", "bicycle", "car"
                                                    , "motorbike"
                                                    , "aeroplane", "bus"
                                                    , "train", "truck", "boat"
                                                    , "traffic light"
                                                    , "fire hydrant"
                                                    , "stop sign", "parking meter"
                                                    , "bench"
                                                    , "bird", "cat", "dog"
                                                    , "horse", "sheep", "cow", "elephant"
                                                    , "bear", "zebra"
                                                    , "giraffe", "backpack", "umbrella"
                                                    , "handbag", "tie"
                                                    , "suitcase", "frisbee", "skis"
                                                    , "snowboard"
                                                    , "sports ball"
                                                    , "kite", "baseball bat"
                                                    , "baseball glove"
                                                    , "skateboard"
                                                    , "surfboard", "tennis racket"
                                                    , "bottle"
                                                    , "wine glass", "cup"
                                                    , "fork", "knife", "spoon", "bowl"
                                                    , "banana"
                                                    , "apple"
                                                    , "sandwich", "orange", "broccoli"
                                                    , "carrot"
                                                    , "hot dog"
                                                    , "pizza", "donut", "cake", "chair"
                                                    , "sofa"
                                                    , "pottedplant"
                                                    , "bed", "diningtable", "toilet"
                                                    , "tvmonitor", "laptop"
                                                    , "mouse", "remote", "keyboard"
                                                    , "cell phone", "microwave"
                                                    , "oven", "toaster", "sink"
                                                    , "refrigerator"
                                                    , "book", "clock"
                                                    , "vase", "scissors", "teddy bear"
                                                    , "hair drier"
                                                    , "toothbrush"};

#endif //DETECTOR_PROPERTY_SAMPLE_DETECTOR_H
