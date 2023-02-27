//
// Created by lcy on 21-6-22.
//

#include "Search.h"

int Search::Search_point_set(const Mat & maze , Point startPoint , uint8_t type, Point &target_point){
    if(maze.empty()){
        LOGPLANNING(INFO) << "输入的地图数据为空.";
        return 0;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x > maze.cols || startPoint.y > maze.rows){
        LOGPLANNING(INFO) << "输入的起点超出地图范围";
        return 0;
    }
    int x_max = maze.cols;
    int y_max = maze.rows;

    vector<vector<uint8_t >> history((unsigned long)maze.cols, vector<uint8_t >((unsigned long)maze.rows));

    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    history[startPoint.x][startPoint.y] = 1;

    while (!openList.empty())
    {

        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            history[t.x][t.y] = 1;

            int x_0 = t.x + 0, y_0 = t.y - 1;
            auto * p = maze.ptr<uchar>(y_0);
            if (x_0 < 0 || x_0 >= x_max || y_0 < 0 || y_0 >= y_max){}
            else if(history[x_0][y_0]){}
            else if(p[x_0] == type)    {
                target_point.x = x_0;
                target_point.y = y_0;

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
            if (x_1 < 0 || x_1 >= x_max || y_1 < 0 || y_1 >= y_max){}
            else if(history[x_1][y_1]){}
            else if(p[x_1] == type)    {
                target_point.x = x_1;
                target_point.y = y_1;

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_1,y_1));
                history[x_1][y_1] = 1;
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
            if (x_2 < 0 || x_2 >= x_max || y_2 < 0 || y_2 >= y_max){}
            else if(history[x_2][y_2]){}
            else if(p[x_2]==type)    {
                target_point.x = x_2;
                target_point.y = y_2;

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

            int x_3 = t.x + 1, y_3 = t.y + 0;
            p = maze.ptr<uchar>(y_3);
            if (x_3 < 0 || x_3 >= x_max || y_3 < 0 || y_3 >= y_max){}
            else if(history[x_3][y_3]){}
            else if(p[x_3]==type)    {
                target_point.x = x_3;
                target_point.y = y_3;

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_3,y_3));
                history[x_3][y_3] = 1;
            }
        }
    }

    vector<vector<uint8_t >>().swap(history);
    list<Point>().swap(openList);

    return 0;
}
int Search::Search_point_out_set(const Mat & maze , Point startPoint , uint8_t type, Point &target_point){
    if(maze.empty()){
        LOGPLANNING(INFO) << "输入的地图数据为空.";
        return 0;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x > maze.cols || startPoint.y > maze.rows){
        LOGPLANNING(INFO) << "输入的起点超出地图范围";
        return 0;
    }
    int x_max = maze.cols;
    int y_max = maze.rows;

    vector<vector<uint8_t >> history((unsigned long)maze.cols, vector<uint8_t >((unsigned long)maze.rows));

    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    history[startPoint.x][startPoint.y] = 1;

    while (!openList.empty())
    {

        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            history[t.x][t.y] = 1;

            int x_0 = t.x + 0, y_0 = t.y - 1;
            auto * p = maze.ptr<uchar>(y_0);
            if (x_0 < 0 || x_0 >= x_max || y_0 < 0 || y_0 >= y_max){}
            else if(history[x_0][y_0]){}
            else if(p[x_0] != type)    {
                target_point.x = x_0;
                target_point.y = y_0;

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
            if (x_1 < 0 || x_1 >= x_max || y_1 < 0 || y_1 >= y_max){}
            else if(history[x_1][y_1]){}
            else if(p[x_1] != type)    {
                target_point.x = x_1;
                target_point.y = y_1;

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_1,y_1));
                history[x_1][y_1] = 1;
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
            if (x_2 < 0 || x_2 >= x_max || y_2 < 0 || y_2 >= y_max){}
            else if(history[x_2][y_2]){}
            else if(p[x_2] != type)    {
                target_point.x = x_2;
                target_point.y = y_2;

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

            int x_3 = t.x + 1, y_3 = t.y + 0;
            p = maze.ptr<uchar>(y_3);
            if (x_3 < 0 || x_3 >= x_max || y_3 < 0 || y_3 >= y_max){}
            else if(history[x_3][y_3]){}
            else if(p[x_3] != type)    {
                target_point.x = x_3;
                target_point.y = y_3;

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_3,y_3));
                history[x_3][y_3] = 1;
            }
        }
    }

    vector<vector<uint8_t >>().swap(history);
    list<Point>().swap(openList);

    return 0;
}
int Search::Search_p2p(const Mat & maze , Point startPoint , Point target_point){
    if(maze.empty()){
        LOGPLANNING(INFO) << "maze is empty !!!";
        return false;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x >= maze.cols || startPoint.y >= maze.rows ||
       target_point.x < 0 || target_point.y < 0 || target_point.x >= maze.cols || target_point.y >= maze.rows){
        LOGPLANNING(INFO) << "point out map : start(" << startPoint.x << " , " << startPoint.y << ") , target(" << target_point.x << " , " << target_point.y<< ")";
        return 0;
    }
    int x_max = maze.cols;
    int y_max = maze.rows;

    vector<vector<uint8_t >> history((unsigned long)maze.cols, vector<uint8_t >((unsigned long)maze.rows));

    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    history[startPoint.x][startPoint.y] = 1;

    while (!openList.empty())
    {

        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            history[t.x][t.y] = 1;

            int x_0 = t.x + 0, y_0 = t.y - 1;
            auto * p = maze.ptr<uchar>(y_0);
            if (x_0 < 0 || x_0 >= x_max || y_0 < 0 || y_0 >= y_max){}
            else if(history[x_0][y_0] || p[x_0] != SearchFreeCell){}
            else if(x_0 == target_point.x && y_0 == target_point.y)    {

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
            if (x_1 < 0 || x_1 >= x_max || y_1 < 0 || y_1 >= y_max){}
            else if(history[x_1][y_1] || p[x_1] != SearchFreeCell){}
            else if(x_1 == target_point.x && y_1 == target_point.y)    {

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_1,y_1));
                history[x_1][y_1] = 1;
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
            if (x_2 < 0 || x_2 >= x_max || y_2 < 0 || y_2 >= y_max){}
            else if(history[x_2][y_2] || p[x_2] != SearchFreeCell){}
            else if(x_2 == target_point.x && y_2 == target_point.y)    {

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

            int x_3 = t.x + 1, y_3 = t.y + 0;
            p = maze.ptr<uchar>(y_3);
            if (x_3 < 0 || x_3 >= x_max || y_3 < 0 || y_3 >= y_max){}
            else if(history[x_3][y_3] || p[x_3] != SearchFreeCell){}
            else if(x_3 == target_point.x && y_3 == target_point.y)    {

                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return 1;
            }else{
                openList.emplace_back( Point(x_3,y_3));
                history[x_3][y_3] = 1;
            }
        }
    }

    vector<vector<uint8_t >>().swap(history);
    list<Point>().swap(openList);

    return 0;//未找到
}
/**
 * 探索目标点
 * @param maze 地图数据
 * @param startPoint 起始点
 * @param target_point 目标点
 * @return 0 未找到 1 找到
 */
