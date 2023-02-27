//
// Created by lcy on 22-8-16.
//

#include "aStar.h"

void aStar::RasterLine_0(Point start , Point end , vector<Point>& linePointList){
    linePointList.clear();
    int dlt_x = start.x - end.x;
    int dlt_y = start.y - end.y;
    int i_min = 0;
    int i_max = 0;
    if(dlt_x == 0){
        i_min = min(start.y,end.y);
        i_max = max(start.y,end.y);
        for(int i = i_min ; i <= i_max ; i++){
            linePointList.emplace_back(Point(start.x,i));
        }
    }else if(dlt_y == 0){
        i_min = min(start.x,end.x);
        i_max = max(start.x,end.x);
        for(int i = i_min ; i <= i_max ; i++){
            linePointList.emplace_back(Point(i,start.y));
        }
    }else if(abs(dlt_y) == abs(dlt_x)){
        i_min = min(start.x,end.x);
        i_max = max(start.x,end.x);
        int a = ((dlt_y > 0 && dlt_x > 0) || (dlt_y < 0 && dlt_x < 0)) ? 1:-1;
        int b= (start.y - a*start.x);
        for (int i = i_min; i <= i_max; i++) {
            linePointList.emplace_back(Point(i, (i * a + b)));
        }
    }else if(abs(dlt_y) > abs(dlt_x)){
        i_min = start.y;
        i_max = end.y;
        float a = (float)dlt_y/dlt_x;
        float b= (start.y - a*start.x);
        if(dlt_y > 0){
            for(int i = i_min ; i >= i_max ; i--){
                linePointList.emplace_back(Point((int)floor((i - 0.5 - b)/a),i));
            }
        }else{
            for(int i = i_min ; i <= i_max ; i++){
                linePointList.emplace_back(Point((int)ceil((i + 0.5 - b)/a),i));
            }
        }
    }else{
        i_min = start.x;
        i_max = end.x;
        float a = (float)dlt_y/dlt_x;
        float b= (start.y - a*start.x);
        if(dlt_x > 0){
            for(int i = i_min ; i >= i_max ; i--){
                linePointList.emplace_back(Point(i,(int)ceil(a*(i + 0.5) + b)));
            }
        }else{
            for(int i = i_min ; i <= i_max ; i++){
                linePointList.emplace_back(Point(i,(int)floor(a*(i - 0.5) + b)));
            }
        }
    }
}
bool aStar::selectpose_0(Mat & src , std::vector<Point>& path){
    if (path.size() < 3) {
        LOGPLANNING(INFO) << "数量太少,不需筛选:" << path.size();
        return false;
    }else{
        LOGPLANNING(INFO) << "需要筛点数量:" << path.size();
    }
    vector<Point>::iterator iter,iter1;
    vector<Point>::reverse_iterator iter2;
    int dis_obs = 0;
    if(path.size() < 14){
        dis_obs = 4;
    }else{
        dis_obs = 4;
    }

    bool sig_effect;
    Point p_same;
    vector<Point> linePointList;
    for(iter1 = path.begin() ; iter1 != path.end() ; iter1++){
        for(iter2 = path.rbegin() ; *iter2 != *iter1 ; iter2++){
            RasterLine_0(*iter1,*iter2,linePointList);
            sig_effect = false;
            for (auto &k : linePointList) {
                if(src.at<uint8_t >(k.y, k.x) > dis_obs){
                    sig_effect = true;
                    break;
                }
            }
            if(sig_effect){
            }else{
                p_same = *iter2;
                iter1 ++;
                while(p_same != *iter1){
                    path.erase(iter1);
                }
                break;
            }
        }
    }

    return true;
}
bool aStar::selectpose(std::vector<Point>& path){
    if (path.size() < 3) {
        LOGPLANNING(INFO) << "数量太少,不需筛选:" << path.size();
        return false;
    }
    int dltX1,dltX2,dltY1,dltY2;
    std::vector<Point>::iterator iter1,iter2,iter3;
    for(iter1 = path.begin() ; iter1 != path.end() ;){
        iter2 = iter1;
        iter2 ++;
        iter3 = iter2;
        iter3 ++;

        if(iter3 == path.end()){
            break;
        }
        dltX1 = iter2->x - iter1->x;
        dltX2 = iter3->x - iter2->x;

        dltY1 = iter2->y - iter1->y;
        dltY2 = iter3->y - iter2->y;

        if((dltX1 == 0 && dltX2 == 0) || (dltY1 == 0 && dltY2 == 0) || ((dltX1 != 0 && dltX2 != 0) && ((dltY1 * 1.0f)/dltX1 == (dltY2 * 1.0f)/dltX2))){
            path.erase(iter2);
        }else {
            iter1 ++;
        }
    }
#if 1
    std::vector<Point> dir_8 = {
            Point(1,   0),
            Point(0,   1),
            Point(-1,   0),
            Point(0,   -1),
            Point(-1,   -1),
            Point(-1,   1),
            Point(1,   -1),
            Point(1,    1)
    };
    int num = 0;
    Point p_tmp;
    for(iter1 = path.begin() ; iter1 != path.end() ; ){
        iter2 = iter1;
        iter2 ++;
        iter3 = iter2;
        iter3 ++;

        if(iter2 == path.end()){
            break;
        }
        num = 0;
        for(int i = 0 ; i < dir_8.size(); i++){
            p_tmp = *iter2 + dir_8[i];
            if(p_tmp == *iter1){
                num ++;
            }
        }
        if(num == 0){
            iter1 ++;
        }else if(num == 1){
            if(iter3 != path.end()){
                path.erase(iter2);
            }else{
                iter1 ++;
            }
        }else{
            iter1 ++;
        }
    }
    for(iter1 = path.begin() ; iter1 != path.end() ;){
        iter2 = iter1;
        iter2 ++;
        iter3 = iter2;
        iter3 ++;

        if(iter3 == path.end()){
            break;
        }
        dltX1 = iter2->x - iter1->x;
        dltX2 = iter3->x - iter2->x;

        dltY1 = iter2->y - iter1->y;
        dltY2 = iter3->y - iter2->y;

        if((dltX1 == 0 && dltX2 == 0) || (dltY1 == 0 && dltY2 == 0) || ((dltX1 != 0 && dltX2 != 0) && ((dltY1 * 1.0f)/dltX1 == (dltY2 * 1.0f)/dltX2))){
            path.erase(iter2);
        }else {
            iter1 ++;
        }
    }
    dir_8.clear();
    std::vector<Point>().swap(dir_8);
#endif
    return true;
}
int aStar::calHeristic(node* n1, node n2) {
    return (abs(n1->coordinateX() - n2.coordinateX()) + abs(n1->coordinateY() - n2.coordinateY()))*10;
}

