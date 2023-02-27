//
// Created by lcy on 21-5-7.
//

#include <opencv2/imgproc.hpp>
#include <zconf.h>
#include "RegionSegmentation.h"

int RegionSegmentation::count_set_type_0(const Mat & src , Point target_point , Point p_min , Point p_max , list<Point> & list_out){
    if(src.empty()){
        LOGPLANNING(INFO) << "count_set_type:图片为空";
        return 1;
    }
    if(target_point.x < p_min.x || target_point.y < p_min.y || target_point.x >= p_max.x || target_point.y >= p_max.y){
        LOGPLANNING(INFO) << "count_set_type:起点超出地图范围";
        return 1;
    }
    list_out.clear();
    uint8_t  target_type = src.at<uchar>(target_point.y,target_point.x);//目标像素值
//    printf("target_type:%d\n",target_type);

    vector<vector<uint8_t >> history((unsigned long)src.cols, vector<uint8_t >((unsigned long)src.rows));

    list<Point> openList;

    openList.emplace_back(target_point);
    list_out.emplace_back(target_point);

    history[target_point.x][target_point.y] = 1;
    while (!openList.empty())
    {
        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            history[t.x][t.y] = 1;

            int x_0 = t.x + 0, y_0 = t.y - 1;
            if (x_0 < p_min.x || x_0 >= p_max.x || y_0 < p_min.y || y_0 >= p_max.y || history[x_0][y_0]){}
            else if(src.at<uchar>(y_0,x_0) == target_type)    {
                openList.emplace_back( Point(x_0,y_0));
                list_out.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

//            int x_1 = t.x + 1, y_1 = t.y - 1;
//            if (x_1 < p_min.x || x_1 >= p_max.x || y_1 < p_min.y || y_1 >= p_max.y || history[x_1][y_1]){}
//            else if(src.at<uchar>(y_1,x_1) == target_type)    {
//                openList.emplace_back( Point(x_1,y_1));
//                list_out.emplace_back( Point(x_1,y_1));
//                history[x_1][y_1] = 1;
//            }

            int x_2 = t.x + 1, y_2 = t.y + 0;
            if (x_2 < p_min.x || x_2 >= p_max.x || y_2 < p_min.y || y_2 >= p_max.y || history[x_2][y_2]){}
            else if(src.at<uchar>(y_2,x_2) == target_type)    {
                openList.emplace_back( Point(x_2,y_2));
                list_out.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

//            int x_3 = t.x + 1, y_3 = t.y + 1;
//            if (x_3 < p_min.x || x_3 >= p_max.x || y_3 < p_min.y || y_3 >= p_max.y || history[x_3][y_3]){}
//            else if(src.at<uchar>(y_3,x_3) == target_type)    {
//                openList.emplace_back( Point(x_3,y_3));
//                list_out.emplace_back( Point(x_3,y_3));
//                history[x_3][y_3] = 1;
//            }

            int x_4 = t.x + 0, y_4 = t.y + 1;
            if (x_4 < p_min.x || x_4 >= p_max.x || y_4 < p_min.y || y_4 >= p_max.y || history[x_4][y_4]){}
            else if(src.at<uchar>(y_4,x_4) == target_type)    {
                openList.emplace_back( Point(x_4,y_4));
                list_out.emplace_back( Point(x_4,y_4));
                history[x_4][y_4] = 1;
            }

//            int x_5 = t.x - 1, y_5 = t.y + 1;
//            if (x_5 < p_min.x || x_5 >= p_max.x || y_5 < p_min.y || y_5 >= p_max.y || history[x_5][y_5]){}
//            else if(src.at<uchar>(y_5,x_5) == target_type)    {
//                openList.emplace_back( Point(x_5,y_5));
//                list_out.emplace_back( Point(x_5,y_5));
//                history[x_5][y_5] = 1;
//            }

            int x_6 = t.x - 1, y_6 = t.y + 0;
            if (x_6 < p_min.x || x_6 >= p_max.x || y_6 < p_min.y || y_6 >= p_max.y || history[x_6][y_6]){}
            else if(src.at<uchar>(y_6,x_6) == target_type)    {
                openList.emplace_back( Point(x_6,y_6));
                list_out.emplace_back( Point(x_6,y_6));
                history[x_6][y_6] = 1;
            }

//            int x_7 = t.x - 1, y_7 = t.y - 1;
//            if (x_7 < p_min.x || x_7 >= p_max.x || y_7 < p_min.y || y_7 >= p_max.y || history[x_7][y_7]){}
//            else if(src.at<uchar>(y_7,x_7) == target_type)    {
//                openList.emplace_back( Point(x_7,y_7));
//                list_out.emplace_back( Point(x_7,y_7));
//                history[x_7][y_7] = 1;
//            }
        }
    }
    vector<vector<uint8_t >>().swap(history);
    std::list<Point>().swap(openList);
    return 0;
}
int RegionSegmentation::Erode_map(const Mat & src , Mat & dst , int g_nStructRlementSize){
//    int g_nStructRlementSize = 3;//内核矩阵的尺寸
    Mat element = getStructuringElement(MORPH_RECT, Size(2 * g_nStructRlementSize + 1, 2 * g_nStructRlementSize + 1), Point(g_nStructRlementSize, g_nStructRlementSize));

    erode(src, dst, element);

    return 0;
}

int RegionSegmentation::Dilate_map(const Mat & src , Mat & dst , int g_nStructRlementSize){
//    int g_nStructRlementSize = 3;//内核矩阵的尺寸
    Mat element = getStructuringElement(MORPH_RECT, Size(2 * g_nStructRlementSize + 1, 2 * g_nStructRlementSize + 1), Point(g_nStructRlementSize, g_nStructRlementSize));

    dilate(src, dst, element);

    return 0;
}

int RegionSegmentation::MarkingPartition(const Mat & image , Mat & dst){
    Mat map = image.clone();
    vector<list<Point>> local_array;
    list<Point> array_tmp;
    uint8_t id = 2;
    for(int i = 0 ; i < map.rows ; i++)
        for(int j = 0 ; j < map.cols ; j++){
            if(map.at<uint8_t >(i,j) == 0){

                count_set_type_0(map,Point(j,i),Point(0,0),Point(map.cols - 1 , map.rows - 1) , array_tmp);
                if(array_tmp.size() <= 81){
                    array_tmp.clear();
                    continue;
                }
                local_array.emplace_back(array_tmp);

                list<Point>::iterator iter;
                for(iter = array_tmp.begin() ; iter != array_tmp.end() ; iter++)
                {
                    map.at<uint8_t >(*iter) = (uint8_t)(id);
                }
                id ++;
            }
        }
    num_partition = (int)local_array.size();
    for(int i = 0 ; i < local_array.size() ; i++){
        Mat map_fill = Mat::zeros(image.rows,image.cols,CV_8UC1);
        int num = 0 ;
        while(!local_array[i].empty()){
            map_fill.at<uint8_t >(local_array[i].front()) = 1;
            local_array[i].pop_front();
            num ++;
        }

        Mat tmp = Mat::zeros(image.rows,image.cols,CV_8UC1);

        Dilate_map(map_fill,tmp , width);
        tmp.copyTo(map_fill);
        tmp.release();

        for(int m = 0 ; m < image.rows ; m++)
            for(int n = 0 ; n < image.cols ; n++){
                if(map_fill.at<uint8_t >(m,n)){
                    dst.at<uint8_t >(m,n) = (uint8_t)((i + 1));
                }
            }
        map_fill.release();
    }
    map.release();
}