int Search::Search_point_explore(const Mat & maze , Point startPoint , Point &target_point){
    if(maze.empty()){
        LOGPLANNING(INFO) << "输入的地图数据为空.";
        return Search_Null;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x > maze.cols || startPoint.y > maze.rows){
        LOGPLANNING(INFO) << "输入的起点超出地图范围";
        return Search_Null;
    }
    int x_max = maze.cols;
    int y_max = maze.rows;

    vector<vector<uint8_t >> history((unsigned long)maze.cols, vector<uint8_t >((unsigned long)maze.rows));
#ifdef HUNT_POINT_TEST
    testMaze.gridMap.clear();
#endif
    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    history[startPoint.x][startPoint.y] = 1;

    while (!openList.empty())
    {

        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            history[t.x][t.y] = 1;

            int x_0 = t.x + 0, y_0 = t.y - 1;
            auto * p = maze.ptr<uchar>(y_0);
            if (x_0 < 0 || x_0 >= x_max || y_0 < 0 || y_0 >= y_max){}
            else if(history[x_0][y_0] || p[x_0] == NavObsCell || p[x_0] == NavObsExpandCell || p[x_0] == NavSensorObsCell){}
            else if(p[x_0] == NavUnknowCell)    {
                target_point = t;

#ifdef HUNT_POINT_TEST
                testMaze.gridMap = history;
#endif
                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return Search_Get;
            }else{
                openList.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
            if (x_1 < 0 || x_1 >= x_max || y_1 < 0 || y_1 >= y_max){}
            else if(history[x_1][y_1] || p[x_1] == NavObsCell || p[x_1] == NavObsExpandCell || p[x_1] == NavSensorObsCell){}
            else if(p[x_1] == NavUnknowCell)    {
                target_point = t;
#ifdef HUNT_POINT_TEST
                testMaze.gridMap = history;
#endif
                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return Search_Get;
            }else{
                openList.emplace_back( Point(x_1,y_1));
                history[x_1][y_1] = 1;
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
            if (x_2 < 0 || x_2 >= x_max || y_2 < 0 || y_2 >= y_max){}
            else if(history[x_2][y_2] || p[x_2] == NavObsCell || p[x_2] == NavObsExpandCell || p[x_2] == NavSensorObsCell){}
            else if(p[x_2]==NavUnknowCell)    {
                target_point = t;
#ifdef HUNT_POINT_TEST
                testMaze.gridMap = history;
#endif
                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return Search_Get;
            }else{
                openList.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

            int x_3 = t.x + 1, y_3 = t.y + 0;
            p = maze.ptr<uchar>(y_3);
            if (x_3 < 0 || x_3 >= x_max || y_3 < 0 || y_3 >= y_max){}
            else if(history[x_3][y_3] || p[x_3] == NavObsCell || p[x_3] == NavObsExpandCell || p[x_3] == NavSensorObsCell){}
            else if(p[x_3]==NavUnknowCell)    {
                target_point = t;
#ifdef HUNT_POINT_TEST
                testMaze.gridMap = history;
#endif
                vector<vector<uint8_t >>().swap(history);
                list<Point>().swap(openList);
                return Search_Get;
            }else{
                openList.emplace_back( Point(x_3,y_3));
                history[x_3][y_3] = 1;
            }
        }
    }
#ifdef HUNT_POINT_TEST
    testMaze.gridMap = history;
#endif
    vector<vector<uint8_t >>().swap(history);
    list<Point>().swap(openList);

    return Search_Null;
}
/**
 * 统计图片中一定范围内和指定点具有相同像素值的个数和点
 * @param src 地图数据
 * @param target_point 目标点
 * @param p_min 角点(小端)
 * @param p_max 角点(大端)
 * @param num 数量
 * @param list_out 目标点
 * @return 1 失败 0 完成
 */
int Search::count_set_type(const Mat & src , Point target_point , Point p_min , Point p_max , int & num ,list<Point> & list_out){
    if(src.empty()){
        LOGPLANNING(INFO) << "count_set_type:图片为空";
        return 1;
    }
    if(target_point.x < p_min.x || target_point.y < p_min.y || target_point.x > p_max.x || target_point.y > p_max.y){
        LOGPLANNING(INFO) << "count_set_type:起点(" << target_point.x << ", " << target_point.y << ")超出地图范围";
        return 1;
    }
    uint8_t  target_type = src.at<uchar>(target_point.y,target_point.x);//目标像素值

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
            if (x_0 < p_min.x || x_0 > p_max.x || y_0 < p_min.y || y_0 > p_max.y || history[x_0][y_0]){}
            else if(src.at<uchar>(y_0,x_0) == target_type)    {
                openList.emplace_back( Point(x_0,y_0));
                list_out.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

//            int x_1 = t.x + 1, y_1 = t.y - 1;
//            if (x_1 < p_min.x || x_1 > p_max.x || y_1 < p_min.y || y_1 > p_max.y || history[x_1][y_1]){}
//            else if(src.at<uchar>(y_1,x_1) == target_type)    {
//                openList.emplace_back( Point(x_1,y_1));
//                list_out.emplace_back( Point(x_1,y_1));
//                history[x_1][y_1] = 1;
//            }

            int x_2 = t.x + 1, y_2 = t.y + 0;
            if (x_2 < p_min.x || x_2 > p_max.x || y_2 < p_min.y || y_2 > p_max.y || history[x_2][y_2]){}
            else if(src.at<uchar>(y_2,x_2) == target_type)    {
                openList.emplace_back( Point(x_2,y_2));
                list_out.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

//            int x_3 = t.x + 1, y_3 = t.y + 1;
//            if (x_3 < p_min.x || x_3 > p_max.x || y_3 < p_min.y || y_3 > p_max.y || history[x_3][y_3]){}
//            else if(src.at<uchar>(y_3,x_3) == target_type)    {
//                openList.emplace_back( Point(x_3,y_3));
//                list_out.emplace_back( Point(x_3,y_3));
//                history[x_3][y_3] = 1;
//            }

            int x_4 = t.x + 0, y_4 = t.y + 1;
            if (x_4 < p_min.x || x_4 > p_max.x || y_4 < p_min.y || y_4 > p_max.y || history[x_4][y_4]){}
            else if(src.at<uchar>(y_4,x_4) == target_type)    {
                openList.emplace_back( Point(x_4,y_4));
                list_out.emplace_back( Point(x_4,y_4));
                history[x_4][y_4] = 1;
            }

//            int x_5 = t.x - 1, y_5 = t.y + 1;
//            if (x_5 < p_min.x || x_5 > p_max.x || y_5 < p_min.y || y_5 > p_max.y || history[x_5][y_5]){}
//            else if(src.at<uchar>(y_5,x_5) == target_type)    {
//                openList.emplace_back( Point(x_5,y_5));
//                list_out.emplace_back( Point(x_5,y_5));
//                history[x_5][y_5] = 1;
//            }

            int x_6 = t.x - 1, y_6 = t.y + 0;
            if (x_6 < p_min.x || x_6 > p_max.x || y_6 < p_min.y || y_6 > p_max.y || history[x_6][y_6]){}
            else if(src.at<uchar>(y_6,x_6) == target_type)    {
                openList.emplace_back( Point(x_6,y_6));
                list_out.emplace_back( Point(x_6,y_6));
                history[x_6][y_6] = 1;
            }

//            int x_7 = t.x - 1, y_7 = t.y - 1;
//            if (x_7 < p_min.x || x_7 > p_max.x || y_7 < p_min.y || y_7 > p_max.y || history[x_7][y_7]){}
//            else if(src.at<uchar>(y_7,x_7) == target_type)    {
//                openList.emplace_back( Point(x_7,y_7));
//                list_out.emplace_back( Point(x_7,y_7));
//                history[x_7][y_7] = 1;
//            }
        }
        if(list_out.size() > num){
            break;
        }
    }
    num = (int)list_out.size();

    vector<vector<uint8_t >>().swap(history);
    std::list<Point>().swap(openList);
    return 0;
}
int Search::count_set_type(const Mat & src , Point target_point , Point p_min , Point p_max , list<Point> & list_out){
    if(src.empty()){
        LOGPLANNING(INFO) << "count_set_type:图片为空";
        return 1;
    }
    if(target_point.x < p_min.x || target_point.y < p_min.y || target_point.x > p_max.x || target_point.y > p_max.y){
        LOGPLANNING(INFO) << "count_set_type:起点(" << target_point.x << ", " << target_point.y << ")超出地图范围";
        return 1;
    }
    uint8_t  target_type = src.at<uchar>(target_point.y,target_point.x);//目标像素值

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
            if (x_0 < p_min.x || x_0 > p_max.x || y_0 < p_min.y || y_0 > p_max.y || history[x_0][y_0]){}
            else if(src.at<uchar>(y_0,x_0) == target_type)    {
                openList.emplace_back( Point(x_0,y_0));
                list_out.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

//            int x_1 = t.x + 1, y_1 = t.y - 1;
//            if (x_1 < p_min.x || x_1 > p_max.x || y_1 < p_min.y || y_1 > p_max.y || history[x_1][y_1]){}
//            else if(src.at<uchar>(y_1,x_1) == target_type)    {
//                openList.emplace_back( Point(x_1,y_1));
//                list_out.emplace_back( Point(x_1,y_1));
//                history[x_1][y_1] = 1;
//            }

            int x_2 = t.x + 1, y_2 = t.y + 0;
            if (x_2 < p_min.x || x_2 > p_max.x || y_2 < p_min.y || y_2 > p_max.y || history[x_2][y_2]){}
            else if(src.at<uchar>(y_2,x_2) == target_type)    {
                openList.emplace_back( Point(x_2,y_2));
                list_out.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

//            int x_3 = t.x + 1, y_3 = t.y + 1;
//            if (x_3 < p_min.x || x_3 > p_max.x || y_3 < p_min.y || y_3 > p_max.y || history[x_3][y_3]){}
//            else if(src.at<uchar>(y_3,x_3) == target_type)    {
//                openList.emplace_back( Point(x_3,y_3));
//                list_out.emplace_back( Point(x_3,y_3));
//                history[x_3][y_3] = 1;
//            }

            int x_4 = t.x + 0, y_4 = t.y + 1;
            if (x_4 < p_min.x || x_4 > p_max.x || y_4 < p_min.y || y_4 > p_max.y || history[x_4][y_4]){}
            else if(src.at<uchar>(y_4,x_4) == target_type)    {
                openList.emplace_back( Point(x_4,y_4));
                list_out.emplace_back( Point(x_4,y_4));
                history[x_4][y_4] = 1;
            }

//            int x_5 = t.x - 1, y_5 = t.y + 1;
//            if (x_5 < p_min.x || x_5 > p_max.x || y_5 < p_min.y || y_5 > p_max.y || history[x_5][y_5]){}
//            else if(src.at<uchar>(y_5,x_5) == target_type)    {
//                openList.emplace_back( Point(x_5,y_5));
//                list_out.emplace_back( Point(x_5,y_5));
//                history[x_5][y_5] = 1;
//            }

            int x_6 = t.x - 1, y_6 = t.y + 0;
            if (x_6 < p_min.x || x_6 > p_max.x || y_6 < p_min.y || y_6 > p_max.y || history[x_6][y_6]){}
            else if(src.at<uchar>(y_6,x_6) == target_type)    {
                openList.emplace_back( Point(x_6,y_6));
                list_out.emplace_back( Point(x_6,y_6));
                history[x_6][y_6] = 1;
            }

//            int x_7 = t.x - 1, y_7 = t.y - 1;
//            if (x_7 < p_min.x || x_7 > p_max.x || y_7 < p_min.y || y_7 > p_max.y || history[x_7][y_7]){}
//            else if(src.at<uchar>(y_7,x_7) == target_type)    {
//                openList.emplace_back( Point(x_7,y_7));
//                list_out.emplace_back( Point(x_7,y_7));
//                history[x_7][y_7] = 1;
//            }
        }
    }

    history.clear();
    vector<vector<uint8_t >>().swap(history);
    openList.clear();
    std::list<Point>().swap(openList);
    return 0;
}
int Search::count_set_type_0(const Mat & src , const Mat & mark , Point target_point , int & num ,list<Point> & list_out){
    if(src.empty() || mark.empty()){
        LOGPLANNING(INFO) << "count_set_type:图片为空";
        return 1;
    }
    if(target_point.x < 0 || target_point.y < 0 || target_point.x >= src.cols || target_point.y >= src.rows){
        LOGPLANNING(INFO) << "count_set_type:起点(" << target_point.x << ", " << target_point.y << ")超出地图范围";
        return 1;
    }
    uint8_t  target_type = src.at<uchar>(target_point.y,target_point.x);//目标像素值
    uint8_t  mark_type = mark.at<uchar>(target_point.y,target_point.x);

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
            if ((mark.at<uchar>(y_0,x_0) != mark_type && mark.at<uchar>(y_0,x_0) != RegionUnknowCell) || history[x_0][y_0]){}
            else if(src.at<uchar>(y_0,x_0) == target_type)    {
                openList.emplace_back( Point(x_0,y_0));
                list_out.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

//            int x_1 = t.x + 1, y_1 = t.y - 1;
//            if (x_1 < p_min.x || x_1 > p_max.x || y_1 < p_min.y || y_1 > p_max.y || history[x_1][y_1]){}
//            else if(src.at<uchar>(y_1,x_1) == target_type)    {
//                openList.emplace_back( Point(x_1,y_1));
//                list_out.emplace_back( Point(x_1,y_1));
//                history[x_1][y_1] = 1;
//            }

            int x_2 = t.x + 1, y_2 = t.y + 0;
            if ((mark.at<uchar>(y_2,x_2) != mark_type && mark.at<uchar>(y_2,x_2) != RegionUnknowCell)|| history[x_2][y_2]){}
            else if(src.at<uchar>(y_2,x_2) == target_type)    {
                openList.emplace_back( Point(x_2,y_2));
                list_out.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

//            int x_3 = t.x + 1, y_3 = t.y + 1;
//            if (x_3 < p_min.x || x_3 > p_max.x || y_3 < p_min.y || y_3 > p_max.y || history[x_3][y_3]){}
//            else if(src.at<uchar>(y_3,x_3) == target_type)    {
//                openList.emplace_back( Point(x_3,y_3));
//                list_out.emplace_back( Point(x_3,y_3));
//                history[x_3][y_3] = 1;
//            }

            int x_4 = t.x + 0, y_4 = t.y + 1;
            if ((mark.at<uchar>(y_4,x_4) != mark_type && mark.at<uchar>(y_4,x_4) != RegionUnknowCell) || history[x_4][y_4]){}
            else if(src.at<uchar>(y_4,x_4) == target_type)    {
                openList.emplace_back( Point(x_4,y_4));
                list_out.emplace_back( Point(x_4,y_4));
                history[x_4][y_4] = 1;
            }

//            int x_5 = t.x - 1, y_5 = t.y + 1;
//            if (x_5 < p_min.x || x_5 > p_max.x || y_5 < p_min.y || y_5 > p_max.y || history[x_5][y_5]){}
//            else if(src.at<uchar>(y_5,x_5) == target_type)    {
//                openList.emplace_back( Point(x_5,y_5));
//                list_out.emplace_back( Point(x_5,y_5));
//                history[x_5][y_5] = 1;
//            }

            int x_6 = t.x - 1, y_6 = t.y + 0;
            if ((mark.at<uchar>(y_6,x_6) != mark_type && mark.at<uchar>(y_6,x_6) != RegionUnknowCell) || history[x_6][y_6]){}
            else if(src.at<uchar>(y_6,x_6) == target_type)    {
                openList.emplace_back( Point(x_6,y_6));
                list_out.emplace_back( Point(x_6,y_6));
                history[x_6][y_6] = 1;
            }

//            int x_7 = t.x - 1, y_7 = t.y - 1;
//            if (x_7 < p_min.x || x_7 > p_max.x || y_7 < p_min.y || y_7 > p_max.y || history[x_7][y_7]){}
//            else if(src.at<uchar>(y_7,x_7) == target_type)    {
//                openList.emplace_back( Point(x_7,y_7));
//                list_out.emplace_back( Point(x_7,y_7));
//                history[x_7][y_7] = 1;
//            }
        }
        if(list_out.size() > num){
            break;
        }
    }
    num = (int)list_out.size();

    vector<vector<uint8_t >>().swap(history);
    std::list<Point>().swap(openList);
    return 0;
}
int Search::count_set_type_1(const Mat & src , const Mat & mark , Point target_point , int & num ,list<Point> & list_out){
    if(src.empty() || mark.empty()){
        LOGPLANNING(INFO) << "count_set_type:图片为空";
        return 1;
    }
    if(target_point.x < 0 || target_point.y < 0 || target_point.x >= src.cols || target_point.y >= src.rows){
        LOGPLANNING(INFO) << "count_set_type:起点(" << target_point.x << ", " << target_point.y << ")超出地图范围";
        return 1;
    }
    uint8_t  target_type = src.at<uchar>(target_point.y,target_point.x);//目标像素值
    uint8_t  mark_type = mark.at<uchar>(target_point.y,target_point.x);

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
            if ((mark.at<uchar>(y_0,x_0) != mark_type) || history[x_0][y_0]){}
            else if(src.at<uchar>(y_0,x_0) == target_type)    {
                openList.emplace_back( Point(x_0,y_0));
                list_out.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

//            int x_1 = t.x + 1, y_1 = t.y - 1;
//            if (x_1 < p_min.x || x_1 > p_max.x || y_1 < p_min.y || y_1 > p_max.y || history[x_1][y_1]){}
//            else if(src.at<uchar>(y_1,x_1) == target_type)    {
//                openList.emplace_back( Point(x_1,y_1));
//                list_out.emplace_back( Point(x_1,y_1));
//                history[x_1][y_1] = 1;
//            }

            int x_2 = t.x + 1, y_2 = t.y + 0;
            if ((mark.at<uchar>(y_2,x_2) != mark_type)|| history[x_2][y_2]){}
            else if(src.at<uchar>(y_2,x_2) == target_type)    {
                openList.emplace_back( Point(x_2,y_2));
                list_out.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

//            int x_3 = t.x + 1, y_3 = t.y + 1;
//            if (x_3 < p_min.x || x_3 > p_max.x || y_3 < p_min.y || y_3 > p_max.y || history[x_3][y_3]){}
//            else if(src.at<uchar>(y_3,x_3) == target_type)    {
//                openList.emplace_back( Point(x_3,y_3));
//                list_out.emplace_back( Point(x_3,y_3));
//                history[x_3][y_3] = 1;
//            }

            int x_4 = t.x + 0, y_4 = t.y + 1;
            if ((mark.at<uchar>(y_4,x_4) != mark_type) || history[x_4][y_4]){}
            else if(src.at<uchar>(y_4,x_4) == target_type)    {
                openList.emplace_back( Point(x_4,y_4));
                list_out.emplace_back( Point(x_4,y_4));
                history[x_4][y_4] = 1;
            }

//            int x_5 = t.x - 1, y_5 = t.y + 1;
//            if (x_5 < p_min.x || x_5 > p_max.x || y_5 < p_min.y || y_5 > p_max.y || history[x_5][y_5]){}
//            else if(src.at<uchar>(y_5,x_5) == target_type)    {
//                openList.emplace_back( Point(x_5,y_5));
//                list_out.emplace_back( Point(x_5,y_5));
//                history[x_5][y_5] = 1;
//            }

            int x_6 = t.x - 1, y_6 = t.y + 0;
            if ((mark.at<uchar>(y_6,x_6) != mark_type) || history[x_6][y_6]){}
            else if(src.at<uchar>(y_6,x_6) == target_type)    {
                openList.emplace_back( Point(x_6,y_6));
                list_out.emplace_back( Point(x_6,y_6));
                history[x_6][y_6] = 1;
            }

//            int x_7 = t.x - 1, y_7 = t.y - 1;
//            if (x_7 < p_min.x || x_7 > p_max.x || y_7 < p_min.y || y_7 > p_max.y || history[x_7][y_7]){}
//            else if(src.at<uchar>(y_7,x_7) == target_type)    {
//                openList.emplace_back( Point(x_7,y_7));
//                list_out.emplace_back( Point(x_7,y_7));
//                history[x_7][y_7] = 1;
//            }
        }
        if(list_out.size() > num){
            break;
        }
    }
    num = (int)list_out.size();

    vector<vector<uint8_t >>().swap(history);
    std::list<Point>().swap(openList);
    return 0;
}
int Search::count_set_type_2(const Mat & src , Point target_point , uint8_t type , Point p_min , Point p_max , list<Point> & list_out){
    if(src.empty()){
        LOGPLANNING(INFO) << "count_set_type:图片为空";
        return 1;
    }
    if(target_point.x < p_min.x || target_point.y < p_min.y || target_point.x > p_max.x || target_point.y > p_max.y){
        LOGPLANNING(INFO) << "count_set_type:起点(" << target_point.x << ", " << target_point.y << ")超出地图范围";
        return 1;
    }
    uint8_t  target_type = src.at<uchar>(target_point.y,target_point.x);//目标像素值

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
            if (x_0 < p_min.x || x_0 > p_max.x || y_0 < p_min.y || y_0 > p_max.y || history[x_0][y_0]){}
            else if(src.at<uchar>(y_0,x_0) == target_type)    {
                openList.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }else if(src.at<uchar>(y_0,x_0) == type)    {
                list_out.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }

            int x_2 = t.x + 1, y_2 = t.y + 0;
            if (x_2 < p_min.x || x_2 > p_max.x || y_2 < p_min.y || y_2 > p_max.y || history[x_2][y_2]){}
            else if(src.at<uchar>(y_2,x_2) == target_type)    {
                openList.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }else if(src.at<uchar>(y_2,x_2) == type)    {
                list_out.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }

            int x_4 = t.x + 0, y_4 = t.y + 1;
            if (x_4 < p_min.x || x_4 > p_max.x || y_4 < p_min.y || y_4 > p_max.y || history[x_4][y_4]){}
            else if(src.at<uchar>(y_4,x_4) == target_type)    {
                openList.emplace_back( Point(x_4,y_4));
                history[x_4][y_4] = 1;
            }else if(src.at<uchar>(y_4,x_4) == type)    {
                list_out.emplace_back( Point(x_4,y_4));
                history[x_4][y_4] = 1;
            }

            int x_6 = t.x - 1, y_6 = t.y + 0;
            if (x_6 < p_min.x || x_6 > p_max.x || y_6 < p_min.y || y_6 > p_max.y || history[x_6][y_6]){}
            else if(src.at<uchar>(y_6,x_6) == target_type)    {
                openList.emplace_back( Point(x_6,y_6));
                history[x_6][y_6] = 1;
            }else if(src.at<uchar>(y_6,x_6) == type)    {
                list_out.emplace_back( Point(x_6,y_6));
                history[x_6][y_6] = 1;
            }

        }
    }

    vector<vector<uint8_t >>().swap(history);
    std::list<Point>().swap(openList);
    return 0;
}

