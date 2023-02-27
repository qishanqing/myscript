//
// Created by hao on 2021/7/5.
//

#ifndef DETECTOR_PROPERTY_SAMPLE_DETECTOR_H
#define DETECTOR_PROPERTY_SAMPLE_DETECTOR_H

#include <string>
#include <vector>

const int NUM_CLASSES = 4; // TODO : as config inner
const std::string MODEL_VERSION = "1.1.0";

const int MODEL_WIDTH = 320;
const int MODEL_HEIGHT = 320;
const int MODEL_CHANNEL = 3;

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

static const std::vector<std::string> INDEMIND_NAMES = {"shoes", "bin", "pedestal", "wire", "socket"};

static const std::vector<std::string> LABEL_NAMES = INDEMIND_NAMES;

enum ClassID
{
    BG = 0,
    SHOES,
    BIN,
    PEDESTAL,
    WIRE,
    SOCKET,
    UNKNOWN,
    PERSON = 100,
    ESCALATOR,
    ESCALATOR_HANDRAILS,
    PERSON_DUMMY,
    ESCALATOR_MODEL,
    ESCALATOR_HANDRAILS_MODEL,
};

#endif //DETECTOR_PROPERTY_SAMPLE_DETECTOR_H
