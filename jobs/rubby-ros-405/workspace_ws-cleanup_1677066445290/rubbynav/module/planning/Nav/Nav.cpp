//
// Created by lcy on 21-6-15.
//
#include "Nav.h"
#include <list>

namespace global_planner {
    vector<vector<int8_t>> vec_cost_mould = {
            /*1*/ {0,-1,-1,0,1,0,0,1},
            /*2*/ {0,-2,-1,-1,1,-1,-2,0,2,0,-1,1,1,1,0,2},
            /*3*/ {0,-3,-2,-2,-1,-2,1,-2,2,-2,-2,-1,2,-1,-3,0,3,0,-2,1,2,1,-2,2,-1,2,1,2,2,2,0,3},
            /*4*/ {0,-4,-2,-3,-1,-3,1,-3,2,-3,-3,-2,3,-2,-3,-1,3,-1,-4,0,4,0,-3,1,3,1,-3,2,3,2,-2,3,-1,3,1,3,2,3,0,4},
            /*5*/ {0,-5,-3,-4,-2,-4,-1,-4,1,-4,2,-4,3,-4,-4,-3,-3,-3,3,-3,4,-3,-4,-2,4,-2,-4,-1,4,-1,-5,0,5,0,-4,1,4,1,-4,2,4,2,-4,3,-3,3,3,3,4,3,-3,4,-2,4,-1,4,1,4,2,4,3,4,0,5},
            /*6*/ {0,-6,-3,-5,-2,-5,-1,-5,1,-5,2,-5,3,-5,-4,-4,4,-4,-5,-3,5,-3,-5,-2,5,-2,-5,-1,5,-1,-6,0,6,0,-5,1,5,1,-5,2,5,2,-5,3,5,3,-4,4,4,4,-3,5,-2,5,-1,5,1,5,2,5,3,5,0,6},
            /*7*/ {0,-7,-3,-6,-2,-6,-1,-6,1,-6,2,-6,3,-6,-4,-5,4,-5,-5,-4,5,-4,-6,-3,6,-3,-6,-2,6,-2,-6,-1,6,-1,-7,0,7,0,-6,1,6,1,-6,2,6,2,-6,3,6,3,-5,4,5,4,-4,5,4,5,-3,6,-2,6,-1,6,1,6,2,6,3,6,0,7},
            /*8*/ {0,-8,-3,-7,-2,-7,-1,-7,1,-7,2,-7,3,-7,-5,-6,-4,-6,4,-6,5,-6,-6,-5,-5,-5,5,-5,6,-5,-6,-4,6,-4,-7,-3,7,-3,-7,-2,7,-2,-7,-1,7,-1,-8,0,8,0,-7,1,7,1,-7,2,7,2,-7,3,7,3,-6,4,6,4,-6,5,-5,5,5,5,6,5,-5,6,-4,6,4,6,5,6,-3,7,-2,7,-1,7,1,7,2,7,3,7,0,8},
            /*9*/ {0,-9,-4,-8,-3,-8,-2,-8,-1,-8,1,-8,2,-8,3,-8,4,-8,-5,-7,-4,-7,4,-7,5,-7,-6,-6,6,-6,-7,-5,7,-5,-8,-4,-7,-4,7,-4,8,-4,-8,-3,8,-3,-8,-2,8,-2,-8,-1,8,-1,-9,0,9,0,-8,1,8,1,-8,2,8,2,-8,3,8,3,-8,4,-7,4,7,4,8,4,-7,5,7,5,-6,6,6,6,-5,7,-4,7,4,7,5,7,-4,8,-3,8,-2,8,-1,8,1,8,2,8,3,8,4,8,0,9},
            /*10*/{0,-10,-4,-9,-3,-9,-2,-9,-1,-9,1,-9,2,-9,3,-9,4,-9,-6,-8,-5,-8,5,-8,6,-8,-7,-7,-6,-7,6,-7,7,-7,-8,-6,-7,-6,7,-6,8,-6,-8,-5,8,-5,-9,-4,9,-4,-9,-3,9,-3,-9,-2,9,-2,-9,-1,9,-1,-10,0,10,0,-9,1,9,1,-9,2,9,2,-9,3,9,3,-9,4,9,4,-8,5,8,5,-8,6,-7,6,7,6,8,6,-7,7,-6,7,6,7,7,7,-6,8,-5,8,5,8,6,8,-4,9,-3,9,-2,9,-1,9,1,9,2,9,3,9,4,9,0,10}
    };