/**
 * 搜索
 * @param maze 地图数据
 * @param startPoint 起始点
 * @param target_point 目标点
 * @param box.point_min 角点(小端)
 * @param box.point_max 角点(大端)
 * @return 0 未找到  1 找到外部点 2 找到内部点
 */
int Search::Search_box_point(const Mat & maze , CleanBox * boxinfo , Point startPoint , Point &target_point , vector<Point> & vpignore){
#if 1
    if(maze.empty()){
        LOGPLANNING(INFO) << "Hunt_point 输入的地图数据为空.";
        return SiggetCleanStart::FindUncleanPoint_Null;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x >= maze.cols || startPoint.y >= maze.rows){
        LOGPLANNING(INFO) << "Hunt_point_mat: 输入的起点超出地图范围(" << startPoint.x << ", " << startPoint.y << ")";
        return SiggetCleanStart::FindUncleanPoint_Null;
    }
    if(!vpignore.empty()){
        vpignore.clear();
    }
    vector<vector<uint8_t >> history((unsigned long)maze.cols, vector<uint8_t >((unsigned long)maze.rows));

    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    history[startPoint.x][startPoint.y] = 1;

    int find_outside_point = 0;//找到指定边界上的点后把该值置1,缩小搜索范围

    Point temp_p;

    BoxInfo box = boxinfo->GetCurBox();
    BoxInfo next_box_info = box;
    LOGPLANNING(INFO) << "search startPoint(" << startPoint.x << ", " << startPoint.y << ")";
    LOGPLANNING(INFO) << "search curbox (" << box.point_min.x << ", " << box.point_min.y << ")-(" << box.point_max.x << ", " << box.point_max.y << ")";
    list<Point> list_out;
    while (!openList.empty())
    {
        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            history[t.x][t.y] = 1;

            int x_3 = t.x + 1, y_3 = t.y + 0;
            auto * p = maze.ptr<uchar>(y_3);
//            if ((x_3 < 0 || x_3 >= maze.cols || y_3 < 0 || y_3 >= maze.rows) || history[x_3][y_3] || p[x_3] == SearchUnknowCell){}
            if (history[x_3][y_3] || p[x_3] == SearchUnknowCell){}
            else if(p[x_3] == SearchCovCell || x_3 == box.point_min.x || x_3 == box.point_max.x || y_3 == box.point_min.y || y_3 == box.point_max.y){
                openList.emplace_back( Point(x_3,y_3));
                history[x_3][y_3] = 1;
            }else if(x_3 < box.point_min.x || x_3 > box.point_max.x || y_3 < box.point_min.y || y_3 > box.point_max.y)    {
                if(find_outside_point == 0){
                    target_point.x = x_3;
                    target_point.y = y_3;
                    int num = num_hunt_global_min;
                    list<Point>().swap(list_out);
                    boxinfo->GetPointInBox(target_point , next_box_info);
                    int flg = count_set_type(maze,target_point,next_box_info.point_min,next_box_info.point_max,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            temp_p = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该框[(" << next_box_info.point_min.x << ", " << next_box_info.point_min.y << ")-(" << next_box_info.point_max.x << ", " << next_box_info.point_max.y << ")]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }
                else{
                    openList.emplace_back( Point(x_3,y_3));
                    history[x_3][y_3] = 1;
                }
            }else {
                target_point.x = x_3;
                target_point.y = y_3;
                int num = num_hunt_local_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return SiggetCleanStart::FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }


            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
//            if ((x_1 < 0 || x_1 >= maze.cols || y_1 < 0 || y_1 >= maze.rows) || history[x_1][y_1] || p[x_1] == SearchUnknowCell){}
            if (history[x_1][y_1] || p[x_1] == SearchUnknowCell){}
            else if(p[x_1] == SearchCovCell || x_1 == box.point_min.x || x_1 == box.point_max.x || y_1 == box.point_min.y || y_1 == box.point_max.y){
                openList.emplace_back( Point(x_1,y_1));
                history[x_1][y_1] = 1;
            }else if(x_1 < box.point_min.x || x_1 > box.point_max.x || y_1 < box.point_min.y || y_1 > box.point_max.y)    {
                if(find_outside_point == 0) {
                    target_point.x = x_1;
                    target_point.y = y_1;
                    int num = num_hunt_global_min;
                    list<Point>().swap(list_out);
                    boxinfo->GetPointInBox(target_point, next_box_info);
                    int flg = count_set_type(maze, target_point, next_box_info.point_min, next_box_info.point_max, num, list_out);
                    if (flg == 0) {
                        if (num > num_hunt_global_min) {
                            temp_p = target_point;//找到了区域外的点,记录下来
                            find_outside_point = 1;
                        } else {
                            LOGPLANNING(INFO) << "该框[(" << next_box_info.point_min.x << ", " << next_box_info.point_min.y << ")-(" << next_box_info.point_max.x << ", " << next_box_info.point_max.y << ")]可清扫区域太少:" << num;
                            while (!list_out.empty()) {
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }
                else{
                    openList.emplace_back( Point(x_1,y_1));
                    history[x_1][y_1] = 1;
                }
            }else {
                target_point.x = x_1;
                target_point.y = y_1;
                int num = num_hunt_local_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return SiggetCleanStart::FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

            int x_0 = t.x + 0, y_0 = t.y - 1;
            p = maze.ptr<uchar>(y_0);
//            if ((x_0 < 0 || x_0 >= maze.cols || y_0 < 0 || y_0 >= maze.rows) || history[x_0][y_0] || p[x_0] == SearchUnknowCell){}
            if (history[x_0][y_0] || p[x_0] == SearchUnknowCell){}
            else if(p[x_0] == SearchCovCell || x_0 == box.point_min.x || x_0 == box.point_max.x || y_0 == box.point_min.y || y_0 == box.point_max.y){
                openList.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }else if(x_0 < box.point_min.x || x_0 > box.point_max.x || y_0 < box.point_min.y || y_0 > box.point_max.y)    {
                if(find_outside_point == 0){
                    target_point.x = x_0;
                    target_point.y = y_0;
                    int num = num_hunt_global_min;
                    list<Point>().swap(list_out);
                    boxinfo->GetPointInBox(target_point , next_box_info);
                    int flg = count_set_type(maze,target_point,next_box_info.point_min,next_box_info.point_max,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            temp_p = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该框[(" << next_box_info.point_min.x << ", " << next_box_info.point_min.y << ")-(" << next_box_info.point_max.x << ", " << next_box_info.point_max.y << ")]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }
                else{
                    openList.emplace_back( Point(x_0,y_0));
                    history[x_0][y_0] = 1;
                }
            }else {
                target_point.x = x_0;
                target_point.y = y_0;
                int num = num_hunt_local_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return SiggetCleanStart::FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
//            if ((x_2 < 0 || x_2 >= maze.cols || y_2 < 0 || y_2 >= maze.rows) || history[x_2][y_2] || p[x_2] == SearchUnknowCell){}
            if (history[x_2][y_2] || p[x_2] == SearchUnknowCell){}
            else if(p[x_2] == SearchCovCell || x_2 == box.point_min.x || x_2 == box.point_max.x || y_2 == box.point_min.y || y_2 == box.point_max.y){
                openList.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }else if(x_2 < box.point_min.x || x_2 > box.point_max.x || y_2 < box.point_min.y || y_2 > box.point_max.y)    {
                if(find_outside_point == 0){
                    target_point.x = x_2;
                    target_point.y = y_2;
                    int num = num_hunt_global_min;
                    list<Point>().swap(list_out);
                    boxinfo->GetPointInBox(target_point , next_box_info);
                    int flg = count_set_type(maze,target_point,next_box_info.point_min,next_box_info.point_max,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            temp_p = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该框[(" << next_box_info.point_min.x << ", " << next_box_info.point_min.y << ")-(" << next_box_info.point_max.x << ", " << next_box_info.point_max.y << ")]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }
                else{
                    openList.emplace_back( Point(x_2,y_2));
                    history[x_2][y_2] = 1;
                }
            }else {
                target_point.x = x_2;
                target_point.y = y_2;
                int num = num_hunt_local_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return SiggetCleanStart::FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

#if 0 //八邻域
            int x_4 = t.x + 1, y_4 = t.y + 1;
            p = maze.ptr<uchar>(y_4);
            if ((x_4 < 0 || x_4 >= maze.cols || y_4 < 0 || y_4 >= maze.rows) || history[x_4][y_4] || p[x_4] == SearchUnknowCell){}
            else if(p[x_4] == SearchCovCell){
                openList.emplace_back( Point(x_4,y_4));
                history[x_4][y_4] = 1;
            }else if(x_4 < box.point_min.x || x_4 > box.point_max.x || y_4 < box.point_min.y || y_4 > box.point_max.y)    {
                if(find_outside_point == 0){
                    target_point.x = x_4;
                    target_point.y = y_4;
                    int num = num_hunt_global_min;
                    list<Point>().swap(list_out);
                    boxinfo->GetPointInBox(target_point , next_box_info);
                    int flg = count_set_type(maze,target_point,next_box_info.point_min,next_box_info.point_max,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            temp_p = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该框[(" << next_box_info.point_min.x << ", " << next_box_info.point_min.y << ")-(" << next_box_info.point_max.x << ", " << next_box_info.point_max.y << ")]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }else{
                    openList.emplace_back( Point(x_4,y_4));
                    history[x_4][y_4] = 1;
                }
            }else {
                target_point.x = x_4;
                target_point.y = y_4;
                int num = num_hunt_local_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }
            int x_5 = t.x + 1, y_5 = t.y - 1;
            p = maze.ptr<uchar>(y_5);
            if ((x_5 < 0 || x_5 >= maze.cols || y_5 < 0 || y_5 >= maze.rows) || history[x_5][y_5] || p[x_5] == SearchUnknowCell){}
            else if(p[x_5] == SearchCovCell){
                openList.emplace_back( Point(x_5,y_5));
                history[x_5][y_5] = 1;
            }else if(x_5 < box.point_min.x || x_5 > box.point_max.x || y_5 < box.point_min.y || y_5 > box.point_max.y)    {
                if(find_outside_point == 0){
                    target_point.x = x_5;
                    target_point.y = y_5;
                    int num = num_hunt_global_min;
                    list<Point>().swap(list_out);
                    boxinfo->GetPointInBox(target_point , next_box_info);
                    int flg = count_set_type(maze,target_point,next_box_info.point_min,next_box_info.point_max,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            temp_p = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该框[(" << next_box_info.point_min.x << ", " << next_box_info.point_min.y << ")-(" << next_box_info.point_max.x << ", " << next_box_info.point_max.y << ")]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }else{
                    openList.emplace_back( Point(x_5,y_5));
                    history[x_5][y_5] = 1;
                }
            }else {
                target_point.x = x_5;
                target_point.y = y_5;
                int num = num_hunt_local_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }
            int x_6 = t.x - 1, y_6 = t.y + 1;
            p = maze.ptr<uchar>(y_6);
            if ((x_6 < 0 || x_6 >= maze.cols || y_6 < 0 || y_6 >= maze.rows) || history[x_6][y_6] || p[x_6] == SearchUnknowCell){}
            else if(p[x_6] == SearchCovCell){
                openList.emplace_back( Point(x_6,y_6));
                history[x_6][y_6] = 1;
            }else if(x_6 < box.point_min.x || x_6 > box.point_max.x || y_6 < box.point_min.y || y_6 > box.point_max.y)    {
                if(find_outside_point == 0){
                    target_point.x = x_6;
                    target_point.y = y_6;
                    int num = num_hunt_global_min;
                    list<Point>().swap(list_out);
                    boxinfo->GetPointInBox(target_point , next_box_info);
                    int flg = count_set_type(maze,target_point,next_box_info.point_min,next_box_info.point_max,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            temp_p = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该框[(" << next_box_info.point_min.x << ", " << next_box_info.point_min.y << ")-(" << next_box_info.point_max.x << ", " << next_box_info.point_max.y << ")]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }else{
                    openList.emplace_back( Point(x_6,y_6));
                    history[x_6][y_6] = 1;
                }
            }else {
                target_point.x = x_6;
                target_point.y = y_6;
                int num = num_hunt_local_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }
            int x_7 = t.x - 1, y_7 = t.y - 1;
            p = maze.ptr<uchar>(y_7);
            if ((x_7 < 0 || x_7 >= maze.cols || y_7 < 0 || y_7 >= maze.rows) || history[x_7][y_7] || p[x_7] == SearchUnknowCell){}
            else if(p[x_7] == SearchCovCell){
                openList.emplace_back( Point(x_7,y_7));
                history[x_7][y_7] = 1;
            }else if(x_7 < box.point_min.x || x_7 > box.point_max.x || y_7 < box.point_min.y || y_7 > box.point_max.y)    {
                if(find_outside_point == 0){
                    target_point.x = x_7;
                    target_point.y = y_7;
                    int num = num_hunt_global_min;
                    list<Point>().swap(list_out);
                    boxinfo->GetPointInBox(target_point , next_box_info);
                    int flg = count_set_type(maze,target_point,next_box_info.point_min,next_box_info.point_max,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            temp_p = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该框[(" << next_box_info.point_min.x << ", " << next_box_info.point_min.y << ")-(" << next_box_info.point_max.x << ", " << next_box_info.point_max.y << ")]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }else{
                    openList.emplace_back( Point(x_7,y_7));
                    history[x_7][y_7] = 1;
                }
            }else {
                target_point.x = x_7;
                target_point.y = y_7;
                int num = num_hunt_local_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }
#endif

        }
    }
    list_out.clear();
    list<Point>().swap(list_out);
    vector<vector<uint8_t >>().swap(history);
    list<Point>().swap(openList);

    if(find_outside_point){
        target_point = temp_p;
//        boxinfo->SetCurBox(next_box_info);
        return SiggetCleanStart::FindUncleanPoint_Out;
    }
#endif
    return SiggetCleanStart::FindUncleanPoint_Null;
}
int Search::Search_box_point_remainder(const Mat & maze , CleanBox * boxinfo , Point startPoint , Point &target_point , vector<Point> & vpignore){
    if(maze.empty()){
        LOGPLANNING(INFO) << "Search_box_point_remainder 输入的地图数据为空.";
        return FindUncleanPoint_Null;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x >= maze.cols || startPoint.y >= maze.rows){
        LOGPLANNING(INFO) << "Search_box_point_remainder: 输入的起点超出地图范围(" << startPoint.x << ", " << startPoint.y << ")";
        return FindUncleanPoint_Null;
    }
    if(!vpignore.empty()){
        vpignore.clear();
    }

    int num_min = num_hunt_local_min/2;
    int num = 0;

    vector<vector<uint8_t >> history((unsigned long)maze.cols, vector<uint8_t >((unsigned long)maze.rows));

    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    history[startPoint.x][startPoint.y] = 1;

    BoxInfo box = boxinfo->GetCurBox();
    LOGPLANNING(INFO) << "Search_box_point_remainder startPoint(" << startPoint.x << ", " << startPoint.y << ")";
    LOGPLANNING(INFO) << "Search_box_point_remainder curbox (" << box.point_min.x << ", " << box.point_min.y << ")-(" << box.point_max.x << ", " << box.point_max.y << ")";
    list<Point> list_out;
    while (!openList.empty())
    {
        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            history[t.x][t.y] = 1;

            int x_3 = t.x + 1, y_3 = t.y + 0;
            auto * p = maze.ptr<uchar>(y_3);
            if (history[x_3][y_3] || p[x_3] == SearchUnknowCell){}
            else if(p[x_3] == SearchCovCell || x_3 == box.point_min.x || x_3 == box.point_max.x || y_3 == box.point_min.y || y_3 == box.point_max.y){
                openList.emplace_back( Point(x_3,y_3));
                history[x_3][y_3] = 1;
            }else if(x_3 < box.point_min.x || x_3 > box.point_max.x || y_3 < box.point_min.y || y_3 > box.point_max.y)    {
            }else {
                target_point.x = x_3;
                target_point.y = y_3;
                num = num_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
            if (history[x_1][y_1] || p[x_1] == SearchUnknowCell){}
            else if(p[x_1] == SearchCovCell || x_1 == box.point_min.x || x_1 == box.point_max.x || y_1 == box.point_min.y || y_1 == box.point_max.y){
                openList.emplace_back( Point(x_1,y_1));
                history[x_1][y_1] = 1;
            }else if(x_1 < box.point_min.x || x_1 > box.point_max.x || y_1 < box.point_min.y || y_1 > box.point_max.y)    {
            }else {
                target_point.x = x_1;
                target_point.y = y_1;
                num = num_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

            int x_0 = t.x + 0, y_0 = t.y - 1;
            p = maze.ptr<uchar>(y_0);
            if (history[x_0][y_0] || p[x_0] == SearchUnknowCell){}
            else if(p[x_0] == SearchCovCell || x_0 == box.point_min.x || x_0 == box.point_max.x || y_0 == box.point_min.y || y_0 == box.point_max.y){
                openList.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }else if(x_0 < box.point_min.x || x_0 > box.point_max.x || y_0 < box.point_min.y || y_0 > box.point_max.y)    {
            }else {
                target_point.x = x_0;
                target_point.y = y_0;
                num = num_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
            if (history[x_2][y_2] || p[x_2] == SearchUnknowCell){}
            else if(p[x_2] == SearchCovCell || x_2 == box.point_min.x || x_2 == box.point_max.x || y_2 == box.point_min.y || y_2 == box.point_max.y){
                openList.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }else if(x_2 < box.point_min.x || x_2 > box.point_max.x || y_2 < box.point_min.y || y_2 > box.point_max.y)    {
            }else {
                target_point.x = x_2;
                target_point.y = y_2;
                num = num_min;
                list<Point>().swap(list_out);
                int flg = count_set_type(maze,target_point,box.point_min,box.point_max,num,list_out);
                if(flg == 0){
                    if(num > num_min){
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return FindUncleanPoint_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }
        }
    }
    list_out.clear();
    list<Point>().swap(list_out);
    vector<vector<uint8_t >>().swap(history);
    list<Point>().swap(openList);

    return FindUncleanPoint_Null;
}
int Search::Search_mark_point(const Mat & maze , const Mat & dividemap , CleanBox * boxinfo , Point startPoint , Point &target_point , vector<Point> & vpignore){
#if 1
    if(maze.empty() || dividemap.empty()){
        LOGPLANNING(INFO) << "Hunt_point 输入的地图数据为空.";
        return SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x >= maze.cols || startPoint.y >= maze.rows){
        LOGPLANNING(INFO) << "Hunt_point_mat: 输入的起点超出地图范围(" << startPoint.x << ", " << startPoint.y << ")";
        return SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;
    }
    if(!vpignore.empty()){
        vpignore.clear();
    }
    vector<vector<uint8_t >> history((unsigned long)maze.cols, vector<uint8_t >((unsigned long)maze.rows));

    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    history[startPoint.x][startPoint.y] = 1;

    int find_outside_point = 0;//找到指定边界上的点后把该值置1,缩小搜索范围

    Point p_next;

    BoxInfo box = boxinfo->GetCurDelimit();
    LOGPLANNING(INFO) << "search startPoint(" << startPoint.x << ", " << startPoint.y << ")";
    LOGPLANNING(INFO) << "search curbox id: " << box.id;
    list<Point> list_out;
    while (!openList.empty())
    {
        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            history[t.x][t.y] = 1;

            int x_3 = t.x + 1, y_3 = t.y + 0;
            auto * p = maze.ptr<uchar>(y_3);
//            if ((x_3 < 0 || x_3 >= maze.cols || y_3 < 0 || y_3 >= maze.rows) || history[x_3][y_3] || p[x_3] == SearchUnknowCell){}
            if (history[x_3][y_3] || p[x_3] == SearchUnknowCell){}
            else if(p[x_3] == SearchCovCell || dividemap.at<uint8_t >(y_3,x_3) == RegionUnknowCell){
                openList.emplace_back( Point(x_3,y_3));
                history[x_3][y_3] = 1;
            }else if(dividemap.at<uint8_t >(y_3,x_3) != box.id){
                if(find_outside_point == 0){
                    target_point.x = x_3;
                    target_point.y = y_3;
                    int num = num_hunt_global_min;
                    list_out.clear();
                    list<Point>().swap(list_out);
                    int flg = count_set_type_1(maze,dividemap,target_point,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            list_out.clear();
                            list<Point>().swap(list_out);
                            p_next = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该区域[id:" << dividemap.at<uint8_t >(target_point) << "]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }
//                else{
//                    openList.emplace_back( Point(x_3,y_3));
//                    history[x_3][y_3] = 1;
//                }
            }else {
                target_point.x = x_3;
                target_point.y = y_3;
                int num = num_hunt_local_min;
                list_out.clear();
                list<Point>().swap(list_out);
                int flg = count_set_type_1(maze,dividemap,target_point,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list_out.clear();
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
//            if ((x_1 < 0 || x_1 >= maze.cols || y_1 < 0 || y_1 >= maze.rows) || history[x_1][y_1] || p[x_1] == SearchUnknowCell){}
            if (history[x_1][y_1] || p[x_1] == SearchUnknowCell){}
            else if(p[x_1] == SearchCovCell || dividemap.at<uint8_t >(y_1,x_1) == RegionUnknowCell){
                openList.emplace_back( Point(x_1,y_1));
                history[x_1][y_1] = 1;
            }else if(dividemap.at<uint8_t >(y_1,x_1) != box.id){
                if(find_outside_point == 0){
                    target_point.x = x_1;
                    target_point.y = y_1;
                    int num = num_hunt_global_min;
                    list_out.clear();
                    list<Point>().swap(list_out);
                    int flg = count_set_type_1(maze,dividemap,target_point,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            list_out.clear();
                            list<Point>().swap(list_out);
                            p_next = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该区域[id:" << dividemap.at<uint8_t >(target_point) << "]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }
//                else{
//                    openList.emplace_back( Point(x_1,y_1));
//                    history[x_1][y_1] = 1;
//                }
            }else {
                target_point.x = x_1;
                target_point.y = y_1;
                int num = num_hunt_local_min;
                list_out.clear();
                list<Point>().swap(list_out);
                int flg = count_set_type_1(maze,dividemap,target_point,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list_out.clear();
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

            int x_0 = t.x + 0, y_0 = t.y - 1;
            p = maze.ptr<uchar>(y_0);
//            if ((x_0 < 0 || x_0 >= maze.cols || y_0 < 0 || y_0 >= maze.rows) || history[x_0][y_0] || p[x_0] == SearchUnknowCell){}
            if (history[x_0][y_0] || p[x_0] == SearchUnknowCell){}
            else if(p[x_0] == SearchCovCell || dividemap.at<uint8_t >(y_0,x_0) == RegionUnknowCell){
                openList.emplace_back( Point(x_0,y_0));
                history[x_0][y_0] = 1;
            }else if(dividemap.at<uint8_t >(y_0,x_0) != box.id){
                if(find_outside_point == 0){
                    target_point.x = x_0;
                    target_point.y = y_0;
                    int num = num_hunt_global_min;
                    list_out.clear();
                    list<Point>().swap(list_out);
                    int flg = count_set_type_1(maze,dividemap,target_point,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            list_out.clear();
                            list<Point>().swap(list_out);
                            p_next = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该区域[id:" << dividemap.at<uint8_t >(target_point) << "]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }
//                else{
//                    openList.emplace_back( Point(x_0,y_0));
//                    history[x_0][y_0] = 1;
//                }
            }else {
                target_point.x = x_0;
                target_point.y = y_0;
                int num = num_hunt_local_min;
                list_out.clear();
                list<Point>().swap(list_out);
                int flg = count_set_type_1(maze,dividemap,target_point,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list_out.clear();
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
//            if ((x_2 < 0 || x_2 >= maze.cols || y_2 < 0 || y_2 >= maze.rows) || history[x_2][y_2] || p[x_2] == SearchUnknowCell){}
            if (history[x_2][y_2] || p[x_2] == SearchUnknowCell){}
            else if(p[x_2] == SearchCovCell || dividemap.at<uint8_t >(y_2,x_2) == RegionUnknowCell){
                openList.emplace_back( Point(x_2,y_2));
                history[x_2][y_2] = 1;
            }else if(dividemap.at<uint8_t >(y_2,x_2) != box.id){
                if(find_outside_point == 0){
                    target_point.x = x_2;
                    target_point.y = y_2;
                    int num = num_hunt_global_min;
                    list_out.clear();
                    list<Point>().swap(list_out);
                    int flg = count_set_type_1(maze,dividemap,target_point,num,list_out);
                    if(flg == 0){
                        if(num > num_hunt_global_min){
                            list_out.clear();
                            list<Point>().swap(list_out);
                            p_next = target_point;//找到了区域外的点,记录下来
                            find_outside_point =1;
                        }else{
                            LOGPLANNING(INFO) << "该区域[id:" << dividemap.at<uint8_t >(target_point) << "]可清扫区域太少:" << num;
                            while(!list_out.empty()){
                                vpignore.emplace_back(list_out.front());
                                history[list_out.front().x][list_out.front().y] = 1;
                                list_out.pop_front();
                            }
                        }
                    }
                }
//                else{
//                    openList.emplace_back( Point(x_2,y_2));
//                    history[x_2][y_2] = 1;
//                }
            }else {
                target_point.x = x_2;
                target_point.y = y_2;
                int num = num_hunt_local_min;
                list_out.clear();
                list<Point>().swap(list_out);
                int flg = count_set_type_1(maze,dividemap,target_point,num,list_out);
                if(flg == 0){
                    if(num > num_hunt_local_min){
                        list_out.clear();
                        list<Point>().swap(list_out);
                        vector<vector<uint8_t >>().swap(history);
                        list<Point>().swap(openList);
                        return SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside;
                    }else{
                        while(!list_out.empty()){
                            vpignore.emplace_back(list_out.front());
                            history[list_out.front().x][list_out.front().y] = 1;
                            list_out.pop_front();
                        }
                    }
                }
            }

        }
    }
    list_out.clear();
    list<Point>().swap(list_out);
    vector<vector<uint8_t >>().swap(history);
    list<Point>().swap(openList);

    if(find_outside_point){
        target_point = p_next;
        return SiggetCleanStartDelimit::FindUncleanPointDelimit_Out;
    }
#endif
    return SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;
}

bool Search::Search_obs_point(const Mat & maze , Point range_min , Point range_max , Point startPoint , Point &target_point , Mat & map){
    if(maze.empty()){
        LOGPLANNING(INFO) << "Search_obs_point 输入的地图数据为空.";
        return false;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x >= maze.cols || startPoint.y >= maze.rows){
        LOGPLANNING(INFO) << "Search_obs_point: 输入的起点超出地图范围(" << startPoint.x << ", " << startPoint.y << ")";
        return false;
    }

    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    map.at<uint8_t >(startPoint) = 1;

    bool flg = false;
    LOGPLANNING(INFO) << "search startPoint(" << startPoint.x << ", " << startPoint.y << ") , p_min(" << range_min.x << ", " << range_min.y << ")-(" << range_max.x << ", " << range_max.y << ")";
    while (!openList.empty())
    {
        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            map.at<uint8_t >(t.y , t.x) = 1;

            int x_3 = t.x + 1, y_3 = t.y + 0;
            auto * p = maze.ptr<uchar>(y_3);
            if (map.at<uint8_t >(y_3 , x_3) || p[x_3] == SearchUnknowCell || x_3 < range_min.x || y_3 < range_min.y || x_3 > range_max.x || y_3 > range_max.y){}
            else if(p[x_3] == SearchCovCell || p[x_3] == SearchFreeCell){
                openList.emplace_back( Point(x_3,y_3));
                map.at<uint8_t >(y_3 , x_3) = 1;
            }else {
                target_point.x = x_3;
                target_point.y = y_3;
                flg = true;
                break;
            }


            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
            if (map.at<uint8_t >(y_1 , x_1) || p[x_1] == SearchUnknowCell || x_1 < range_min.x || y_1 < range_min.y || x_1 > range_max.x || y_1 > range_max.y){}
            else if(p[x_1] == SearchCovCell || p[x_1] == SearchFreeCell){
                openList.emplace_back( Point(x_1,y_1));
                map.at<uint8_t >(y_1 , x_1) = 1;
            }else {
                target_point.x = x_1;
                target_point.y = y_1;
                flg = true;
                break;
            }

            int x_0 = t.x + 0, y_0 = t.y - 1;
            p = maze.ptr<uchar>(y_0);
            if (map.at<uint8_t >(y_0 , x_0) || p[x_0] == SearchUnknowCell || x_0 < range_min.x || y_0 < range_min.y || x_0 > range_max.x || y_0 > range_max.y){}
            else if(p[x_0] == SearchCovCell || p[x_0] == SearchFreeCell){
                openList.emplace_back( Point(x_0,y_0));
                map.at<uint8_t >(y_0 , x_0) = 1;
            }else {
                target_point.x = x_0;
                target_point.y = y_0;
                flg = true;
                break;
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
            if (map.at<uint8_t >(y_2 , x_2) || p[x_2] == SearchUnknowCell || x_2 < range_min.x || y_2 < range_min.y || x_2 > range_max.x || y_2 > range_max.y){}
            else if(p[x_2] == SearchCovCell || p[x_2] == SearchFreeCell){
                openList.emplace_back( Point(x_2,y_2));
                map.at<uint8_t >(y_2 , x_2) = 1;
            }else {
                target_point.x = x_2;
                target_point.y = y_2;
                flg = true;
                break;
            }
        }
        if(flg){
            break;
        }
    }

    openList.clear();
    list<Point>().swap(openList);

    return flg;
}
bool Search::Search_obs_point_marking(const Mat & maze , const Mat & dividemap , int id , Point range_min , Point range_max , Point startPoint , Point &target_point , Mat & map){
    if(maze.empty()){
        LOGPLANNING(INFO) << "Search_obs_point_marking 输入的地图数据为空.";
        return false;
    }
    if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x >= maze.cols || startPoint.y >= maze.rows){
        LOGPLANNING(INFO) << "Search_obs_point_marking: 输入的起点超出地图范围(" << startPoint.x << ", " << startPoint.y << ")";
        return false;
    }

    list<Point> openList;

    openList.emplace_back(Point(startPoint.x,startPoint.y));

    map.at<uint8_t >(startPoint) = 1;

    bool flg = false;
    LOGPLANNING(INFO) << "Search_obs_point_marking (" << startPoint.x << ", " << startPoint.y << ") , p_min(" << range_min.x << ", " << range_min.y << ")-(" << range_max.x << ", " << range_max.y << ")";
    LOGPLANNING(INFO) << "Search_obs_point_marking id:" << id;
    while (!openList.empty())
    {
        auto len = (int)openList.size();
        for (int i = 0; i < len; ++i) {

            auto t = openList.front();
            openList.pop_front();
            map.at<uint8_t >(t.y , t.x) = 1;

            int x_3 = t.x + 1, y_3 = t.y + 0;
            auto * p = maze.ptr<uchar>(y_3);
            if (map.at<uint8_t >(y_3 , x_3) || p[x_3] == SearchUnknowCell || x_3 < range_min.x || y_3 < range_min.y || x_3 > range_max.x || y_3 > range_max.y || dividemap.at<uint8_t >(y_3,x_3) != id){}
            else if(p[x_3] == SearchCovCell || p[x_3] == SearchFreeCell){
                openList.emplace_back( Point(x_3,y_3));
                map.at<uint8_t >(y_3 , x_3) = 1;
            }else if(p[x_3] == SearchObsCell){
                target_point.x = x_3;
                target_point.y = y_3;
                flg = true;
                break;
            }


            int x_1 = t.x - 1, y_1 = t.y + 0;
            p = maze.ptr<uchar>(y_1);
            if (map.at<uint8_t >(y_1 , x_1) || p[x_1] == SearchUnknowCell || x_1 < range_min.x || y_1 < range_min.y || x_1 > range_max.x || y_1 > range_max.y || dividemap.at<uint8_t >(y_1,x_1) != id){}
            else if(p[x_1] == SearchCovCell || p[x_1] == SearchFreeCell){
                openList.emplace_back( Point(x_1,y_1));
                map.at<uint8_t >(y_1 , x_1) = 1;
            }else if(p[x_1] == SearchObsCell){
                target_point.x = x_1;
                target_point.y = y_1;
                flg = true;
                break;
            }

            int x_0 = t.x + 0, y_0 = t.y - 1;
            p = maze.ptr<uchar>(y_0);
            if (map.at<uint8_t >(y_0 , x_0) || p[x_0] == SearchUnknowCell || x_0 < range_min.x || y_0 < range_min.y || x_0 > range_max.x || y_0 > range_max.y || dividemap.at<uint8_t >(y_0,x_0) != id){}
            else if(p[x_0] == SearchCovCell || p[x_0] == SearchFreeCell){
                openList.emplace_back( Point(x_0,y_0));
                map.at<uint8_t >(y_0 , x_0) = 1;
            }else if(p[x_0] == SearchObsCell){
                target_point.x = x_0;
                target_point.y = y_0;
                flg = true;
                break;
            }

            int x_2 = t.x + 0, y_2 = t.y + 1;
            p = maze.ptr<uchar>(y_2);
            if (map.at<uint8_t >(y_2 , x_2) || p[x_2] == SearchUnknowCell || x_2 < range_min.x || y_2 < range_min.y || x_2 > range_max.x || y_2 > range_max.y || dividemap.at<uint8_t >(y_2,x_2) != id){}
            else if(p[x_2] == SearchCovCell || p[x_2] == SearchFreeCell){
                openList.emplace_back( Point(x_2,y_2));
                map.at<uint8_t >(y_2 , x_2) = 1;
            }else if(p[x_2] == SearchObsCell){
                target_point.x = x_2;
                target_point.y = y_2;
                flg = true;
                break;
            }
        }
        if(flg){
            break;
        }
    }

    openList.clear();
    list<Point>().swap(openList);

    return flg;
}
