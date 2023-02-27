//
// Created by donglijian on 11/22/21.
//

#include "src/test/validate/validate_map.h"

typedef struct
{
    double precision;
    double recall;
    int tp, fp, fn;
    double f1;
    double ap;
    double score;
} pr_t;

// change to use Overlap in box.h
float overlap(float x1, float w1, float x2, float w2)
{
    float l1 = x1 - w1 / 2;
    float l2 = x2 - w2 / 2;
    float left = l1 > l2 ? l1 : l2;
    float r1 = x1 + w1 / 2;
    float r2 = x2 + w2 / 2;
    float right = r1 < r2 ? r1 : r2;
    return right - left;
}

// change to use Intersection
float box_intersection(Box a, Box b)
{
    float w = overlap(a.x, a.w, b.x, b.w);
    float h = overlap(a.y, a.h, b.y, b.h);
    if (w < 0 || h < 0) return 0;
    float area = w * h;
    return area;
}

//change to use Box::Union
float box_union(Box a, Box b)
{
    float i = box_intersection(a, b);
    float u = a.w * a.h + b.w * b.h - i;
    return u;
}

//change to use Box::IoU(Box a, Box b)
float box_iou(Box a, Box b)
{
    //return box_intersection(a, b)/box_union(a, b);

    float I = box_intersection(a, b);
    float U = box_union(a, b);
    if (I == 0 || U == 0)
    {
        return 0;
    }
    return I / U;
}

// change to use Box::NMSComparator
int detections_comparator(const void *pa, const void *pb)
{
    box_prob a = *(const box_prob *) pa;
    box_prob b = *(const box_prob *) pb;
    float diff = a.p - b.p;
    if (diff < 0) return 1;
    else if (diff > 0) return -1;
    return 0;
}

void WriteFpFile(FILE *fpFile, const box_prob &d, const std::string imageFile, const char *names[])
{
    std::string writeFile = names[d.class_id];
    writeFile = imageFile + " " + writeFile;
    fprintf(fpFile, "%s\n", writeFile.c_str());
}

float calculateAp(const int classId, pr_t **pr, const int detections_count
                  , const float precisionThreshold)
{

    double avg_precision = 0;

    double last_recall = pr[classId][detections_count - 1].recall;
    double last_precision = pr[classId][detections_count - 1].precision;
    for (int rank = detections_count - 2; rank >= 0; --rank)
    {
        double delta_recall = last_recall - pr[classId][rank].recall;
        last_recall = pr[classId][rank].recall;

        if (pr[classId][rank].precision > last_precision)
        {
            last_precision = pr[classId][rank].precision;
        }
        if (last_precision < precisionThreshold) continue;
        avg_precision += delta_recall * last_precision;
    }
    double delta_recall = last_recall - 0;
    avg_precision += delta_recall * last_precision;
    return avg_precision;
}

void WriteFile(std::ofstream &outFile, const int classes, pr_t **pr,const char *names[]
               , const int *detection_per_class_count, const int *truth_classes_count
               , const int detections_count, const std::vector<int> index, std::string title)
{
    if (outFile.is_open())
    {
        outFile << title <<", "<< std::endl;
    }

    for (int i = 0; i < classes; ++i)
    {
        if (index[i] < 0)
        {
            outFile<< "," << "," << names[i]<<std::endl;
            continue;
        }
        double class_precision = pr[i][index[i]].precision;
        double avg_precision = calculateAp(i,pr, detections_count, class_precision);
        double class_recall = pr[i][index[i]].recall;
        int tp = pr[i][index[i]].tp;
        int fp = pr[i][index[i]].fp;
        printf("%s : class_id =%2d, name =%16s, count = (%7d/%7d), ap = %2.2f%%   "
               "(TP = %d, FP = %d, FN = %d, precision = %2.1f%%, recall = %2.1f%%, F1 = %2.1f%%, precision_threshold: %2.1f%%) \n",
               title.c_str(), i, names[i], detection_per_class_count[i], truth_classes_count[i], avg_precision * 100,
               tp, fp, truth_classes_count[i] - tp, class_precision * 100, class_recall * 100,
               2 * class_precision * class_recall / (class_precision + class_recall) * 100, pr[i][index[i]].score);

        if (outFile.is_open())
        {
            outFile<< "," << "," << names[i] << "," << tp << "," << fp << "," << truth_classes_count[i] - tp << ","
                   <<2 * class_precision * class_recall / (class_precision + class_recall) * 100 <<"%,"
                   << avg_precision * 100 << "%," << class_precision * 100 << "%," << class_recall * 100 << "%," <<pr[i][index[i]].score/100.0<<std::endl;
        }
    }
}