    int NavPath::add_cost(Mat &src){
        if(src.empty()){
            return 0;
        }
        Mat tmpmap = src.clone();
        Point l_t = Point(0,0);
        Point r_d = Point(src.cols,src.rows);
        int row_;
        int col_;
        for(int i = l_t.y ; i < r_d.y ; i ++){
            for(int j = l_t.x ; j < r_d.x ; j++){
                if(tmpmap.at<uint8_t >(i,j) == NavObsCell || tmpmap.at<uint8_t >(i,j) == NavSensorObsCell){
                    if(tmpmap.at<uint8_t >(i,j + 1) != NavFreeCell){
                        if(tmpmap.at<uint8_t >(i,j - 1) != NavFreeCell){
                            if(tmpmap.at<uint8_t >(i + 1,j) != NavFreeCell){
                                if(tmpmap.at<uint8_t >(i - 1,j) != NavFreeCell){
                                    continue;
                                }
                            }
                        }
                    }
                    for(int i_ = 0 ; i_ < addcost ; i_ ++) {
                        for (int j_ = 0; j_ < vec_cost_mould[i_].size(); j_++) {
                            col_ = j + (int)vec_cost_mould[i_][j_]; //col
                            row_ = i + (int)vec_cost_mould[i_][j_ + 1];//row
                            if(row_ >= l_t.y && col_ >= l_t.x && row_ < r_d.y && col_ < r_d.x){
                                if (src.at<uint8_t>(row_, col_) == NavFreeCell) {
                                    src.at<uint8_t>(row_, col_) = NavObsCell;
                                }
                            }
                            j_++;
                        }
                    }
                }
            }
        }

        return 1;
    }

    bool NavPath::connect_p2p(Mat & maze , Point startPoint , Point target_point){
        if(maze.empty()){
            LOGPLANNING(INFO) << "maze is empty !!!";
            return false;
        }
        if(startPoint.x < 0 || startPoint.y < 0 || startPoint.x >= maze.cols || startPoint.y >= maze.rows ||
                target_point.x < 0 || target_point.y < 0 || target_point.x >= maze.cols || target_point.y >= maze.rows){
            LOGPLANNING(INFO) << "point out map : start(" << startPoint.x << " , " << startPoint.y << ") , target(" << target_point.x << " , " << target_point.y<< ")";
            return false;
        }
        if(startPoint == target_point){
            return true;
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
                else if(history[x_0][y_0] || p[x_0] != NavFreeCell){}
                else if(x_0 == target_point.x && y_0 == target_point.y)    {

                    vector<vector<uint8_t >>().swap(history);
                    list<Point>().swap(openList);
                    return true;
                }else{
                    openList.emplace_back( Point(x_0,y_0));
                    history[x_0][y_0] = 1;
                }

                int x_1 = t.x - 1, y_1 = t.y + 0;
                p = maze.ptr<uchar>(y_1);
                if (x_1 < 0 || x_1 >= x_max || y_1 < 0 || y_1 >= y_max){}
                else if(history[x_1][y_1] || p[x_1] != NavFreeCell){}
                else if(x_1 == target_point.x && y_1 == target_point.y)    {

                    vector<vector<uint8_t >>().swap(history);
                    list<Point>().swap(openList);
                    return true;
                }else{
                    openList.emplace_back( Point(x_1,y_1));
                    history[x_1][y_1] = 1;
                }

                int x_2 = t.x + 0, y_2 = t.y + 1;
                p = maze.ptr<uchar>(y_2);
                if (x_2 < 0 || x_2 >= x_max || y_2 < 0 || y_2 >= y_max){}
                else if(history[x_2][y_2] || p[x_2] != NavFreeCell){}
                else if(x_2 == target_point.x && y_2 == target_point.y)    {

                    vector<vector<uint8_t >>().swap(history);
                    list<Point>().swap(openList);
                    return true;
                }else{
                    openList.emplace_back( Point(x_2,y_2));
                    history[x_2][y_2] = 1;
                }

                int x_3 = t.x + 1, y_3 = t.y + 0;
                p = maze.ptr<uchar>(y_3);
                if (x_3 < 0 || x_3 >= x_max || y_3 < 0 || y_3 >= y_max){}
                else if(history[x_3][y_3] || p[x_3] != NavFreeCell){}
                else if(x_3 == target_point.x && y_3 == target_point.y)    {

                    vector<vector<uint8_t >>().swap(history);
                    list<Point>().swap(openList);
                    return true;
                }else{
                    openList.emplace_back( Point(x_3,y_3));
                    history[x_3][y_3] = 1;
                }
            }
        }