bool aStar::verifyNode(node rhs) {
    return !(rhs.coordinateX() < 0 || rhs.coordinateX() >= mapSize_X
             || rhs.coordinateY() < 0 || rhs.coordinateY() >= mapSize_Y);
}

bool aStar::astar(const node& startNode, const node& goalNode, vector<vector<node*>>& roadmap , vector<Point> &path) {
    auto cmp = [](node* left, node* right) { return left->f_() > right->f_(); };
    priority_queue<node*, vector<node*>, decltype(cmp)> O(cmp);

    node* tmp = roadmap[startNode.coordinateX()][startNode.coordinateY()];
    O.push(tmp);

    std::vector<node> motion = {
            node(1,   0,  10),
            node(0,   1,  10),
            node(-1,   0,  10),
            node(0,   -1,  10)
//            node(-1,   -1,  14),
//            node(-1,   1,  14),
//            node(1,   -1,  14),
//            node(1,    1,  14)
    };
    node tmpNode;
    bool flg_find = false;
    int tmp_cost = 0;
    while (!O.empty()) {
        node* nBest = O.top();
        O.pop();
        nBest->setIsVisited();

        if (*nBest == goalNode){
            flg_find = true;
            break;
        }

        for (int i = 0 ; i < motion.size() ; i++) {
            tmpNode = motion[i];
            tmpNode += *nBest;//当前节点
            node* tmpNodePointer = roadmap[tmpNode.coordinateX()][tmpNode.coordinateY()];//原节点
            if(!tmpNodePointer->returnIsVisited()){
                if (verifyNode(*tmpNodePointer) && !tmpNodePointer->isObstacle()) {
                    tmpNodePointer->setG(tmpNode.g_());
//                    tmpNodePointer->setF((tmpNodePointer->g_() + tmpNodePointer->h_()) * ((int)exp(tmpNodePointer->c_())));
                    tmp_cost = tmpNodePointer->c_() > 0 ? tmpNodePointer->c_():1;
                    tmpNodePointer->setF((tmpNodePointer->g_() + tmpNodePointer->h_()) * tmp_cost);
                    tmpNodePointer->setIsVisited();
                    tmpNodePointer->setParentpoint(nBest);
                    O.push(tmpNodePointer);
                }
            }
            else{
                if(tmpNode.g_() < tmpNodePointer->g_()){
                    tmpNodePointer->setG(tmpNode.g_());
//                    tmpNodePointer->setF((tmpNodePointer->g_() + tmpNodePointer->h_()) * ((int)exp(tmpNodePointer->c_())));
                    tmp_cost = tmpNodePointer->c_() > 0 ? tmpNodePointer->c_():1;
                    tmpNodePointer->setF((tmpNodePointer->g_() + tmpNodePointer->h_()) * tmp_cost);
                    tmpNodePointer->setParentpoint(nBest);
                }
            }
        }
    }

    motion.clear();
    std::vector<node>().swap(motion);

    while(!O.empty()){
        O.pop();
    }

    if(flg_find){
        tmp = roadmap[goalNode.coordinateX()][goalNode.coordinateY()];
        path.emplace_back(Point(goalNode.coordinateX(),goalNode.coordinateY()));
        while (!(*tmp == startNode)) {
            tmp = tmp->returnparentpoint();
            path.emplace_back(Point(tmp->coordinateX(),tmp->coordinateY()));
        }
        return true;
    }else{
        return false;
    }
}