void GetIndex(const int classes, const int detections_count, pr_t **pr, std::vector<int> &index, bool threshold80)
{
    for (int i = 0; i < classes; ++i)
    {
        int bestIndex = -1;
        if (threshold80)
        {
            for (int rank = 0; rank < detections_count; ++rank)
            {
                if (pr[i][rank].score >= 80)
                {
                    bestIndex = rank;
                }
            }
            index.at(i) = bestIndex;
        }
        else
        {
            double f1Max = 0;
            for (int rank = 0; rank < detections_count; ++rank)
            {
                if (pr[i][rank].f1 > f1Max)
                {
                    bestIndex = rank;
                    f1Max = pr[i][rank].f1;
                }
            }
            index.at(i) = bestIndex;
        }
    }
}

void validate_detector_map(std::vector<std::vector<box_prob>> &boxes_all,
                           std::vector<std::vector<box_label>> &truth1, float thresh_calc_avg_iou,
                           const float iou_thresh, int map_points, const std::string listFiles
                           , const std::vector<std::string> &imageLists)
{

    FILE *reinforcement_fd = NULL;

    FILE *fpFile = NULL;
    std::ofstream outFile;

    if (listFiles != "")
    {
        std::string fpListFile = listFiles.substr(0, listFiles.find_last_of('.')) + "_fp.txt";
        std::string resultFile = listFiles.substr(0, listFiles.find_last_of('.')) + "_result.csv";
        fpFile = fopen(fpListFile.c_str(), "w");
        outFile.open(resultFile,std::ios::out);
        if (outFile.is_open())
        {
            outFile<< "," << "," << "class" << "," << "TP" << "," << "FP" << "," <<"FN" << ","
                   << "F1" << "," << "Ap" << "," << "Precision" << "," << "Recall" <<std::endl;
        }
    }

    srand(time(0));
    printf("\n calculation mAP (mean average precision)...\n");
    static const char *names[] = {
            "person", "escalator", "escalator_handrails", "person_dummy", "5", "6"
    };
    //定义一些超参数和阈值
    int classes = 6;

    int m = boxes_all.size();
    int i0 = 0;
    int t;

    const float thresh = .25;
    const float nms = .45;

    int nthreads = 4;
    int num_labels = classes;

    int w = 416;
    int h = 416;
    int c = 3;

    //const float thresh_calc_avg_iou = 0.24;
    float avg_iou = 0;
    int tp_for_thresh = 0;
    int fp_for_thresh = 0;

    //定义预测box和标签box TODO change to define
    box_prob *detections = (box_prob *) calloc(1, sizeof(box_prob));

    int detections_count = 0;
    int unique_truth_count = 0;

    //TODO change to define
    int *truth_classes_count = (int *) calloc(classes, sizeof(int));

    // For multi-class precision and recall computation TODO change to define
    float *avg_iou_per_class = (float *) calloc(classes, sizeof(float));
    int *tp_for_thresh_per_class = (int *) calloc(classes, sizeof(int));
    int *fp_for_thresh_per_class = (int *) calloc(classes, sizeof(int));
    char **file_paths = (char **) calloc(m, 100);

    for (int imageIndex = 0; imageIndex < m; imageIndex++)
    {


        std::vector<box_prob> boxes = boxes_all[imageIndex];
        int nums_prob = boxes.size();

        std::vector<box_label> truth = truth1[imageIndex];

        int nums_labels = truth.size();
//         = truth1;

        int j;
        for (j = 0; j < nums_labels; ++j)
        {
            truth_classes_count[truth[j].id]++;
        }
        const int checkpoint_detections_count = detections_count;

        int i;
        for (i = 0; i < nums_prob; ++i)
        {
            int class_id;
            for (class_id = 0; class_id < classes; ++class_id)
            {
                float prob = boxes[i].p;
                if (prob > 0 && boxes[i].class_id == class_id)
                {
                    detections_count++;
                    // TODO change to define
                    detections = (box_prob *) realloc(detections, detections_count * sizeof(box_prob));
                    detections[detections_count - 1].b = boxes[i].b;
                    detections[detections_count - 1].p = prob;
                    detections[detections_count - 1].image_index = imageIndex;
                    detections[detections_count - 1].class_id = class_id;
                    detections[detections_count - 1].truth_flag = 0;
                    detections[detections_count - 1].unique_truth_index = -1;

                    int truth_index = -1;
                    float max_iou = 0;
                    for (j = 0; j < nums_labels; ++j)
                    {
                        Box t = {truth[j].x, truth[j].y, truth[j].w, truth[j].h};
                        float current_iou = box_iou(boxes[i].b, t);
                        if (current_iou > iou_thresh && class_id == truth[j].id)
                        {
                            if (current_iou > max_iou)
                            {
                                max_iou = current_iou;
                                truth_index = unique_truth_count + j;
                            }
                        }
                    }

                    if (truth_index > -1)
                    {
                        detections[detections_count - 1].truth_flag = 1;
                        detections[detections_count - 1].unique_truth_index = truth_index;
                    }

                    if (prob > thresh_calc_avg_iou)
                    {
                        int z, found = 0;
                        for (z = checkpoint_detections_count; z < detections_count - 1; ++z)
                        {
                            if (detections[z].unique_truth_index == truth_index)
                            {
                                found = 1;
                                break;
                            }
                        }

                        if (truth_index > -1 && found == 0)
                        {
                            avg_iou += max_iou;
                            ++tp_for_thresh;
                            avg_iou_per_class[class_id] += max_iou;
                            tp_for_thresh_per_class[class_id]++;
                        } else
                        {
                            fp_for_thresh++;
                            fp_for_thresh_per_class[class_id]++;
                        }
                    }
                }
            }
        }
        unique_truth_count += nums_labels;
    }

    if ((tp_for_thresh + fp_for_thresh) > 0)
        avg_iou = avg_iou / (tp_for_thresh + fp_for_thresh);

    int class_id;
    for (class_id = 0; class_id < classes; class_id++)
    {
        if ((tp_for_thresh_per_class[class_id] + fp_for_thresh_per_class[class_id]) > 0)
            avg_iou_per_class[class_id] = avg_iou_per_class[class_id] /
                                          (tp_for_thresh_per_class[class_id] + fp_for_thresh_per_class[class_id]);
    }

    qsort(detections, detections_count, sizeof(box_prob), detections_comparator);



    // for PR-curve   //TODO use define to calloc
    pr_t **pr = (pr_t **) calloc(classes, sizeof(pr_t *));
    int i;
    for (i = 0; i < classes; ++i)
    {
        //TODO use define to calloc
        pr[i] = (pr_t *) calloc(detections_count, sizeof(pr_t));
    }

    //TODO use define to calloc
    int *detection_per_class_count = (int *) calloc(classes, sizeof(int));
    for (int j = 0; j < detections_count; ++j)
    {

        detection_per_class_count[detections[j].class_id]++;
    }

    //TODO use define to calloc
    int *truth_flags = (int *) calloc(unique_truth_count, sizeof(int));

    int rank;
    for (rank = 0; rank < detections_count; ++rank)
    {
        if (rank > 0)
        {
            int class_id;
            for (class_id = 0; class_id < classes; ++class_id)
            {
                pr[class_id][rank].tp = pr[class_id][rank - 1].tp;
                pr[class_id][rank].fp = pr[class_id][rank - 1].fp;
            }
        }

        box_prob d = detections[rank];
        if (d.truth_flag == 1)
        {
            if (truth_flags[d.unique_truth_index] == 0)
            {
                truth_flags[d.unique_truth_index] = 1;
                pr[d.class_id][rank].tp++;    // true-positive

            } else
            {
                pr[d.class_id][rank].fp++;
                if (fpFile != NULL)
                {
                    WriteFpFile(fpFile, d, imageLists[d.image_index], names);
                }
            }
        } else
        {
            pr[d.class_id][rank].fp++;    // false-positive
            if (fpFile != NULL)
            {
                WriteFpFile(fpFile, d, imageLists[d.image_index], names);
            }
        }

        pr[d.class_id][rank].score = d.p;
        for (i = 0; i < classes; ++i)
        {
            const int tp = pr[i][rank].tp;
            const int fp = pr[i][rank].fp;
            const int fn = truth_classes_count[i] - tp;    // false-negative = objects - true-positive
            pr[i][rank].fn = fn;

            if ((tp + fp) > 0) pr[i][rank].precision = (double) tp / (double) (tp + fp);
            else pr[i][rank].precision = 0;

            if ((tp + fn) > 0) pr[i][rank].recall = (double) tp / (double) (tp + fn);
            else pr[i][rank].recall = 0;

            if (pr[i][rank].precision > 0 or pr[i][rank].recall > 0)
            {
                pr[i][rank].f1 = 2 * pr[i][rank].precision * pr[i][rank].recall
                                / (pr[i][rank].precision + pr[i][rank].recall);
            }
            else
            {
                pr[i][rank].f1 = 0;
            }

            if (rank == (detections_count - 1) && detection_per_class_count[i] != (tp + fp))
            {    // check for last rank
                printf(" class_id: %d - detections = %7d, tp+fp = %7d, tp = %6d, fp = %6d , fn = %6d \n", i,
                       detection_per_class_count[i], tp + fp, tp, fp, fn);
            }
        }
    }

    free(truth_flags);

    std::vector<int> bestF1Index(classes);
    GetIndex(classes, detections_count, pr, bestF1Index, false);
    WriteFile(outFile, classes, pr,names, detection_per_class_count, truth_classes_count
              , detections_count, bestF1Index, "best f1");

//    std::vector<int> threshold80_Index(classes);
//    GetIndex(classes, detections_count, pr, threshold80_Index, true);
//    WriteFile(outFile, classes, pr,names, detection_per_class_count, truth_classes_count
//            , detections_count, threshold80_Index, "threshold80");

    double mean_average_precision = 0;

    if (outFile.is_open())
    {
        outFile << "original: " << std::endl;
    }

    for (i = 0; i < classes; ++i)
    {
        double avg_precision = 0;
        if (map_points == 0)
        {
            double last_recall = pr[i][detections_count - 1].recall;
            double last_precision = pr[i][detections_count - 1].precision;
            for (rank = detections_count - 2; rank >= 0; --rank)
            {
                double delta_recall = last_recall - pr[i][rank].recall;
                last_recall = pr[i][rank].recall;

                if (pr[i][rank].precision > last_precision)
                {
                    last_precision = pr[i][rank].precision;
                }

                avg_precision += delta_recall * last_precision;
            }
            double delta_recall = last_recall - 0;
            avg_precision += delta_recall * last_precision;
        } else
        {
            int point;
            for (point = 0; point < map_points; ++point)
            {
                double cur_recall = point * 1.0 / (map_points - 1);
                double cur_precision = 0;
                for (rank = 0; rank < detections_count; ++rank)
                {
                    if (pr[i][rank].recall >= cur_recall)
                    {    // > or >=
                        if (pr[i][rank].precision > cur_precision)
                        {
                            cur_precision = pr[i][rank].precision;
                        }
                    }
                }
                avg_precision += cur_precision;
            }
            avg_precision = avg_precision / map_points;
        }

        float class_precision = (float) tp_for_thresh_per_class[i] /
                                ((float) tp_for_thresh_per_class[i] + (float) fp_for_thresh_per_class[i]);
        float class_recall = (float) tp_for_thresh_per_class[i] /
                             ((float) tp_for_thresh_per_class[i] +
                              (float) (truth_classes_count[i] - tp_for_thresh_per_class[i]));
        //printf("Precision = %1.2f, Recall = %1.2f, avg IOU = %2.2f%% \n\n", class_precision, class_recall, avg_iou_per_class[i]);

        printf("class_id =%2d, name =%16s, count = (%7d/%7d), ap = %2.2f%%   "
               "(TP = %d, FP = %d, FN = %d, precision = %2.1f%%, recall = %2.1f%%, F1 = %2.1f%%, avg_iou = %2.1f%%) \n",
               i, names[i], detection_per_class_count[i], truth_classes_count[i], avg_precision * 100,
               tp_for_thresh_per_class[i], fp_for_thresh_per_class[i],
               truth_classes_count[i] - tp_for_thresh_per_class[i], class_precision * 100, class_recall * 100,
               2 * class_precision * class_recall / (class_precision + class_recall) * 100, avg_iou_per_class[i] * 100);

        if (outFile.is_open())
        {
            outFile<< "," << "," << names[i] << "," << tp_for_thresh_per_class[i] << "," << fp_for_thresh_per_class[i]
            << "," << truth_classes_count[i] - tp_for_thresh_per_class[i] << ","
            <<2 * class_precision * class_recall / (class_precision + class_recall) * 100 <<"%,"
            << avg_precision * 100 << "%," << class_precision * 100 << "%," << class_recall * 100 << "%" <<std::endl;
        }

        mean_average_precision += avg_precision;
    }

    const float cur_precision = (float) tp_for_thresh / ((float) tp_for_thresh + (float) fp_for_thresh);
    const float cur_recall =
            (float) tp_for_thresh / ((float) tp_for_thresh + (float) (unique_truth_count - tp_for_thresh));
    const float f1_score = 2.F * cur_precision * cur_recall / (cur_precision + cur_recall);
    printf("\n for conf_thresh = %1.2f, precision = %1.2f, recall = %1.2f, F1-score = %1.2f \n",
           thresh_calc_avg_iou, cur_precision, cur_recall, f1_score);

    printf(" for conf_thresh = %0.2f, TP = %d, FP = %d, FN = %d, average IoU = %2.2f %% \n",
           thresh_calc_avg_iou, tp_for_thresh, fp_for_thresh, unique_truth_count - tp_for_thresh, avg_iou * 100);

    mean_average_precision = mean_average_precision / classes;
    printf("\n IoU threshold = %2.0f %%, ", iou_thresh * 100);
    if (map_points) printf("used %d Recall-points \n", map_points);
    else printf("used Area-Under-Curve for each unique Recall \n");

    printf(" mean average precision (mAP@%0.2f) = %f, or %2.2f %% \n", iou_thresh, mean_average_precision,
           mean_average_precision * 100);

//    for (i = 0; i < classes; ++i) {
//        free(pr[i]);
//    }

    free(pr);
    free(detections);
    free(truth_classes_count);
    free(detection_per_class_count);
//
    free(avg_iou_per_class);
    free(tp_for_thresh_per_class);
    free(fp_for_thresh_per_class);;
    if (reinforcement_fd != NULL) fclose(reinforcement_fd);

    if (fpFile != NULL) fclose(fpFile);
    if (outFile.is_open())
    {
        outFile.close();
        std::cout<<"close the file!\n";

    }

//    return mean_average_precision;
//    return 0;
}

