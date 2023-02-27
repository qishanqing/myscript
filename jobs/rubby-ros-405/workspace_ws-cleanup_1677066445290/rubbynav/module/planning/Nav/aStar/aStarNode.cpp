//
// Created by lcy on 22-8-16.
//

#include "aStarNode.h"


node::node(int _x, int _y, int _g, int _h, int _c)
        : x(_x),
          y(_y),
          h(_h),
          g(_g),
          cost(_c),
          obstacle(false),
          parentpoint(nullptr),
          isVisited(false)
{}

node::~node() {

}

void node::setObstacle() {
    obstacle = true;
}

bool node::isObstacle() {
    return obstacle;
}

node& node::operator+=(node rhs) {
    x += rhs.x;
    y += rhs.y;
    g += rhs.g;
    return *this;
}

node& node::operator=(node rhs) {
    x = rhs.x;
    y = rhs.y;
    g = rhs.g;
    return *this;
}

bool node::operator==(const node& rhs) {
    return x == rhs.x && y == rhs.y;
}

int node::coordinateX() const {
    return x;
}

int node::coordinateY() const{
    return y;
}

void node::setParentpoint(node* rhs) {
    parentpoint = rhs;
}

node* node::returnparentpoint() {
    return parentpoint;
}
void node::setC(int _c) {
    cost = _c;
}
void node::setH(int _h) {
    h = _h;
}
void node::setG(int _g) {
    g = _g;
}
void node::setF(int _f) {
    f = _f;
}
void node::setX(int _x) {
    x = _x;
}
void node::setY(int _y) {
    y = _y;
}


int node::h_() {
    return h;
}

int node::g_() {
    return g;
}
int node::f_() {
    return f;
}
int node::c_() {
    return cost;
}

bool node::returnIsVisited() {
    return isVisited;
}

void node::setIsVisited() {
    isVisited = true;
}