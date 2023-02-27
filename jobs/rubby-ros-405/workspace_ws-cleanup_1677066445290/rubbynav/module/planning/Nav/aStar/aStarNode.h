//
// Created by lcy on 22-8-16.
//

#ifndef TEST_ASTARNODE_H
#define TEST_ASTARNODE_H

#include <iostream>
#include <vector>
#include<opencv2/highgui/highgui.hpp>
#include "Common.h"

using namespace cv;

class node {
public:
    node(int _x = 0, int _y = 0, int _g = 0, int _h = 0, int _c = 0);
    ~node();
    void setObstacle();
    bool isObstacle();
    bool operator==(const node& rhs);
    node& operator+=(node rhs);
    node& operator=(node rhs);
    int coordinateX() const;
    int coordinateY() const;
    void setParentpoint(node* rhs);
    node* returnparentpoint();
    void setX(int _x);
    void setY(int _y);
    void setC(int _c);
    void setH(int _h);
    void setG(int _g);
    void setF(int _f);
    int h_();
    int g_();
    int f_();
    int c_();
    bool returnIsVisited();
    void setIsVisited();
private:
    int x, y;
    int f;
    int g;
    int h;
    int cost;
    bool obstacle;
    node* parentpoint;
    bool isVisited;
};

#endif //TEST_ASTARNODE_H