unsigned long custom_hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void LabelResizeByPadding(box_label &truth, const int w, const int h, cv::Size dstSize)
{
    float ratioWidth = w / float(dstSize.width);
    float ratioHeight = h / float(dstSize.height);
    float ratio = 1.0;
    bool heightBigger = ratioHeight > ratioWidth;

    if (heightBigger)
    {
        ratio = (1 / ratioHeight) * ratioWidth;
        truth.x *= ratio;
        truth.w *= ratio;
    }
    else
    {
        ratio = (1 / ratioWidth) * ratioHeight;
        truth.y *= ratio;
        truth.h *= ratio;
    }
}

void read_boxes(char *filename, int *n, std::vector<box_label> *truth)
{
//    box_label* boxes = (box_label*)xcalloc(1, sizeof(box_label));

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Can't open label file. (This can be normal only if you use MSCOCO): %s \n", filename);

    }
    const int max_obj_img = 4000;// 30000;
    const int img_hash = (custom_hash(filename) % max_obj_img) * max_obj_img;
    //printf(" img_hash = %d, filename = %s; ", img_hash, filename);
    float x, y, h, w;
    int id;
    int count = 0;
    while (fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
    {
        box_label t;
//        boxes = (box_label*)xrealloc(boxes, (count + 1) * sizeof(box_label));
        t.track_id = count + img_hash;
        //printf(" boxes[count].track_id = %d, count = %d \n", boxes[count].track_id, count);
        t.id = id;
        t.x = x;
        t.y = y;
        t.h = h;
        t.w = w;
        t.left = x - w / 2;
        t.right = x + w / 2;
        t.top = y - h / 2;
        t.bottom = y + h / 2;

        truth->push_back(t);
        ++count;
    }
    fclose(file);
    *n = count;
    return;
}

