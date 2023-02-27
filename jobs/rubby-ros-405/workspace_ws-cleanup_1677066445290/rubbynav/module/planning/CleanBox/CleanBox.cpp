//
// Created by lcy on 21-6-22.
//
#include <list>
#include <Map.h>
#include "CleanBox.h"

//#include "Planning.h"

bool CleanBox::CleanBoxInit(Mat & map , Point start){
    int nx = map.cols;
    int ny = map.rows;
//    printf("nx:%d , ny:%d\n",nx,ny);

    Mat mapbox(map.rows, map.cols, CV_8UC1, Scalar(0));
    Mat mapfill(map.rows, map.cols, CV_8UC1, Scalar(0));
    Mat mapline(map.rows, map.cols, CV_8UC1, Scalar(0));

    int degree = sidelength/2;

    std::list<int> array_x;
    std::list<int> array_y;

    Point tmp = start;
    while(tmp.x + degree <= nx){
        array_x.emplace_back(tmp.x);
        tmp.x += (sidelength);
    }

    tmp = start;
    while(tmp.x - degree >= 0){
        array_x.emplace_front(tmp.x);
        tmp.x -= (sidelength);
    }

    tmp = start;
    while(tmp.y  + degree <= ny){
        array_y.emplace_back(tmp.y);
        tmp.y += (sidelength);
    }

    tmp = start;
    while(tmp.y - degree >= 0){
        array_y.emplace_front(tmp.y);
        tmp.y -= (sidelength);
    }
//    std::list<int>::iterator iterx;
//    std::list<int>::iterator itery;
//    for(iterx = array_x.begin(); iterx != array_x.end() ; iterx++){
//        printf("%d ",*iterx);
//    }
//    printf("\n");
//    for(itery = array_y.begin(); itery != array_y.end() ; itery++){
//        printf("%d ",*itery);
//    }
//    printf("\n");

//    printf("array_x size:%ld array_y size:%ld\n",array_x.size(),array_y.size());
    array_x.sort();
    array_x.unique();
    array_y.sort();
    array_y.unique();
//    printf("array_x size:%ld array_y size:%ld\n",array_x.size(),array_y.size());
//    for(iterx = array_x.begin(); iterx != array_x.end() ; iterx++){
//        printf("%d ",*iterx);
//    }
//    printf("\n");
//    for(itery = array_y.begin(); itery != array_y.end() ; itery++){
//        printf("%d ",*itery);
//    }
//    printf("\n");

    Point root_points[1][4];
    const Point* ppt[1] = {root_points[0]};
    int npt[] = {4};
    int x_0 = 0,x_1 = 0,x_2 = 0,x_3 = 0,y_0 = 0,y_1 = 0,y_2 = 0,y_3 = 0;
    uint8_t id_box = 1;
    BoxInfo boxtmp;
    std::list<int>::iterator iter1;
    std::list<int>::iterator iter2;
    for(iter2 = array_x.begin(); iter2 != array_x.end() ; iter2++)
        for(iter1 = array_y.begin(); iter1 != array_y.end() ; iter1++){
            if(iter2 == array_x.begin() || iter1 == array_y.begin() || iter2 == prev(array_x.end()) || iter1 == prev(array_y.end())){

                x_0 = *iter2 - degree; y_0 = *iter1 - degree;
                x_1 = *iter2 + degree; y_1 = *iter1 - degree;
                x_2 = *iter2 + degree; y_2 = *iter1 + degree;
                x_3 = *iter2 - degree; y_3 = *iter1 + degree;

                if(iter2 == array_x.begin()){
                    x_0 = 0;
                    x_3 = 0;
                }
                if(iter1 == array_y.begin()){
                    y_0 = 0;
                    y_1 = 0;
                }
                if(iter2 == prev(array_x.end())){
                    x_1 = nx - 1;
                    x_2 = nx - 1;
                }
                if(iter1 == prev(array_y.end())){
                    y_2 = ny - 1;
                    y_3 = ny - 1;
                }

                root_points[0][0] = Point(x_0 , y_0);
                root_points[0][1] = Point(x_1 , y_1);
                root_points[0][2] = Point(x_2 , y_2);
                root_points[0][3] = Point(x_3 , y_3);

            }else{
                root_points[0][0] = Point(*iter2 - degree , *iter1 - degree);
                root_points[0][1] = Point(*iter2 + degree , *iter1 - degree);
                root_points[0][2] = Point(*iter2 + degree , *iter1 + degree);
                root_points[0][3] = Point(*iter2 - degree , *iter1 + degree);
            }

//            root_points[0][0] = Point(*iter2 - degree , *iter1 - degree);
//            root_points[0][1] = Point(*iter2 + degree , *iter1 - degree);
//            root_points[0][2] = Point(*iter2 + degree , *iter1 + degree);
//            root_points[0][3] = Point(*iter2 - degree , *iter1 + degree);
//            printf("id_box:%d\n",id_box);
//            string tmp_txt = std::to_string(id_box);
//            putText(mapbox, tmp_txt, Point(*iter2, *iter1), FONT_HERSHEY_TRIPLEX, 1.0, Scalar(100), 1, 4);//添加字符串
            polylines(mapbox, ppt, npt, 1, true, Scalar(id_box),1,8,0);

            fillPoly(mapfill, ppt, npt, 1, Scalar(id_box));
//            putText(mapfill, tmp_txt, Point(*iter2, *iter1), FONT_HERSHEY_TRIPLEX, 1.0, Scalar(100), 1, 4);//添加字符串

            line(mapline, root_points[0][0], root_points[0][3], Scalar(0), 1);
            line(mapline, root_points[0][3], root_points[0][2], Scalar(50), 1);
            line(mapline, root_points[0][2], root_points[0][1], Scalar(100), 1);
            line(mapline, root_points[0][1], root_points[0][0], Scalar(150), 1);

            boxtmp.id = id_box;
            boxtmp.point_min = root_points[0][0];
            boxtmp.point_max = root_points[0][2];
            boxarray.emplace_back(boxtmp);

            id_box ++;
    }
//    namedWindow("mapbox", CV_WINDOW_NORMAL);
//    imshow("mapbox", mapbox);
//
//    namedWindow("mapfill", CV_WINDOW_NORMAL);
//    imshow("mapfill", mapfill);
//
//    namedWindow("mapline", CV_WINDOW_NORMAL);
//    imshow("mapline", mapline);

    for(int i = 0 ; i < map.rows ; i++)
        for(int j = 0 ; j < map.cols ; j++){
            map.at<Vec3b>(i,j)[0] = mapbox.at<uint8_t >(i,j) == 255 ? (uint8_t)0:mapbox.at<uint8_t >(i,j);
            map.at<Vec3b>(i,j)[1] = mapline.at<uint8_t >(i,j) == 255 ? (uint8_t)0:mapline.at<uint8_t >(i,j);
            map.at<Vec3b>(i,j)[2] = mapfill.at<uint8_t >(i,j) == 255 ? (uint8_t)0:mapfill.at<uint8_t >(i,j);
        }

//    namedWindow("map", CV_WINDOW_NORMAL);
//    imshow("map", map);

    mapbox.release();
    mapfill.release();
    mapline.release();
    boxarea = cv::Mat::zeros(map.rows, map.cols, CV_8UC1);
    boxmap = map.clone();

    return true;
}