int aStar::aStartest() {
    Mat src = imread("/home/lcy/CLionProjects/test/nav_dilate.png",0);
    printf("src cols:%d , rows:%d\n" , src.cols , src.rows);
//    namedWindow("src", CV_WINDOW_NORMAL);
//    imshow("src", src);
    Mat showmap = src.clone();
    printf("map size :%d , %d\n" , src.cols , src.rows);

    add_cost_full(src);

//    node startNode(588,806);
//    node goalNode(812,571);
//    node startNode(616,798);
//    node goalNode(805,584);

    Point start = Point(616,798);
//    Point target = Point(805,584);
    Point target = Point(785,797);

    Point start_reset = start;
    Point target_reset = target;
    resetpoint_min(src , start_reset);
    resetpoint_min(src , target_reset);
#if 0
    Point p_min = Point(startNode.coordinateX() - 5,startNode.coordinateY() - 5);
    Point p_max = Point(startNode.coordinateX() + 5,startNode.coordinateY() + 5);
    for(int i = p_min.x ; i < p_max.x ; i++)
        for(int j = p_min.y ; j < p_max.y ; j++){
        if(src.at<uint8_t >(j,i) != 0 && src.at<uint8_t >(j,i) != 255){
            src.at<uint8_t >(j,i) = (uint8_t)(src.at<uint8_t >(j,i) * 50);
        }
    }
    p_min = Point(goalNode.coordinateX() - 5,goalNode.coordinateY() - 5);
    p_max = Point(goalNode.coordinateX() + 5,goalNode.coordinateY() + 5);
    for(int i = p_min.x ; i < p_max.x ; i++)
        for(int j = p_min.y ; j < p_max.y ; j++){
            if(src.at<uint8_t >(j,i) != 0 && src.at<uint8_t >(j,i) != 255){
                src.at<uint8_t >(j,i) = (uint8_t)(src.at<uint8_t >(j,i) * 50);
            }
        }
#endif
    node startNode(start_reset.x,start_reset.y);
    node goalNode(target_reset.x,target_reset.y);
    mapSize_X = src.cols;
    mapSize_Y = src.rows;
    vector<vector<node*>> roadmap;
    for (int i = 0; i < src.cols; i++) {
        vector<node*> tmp;
        for (int j = 0; j < src.rows; j++) {
            node* tmpNode = new node(i, j);
            if(src.at<uint8_t >(j,i) == 255){
                tmpNode->setH(INT_MAX);
                tmpNode->setObstacle();
            }else{
                tmpNode->setH(calHeristic(tmpNode, goalNode));
                tmpNode->setC(src.at<uint8_t >(j,i));
            }
            tmpNode->setG(0);
            tmpNode->setX(i);
            tmpNode->setY(j);
            tmp.push_back(tmpNode);
        }
        roadmap.push_back(tmp);
    }
    Mat map = src.clone();
    vector<Point> path;
    bool flg = astar(startNode, goalNode, roadmap, path);
    printf("path size :%ld\n",path.size());
    if(flg){
        printf("flg start:(%d,%d)\n",start.x , start.y);
        if(start_reset != start){
            path.emplace_back(start);
        }
        reverse(path.begin() , path.end());
        if(target_reset != target){
            path.emplace_back(target);
        }

        for(int i = 0 ; i < path.size() ; i++){
            printf("%d,(%d,%d)\n",i,path[i].x,path[i].y);
            map.at<uint8_t >(path[i]) = 150;
        }
        namedWindow("map", CV_WINDOW_NORMAL);
        imshow("map", map);
        selectpose_0(src,path);
        selectpose(path);

        printf("select path size :%ld\n" , path.size());
        for(int i = 0 ; i < path.size() ; i++){
            printf("%d,(%d,%d)\n",i,path[i].x,path[i].y);
        }
    }
    printf("flg:%d\n",flg);

    for(int i = 0 ; i < mapSize_X ; i++)
        for(int j = 0 ; j < mapSize_Y ; j++){
            delete roadmap[i][j];
        }
    for(int i = 0 ; i < roadmap.size() ; i++){
        roadmap[i].clear();
    }
    vector<vector<node*>>().swap(roadmap);

    vector<Point>::iterator iter1 , iter2;
    for(iter1 = path.begin() ; iter1 != path.end() ; iter1 ++){
        iter2 = iter1;
        iter2 ++;
        if(iter2 != path.end()){
            line(src,*iter1,*iter2,Scalar(100));
        }

    }
    int num = 0;
    while(!path.empty()){
        src.at<uint8_t >(path.back()) = 40;
//        printf("path %d:(%d,%d)\n",num,path.back().x,path.back().y);
        path.pop_back();
        num ++;
    }

    namedWindow("showmap", CV_WINDOW_NORMAL);
    imshow("showmap", src);
    src.release();
    map.release();
    waitKey();
    return 0;
}
int aStar::add_cost_full(Mat &src){
    if(src.empty()){
        LOGPLANNING(INFO) << "src is empty !!!";
        return 1;
    }
    for(int i = 0 ; i < src.rows ; i++)
        for(int j = 0 ; j < src.cols ; j++){//二值化,可通行区域的值为255,不可通行的值为0.
            if(src.at<uint8_t >(i,j) == NavFreeCell)
            {
                src.at<uint8_t>(i,j) = 255;
            }else {
                src.at<uint8_t>(i,j) = 0;
            }
        }
    Mat map = src;
    distanceTransform(map, map, CV_DIST_C, DIST_MASK_3, DIST_LABEL_PIXEL);

    for(int i = 0 ; i < map.rows ; i++)
        for(int j = 0 ; j < map.cols ; j++){
            if(src.at<uint8_t >(i,j) == 0){
                src.at<uint8_t >(i,j) = 255;
            }else{
                if(map.at<float>(i,j) > 5){
                    src.at<uint8_t >(i,j) = 0;
                }else{
                    src.at<uint8_t >(i,j) = (uint8_t)((6 - map.at<float>(i,j)));
                }
            }
        }
    map.release();
    return 0;
}
bool aStar::resetpoint(Mat &src , Point & p){
    vector<cv::Point> expand_3 = {
            cv::Point(0,-3),cv::Point(-2,-2),cv::Point(-1,-2),cv::Point(1,-2),cv::Point(2,-2),cv::Point(-2,-1),cv::Point(2,-1),cv::Point(-3,0),cv::Point(3,0),cv::Point(-2,1),cv::Point(2,1),cv::Point(-2,2),cv::Point(-1,2),cv::Point(1,2),cv::Point(2,2),cv::Point(0,3)
    };
    vector<Point> p_255;
    vector<Point> p_a;
    vector<Point>::iterator iter;
    Point p255;
    for(int i = 0 ; i < expand_3.size() ; i++){
        p255 = p + expand_3[i];
        if(src.at<uint8_t >(p255) != 255){
            p_255.emplace_back(expand_3[i]);
            p_a.emplace_back(p255);
        }
    }
    LOGPLANNING(INFO) << "p_a.size: " << p_a.size();
    Point p_t;
    for(int i = 0 ; i < p_255.size() ; i++){
        p_t += p_255[i];
    }
    p_t = p_t + p;
    Point p_min_ = Point(min(p.x,p_t.x),min(p.y,p_t.y));
    Point p_max_ = Point(max(p.x,p_t.x),max(p.y,p_t.y));
    LOGPLANNING(INFO) << "p_min_(" << p_min_.x << ", " << p_min_.y << ") , p_max_(" << p_max_.x << ", " << p_max_.y << ")";
    int num = 0;
    p_t = Point(0,0);
    if(p_min_ != p_max_){
        if(p_min_.x == p_max_.x && p_min_.y != p_max_.y){
            for(int j = p_min_.y ; j < p_max_.y ; j++){
                for(iter = p_a.begin() ; iter != p_a.end() ; iter++){
                    if(Point(p_min_.x,j) == *iter){
                        num ++;
                        p_t = p_t + Point(p_min_.x,j);
                        p_a.erase(iter);
                        break;
                    }
                }
            }
        }else if(p_min_.x != p_max_.x && p_min_.y == p_max_.y){
            for(int i = p_min_.x ; i < p_max_.x ; i++){
                for(iter = p_a.begin() ; iter != p_a.end() ; iter++){
                    if(Point(i,p_min_.y) == *iter){
                        num ++;
                        p_t = p_t + Point(i,p_min_.y);
                        p_a.erase(iter);
                        break;
                    }
                }
            }
        }else{
            for(int i = p_min_.x ; i < p_max_.x ; i++)
                for(int j = p_min_.y ; j < p_max_.y ; j++){
                    for(iter = p_a.begin() ; iter != p_a.end() ; iter++){
                        if(Point(i,j) == *iter){
                            num ++;
                            p_t = p_t + Point(i,j);
                            p_a.erase(iter);
                            break;
                        }
                    }
                }
        }
        if(num > 0){
            p.x = p_t.x/num;
            p.y = p_t.y/num;
        }

    }

    p_255.clear();
    vector<Point>().swap(p_255);
    p_a.clear();
    vector<Point>().swap(p_a);
    expand_3.clear();
    vector<cv::Point>().swap(expand_3);
    return true;
}
bool aStar::resetpoint_min(Mat &src , Point & p){
    vector<cv::Point> around_8 = {
            cv::Point(0,-1),cv::Point(0,1),cv::Point(-1,0),cv::Point(1,0),
            cv::Point(1,-1),cv::Point(1,1),cv::Point(-1,-1),cv::Point(-1,1)
    };
    if(src.at<uint8_t >(p + around_8[0]) == 255 ||
       src.at<uint8_t >(p + around_8[1]) == 255 ||
       src.at<uint8_t >(p + around_8[2]) == 255 ||
       src.at<uint8_t >(p + around_8[3]) == 255 ||
       src.at<uint8_t >(p + around_8[4]) == 255 ||
       src.at<uint8_t >(p + around_8[5]) == 255 ||
       src.at<uint8_t >(p + around_8[6]) == 255 ||
       src.at<uint8_t >(p + around_8[7]) == 255){
    }else{
        return false;
    }
    vector<cv::Point> expand_3 = {
            cv::Point(0,-3),cv::Point(-2,-2),cv::Point(-1,-2),cv::Point(1,-2),cv::Point(2,-2),cv::Point(-2,-1),cv::Point(2,-1),cv::Point(-3,0),cv::Point(3,0),cv::Point(-2,1),cv::Point(2,1),cv::Point(-2,2),cv::Point(-1,2),cv::Point(1,2),cv::Point(2,2),cv::Point(0,3)
    };
    int cost_max = src.at<uint8_t >(p);
    int cost_max_id = -1;
    Point p_tmp;
    for(int i = 0 ; i < expand_3.size() ; i++){
        p_tmp = p + expand_3[i];
        if(src.at<uint8_t >(p_tmp) != 255){
            if(src.at<uint8_t >(p_tmp) < cost_max){
                cost_max = src.at<uint8_t >(p_tmp);
                cost_max_id = i;
            }
        }
    }
    if(cost_max_id >= 0){
        p = p + expand_3[cost_max_id];
    }

    around_8.clear();
    vector<Point>().swap(around_8);
    expand_3.clear();
    vector<cv::Point>().swap(expand_3);
    return true;
}