void find_replace(const char *str, const char *orig, const char *rep, char *output)
{
    char *buffer = (char *) calloc(8192, sizeof(char));
    char *p;

    sprintf(buffer, "%s", str);
    if (!(p = strstr(buffer, orig)))
    {  // Is 'orig' even in 'str'?
        sprintf(output, "%s", buffer);
        free(buffer);
        return;
    }

    *p = '\0';

    sprintf(output, "%s%s%s", buffer, rep, p + strlen(orig));
    free(buffer);
}

void trim(char *str)
{
    //TODO use define
    char *buffer = (char *) calloc(8192, sizeof(char));
    sprintf(buffer, "%s", str);

    char *p = buffer;
    while (*p == ' ' || *p == '\t') ++p;

    char *end = p + strlen(p) - 1;
    while (*end == ' ' || *end == '\t')
    {
        *end = '\0';
        --end;
    }
    sprintf(str, "%s", p);

    free(buffer);
}

void find_replace_extension(char *str, const char *orig,const char *rep, char *output)
{
    char *buffer = (char *) calloc(8192, sizeof(char));

    sprintf(buffer, "%s", str);
    char *p = strstr(buffer, orig);
    int offset = (p - buffer);
    int chars_from_end = strlen(buffer) - offset;
    if (!p || chars_from_end != strlen(orig))
    {  // Is 'orig' even in 'str' AND is 'orig' found at the end of 'str'?
        sprintf(output, "%s", buffer);
        free(buffer);
        return;
    }

    *p = '\0';
    sprintf(output, "%s%s%s", buffer, rep, p + strlen(orig));
    free(buffer);
}