        vector<vector<uint8_t >>().swap(history);
        list<Point>().swap(openList);

        return false;//未找到
    }

    bool NavPath::nav(Mat & map , Point start , Point target , std::vector<Point>& plan){
        plan.clear();

        map.at<uint8_t >(start) = NavFreeCell;

        int nx = map.cols, ny = map.rows;

        setSize(nx, ny);
        potential_array_ = new float[nx * ny];
//        printf("nx:%d , ny:%d\n",nx , ny);

        outlineMap(map.data, nx, ny, NavObsCell);

        bool found_legal = calculatePotentials(map.data, start, target, nx * ny * 2, potential_array_);
        if(found_legal){
        } else{
            return false;
        }
        std::vector<std::pair<int, int> > path;
        if (!getPath(potential_array_, start, target, path)) {
            LOGPLANNING(INFO) << "NO PATH!";
            return false;
        }

        if(path.empty()){
            return false;
        }else{
//            printf("path size:%ld\n",path.size());
            for (int i = (int)path.size() -1; i>=0; i--) {
                std::pair<int, int> point = path[i];
                plan.emplace_back(Point(point.first, point.second));
            }
        }

#if 0
        FILE * fp;
    if((fp = fopen("/home/lcy/CLionProjects/robot_matlab/Map/map.txt","wb"))==NULL){
        LOGPLANNING(INFO) << "cant open the file";
        exit(0);
    }
    Mat map_potential_array = Mat(costmap_->map.rows,costmap_->map.cols,CV_64FC1);
    printf("nx:%d , ny:%d , nx*ny:%d\n",nx,ny,nx*ny);
    for(int i = 0 ; i < nx*ny ; i++){
        fprintf(fp,"%f\n",potential_array_[i]==POT_HIGH ? potential_array_[i] - POT_HIGH:potential_array_[i]);
        map_potential_array.at<double_t >(i/nx,i%nx) = potential_array_[i];
    }
    fclose(fp);


    namedWindow("map_potential", CV_WINDOW_NORMAL);
    imshow("map_potential", map_potential_array);
#endif

        delete[] potential_array_;
        return true;
    }
    bool NavPath::selectpose(Mat & src , std::vector<Point>& path){
        if (path.size() < 3) {
            LOGPLANNING(INFO) << "数量太少,不需筛选:" << path.size();
            return false;
        }else{
//            printf("需要筛点数量:%ld\n",path.size());
        }
        vector<Point> p_out;
        while(!path.empty()){
            p_out.emplace_back(path.front());
            auto k = path.begin();
            path.erase(k);
        }
        vector<Point> p_out_1;
        p_out_1.push_back(p_out.front());
        vector<Point> linePointList;
        int sig_effect = 0;
        for(int i = 0 ; i < p_out.size() ; i++){
            for(auto j = (int)(p_out.size() - 1) ; j > i ; j--){
                RasterLine_0(p_out[i],p_out[j],linePointList);
                sig_effect = 0;
                for (auto &k : linePointList) {
                    if(src.at<uint8_t >(k.y, k.x) != NavFreeCell){
                        sig_effect = 1;
                        break;
                    }
                }
                if(sig_effect){
                }else{
                    p_out_1.push_back(p_out[j]);
                    i = j;
                    break;
                }
            }
        }
        linePointList.clear();
        vector<Point>().swap(linePointList);
//        printf("经过筛点数量:%ld\n",p_out_1.size());
        vector<Point>().swap(path);
        int num = 0;
//        if(p_out_1.front().x != p_out.front().x || p_out_1.front().y != p_out.front().y){
//            path.push_back(p_out.front());
//        }
        while(num < p_out_1.size()){
            path.push_back(p_out_1[num]);
//            printf("p_out_1:%d,(%d,%d)\n",num,p_out_1[num].x,p_out_1[num].y);
            num ++;
        }
//        if(p_out_1.back().x != p_out.back().x || p_out_1.back().y != p_out.back().y){
//            path.push_back(p_out.back());
//        }
        if(p_out_1.back() != p_out.back()){
            path.push_back(p_out.back());
        }

        vector<Point>().swap(p_out_1);
        vector<Point>().swap(p_out);
        return true;
    }
    bool NavPath::raster_Line(Point p_s , Point p_e , vector<Point> &list){
        list.clear();
        Vector2d O(p_s.x, p_s.y);
        Vector2d E(p_e.x, p_e.y);
        std::pair<Vector2d, Vector2d> line(O, E);
        vector<Vector2d> linePointList;

        RasterLine(line, linePointList);
        if(linePointList.empty()){
            return false;
        }
        for (auto &i : linePointList) {
            list.emplace_back(Point((int) i.x,(int) i.y));
        }
        vector<Vector2d>().swap(linePointList);
        return true;
    }