bool CleanBox::GetPointInBox(Point p , BoxInfo & box){
    if(p.x < 0){
        p.x = 0;
    }
    if(p.x >= boxmap.cols){
        p.x = boxmap.cols - 1;
    }
    if(p.y < 0){
        p.y = 0;
    }
    if(p.y >= boxmap.rows){
        p.y = boxmap.rows - 1;
    }
    int tmp = boxmap.at<Vec3b>(p)[2] - 1;
    if(tmp < boxarray.size()){
        box = boxarray[tmp];
//        printf("box:(%d,%d)-(%d,%d) tmp:%d\n",box.point_min.x,box.point_min.y,box.point_max.x,box.point_max.y,tmp);
    }else{
        return false;
    }
    return true;
}
bool CleanBox::GetPointInDelimit(Point p , BoxInfo & box){
    box.id = s_map.RegionSegmentationMap.at<uint8_t >(p);

    if(box.id != RegionUnknowCell) {
        box.point_min = Point(s_map.width,s_map.height);
        box.point_max = Point(0,0);
        for(int i = 0 ; i < s_map.perceptiondata.delimit[box.id].size() ; i++){
            if(box.point_min.x > s_map.perceptiondata.delimit[box.id][i].point.x){
                box.point_min.x = s_map.perceptiondata.delimit[box.id][i].point.x;
            }
            if(box.point_min.y > s_map.perceptiondata.delimit[box.id][i].point.y){
                box.point_min.y = s_map.perceptiondata.delimit[box.id][i].point.y;
            }
            if(box.point_max.x < s_map.perceptiondata.delimit[box.id][i].point.x){
                box.point_max.x = s_map.perceptiondata.delimit[box.id][i].point.x;
            }
            if(box.point_max.y < s_map.perceptiondata.delimit[box.id][i].point.y){
                box.point_max.y = s_map.perceptiondata.delimit[box.id][i].point.y;
            }
        }
        box.pose_min = s_map.MapToWorld(box.point_min);
        box.pose_max = s_map.MapToWorld(box.point_max);

//        printf("%d:(%d,%d)-(%d,%d)\n",box.id,box.point_min.x,box.point_min.y,box.point_max.x,box.point_max.y);
    }else{
        box.id = -1;
        LOGPLANNING(INFO) << "start:(" << p .x << " , " << p.y <<") is out of RegionSegmentation !!!";
//        return false;
    }
    return true;
}
int CleanBox::GetPointInBoxId(Point p){
    if(p.x < 0 || p.x >= boxmap.cols || p.y < 0 || p.y >= boxmap.rows){
        return -1;
    }
    return boxmap.at<Vec3b>(p)[2];
}
BoxInfo CleanBox::GetCurBox(){
    bool flg = false;
    std::vector<BoxInfo>::iterator iter;
    for(iter = CleanBox_history.begin() ; iter != CleanBox_history.end() ; iter ++){
        if(iter->id == curbox.id){
            flg = true;
            break;
        }
    }
    if(!flg){
        CleanBox_history.emplace_back(curbox);
//        if(curbox.point_min.x < s_map.originPointOccupyMin.x){
//            s_map.originPointOccupyMin.x = (curbox.point_min.x - sidelength/2) < 0 ? 0:(curbox.point_min.x - sidelength/2);
//        }
//        if(curbox.point_min.y < s_map.originPointOccupyMin.y){
//            s_map.originPointOccupyMin.y = (curbox.point_min.y - sidelength/2) < 0 ? 0:(curbox.point_min.y - sidelength/2);
//        }
//        if(curbox.point_max.x > s_map.originPointOccupyMax.x){
//            s_map.originPointOccupyMax.x = (curbox.point_max.x + sidelength/2) > (s_map.width - 1) ? (s_map.width - 1):(curbox.point_max.x + sidelength/2);
//        }
//        if(curbox.point_max.y > s_map.originPointOccupyMax.y){
//            s_map.originPointOccupyMax.y = (curbox.point_max.y + sidelength/2) > (s_map.height - 1) ? (s_map.width - 1):(curbox.point_max.y + sidelength/2);
//        }
//        s_map.widthOccupy = s_map.originPointOccupyMax.x - s_map.originPointOccupyMin.x;
//        s_map.heightOccupy = s_map.originPointOccupyMax.y - s_map.originPointOccupyMin.y;
//        s_map.originPoseOccupy = s_map.MapToWorld(s_map.originPointOccupyMin) - Point2f(0.025f,0.025f);
    }

    return curbox;
}
void CleanBox::SetCurBox(const BoxInfo &box){
    LOGPLANNING(INFO) << "SetCurBox bix:(" << box.point_min.x << " , " << box.point_min.y << ")-(" << box.point_max.x << " , " << box.point_max.y << ") , id:" <<box.id;

    curbox = box;
    if(curbox.id == -1){
        return;
    }

    bool flg = false;
    std::vector<BoxInfo>::iterator iter;
    for(iter = CleanBox_history.begin() ; iter != CleanBox_history.end() ; iter ++){
        if(iter->id == curbox.id){
            flg = true;
            break;
        }
    }

    if(!flg){
        CleanBox_history.emplace_back(curbox);
        if(curbox.point_min.x < s_map.originPointOccupyMin.x){
            s_map.originPointOccupyMin.x = (curbox.point_min.x - sidelength/2) < 0 ? 0:(curbox.point_min.x - sidelength/2);
        }
        if(curbox.point_min.y < s_map.originPointOccupyMin.y){
            s_map.originPointOccupyMin.y = (curbox.point_min.y - sidelength/2) < 0 ? 0:(curbox.point_min.y - sidelength/2);
        }
        if(curbox.point_max.x > s_map.originPointOccupyMax.x){
            s_map.originPointOccupyMax.x = (curbox.point_max.x + sidelength/2) > (s_map.width - 1) ? (s_map.width - 1):(curbox.point_max.x + sidelength/2);
        }
        if(curbox.point_max.y > s_map.originPointOccupyMax.y){
            s_map.originPointOccupyMax.y = (curbox.point_max.y + sidelength/2) > (s_map.height - 1) ? (s_map.width - 1):(curbox.point_max.y + sidelength/2);
        }
        s_map.widthOccupy = s_map.originPointOccupyMax.x - s_map.originPointOccupyMin.x;
        s_map.heightOccupy = s_map.originPointOccupyMax.y - s_map.originPointOccupyMin.y;
        s_map.originPoseOccupy = s_map.MapToWorld(s_map.originPointOccupyMin) - Point2f(0.025f,0.025f);
        LOGPLANNING(INFO) << "s_map.originPointOccupy:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")-(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";
    }
}
void CleanBox::SetCurBox_loadmap(const BoxInfo &box){
    for(int i = 0 ; i < s_map.NavMap.rows ; i++)
        for(int j = 0 ; j < s_map.NavMap.cols ; j++){
            if(s_map.NavMap.at<uint8_t >(i,j) == NavFreeCell){
                if(j < s_map.originPointOccupyMin.x){
                    s_map.originPointOccupyMin.x = j;
                }
                if(i < s_map.originPointOccupyMin.y){
                    s_map.originPointOccupyMin.y = i;
                }
                if(s_map.originPointOccupyMax.x < j){
                    s_map.originPointOccupyMax.x = j;
                }
                if(s_map.originPointOccupyMax.y < i){
                    s_map.originPointOccupyMax.y = i;
                }
            }
        }
    int side_add = (int)(1.0f/s_map.resolving);
    s_map.originPointOccupyMin.x = (s_map.originPointOccupyMin.x - side_add) < 0 ? 0:(s_map.originPointOccupyMin.x - side_add);
    s_map.originPointOccupyMin.y = (s_map.originPointOccupyMin.y - side_add) < 0 ? 0:(s_map.originPointOccupyMin.y - side_add);
    s_map.originPointOccupyMax.x = (s_map.originPointOccupyMax.x + side_add) > (s_map.width - 1) ? (s_map.width - 1):(s_map.originPointOccupyMax.x + side_add);
    s_map.originPointOccupyMax.y = (s_map.originPointOccupyMax.y + side_add) > (s_map.height - 1) ? (s_map.width - 1):(s_map.originPointOccupyMax.y + side_add);

    s_map.widthOccupy = s_map.originPointOccupyMax.x - s_map.originPointOccupyMin.x;
    s_map.heightOccupy = s_map.originPointOccupyMax.y - s_map.originPointOccupyMin.y;
    s_map.originPoseOccupy = s_map.MapToWorld(s_map.originPointOccupyMin) - Point2f(0.025f,0.025f);
    LOGPLANNING(INFO) << "s_map.originPointOccupy:(" << s_map.originPointOccupyMin.x << " , " << s_map.originPointOccupyMin.y << ")-(" << s_map.originPointOccupyMax.x << " , " << s_map.originPointOccupyMax.y << ")";

    curbox = box;
    if(curbox.id == -1){
        return;
    }

    bool flg = false;
    std::vector<BoxInfo>::iterator iter;
    for(iter = CleanBox_history.begin() ; iter != CleanBox_history.end() ; iter ++){
        if(iter->id == curbox.id){
            flg = true;
            break;
        }
    }

    if(!flg){
        CleanBox_history.emplace_back(curbox);
    }
}
BoxInfo CleanBox::GetCurDelimit(){
    bool flg = false;
    std::vector<BoxInfo>::iterator iter;
    for(iter = CleanBox_history.begin() ; iter != CleanBox_history.end() ; iter ++){
        if(iter->id == curbox.id){
            flg = true;
            break;
        }
    }
    if(!flg){
        CleanBox_history.emplace_back(curbox);
//        if(curbox.point_min.x < s_map.originPointOccupyMin.x){
//            s_map.originPointOccupyMin.x = (curbox.point_min.x - sidelength/2) < 0 ? 0:(curbox.point_min.x - sidelength/2);
//        }
//        if(curbox.point_min.y < s_map.originPointOccupyMin.y){
//            s_map.originPointOccupyMin.y = (curbox.point_min.y - sidelength/2) < 0 ? 0:(curbox.point_min.y - sidelength/2);
//        }
//        if(curbox.point_max.x > s_map.originPointOccupyMax.x){
//            s_map.originPointOccupyMax.x = (curbox.point_max.x + sidelength/2) > (s_map.width - 1) ? (s_map.width - 1):(curbox.point_max.x + sidelength/2);
//        }
//        if(curbox.point_max.y > s_map.originPointOccupyMax.y){
//            s_map.originPointOccupyMax.y = (curbox.point_max.y + sidelength/2) > (s_map.height - 1) ? (s_map.width - 1):(curbox.point_max.y + sidelength/2);
//        }
//        s_map.widthOccupy = s_map.originPointOccupyMax.x - s_map.originPointOccupyMin.x;
//        s_map.heightOccupy = s_map.originPointOccupyMax.y - s_map.originPointOccupyMin.y;
//        s_map.originPoseOccupy = s_map.MapToWorld(s_map.originPointOccupyMin) - Point2f(0.025f,0.025f);
    }

    return curbox;
}