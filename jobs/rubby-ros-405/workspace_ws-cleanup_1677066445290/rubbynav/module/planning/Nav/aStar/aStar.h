//
// Created by lcy on 22-8-16.
//

#ifndef TEST_ASTAR_H
#define TEST_ASTAR_H

#include<opencv2/opencv.hpp>
#include <list>
#include <queue>
#include "aStarNode.h"

using namespace std;

class aStar{

public:
    aStar(){};
    ~aStar(){};

    bool aStartest(const Mat & map , Point start , Point target , vector<Point> &path);

private:
    bool resetpoint(Mat &src , Point & p);
    bool resetpoint_min(Mat &src , Point & p);
    int aStartest();
    int add_cost_full(Mat &src);
    bool selectpose(std::vector<Point>& path);
    int calHeristic(node* n1, node n2);
    bool verifyNode(node rhs);
    bool astar(const node& startNode, const node& goalNode, vector<vector<node*>>& roadmap , vector<Point> &path);

    void RasterLine_0(Point start , Point end , vector<Point>& linePointList);
    bool selectpose_0(Mat & src , std::vector<Point>& path);

    int mapSize_X = 1500;
    int mapSize_Y = 1500;
};


#endif //TEST_ASTAR_H