void replace_image_to_label(const char *input_path, char *output_path)
{
    find_replace(input_path, "/images/train2017/", "/labels/train2017/", output_path);    // COCO
    find_replace(output_path, "/images/val2017/", "/labels/val2017/", output_path);        // COCO
    find_replace(output_path, "/JPEGImages/", "/labels/", output_path);    // PascalVOC
    find_replace(output_path, "\\images\\train2017\\", "\\labels\\train2017\\", output_path);    // COCO
    find_replace(output_path, "\\images\\val2017\\", "\\labels\\val2017\\", output_path);        // COCO

    find_replace(output_path, "\\images\\train2014\\", "\\labels\\train2014\\", output_path);    // COCO
    find_replace(output_path, "\\images\\val2014\\", "\\labels\\val2014\\", output_path);        // COCO
    find_replace(output_path, "/images/train2014/", "/labels/train2014/", output_path);    // COCO
    find_replace(output_path, "/images/val2014/", "/labels/val2014/", output_path);        // COCO

    find_replace(output_path, "\\JPEGImages\\", "\\labels\\", output_path);    // PascalVOC
    find_replace(output_path, "/images/", "/labels/", output_path);    // COCO
    find_replace(output_path, "/VOC2007/JPEGImages/", "/VOC2007/labels/", output_path);        // PascalVOC
    find_replace(output_path, "/VOC2012/JPEGImages/", "/VOC2012/labels/", output_path);        // PascalVOC

    if (!strcmp(input_path, output_path))
    {
        find_replace(output_path, "/images/", "/labels/", output_path);
        find_replace(output_path, "\\images\\", "\\labels\\", output_path);
    }
    trim(output_path);

    std::vector<std::string> extList = {".jpg", ".JPG", ".jpeg", ".JPEG", ".png", ".PNG", ".bmp", ".BMP", ".ppm",
                                        ".tiff", ".TIFF",};
    for (auto ext : extList)
    {
        find_replace_extension(output_path, ext.c_str(), ".txt", output_path);
    }

    // Check file ends with txt:
    if (strlen(output_path) > 4)
    {
        char *output_path_ext = output_path + strlen(output_path) - 4;
        if (strcmp(".txt", output_path_ext) != 0)
        {
            fprintf(stderr, "Failed to infer label file name (check image extension is supported): %s \n", output_path);
        }
    } else
    {
        fprintf(stderr, "Label file name is too short: %s \n", output_path);
    }
}
