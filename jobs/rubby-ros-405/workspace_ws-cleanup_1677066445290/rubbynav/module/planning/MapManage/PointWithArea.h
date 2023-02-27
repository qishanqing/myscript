//
// Created by lcy on 22-10-24.
//

#ifndef TEST_POINTWITHAREA_H
#define TEST_POINTWITHAREA_H

#include <cstdio>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PointWithArea{
private:
    double eps = 1e-10;
    double Cross_p2p(Point a,Point b)
    {
        return a.x*b.y-b.x*a.y;
    }
    bool atedge(Point p0,Point p1,Point p2)
    {
        return fabs(Cross_p2p((p0 - p1) , (p2 - p1)))<eps &&
                min(p1.x,p2.x) - eps <= p0.x &&
                p0.x - eps <= max(p1.x,p2.x) &&
                min(p1.y,p2.y) - eps <= p0.y &&
                p0.y - eps <= max(p1.y,p2.y);//边上判断，使用叉积
    }
    bool cross(Point p1,Point p2,Point p3,Point p4)
    {
        if(!(max(p1.x,p2.x) >= min(p3.x,p4.x) + eps &&
            max(p3.x,p4.x)>= min(p1.x,p2.x) + eps))
            return false;
        if(!(max(p1.y,p2.y) >= min(p3.y,p4.y) + eps &&
            max(p3.y,p4.y) >= min(p1.y,p2.y) + eps))
            return false;
        if((Cross_p2p((p3-p1) , (p2-p1))) * (Cross_p2p((p4-p1) , (p2-p1))) > eps)
            return false;
        if((Cross_p2p((p1-p3) , (p4-p3))) * (Cross_p2p((p2-p3) , (p4-p3))) > eps)
            return false;
        return true;
    }
    vector<Point> DIR = {Point(0,0),Point(1200 - 1,1200 - 1),Point(0,1200 - 1),Point(1200 - 1,0)};
public:
    int CheckPoint2Area(vector<Point> d, Point p)
    {
        if(d.empty()){
            return 0;//区域数据为空。
        }
        for(int i = 0 ; i < d.size() ; i++){
            if(atedge(p,d[i],d[i == d.size() - 1 ? 0:i+1])){
                return 1;//边上
            }
        }
        Point prand;
        bool flag;
        /*while(true)
        {
            flag = true;
            prand.x = (int)((random()<<15) + random() + 1e7);
            prand.y = (int)((random()<<15) + random() + 1e7);
            for(int i = 0 ; i < d.size() && flag ; i++) {
                if (atedge(d[i], p, prand)){
                    flag = false;
                }
            }
            if(flag) break;
        }*/
        for(int k = 0 ; k < DIR.size() ; k++){
            flag = true;
            prand = DIR[k];
            for(int i = 0 ; i < d.size() && flag ; i++) {
                if (atedge(d[i], p, prand)){
                    flag = false;
                }
            }
            if(flag) break;
        }
        int sum = 0;
        for(int i = 0 ; i < d.size() ; i++){
            sum += cross(p,prand,d[i],d[i == d.size() - 1 ? 1:i+1]);
        }
        if(sum%2){
            return 2;//内部
        }else {
            return 3;//外部
        }
    }

};


#endif //TEST_POINTWITHAREA_H
