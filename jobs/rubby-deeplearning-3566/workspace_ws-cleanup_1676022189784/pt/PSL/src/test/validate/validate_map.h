//////////////////////////////////////////////////////////////////////
///  @file     validate_map.h
///  @brief    validate model's map
///  Details.
///
///  @author   donglijian
///  @version  1.7.61
///  @date     2021.11.21
//////////////////////////////////////////////////////////////////////

#ifndef DETECTOR_SAMPLE_VALIDATE_MAP_H
#define DETECTOR_SAMPLE_VALIDATE_MAP_H

#include <gtest/gtest.h>
#include "src/perception.h"
#include "src/utils/utils.h"
#include "sample/utils/utils.h"
#include "src/test/module/init_yolov3.h"

static std::vector<std::string> class_names{"person", "escalator", "unknown"};

//TODO use define to calloc
//#define XMALLOC(ptr, size, type) do
//{
//    ptr = malloc(size);
//    if(!ptr)
//    {
//
//}
//}while(0)

typedef struct
{
    Box b;
    float p;
    int class_id;
    int image_index;
    int truth_flag;
    int unique_truth_index;
} box_prob;

typedef struct box_label
{
    int id;
    int track_id;
    float x, y, w, h;
    float left, right, top, bottom;
} box_label;



float overlap(float x1, float w1, float x2, float w2);

float box_intersection(Box a, Box b);

float box_union(Box a, Box b);

float box_iou(Box a, Box b);

int detections_comparator(const void *pa, const void *pb);

void validate_detector_map(std::vector<std::vector<box_prob>> &boxes_all,
                           std::vector<std::vector<box_label>> &truth1, float thresh_calc_avg_iou,
                           const float iou_thresh, int map_points, const std::string listFiles = ""
                           ,const std::vector<std::string> &imageLists = {});

unsigned long custom_hash(char *str);

void read_boxes(char *filename, int *n, std::vector<box_label> *truth);

void find_replace(const char *str, const char *orig, const char *rep, char *output);

void trim(char *str);

void find_replace_extension(char *str, const char *orig, const char *rep, char *output);

void replace_image_to_label(const char *input_path, char *output_path);

void LabelResizeByPadding(box_label &truth, const int w, const int h, cv::Size dstSize);
#endif //DETECTOR_SAMPLE_VALIDATE_MAP_H