bool aStar::aStartest(const Mat & map , Point start , Point target , vector<Point> &path){
    LOGPLANNING(INFO) << "nav start(" << start.x << ", " << start.y << ") target(" << target.x << ", " << target.y << ")";
    path.clear();
    if(map.empty()){
        return false;
    }
    if(start.x < 0 || start.x >= map.cols || start.y < 0 || start.y >= map.rows){
        return false;
    }
    bool sig_effect = false;
    vector<Point> linePointList;
    RasterLine_0(start,target,linePointList);
    for (auto &k : linePointList) {
        if(map.at<uint8_t >(k) != NavFreeCell){
            sig_effect = true;
            break;
        }
    }
    linePointList.clear();
    vector<Point>().swap(linePointList);
    if(!sig_effect){
        path.emplace_back(start);
        path.emplace_back(target);

        LOGPLANNING(INFO) << "start - target is through !!!";
        for(int i = 0 ; i < path.size() ; i++){
            LOGPLANNING(INFO) << i << "-(" << path[i].x << ", " << path[i].y << ")";
        }
        return true;
    }
    Mat src = map.clone();
    add_cost_full(src);

    Point start_reset = start;
    Point target_reset = target;
    resetpoint_min(src , start_reset);
    resetpoint_min(src , target_reset);
#if 0
    Point p_min = Point(start_reset.x - 5,start_reset.y - 5);
    Point p_max = Point(start_reset.x + 5,start_reset.y + 5);
    for(int i = p_min.x ; i < p_max.x ; i++)
        for(int j = p_min.y ; j < p_max.y ; j++){
            if(src.at<uint8_t >(j,i) > 0 && src.at<uint8_t >(j,i) <= 5){
                src.at<uint8_t >(j,i) = (uint8_t)(src.at<uint8_t >(j,i) * 50);
            }
        }
    p_min = Point(target_reset.x - 5,target_reset.y - 5);
    p_max = Point(target_reset.x + 5,target_reset.y + 5);
    for(int i = p_min.x ; i < p_max.x ; i++)
        for(int j = p_min.y ; j < p_max.y ; j++){
            if(src.at<uint8_t >(j,i) > 0 && src.at<uint8_t >(j,i) <= 5){
                src.at<uint8_t >(j,i) = (uint8_t)(src.at<uint8_t >(j,i) * 50);
            }
        }
#endif
    node startNode(start_reset.x,start_reset.y);
    node goalNode(target_reset.x,target_reset.y);
    mapSize_X = src.cols;
    mapSize_Y = src.rows;
    vector<vector<node*>> roadmap;
    for (int i = 0; i < src.cols; i++) {
        vector<node*> tmp;
        for (int j = 0; j < src.rows; j++) {
            node* tmpNode = new node(i, j);
            if(src.at<uint8_t >(j,i) == NavUnknowCell){
                tmpNode->setH(INT_MAX);
                tmpNode->setObstacle();
            }else{
                tmpNode->setH(calHeristic(tmpNode, goalNode));
                tmpNode->setC(src.at<uint8_t >(j,i));
            }
            tmpNode->setG(0);
            tmpNode->setX(i);
            tmpNode->setY(j);
            tmp.push_back(tmpNode);
        }
        roadmap.push_back(tmp);
    }

    bool flg = astar(startNode, goalNode, roadmap, path);
    LOGPLANNING(INFO) << "path size :" << path.size();
    if(flg){
        LOGPLANNING(INFO) << "flg start:(" << start.x << ", " << start.y << ")";
        if(start_reset != start){
            path.emplace_back(start);
        }
        reverse(path.begin() , path.end());
        if(target_reset != target){
            path.emplace_back(target);
        }
        for(int i = 0 ; i < path.size() ; i++){
            LOGPLANNING(INFO) << i << "-(" << path[i].x << ", " << path[i].y << ")";
        }
        selectpose_0(src,path);
        selectpose(path);

        LOGPLANNING(INFO) << "select path size : " << path.size();
        for(int i = 0 ; i < path.size() ; i++){
            LOGPLANNING(INFO) << i << "-(" << path[i].x << ", " << path[i].y << ")";
        }
    }
    LOGPLANNING(INFO) << "flg : " << flg;

    src.release();

    for(int i = 0 ; i < mapSize_X ; i++)
        for(int j = 0 ; j < mapSize_Y ; j++){
            delete roadmap[i][j];
        }
    for(int i = 0 ; i < roadmap.size() ; i++){
        roadmap[i].clear();
    }
    vector<vector<node*>>().swap(roadmap);

    return flg;
}