//    bool NavPath::calculatePotentials(unsigned char* costs, double start_x, double start_y, double end_x, double end_y, int cycles, float* potential) {
    bool NavPath::calculatePotentials(unsigned char* costs, Point start, Point end, int cycles, float* potential) {
        queue_.clear();
        int start_i = toIndex(start.x, start.y);
        queue_.emplace_back(Index(start_i, 0));

        std::fill(potential, potential + ns_, POT_HIGH);
        potential[start_i] = 0;

        int goal_i = toIndex(end.x, end.y);
        int cycle = 0;

        while (!queue_.empty() && cycle < cycles) {
            Index top = queue_[0];
            std::pop_heap(queue_.begin(), queue_.end(), greater1());
            queue_.pop_back();

            int i = top.i;
            if (i == goal_i)
                return true;

            add(costs, potential, potential[i], i + 1, end.x, end.y);
            add(costs, potential, potential[i], i - 1, end.x, end.y);
            add(costs, potential, potential[i], i + nx_, end.x, end.y);
            add(costs, potential, potential[i], i - nx_, end.x, end.y);

            cycle++;
        }

        return false;
    }

    void NavPath::add(unsigned char* costs, float* potential, float prev_potential, int next_i, int end_x, int end_y) {
        if (next_i < 0 || next_i >= ns_)
            return;

        if (potential[next_i] < POT_HIGH)
            return;

//    if(costs[next_i]>=lethal_cost_ && !(unknown_ && costs[next_i]==UnknowCell))
//        return;
        if(costs[next_i]>=lethal_cost_ || costs[next_i] == NavUnknowCell)
            return;

        potential[next_i] = calculatePotential(potential, costs[next_i] + neutral_cost_, next_i, prev_potential);
        int x = next_i % nx_, y = next_i / nx_;
        float distance = abs(end_x - x) + abs(end_y - y);

        queue_.emplace_back(Index(next_i, potential[next_i] + distance * neutral_cost_));
        std::push_heap(queue_.begin(), queue_.end(), greater1());
    }
    bool NavPath::getPath(float* potential, Point start, Point end, std::vector<std::pair<int, int> >& path) {
        std::pair<int, int> current;
        current.first = end.x;
        current.second = end.y;

        int start_index = getIndex(start.x, start.y);

        path.push_back(current);
        int c = 0;
        int ns = nx_ * ny_;

        while (getIndex(current.first, current.second) != start_index) {
            float min_val = 1e10;
            int min_x = 0, min_y = 0;
            for (int xd = -1; xd <= 1; xd++) {
                for (int yd = -1; yd <= 1; yd++) {
                    if (xd == 0 && yd == 0)
                        continue;
                    int x = current.first + xd, y = current.second + yd;
                    int index = getIndex(x, y);
                    if (potential[index] < min_val) {
                        min_val = potential[index];
                        min_x = x;
                        min_y = y;
                    }
                }
            }
            if (min_x == 0 && min_y == 0)
                return false;
            current.first = min_x;
            current.second = min_y;
            path.push_back(current);

            if(c++>ns*4){
                return false;
            }

        }
        return true;
    }
    float NavPath::calculatePotential(float *potential, unsigned char cost, int n, float prev_potential) {
        // get neighbors
        float u, d, l, r;
        l = potential[n - 1];
        r = potential[n + 1];
        u = potential[n - nx_];
        d = potential[n + nx_];

        // find lowest, and its lowest neighbor
        float ta, tc;
        if (l < r)
            tc = l;
        else
            tc = r;
        if (u < d)
            ta = u;
        else
            ta = d;

        float hf = cost; // traversability factor
        float dc = tc - ta;        // relative cost between ta,tc
        if (dc < 0)         // tc is lowest
        {
            dc = -dc;
            ta = tc;
        }

        // calculate new potential
        if (dc >= hf)        // if too large, use ta-only update
            return ta + hf;
        else            // two-neighbor interpolation update
        {
            // use quadratic approximation
            // might speed this up through table lookup, but still have to
            //   do the divide
            d = dc / hf;
            auto v = (float) (-0.2301 * d * d + 0.5307 * d + 0.7040);
            return ta + hf * v;
        }
    }
    void NavPath::outlineMap(unsigned char* costarr, int nx, int ny, unsigned char value) {
        unsigned char* pc = costarr;
        for (int i = 0; i < nx; i++)
            *pc++ = value;
        pc = costarr + (ny - 1) * nx;
        for (int i = 0; i < nx; i++)
            *pc++ = value;
        pc = costarr;
        for (int i = 0; i < ny; i++, pc += nx)
            *pc = value;
        pc = costarr + nx - 1;
        for (int i = 0; i < ny; i++, pc += nx)
            *pc = value;
    }
    void NavPath::RasterLine(std::pair<Vector2d, Vector2d> line, std::vector<Vector2d>& linePointList){
        Vector2d vecLine = line.second - line.first;
        double lineLength = vecLine.Mod();
        double step = 1.0;

        //根据距离逐步取
        vector<Vector2d> tmpPointList;
        double curLength = 0;
        while (curLength < lineLength)
        {
            curLength = curLength + step;
            Vector2d P = line.first + vecLine.Scalar(curLength / lineLength);
            P.x = (P.x + 0.5);
            P.y = (P.y + 0.5);
            tmpPointList.push_back(P);
        }

        //与最后一个值比较，去重
        linePointList.push_back(line.first);
        for (auto &i : tmpPointList) {
            //与最后一个值比较，去重
            if (!i.Equel(linePointList[linePointList.size() - 1]))
            {
                linePointList.push_back(i);
            }
        }

        if (!linePointList[linePointList.size() - 1].Equel(line.second))
        {
            linePointList.push_back(line.second);
        }
        tmpPointList.clear();
        vector<Vector2d>().swap(tmpPointList);
    }
    void NavPath::RasterLine_0(Point start , Point end , vector<Point>& linePointList){
        linePointList.clear();
        int dlt_x = start.x - end.x;
        int dlt_y = start.y - end.y;
//    printf("s(%d,%d) , e(%d,%d)\n",start.x,start.y,end.x,end.y);
//    printf("dlt_x:%d , dlt_y:%d\n",dlt_x,dlt_y);
        int i_min = 0;
        int i_max = 0;
        if(dlt_x == 0){
//        printf("1\n");
            i_min = min(start.y,end.y);
            i_max = max(start.y,end.y);
            for(int i = i_min ; i <= i_max ; i++){
                linePointList.emplace_back(Point(start.x,i));
            }
        }else if(dlt_y == 0){
//        printf("2\n");
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
//            printf("3\n");
            for (int i = i_min; i <= i_max; i++) {
                linePointList.emplace_back(Point(i, (i * a + b)));
            }
        }else if(abs(dlt_y) > abs(dlt_x)){
            i_min = start.y;
            i_max = end.y;
//        printf("i_min:%d , i_max:%d\n",i_min,i_max);
            float a = (float)dlt_y/dlt_x;
            float b= (start.y - a*start.x);
            if(dlt_y > 0){
//            printf("5\n");
                for(int i = i_min ; i >= i_max ; i--){
                    linePointList.emplace_back(Point((int)floor((i - 0.5 - b)/a),i));
//                printf("(%d,%d)\n",(int)ceil((i - 0.5 - b)/a),i);
                }
            }else{
//            printf("6\n");
                for(int i = i_min ; i <= i_max ; i++){
                    linePointList.emplace_back(Point((int)ceil((i + 0.5 - b)/a),i));
//                printf("(%d,%d)\n",(int)floor((i + 0.5 - b)/a),i);
                }
            }
        }else{
            i_min = start.x;
            i_max = end.x;
//        printf("i_min:%d , i_max:%d\n",i_min,i_max);
            float a = (float)dlt_y/dlt_x;
            float b= (start.y - a*start.x);
            if(dlt_x > 0){
//            printf("7\n");
                for(int i = i_min ; i >= i_max ; i--){
                    linePointList.emplace_back(Point(i,(int)ceil(a*(i + 0.5) + b)));
//                printf("(%d,%d)\n",i,(int)ceil(a*(i - 0.5) + b));
                }
            }else{
//            printf("8\n");
                for(int i = i_min ; i <= i_max ; i++){
                    linePointList.emplace_back(Point(i,(int)floor(a*(i - 0.5) + b)));
//                printf("(%d,%d)\n",i,(int)floor(a*(i + 0.5) + b));
                }
            }
        }
    }

} //end namespace global_planner
