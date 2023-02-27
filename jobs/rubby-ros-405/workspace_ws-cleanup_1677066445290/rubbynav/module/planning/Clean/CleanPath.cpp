//
// Created by lcy on 21-6-24.
//

#include "CleanPath.h"
#include <ctime>
#include <chrono>
#include "Map.h"

int CleanPath::find_corner_point(const Mat & src , Point input , Point p_min , Point p_max){
    if(src.empty()){
        LOGPLANNING(INFO) << "地图数据为空 !!!";
        return 0;
    }
    if(input.x < p_min.x || input.y < p_min.y || input.x > p_max.x || input.y > p_max.y){
        LOGPLANNING(INFO) << "起点超出范围: (" << input.x << ", " << input.y << ")";
        return 0;
    }
    LOGPLANNING(INFO) << "corner input: (" << input.x << ", " << input.y << ")";
    LOGPLANNING(INFO) << "corner p_min: (" << p_min.x << ", " << p_min.y << ") p_max: (" << p_max.x << ", " << p_max.y << ")";
    Point p_up = input,p_down = input,p_left = input,p_right = input;

    Point maprange_min = p_min;
    Point maprange_max = p_max;

    int x_,y_;

    if(Dir_Cleanning_Path == SPECIFIED_LOCK_DIE_X){
        LOGPLANNING(INFO) << "SPECIFIED_LOCK_DIE_X";
        x_ = p_right.x;
        y_ = p_right.y;
        while(true){
            if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    x_ ++;
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }else{
                if(x_ > p_right.x){
                    p_right.x = x_ - 1;
                }
                break;
            }
        }
        x_ = p_left.x;
        y_ = p_left.y;
        while(true){
            if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    x_ --;
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }else{
                if(x_ < p_left.x){
                    p_left.x = x_ + 1;
                }
                break;
            }
        }
        for(int i = p_left.x ; i <= p_right.x ; i ++){
            x_ = i;
            y_ = input.y;
            while(true) {
                if (y_ >= maprange_min.y && y_ <= maprange_max.y) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        y_++;
                    } else {
                        if (y_ > p_up.y) {
                            int X = i;
                            int Y = y_ - 1;
                            int X_min = 0;
                            int X_max = 0;
                            while(true){
                                if (X >= maprange_min.x && X <= maprange_max.x) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X --;
                                    }else{
                                        if(X < i){
                                            X_min = X ++;
                                        }else{
                                            X_min = X;
                                        }
                                        break;
                                    }
                                }else{
                                    X_min = maprange_min.x;
                                    break;
                                }
                            }
                            X = i;
                            Y = y_ - 1;
                            while(true){
                                if (X >= maprange_min.x && X <= maprange_max.x) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X ++;
                                    }else{
                                        if(X > i){
                                            X_max = X --;
                                        }else{
                                            X_max = X;
                                        }
                                        break;
                                    }
                                }else{
                                    X_max = maprange_max.x;
                                    break;
                                }
                            }
                            if(abs(X_max - X_min) > 5){
                                p_up.x = i;
                                p_up.y = y_ - 1;
                            }
                        }
                        break;
                    }
                }else{
                    if (y_ > p_up.y) {
                        int X = i;
                        int Y = y_ - 1;
                        int X_min = 0;
                        int X_max = 0;
                        while(true){
                            if (X >= maprange_min.x && X <= maprange_max.x) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X --;
                                }else{
                                    if(X < i){
                                        X_min = X ++;
                                    }else{
                                        X_min = X;
                                    }
                                    break;
                                }
                            }else{
                                X_min = maprange_min.x;
                                break;
                            }
                        }
                        X = i;
                        Y = y_ - 1;
                        while(true){
                            if (X >= maprange_min.x && X <= maprange_max.x) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X ++;
                                }else{
                                    if(X > i){
                                        X_max = X --;
                                    }else{
                                        X_max = X;
                                    }
                                    break;
                                }
                            }else{
                                X_max = maprange_max.x;
                                break;
                            }
                        }
                        if(abs(X_max - X_min) > 5){
                            p_up.x = i;
                            p_up.y = y_ - 1;
                        }
                    }
                    break;
                }
            }
            x_ = i;
            y_ = input.y;
            while(true) {
                if (y_ >= maprange_min.y && y_ <= maprange_max.y) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        y_--;
                    } else {
                        if (y_ < p_down.y) {
                            int X = i;
                            int Y = y_ + 1;
                            int X_min = 0;
                            int X_max = 0;
                            while(true){
                                if (X >= maprange_min.x && X <= maprange_max.x) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X --;
                                    }else{
                                        if(X < i){
                                            X_min = (X + 1);
                                        }else{
                                            X_min = X;
                                        }
                                        break;
                                    }
                                }else{
                                    X_min = maprange_min.x;
                                    break;
                                }
                            }
                            X = i;
                            Y = y_ + 1;
                            while(true){
                                if (X >= maprange_min.x && X <= maprange_max.x) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X ++;
                                    }else{
                                        if(X > i){
                                            X_max = (X - 1);
                                        }else{
                                            X_max = X;
                                        }
                                        break;
                                    }
                                }else{
                                    X_max = maprange_max.x;
                                    break;
                                }
                            }
                            if(abs(X_max - X_min) > 5){
                                p_down.x = i;
                                p_down.y = y_ + 1;
                            }
                        }
                        break;
                    }
                }else{
                    if (y_ < p_down.y) {
                        int X = i;
                        int Y = y_ + 1;
                        int X_min = 0;
                        int X_max = 0;
                        while(true){
                            if (X >= maprange_min.x && X <= maprange_max.x) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X --;
                                }else{
                                    if(X < i){
                                        X_min = (X + 1);
                                    }else{
                                        X_min = X;
                                    }
                                    break;
                                }
                            }else{
                                X_min = maprange_min.x;
                                break;
                            }
                        }
                        X = i;
                        Y = y_ + 1;
                        while(true){
                            if (X >= maprange_min.x && X <= maprange_max.x) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X ++;
                                }else{
                                    if(X > i){
                                        X_max = (X - 1);
                                    }else{
                                        X_max = X;
                                    }
                                    break;
                                }
                            }else{
                                X_max = maprange_max.x;
                                break;
                            }
                        }
                        if(abs(X_max - X_min) > 5){
                            p_down.x = i;
                            p_down.y = y_ + 1;
                        }
                    }
                    break;
                }
            }
        }
        LOGPLANNING(INFO) << "left: (" << p_left.x << ", " << p_left.y << ") right: (" << p_right.x << ", " << p_right.y << ") up: (" << p_up.x << ", " << p_up.y << ") down: (" << p_down.x << ", " << p_down.y << ")";
        if(abs(p_up.y - input.y) > abs(input.y - p_down.y)){
            dir_clean_lock = 1;//锁Y轴+
            x_ = p_down.x;
            y_ = p_down.y;
            p_right.x = x_;
            while(true){
                if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ ++;
                    }else{
                        if(x_ > p_right.x){
                            p_right.x = x_ - 1;
                        }
                        break;
                    }
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }
            x_ = p_down.x;
            y_ = p_down.y;
            p_left.x = x_;
            while(true){
                if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ --;
                    }else{
                        if(x_ < p_left.x){
                            p_left.x = x_ + 1;
                        }
                        break;
                    }
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "+ left: (" << p_left.x << ") right: (" << p_right.x << ")";
            if(abs(input.x - p_left.x) > abs(p_right.x - input.x)){
                dir_clean_path_first = 2;//X-方向
            }else{
                dir_clean_path_first = 1;//X+方向
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }else{
            dir_clean_lock = 2;//锁Y轴-
            x_ = p_up.x;
            y_ = p_up.y;
            p_right.x = x_;
            while(true){
                if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ ++;
                    }else{
                        if(x_ > p_right.x){
                            p_right.x = x_ - 1;
                        }
                        break;
                    }
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }
            x_ = p_up.x;
            y_ = p_up.y;
            p_left.x = x_;
            while(true){
                if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ --;
                    }else{
                        if(x_ < p_left.x){
                            p_left.x = x_ + 1;
                        }
                        break;
                    }
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "- left: (" << p_left.x << ") right: (" << p_right.x << ")";
            if(abs(input.x - p_left.x) > abs(p_right.x - input.x)){
                dir_clean_path_first = 2;//X-方向
            }else{
                dir_clean_path_first = 1;//X+方向
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }

    }else if(Dir_Cleanning_Path == SPECIFIED_LOCK_DIE_Y){
        LOGPLANNING(INFO) << "SPECIFIED_LOCK_DIE_Y";
        x_ = p_up.x;
        y_ = p_up.y;
        while(true){
            if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    y_ ++;
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }else{
                if(y_ > p_up.y){
                    p_up.y = y_ - 1;
                }
                break;
            }
        }
        x_ = p_down.x;
        y_ = p_down.y;
        while(true){
            if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    y_ --;
                }else{
                    if(y_ < p_down.y){
                        p_down.x = y_ + 1;
                    }
                    break;
                }
            }else{
                if(y_ < p_down.y){
                    p_down.y = y_ + 1;
                }
                break;
            }
        }
        for(int i = p_down.y ; i <= p_up.y ; i ++){
            x_ = input.x;
            y_ = i;
            while(true) {
                if (x_ >= maprange_min.x && x_ <= maprange_max.x) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        x_++;
                    } else {
                        if (x_ > p_right.x) {
                            int X = x_ - 1;
                            int Y = i;
                            int Y_min = 0;
                            int Y_max = 0;
                            while(true){
                                if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y --;
                                    }else{
                                        if(Y < i){
                                            Y_min = (Y + 1);
                                        }else{
                                            Y_min = Y;
                                        }
                                        break;
                                    }
                                }else{
                                    Y_min = maprange_min.y;
                                    break;
                                }
                            }
                            X = x_ - 1;
                            Y = i;
                            while(true){
                                if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y ++;
                                    }else{
                                        if(Y > i){
                                            Y_max = (Y - 1);
                                        }else{
                                            Y_max = Y;
                                        }
                                        break;
                                    }
                                }else{
                                    Y_max = maprange_max.y;
                                    break;
                                }
                            }
                            if(abs(Y_max - Y_min) > 5){
                                p_right.x = x_ - 1;
                                p_right.y = i;
                            }
                        }
                        break;
                    }
                }else{
                    if (x_ > p_right.x) {
                        int X = x_ - 1;
                        int Y = i;
                        int Y_min = 0;
                        int Y_max = 0;
                        while(true){
                            if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y --;
                                }else{
                                    if(Y < i){
                                        Y_min = (Y + 1);
                                    }else{
                                        Y_min = Y;
                                    }
                                    break;
                                }
                            }else{
                                Y_min = maprange_min.y;
                                break;
                            }
                        }
                        X = x_ - 1;
                        Y = i;
                        while(true){
                            if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y ++;
                                }else{
                                    if(Y > i){
                                        Y_max = (Y - 1);
                                    }else{
                                        Y_max = Y;
                                    }
                                    break;
                                }
                            }else{
                                Y_max = maprange_max.y;
                                break;
                            }
                        }
                        if(abs(Y_max - Y_min) > 5){
                            p_right.x = x_ - 1;
                            p_right.y = i;
                        }
                    }
                    break;
                }
            }
            x_ = input.x;
            y_ = i;
            while(true) {
                if (x_ >= maprange_min.x && x_ <= maprange_max.x) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        x_--;
                    } else {
                        if (x_ < p_left.x) {
                            int X = x_ + 1;
                            int Y = i;
                            int Y_min = 0;
                            int Y_max = 0;
                            while(true){
                                if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y --;
                                    }else{
                                        if(Y < i){
                                            Y_min = (Y + 1);
                                        }else{
                                            Y_min = Y;
                                        }
                                        break;
                                    }
                                }else{
                                    Y_min = maprange_min.y;
                                    break;
                                }
                            }
                            X = x_ + 1;
                            Y = i;
                            while(true){
                                if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y ++;
                                    }else{
                                        if(Y > i){
                                            Y_max = (Y - 1);
                                        }else{
                                            Y_max = Y;
                                        }
                                        break;
                                    }
                                }else{
                                    Y_max = maprange_max.y;
                                    break;
                                }
                            }
                            if(abs(Y_max - Y_min) > 5){
                                p_left.x = x_ + 1;
                                p_left.y = i;
                            }

                        }
                        break;
                    }
                }else{
                    if (y_ < p_left.y) {
                        int X = x_ + 1;
                        int Y = i;
                        int Y_min = 0;
                        int Y_max = 0;
                        while(true){
                            if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y --;
                                }else{
                                    if(Y < i){
                                        Y_min = (Y + 1);
                                    }else{
                                        Y_min = Y;
                                    }
                                    break;
                                }
                            }else{
                                Y_min = maprange_min.y;
                                break;
                            }
                        }
                        X = x_ + 1;
                        Y = i;
                        while(true){
                            if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y ++;
                                }else{
                                    if(Y > i){
                                        Y_max = (Y - 1);
                                    }else{
                                        Y_max = Y;
                                    }
                                    break;
                                }
                            }else{
                                Y_max = maprange_max.y;
                                break;
                            }
                        }
                        if(abs(Y_max - Y_min) > 5){
                            p_left.x = x_ + 1;
                            p_left.y = i;
                        }
                    }
                    break;
                }
            }
        }
        LOGPLANNING(INFO) << "left: (" << p_left.x << ", " << p_left.y << ") right: (" << p_right.x << ", " << p_right.y << ") up: (" << p_up.x << ", " << p_up.y << ") down: (" << p_down.x << ", " << p_down.y << ")";
        if(abs(p_right.x - input.x) > abs(input.x - p_left.x)){
            dir_clean_lock = 3;//锁X轴+
            x_ = p_left.x;
            y_ = p_left.y;
            p_up.y = y_;
            while(true){
                if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ ++;
                    }else{
                        if(y_ > p_up.y){
                            p_up.y = y_ - 1;
                        }
                        break;
                    }
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }
            x_ = p_left.x;
            y_ = p_left.y;
            p_down.y = y_;
            while(true){
                if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ --;
                    }else{
                        if(y_ < p_down.y){
                            p_down.y = y_ + 1;
                        }
                        break;
                    }
                }else{
                    if(y_ < p_down.y){
                        p_down.y = y_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "+ down: (" << p_down.y << ") up: (" << p_up.y << ")";
            if(abs(input.y - p_down.y) > abs(p_up.y - input.y)){
                dir_clean_path_first = 2;//Y-方向
            }else{
                dir_clean_path_first = 1;//Y+方向
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }else{
            dir_clean_lock = 4;//锁X轴-
            x_ = p_right.x;
            y_ = p_right.y;
            p_up.y = y_;
            while(true){
                if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ ++;
                    }else{
                        if(y_ > p_up.y){
                            p_up.y = y_ - 1;
                        }
                        break;
                    }
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }
            x_ = p_right.x;
            y_ = p_right.y;
            p_down.y = y_;
            while(true){
                if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ --;
                    }else{
                        if(y_ < p_down.y){
                            p_down.y = y_ + 1;
                        }
                        break;
                    }
                }else{
                    if(y_ < p_down.y){
                        p_down.y = y_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "- down: (" << p_down.y << ") up: (" << p_up.y << ")";
            if(abs(input.y - p_down.y) > abs(p_up.y - input.y)){
                dir_clean_path_first = 2;//Y-方向
            }else{
                dir_clean_path_first = 1;//Y+方向
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }

    }
    return 1;
}
int CleanPath::find_corner_point_mark(const Mat & src , const Mat & mark , Point input , int id){
    if(src.empty()){
        LOGPLANNING(INFO) << "src is empty !!!";
        return 0;
    }
    if(mark.empty()){
        LOGPLANNING(INFO) << "mark is empty !!!";
        return 0;
    }
    if(mark.at<uint8_t >(input) != id){
        LOGPLANNING(INFO) << "起点超出范围: (" << input.x << ", " << input.y << ")-id:"<<mark.at<uint8_t >(input)<< " ,id:"<<id;
        return 0;
    }
    LOGPLANNING(INFO) << "corner input: (" << input.x << ", " << input.y << ")";
    Point p_up = input,p_down = input,p_left = input,p_right = input;

//    Point maprange_min = p_min;
//    Point maprange_max = p_max;

    int x_,y_;

    if(Dir_Cleanning_Path == SPECIFIED_LOCK_DIE_X){
        LOGPLANNING(INFO) << "SPECIFIED_LOCK_DIE_X";
        x_ = p_right.x;
        y_ = p_right.y;
        while(true){
            if(mark.at<uint8_t >(y_,x_) == id){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    x_ ++;
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }else{
                if(x_ > p_right.x){
                    p_right.x = x_ - 1;
                }
                break;
            }
        }
        x_ = p_left.x;
        y_ = p_left.y;
        while(true){
            if(mark.at<uint8_t >(y_,x_) == id){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    x_ --;
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }else{
                if(x_ < p_left.x){
                    p_left.x = x_ + 1;
                }
                break;
            }
        }
        for(int i = p_left.x ; i <= p_right.x ; i ++){
            x_ = i;
            y_ = input.y;
            while(true) {
                if (mark.at<uint8_t >(y_,x_) == id) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        y_++;
                    } else {
                        if (y_ > p_up.y) {
                            int X = i;
                            int Y = y_ - 1;
                            int X_min = 0;
                            int X_max = 0;
                            while(true){
                                if (mark.at<uint8_t >(Y,X) == id) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X --;
                                    }else{
                                        if(X < i){
                                            X_min = X ++;
                                        }else{
                                            X_min = X;
                                        }
                                        break;
                                    }
                                }else{
//                                    X_min = maprange_min.x;
                                    X_min = X;/*---------------------??????????---------------------------*/
                                    break;
                                }
                            }
                            X = i;
                            Y = y_ - 1;
                            while(true){
                                if (mark.at<uint8_t >(Y,X) == id) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X ++;
                                    }else{
                                        if(X > i){
                                            X_max = X --;
                                        }else{
                                            X_max = X;
                                        }
                                        break;
                                    }
                                }else{
//                                    X_max = maprange_max.x;
                                    X_max = X;/*---------------------??????????---------------------------*/
                                    break;
                                }
                            }
                            if(abs(X_max - X_min) > 5){
                                p_up.x = i;
                                p_up.y = y_ - 1;
                            }
                        }
                        break;
                    }
                }else{
                    if (y_ > p_up.y) {
                        int X = i;
                        int Y = y_ - 1;
                        int X_min = 0;
                        int X_max = 0;
                        while(true){
                            if (mark.at<uint8_t >(Y,X) == id) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X --;
                                }else{
                                    if(X < i){
                                        X_min = X ++;
                                    }else{
                                        X_min = X;
                                    }
                                    break;
                                }
                            }else{
//                                X_min = maprange_min.x;
                                X_min = X;/*---------------------??????????---------------------------*/
                                break;
                            }
                        }
                        X = i;
                        Y = y_ - 1;
                        while(true){
                            if (mark.at<uint8_t >(Y,X) == id) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X ++;
                                }else{
                                    if(X > i){
                                        X_max = X --;
                                    }else{
                                        X_max = X;
                                    }
                                    break;
                                }
                            }else{
//                                X_max = maprange_max.x;
                                X_max = X;/*---------------------??????????---------------------------*/
                                break;
                            }
                        }
                        if(abs(X_max - X_min) > 5){
                            p_up.x = i;
                            p_up.y = y_ - 1;
                        }
                    }
                    break;
                }
            }
            x_ = i;
            y_ = input.y;
            while(true) {
                if (mark.at<uint8_t >(y_,x_) == id) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        y_--;
                    } else {
                        if (y_ < p_down.y) {
                            int X = i;
                            int Y = y_ + 1;
                            int X_min = 0;
                            int X_max = 0;
                            while(true){
                                if (mark.at<uint8_t >(Y,X) == id) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X --;
                                    }else{
                                        if(X < i){
                                            X_min = (X + 1);
                                        }else{
                                            X_min = X;
                                        }
                                        break;
                                    }
                                }else{
//                                    X_min = maprange_min.x;
                                    X_min = X;/*---------------------??????????---------------------------*/
                                    break;
                                }
                            }
                            X = i;
                            Y = y_ + 1;
                            while(true){
                                if (mark.at<uint8_t >(Y,X) == id) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X ++;
                                    }else{
                                        if(X > i){
                                            X_max = (X - 1);
                                        }else{
                                            X_max = X;
                                        }
                                        break;
                                    }
                                }else{
//                                    X_max = maprange_max.x;
                                    X_max = X;/*---------------------??????????---------------------------*/
                                    break;
                                }
                            }
                            if(abs(X_max - X_min) > 5){
                                p_down.x = i;
                                p_down.y = y_ + 1;
                            }
                        }
                        break;
                    }
                }else{
                    if (y_ < p_down.y) {
                        int X = i;
                        int Y = y_ + 1;
                        int X_min = 0;
                        int X_max = 0;
                        while(true){
                            if (mark.at<uint8_t >(Y,X) == id) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X --;
                                }else{
                                    if(X < i){
                                        X_min = (X + 1);
                                    }else{
                                        X_min = X;
                                    }
                                    break;
                                }
                            }else{
//                                X_min = maprange_min.x;
                                X_min = X;/*---------------------??????????---------------------------*/
                                break;
                            }
                        }
                        X = i;
                        Y = y_ + 1;
                        while(true){
                            if (mark.at<uint8_t >(Y,X) == id) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X ++;
                                }else{
                                    if(X > i){
                                        X_max = (X - 1);
                                    }else{
                                        X_max = X;
                                    }
                                    break;
                                }
                            }else{
//                                X_max = maprange_max.x;
                                X_max = X;/*---------------------??????????---------------------------*/
                                break;
                            }
                        }
                        if(abs(X_max - X_min) > 5){
                            p_down.x = i;
                            p_down.y = y_ + 1;
                        }
                    }
                    break;
                }
            }
        }
        LOGPLANNING(INFO) << "left: (" << p_left.x << ", " << p_left.y << ") right: (" << p_right.x << ", " << p_right.y << ") up: (" << p_up.x << ", " << p_up.y << ") down: (" << p_down.x << ", " << p_down.y << ")";
        if(abs(p_up.y - input.y) > abs(input.y - p_down.y)){
            dir_clean_lock = 1;//锁Y轴+
            x_ = p_down.x;
            y_ = p_down.y;
            p_right.x = x_;
            while(true){
                if(mark.at<uint8_t >(y_,x_) == id){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ ++;
                    }else{
                        if(x_ > p_right.x){
                            p_right.x = x_ - 1;
                        }
                        break;
                    }
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }
            x_ = p_down.x;
            y_ = p_down.y;
            p_left.x = x_;
            while(true){
                if(mark.at<uint8_t >(y_,x_) == id){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ --;
                    }else{
                        if(x_ < p_left.x){
                            p_left.x = x_ + 1;
                        }
                        break;
                    }
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "+ left: (" << p_left.x << ") right: (" << p_right.x << ")";
            if(abs(input.x - p_left.x) > abs(p_right.x - input.x)){
                dir_clean_path_first = 2;//X-方向
            }else{
                dir_clean_path_first = 1;//X+方向
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }else{
            dir_clean_lock = 2;//锁Y轴-
            x_ = p_up.x;
            y_ = p_up.y;
            p_right.x = x_;
            while(true){
                if(mark.at<uint8_t >(y_,x_) == id){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ ++;
                    }else{
                        if(x_ > p_right.x){
                            p_right.x = x_ - 1;
                        }
                        break;
                    }
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }
            x_ = p_up.x;
            y_ = p_up.y;
            p_left.x = x_;
            while(true){
                if(mark.at<uint8_t >(y_,x_) == id){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ --;
                    }else{
                        if(x_ < p_left.x){
                            p_left.x = x_ + 1;
                        }
                        break;
                    }
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "- left: (" << p_left.x << ") right: (" << p_right.x << ")";
            if(abs(input.x - p_left.x) > abs(p_right.x - input.x)){
                dir_clean_path_first = 2;//X-方向
            }else{
                dir_clean_path_first = 1;//X+方向
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }

    }else if(Dir_Cleanning_Path == SPECIFIED_LOCK_DIE_Y){
        LOGPLANNING(INFO) << "SPECIFIED_LOCK_DIE_Y";
        x_ = p_up.x;
        y_ = p_up.y;
        while(true){
            if(mark.at<uint8_t >(y_,x_) == id){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    y_ ++;
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }else{
                if(y_ > p_up.y){
                    p_up.y = y_ - 1;
                }
                break;
            }
        }
        x_ = p_down.x;
        y_ = p_down.y;
        while(true){
            if(mark.at<uint8_t >(y_,x_) == id){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    y_ --;
                }else{
                    if(y_ < p_down.y){
                        p_down.x = y_ + 1;
                    }
                    break;
                }
            }else{
                if(y_ < p_down.y){
                    p_down.y = y_ + 1;
                }
                break;
            }
        }
        for(int i = p_down.y ; i <= p_up.y ; i ++){
            x_ = input.x;
            y_ = i;
            while(true) {
                if (mark.at<uint8_t >(y_,x_) == id) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        x_++;
                    } else {
                        if (x_ > p_right.x) {
                            int X = x_ - 1;
                            int Y = i;
                            int Y_min = 0;
                            int Y_max = 0;
                            while(true){
                                if (mark.at<uint8_t >(Y,X) == id) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y --;
                                    }else{
                                        if(Y < i){
                                            Y_min = (Y + 1);
                                        }else{
                                            Y_min = Y;
                                        }
                                        break;
                                    }
                                }else{
//                                    Y_min = maprange_min.y;
                                    Y_min = Y;/*---------------------??????????---------------------------*/
                                    break;
                                }
                            }
                            X = x_ - 1;
                            Y = i;
                            while(true){
                                if (mark.at<uint8_t >(Y,X) == id) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y ++;
                                    }else{
                                        if(Y > i){
                                            Y_max = (Y - 1);
                                        }else{
                                            Y_max = Y;
                                        }
                                        break;
                                    }
                                }else{
//                                    Y_max = maprange_max.y;
                                    Y_max = Y;/*---------------------??????????---------------------------*/
                                    break;
                                }
                            }
                            if(abs(Y_max - Y_min) > 5){
                                p_right.x = x_ - 1;
                                p_right.y = i;
                            }
                        }
                        break;
                    }
                }else{
                    if (x_ > p_right.x) {
                        int X = x_ - 1;
                        int Y = i;
                        int Y_min = 0;
                        int Y_max = 0;
                        while(true){
                            if (mark.at<uint8_t >(Y,X) == id) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y --;
                                }else{
                                    if(Y < i){
                                        Y_min = (Y + 1);
                                    }else{
                                        Y_min = Y;
                                    }
                                    break;
                                }
                            }else{
//                                Y_min = maprange_min.y;
                                Y_min = Y;/*---------------------??????????---------------------------*/
                                break;
                            }
                        }
                        X = x_ - 1;
                        Y = i;
                        while(true){
                            if (mark.at<uint8_t >(Y,X) == id) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y ++;
                                }else{
                                    if(Y > i){
                                        Y_max = (Y - 1);
                                    }else{
                                        Y_max = Y;
                                    }
                                    break;
                                }
                            }else{
//                                Y_max = maprange_max.y;
                                Y_max = Y;/*---------------------??????????---------------------------*/
                                break;
                            }
                        }
                        if(abs(Y_max - Y_min) > 5){
                            p_right.x = x_ - 1;
                            p_right.y = i;
                        }
                    }
                    break;
                }
            }
            x_ = input.x;
            y_ = i;
            while(true) {
                if (mark.at<uint8_t >(y_,x_) == id) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        x_--;
                    } else {
                        if (x_ < p_left.x) {
                            int X = x_ + 1;
                            int Y = i;
                            int Y_min = 0;
                            int Y_max = 0;
                            while(true){
                                if (mark.at<uint8_t >(Y,X) == id) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y --;
                                    }else{
                                        if(Y < i){
                                            Y_min = (Y + 1);
                                        }else{
                                            Y_min = Y;
                                        }
                                        break;
                                    }
                                }else{
//                                    Y_min = maprange_min.y;
                                    Y_min = Y;/*---------------------??????????---------------------------*/
                                    break;
                                }
                            }
                            X = x_ + 1;
                            Y = i;
                            while(true){
                                if (mark.at<uint8_t >(Y,X) == id) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y ++;
                                    }else{
                                        if(Y > i){
                                            Y_max = (Y - 1);
                                        }else{
                                            Y_max = Y;
                                        }
                                        break;
                                    }
                                }else{
//                                    Y_max = maprange_max.y;
                                    Y_max = Y;/*---------------------??????????---------------------------*/
                                    break;
                                }
                            }
                            if(abs(Y_max - Y_min) > 5){
                                p_left.x = x_ + 1;
                                p_left.y = i;
                            }

                        }
                        break;
                    }
                }else{
                    if (y_ < p_left.y) {
                        int X = x_ + 1;
                        int Y = i;
                        int Y_min = 0;
                        int Y_max = 0;
                        while(true){
                            if (mark.at<uint8_t >(Y,X) == id) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y --;
                                }else{
                                    if(Y < i){
                                        Y_min = (Y + 1);
                                    }else{
                                        Y_min = Y;
                                    }
                                    break;
                                }
                            }else{
//                                Y_min = maprange_min.y;
                                Y_min = Y;/*---------------------??????????---------------------------*/
                                break;
                            }
                        }
                        X = x_ + 1;
                        Y = i;
                        while(true){
                            if (mark.at<uint8_t >(Y,X) == id) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y ++;
                                }else{
                                    if(Y > i){
                                        Y_max = (Y - 1);
                                    }else{
                                        Y_max = Y;
                                    }
                                    break;
                                }
                            }else{
//                                Y_max = maprange_max.y;
                                Y_max = Y;/*---------------------??????????---------------------------*/
                                break;
                            }
                        }
                        if(abs(Y_max - Y_min) > 5){
                            p_left.x = x_ + 1;
                            p_left.y = i;
                        }
                    }
                    break;
                }
            }
        }
        LOGPLANNING(INFO) << "left: (" << p_left.x << ", " << p_left.y << ") right: (" << p_right.x << ", " << p_right.y << ") up: (" << p_up.x << ", " << p_up.y << ") down: (" << p_down.x << ", " << p_down.y << ")";
        if(abs(p_right.x - input.x) > abs(input.x - p_left.x)){
            dir_clean_lock = 3;//锁X轴+
            x_ = p_left.x;
            y_ = p_left.y;
            p_up.y = y_;
            while(true){
                if(mark.at<uint8_t >(y_,x_) == id){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ ++;
                    }else{
                        if(y_ > p_up.y){
                            p_up.y = y_ - 1;
                        }
                        break;
                    }
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }
            x_ = p_left.x;
            y_ = p_left.y;
            p_down.y = y_;
            while(true){
                if(mark.at<uint8_t >(y_,x_) == id){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ --;
                    }else{
                        if(y_ < p_down.y){
                            p_down.y = y_ + 1;
                        }
                        break;
                    }
                }else{
                    if(y_ < p_down.y){
                        p_down.y = y_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "+ down: (" << p_down.y << ") up: (" << p_up.y << ")";
            if(abs(input.y - p_down.y) > abs(p_up.y - input.y)){
                dir_clean_path_first = 2;//Y-方向
            }else{
                dir_clean_path_first = 1;//Y+方向
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }else{
            dir_clean_lock = 4;//锁X轴-
            x_ = p_right.x;
            y_ = p_right.y;
            p_up.y = y_;
            while(true){
                if(mark.at<uint8_t >(y_,x_) == id){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ ++;
                    }else{
                        if(y_ > p_up.y){
                            p_up.y = y_ - 1;
                        }
                        break;
                    }
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }
            x_ = p_right.x;
            y_ = p_right.y;
            p_down.y = y_;
            while(true){
                if(mark.at<uint8_t >(y_,x_) == id){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ --;
                    }else{
                        if(y_ < p_down.y){
                            p_down.y = y_ + 1;
                        }
                        break;
                    }
                }else{
                    if(y_ < p_down.y){
                        p_down.y = y_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "- down: (" << p_down.y << ") up: (" << p_up.y << ")";
            if(abs(input.y - p_down.y) > abs(p_up.y - input.y)){
                dir_clean_path_first = 2;//Y-方向
            }else{
                dir_clean_path_first = 1;//Y+方向
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }

    }
    return 1;
}
int CleanPath::find_Endpoint(const Mat & src , Point input , Point p_min , Point p_max , Point & output){
    output = input;
    if(src.empty()){
        LOGPLANNING(INFO) << "src is empty !!!";
        return 0;
    }
    LOGPLANNING(INFO) << "Endpoint: input(" << input.x << ", " << input.y << ") (" << p_min.x << ", " << p_min.y << ")-(" << p_max.x << ", " << p_max.y << ")";
    if(input.x < p_min.x || input.x > p_max.x || input.y < p_min.y || input.y > p_max.y){
        LOGPLANNING(INFO) << "input is outof p_min-p_max!!!";
        return 0;
    }
    Point p_1 = input;
    Point p_2 = input;
    while(1){
        p_1.x = p_1.x - 1;
        p_2.x = p_2.x + 1;

        if(p_1.x <= p_min.x){
            p_1.x = p_1.x + 1;
            output = p_1;
            break;
        }
        if(p_2.x >= p_max.x){
            p_2.x = p_2.x - 1;
            output = p_2;
            break;
        }

        if(src.at<uint8_t >(p_1) != SearchFreeCell){
            p_1.x = p_1.x + 1;
            if(src.at<uint8_t >(p_1.y,p_1.x + 1) == SearchFreeCell && src.at<uint8_t >(p_1.y,p_1.x + 2) == SearchFreeCell){
                p_1.x = p_1.x + 2;
            }else if(src.at<uint8_t >(p_1.y,p_1.x + 1) == SearchFreeCell){
                p_1.x = p_1.x + 1;
            }
            output = p_1;
            break;
        }
        if(src.at<uint8_t >(p_2) != SearchFreeCell){
            p_2.x = p_2.x - 1;
            if(src.at<uint8_t >(p_2.y,p_2.x - 1) == SearchFreeCell && src.at<uint8_t >(p_2.y,p_2.x - 2) == SearchFreeCell){
                p_2.x = p_2.x - 2;
            }else if(src.at<uint8_t >(p_2.y,p_2.x - 1) == SearchFreeCell){
                p_2.x = p_2.x - 1;
            }
            output = p_2;
            break;
        }

    }
    if(output.x <= p_min.x || output.x >= p_max.x || src.at<uint8_t >(output) != SearchFreeCell){
        output = input;
    }
    return 1;
}
int CleanPath::find_Endpoint_mark(const Mat & src , const Mat & mark , int id , Point input , Point & output){
    output = input;
    if(src.empty()){
        LOGPLANNING(INFO) << "src is empty !!!";
        return 0;
    }
    if(mark.empty()){
        LOGPLANNING(INFO) << "mark is empty !!!";
        return 0;
    }
    LOGPLANNING(INFO) << "Endpoint: input(" << input.x << ", " << input.y << ") ,id:" << id;

    if(mark.at<uint8_t >(input) != id){
        LOGPLANNING(INFO) << "input is outof mark !!!";
        return 0;
    }
    Point p_1 = input;
    Point p_2 = input;
    while(1){
        p_1.x = p_1.x - 1;
        p_2.x = p_2.x + 1;

        if(mark.at<uint8_t >(p_1) != id){
            p_1.x = p_1.x + 1;
            output = p_1;
            break;
        }
        if(mark.at<uint8_t >(p_2) != id){
            p_2.x = p_2.x - 1;
            output = p_2;
            break;
        }

        if(src.at<uint8_t >(p_1) != SearchFreeCell){
            p_1.x = p_1.x + 1;
            if(src.at<uint8_t >(p_1.y,p_1.x + 1) == SearchFreeCell && src.at<uint8_t >(p_1.y,p_1.x + 2) == SearchFreeCell){
                p_1.x = p_1.x + 2;
            }else if(src.at<uint8_t >(p_1.y,p_1.x + 1) == SearchFreeCell){
                p_1.x = p_1.x + 1;
            }
            output = p_1;
            break;
        }
        if(src.at<uint8_t >(p_2) != SearchFreeCell){
            p_2.x = p_2.x - 1;
            if(src.at<uint8_t >(p_2.y,p_2.x - 1) == SearchFreeCell && src.at<uint8_t >(p_2.y,p_2.x - 2) == SearchFreeCell){
                p_2.x = p_2.x - 2;
            }else if(src.at<uint8_t >(p_2.y,p_2.x - 1) == SearchFreeCell){
                p_2.x = p_2.x - 1;
            }
            output = p_2;
            break;
        }
    }
    if(mark.at<uint8_t >(output) != id || src.at<uint8_t >(output) != SearchFreeCell){
        output = input;
    }
    return 1;
}

bool CleanPath::clean_up(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e){//X-
    LOGPLANNING(INFO) << "up start: (" << start.point.x << ", " << start.point.y << ")" << "box min:(" << box.point_min.x << "," << box.point_min.y <<")" << "box max:(" << box.point_max.x << "," <<box.point_max.y << ")";

    if(start.point.x < box.point_min.x - 3 || start.point.x > box.point_max.x + 3 || start.point.y < box.point_min.y || start.point.y > box.point_max.y){
        LOGPLANNING(INFO) << "up start is outof range !!!";
        return false;
    }
    int i = start.point.x - 1;
    int num = 0;
    Point p = start.point;
    int y_,y_u,y_d;
    y_ = start.point.y;
    y_u = start.point.y + 1;
    y_d = start.point.y - 1;

    bool flg_obs = false;
    while(i > box.point_min.x) {
        if (((s_map.NavMap.at<uint8_t >(y_,i) == NavFreeCell || s_map.NavMap.at<uint8_t >(y_,i) == NavUnknowCell) &&
             s_map.ForbidenMap.at<uint8_t>(y_, i) == FollowUnknowCell && s_map.DeepLearningMap.at<uint8_t>(y_, i) != ObsCell)) {

            if (s_map.CoverMap.at<uint8_t>(y_, i) != CoverCell ||
                (s_map.CoverMap.at<uint8_t>(y_u, i) != CoverCell &&
                 y_u < box.point_max.y) ||
                (s_map.CoverMap.at<uint8_t>(y_d, i) != CoverCell &&
                 y_d > box.point_min.y)) {
                num++;
                p = Point(i, y_);
            }else {
                break;
            }
        }else{
            flg_obs = true;
            break;
        }
        i = i - 1;
    }

#if 1
    if(num > 2){
        p_s = start;
        if(flg_obs){
            p.x = p.x + 2;
        }
        p_e.pose = s_map.MapToWorld(p);
        p_e.point = p;
        LOGPLANNING(INFO) << "up p_s - p_e (" << p_s.point.x << ", " << p_s.point.y << ")-(" << p_e.point.x <<","<<p_e.point.y<<")";
        return true;
    }else{
        LOGPLANNING(INFO) << "num:" << num;
        return false;
    }
#endif
}
bool CleanPath::clean_up_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e){//X-
    LOGPLANNING(INFO) << "clean_up_mark start: (" << start.point.x << ", " << start.point.y << ")";

    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point) != box.id){
        LOGPLANNING(INFO) << "clean_up_mark start is outof range !!!";
        return false;
    }

    int i = start.point.x - 1;
    int num = 0;
    Point p = start.point;
    int y_,y_u,y_d;
    y_ = start.point.y;
    y_u = start.point.y + 1;
    y_d = start.point.y - 1;

    bool flg_obs = false;
    while(s_map.RegionSegmentationMap.at<uint8_t >(y_,i) == box.id){
        if(((s_map.NavMap.at<uint8_t >(y_,i) == NavFreeCell || s_map.NavMap.at<uint8_t >(y_,i) == NavUnknowCell) &&
            s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell && s_map.DeepLearningMap.at<uint8_t >(y_,i) != ObsCell)){

            if(s_map.CoverMap.at<uint8_t >(y_,i) != CoverCell ||
               (s_map.CoverMap.at<uint8_t>(y_u, i) != CoverCell &&
                s_map.RegionSegmentationMap.at<uint8_t >(y_u,i) == box.id) ||
               (s_map.CoverMap.at<uint8_t>(y_d, i) != CoverCell &&
                s_map.RegionSegmentationMap.at<uint8_t >(y_d,i) == box.id)){
                num ++;
                p = Point(i,y_);
            }else {
                break;
            }
        }else{
            flg_obs = true;
            break;
        }
        i = i - 1;
    }
    if(num > 2){
        p_s = start;
        if(flg_obs){
            p.x = p.x + 2;
        }
        p_e.pose = s_map.MapToWorld(p);
        p_e.point = p;
        LOGPLANNING(INFO) << "clean_up_mark p_s - p_e (" << p_s.point.x << ", " << p_s.point.y << ")-(" << p_e.point.x <<","<<p_e.point.y<<")";

        return true;
    }else{
        LOGPLANNING(INFO) << "num:" << num;
        return false;
    }
}
bool CleanPath::clean_down(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e){//X+
    LOGPLANNING(INFO) << "down start: (" << start.point.x << ", " << start.point.y << ")" << "box min:(" << box.point_min.x << "," << box.point_min.y <<")" << "box max:(" << box.point_max.x << "," <<box.point_max.y << ")";

    if(start.point.x < box.point_min.x - 3 || start.point.x > box.point_max.x + 3 || start.point.y < box.point_min.y || start.point.y > box.point_max.y){
        LOGPLANNING(INFO) << "down start is outof range !!!";
        return false;
    }
    int i = start.point.x + 1;
    int num = 0;
    Point p = start.point;
    int y_,y_u,y_d;
    y_ = start.point.y;
    y_u = start.point.y + 1;
    y_d = start.point.y - 1;

    bool flg_obs = false;
    while(i < box.point_max.x){
        if(((s_map.NavMap.at<uint8_t >(y_,i) == NavFreeCell || s_map.NavMap.at<uint8_t >(y_,i) == NavUnknowCell) &&
            s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell && s_map.DeepLearningMap.at<uint8_t >(y_,i) != ObsCell)){

            if(s_map.CoverMap.at<uint8_t >(y_,i) != CoverCell ||
               (s_map.CoverMap.at<uint8_t >(y_u,i) != CoverCell &&
                y_u < box.point_max.y) ||
               (s_map.CoverMap.at<uint8_t >(y_d,i) != CoverCell &&
                y_d > box.point_min.y)){
                num ++;
                p = Point(i,y_);
            }else {
                break;
            }
        }else{
            flg_obs = true;
            break;
        }
        i = i + 1;
    }

#if 1
    if(num > 2){
        p_s = start;
        if(flg_obs){
            p.x = p.x - 2;
        }
        p_e.pose = s_map.MapToWorld(p);
        p_e.point = p;
        LOGPLANNING(INFO) << "down p_s - p_e (" << p_s.point.x << ", " << p_s.point.y << ")-(" << p_e.point.x <<","<<p_e.point.y<<")";
        return true;
    }else{
        LOGPLANNING(INFO) << "num:" << num;
        return false;
    }
#endif
}
bool CleanPath::clean_down_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e){//X+
    LOGPLANNING(INFO) << "clean_down_mark start: (" << start.point.x << ", " << start.point.y << ")";

    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point) != box.id){
        LOGPLANNING(INFO) << "clean_down_mark start is outof range !!!";
        return false;
    }

    int i = start.point.x + 1;
    int num = 0;
    Point p = start.point;
    int y_,y_u,y_d;
    y_ = start.point.y;
    y_u = start.point.y + 1;
    y_d = start.point.y - 1;

    bool flg_obs = false;
    while(s_map.RegionSegmentationMap.at<uint8_t >(y_,i) == box.id){
        if(((s_map.NavMap.at<uint8_t >(y_,i) == NavFreeCell || s_map.NavMap.at<uint8_t >(y_,i) == NavUnknowCell) &&
            s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell && s_map.DeepLearningMap.at<uint8_t >(y_,i) != ObsCell)){

            if(s_map.CoverMap.at<uint8_t >(y_,i) != CoverCell ||
               (s_map.CoverMap.at<uint8_t>(y_u, i) != CoverCell &&
                s_map.RegionSegmentationMap.at<uint8_t >(y_u,i) == box.id) ||
               (s_map.CoverMap.at<uint8_t>(y_d, i) != CoverCell &&
                s_map.RegionSegmentationMap.at<uint8_t >(y_d,i) == box.id)){
                num ++;
                p = Point(i,y_);
            }else {
                break;
            }
        }else{
            flg_obs = true;
            break;
        }
        i = i + 1;
    }

    if(num > 2){
        p_s = start;
        if(flg_obs){
            p.x = p.x - 2;
        }
        p_e.pose = s_map.MapToWorld(p);
        p_e.point = p;
        LOGPLANNING(INFO) << "clean_down_mark p_s - p_e (" << p_s.point.x << ", " << p_s.point.y << ")-(" << p_e.point.x <<","<<p_e.point.y<<")";
        return true;
    }else{
        LOGPLANNING(INFO) << "num:" << num;
        return false;
    }
}
bool CleanPath::clean_left(BoxInfo box , Point2f start , Point2f & p_s , Point2f & p_e , int dir){//Y-
    Point start_ = s_map.WorldToMap(start);
    int offset_dir = dir ? 1:-1;
    int offset = offset_dir * 3;
    int i = start_.x;
    int y_,y_u,y_d;
    y_ = start_.y + offset;
    y_u = start_.y + offset + 1;
    y_d = start_.y + offset - 1;
//    s_map.NavMapMutex.readLock();
//    s_map.ForbidenMapMutex.readLock();
    if(offset_dir > 0){
        while((i <= box.point_max.x && y_ >= box.point_min.y && y_ <= box.point_max.y) &&
              ((s_map.FollowMap.at<uint8_t >(y_,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell) ||
               (s_map.FollowMap.at<uint8_t >(y_u,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_u,i) == FollowUnknowCell) ||
               (s_map.FollowMap.at<uint8_t >(y_d,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_d,i) == FollowUnknowCell))){
            if((s_map.CoverMap.at<uint8_t >(y_,i) == NavUnknowCell) ||
               (s_map.CoverMap.at<uint8_t >(y_u,i) == NavUnknowCell) ||
               (s_map.CoverMap.at<uint8_t >(y_d,i) == NavUnknowCell)){
                p_s = s_map.MapToWorld(Point(i,y_));
                p_e = p_s;
                p_s.x = s_map.origin.x + fabs(s_map.origin.x)*2;
                p_e.x = s_map.origin.x;
                return true;
            }
            i = i + offset_dir;
        }
    }else{
        while((i >= box.point_min.x && y_ >= box.point_min.y && y_ <= box.point_max.y) &&
              ((s_map.FollowMap.at<uint8_t >(y_,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell) ||
               (s_map.FollowMap.at<uint8_t >(y_u,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_u,i) == FollowUnknowCell) ||
               (s_map.FollowMap.at<uint8_t >(y_d,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_d,i) == FollowUnknowCell))){
            if((s_map.CoverMap.at<uint8_t >(y_,i) == NavUnknowCell) ||
               (s_map.CoverMap.at<uint8_t >(y_u,i) == NavUnknowCell) ||
               (s_map.CoverMap.at<uint8_t >(y_d,i) == NavUnknowCell)){
                p_s = s_map.MapToWorld(Point(i,y_));
                p_e = p_s;
                p_s.x = s_map.origin.x + fabs(s_map.origin.x)*2;
                p_e.x = s_map.origin.x;
                return true;
            }
            i = i + offset_dir;
        }
    }

//    s_map.NavMapMutex.readUnlock();
//    s_map.ForbidenMapMutex.readUnlock();
    return false;
}
bool CleanPath::clean_leftAcute(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir){//Y-
    LOGPLANNING(INFO) << "leftAcute start: (" << start.point.x << ", " << start.point.y << ") , dir:"<< dir;

    int offset_dir = dir ? 1:-1;
    if((start.point.y + offset_dir * 3) < box.point_min.y || (start.point.y + offset_dir * 3) > box.point_max.y){
        LOGPLANNING(INFO) << "leftAcute start near box side";
        return false;
    }

    if((start.point.x + offset_dir * 3) > box.point_min.x && (start.point.x + offset_dir * 3) < box.point_max.x){

        if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x + offset_dir * 4) , (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "leftAcute 45 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }
    }
    return false;
}
bool CleanPath::clean_leftAcute_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir){//Y-
    LOGPLANNING(INFO) << "clean_leftAcute_mark start: (" << start.point.x << ", " << start.point.y << ") , dir:"<< dir;
    int offset_dir = dir ? 1:-1;
    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point.y +  + offset_dir,start.point.x) != box.id){
        LOGPLANNING(INFO) << "clean_leftAcute_mark start near box side";
        return false;
    }

    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point.y,start.point.x + offset_dir * 3) == box.id){

        if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x + offset_dir * 4) , (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "clean_leftAcute_mark 45 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }
    }
    return false;
}

bool CleanPath::clean_leftObtuse(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir){
    LOGPLANNING(INFO) << "leftObtuse start: (" << start.point.x << ", " << start.point.y << ") , dir:" << dir << " box: " << box.point_min.x << " - " << box.point_max.x;

    int offset_dir = dir ? 1:-1;

    if((start.point.y + offset_dir * 3) < box.point_min.y || (start.point.y + offset_dir * 3) > box.point_max.y){
        LOGPLANNING(INFO) << "leftObtuse start near box side";
        return false;
    }

    if((start.point.x - offset_dir * 3) > box.point_min.x && (start.point.x - offset_dir * 3) < box.point_max.x){

        if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x - offset_dir * 4) , (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "leftObtuse 45 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }

        if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x), (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x  - offset_dir) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x - offset_dir), (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x  + offset_dir) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x + offset_dir), (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "leftObtuse 90 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }

    }else{
        if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x), (start.point.y + offset_dir * 3)))){

                    if(clean_isfit_NoObs(Point((start.point.x  - offset_dir) , (start.point.y + offset_dir)))){
                        if(clean_isfit_NoObs(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit_NoObs(Point((start.point.x - offset_dir), (start.point.y + offset_dir * 3)))){

                                if(clean_isfit_NoObs(Point((start.point.x  + offset_dir) , (start.point.y + offset_dir)))){
                                    if(clean_isfit_NoObs(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit_NoObs(Point((start.point.x + offset_dir), (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "leftObtuse 90 side p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }
    }
    return false;
}
bool CleanPath::clean_leftObtuse_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir){
    LOGPLANNING(INFO) << "clean_leftObtuse_mark start: (" << start.point.x << ", " << start.point.y << ") , dir:" << dir;
    int offset_dir = dir ? 1:-1;

    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point.y + offset_dir * 3 , start.point.x) != box.id){
        LOGPLANNING(INFO) << "clean_leftObtuse_mark start near box side";
        return false;
    }

    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point) == box.id &&
       (s_map.RegionSegmentationMap.at<uint8_t >(start.point.y , start.point.x + 1) != box.id || s_map.RegionSegmentationMap.at<uint8_t >(start.point.y , start.point.x - 1) != box.id)){
        if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x), (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x  - offset_dir) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x - offset_dir), (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x  + offset_dir) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x + offset_dir), (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "clean_leftObtuse_mark 90 side p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }
    }

    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point.y , start.point.x - offset_dir * 3) == box.id){

        if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x - offset_dir * 4) , (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "clean_leftObtuse_mark 45 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }

        if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x), (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x  - offset_dir) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x - offset_dir), (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x  + offset_dir) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x + offset_dir), (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "clean_leftObtuse_mark 90 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }

    }
    return false;
}

bool CleanPath::clean_left_mark(BoxInfo box , Point2f start , Point2f & p_s , Point2f & p_e , int dir){//Y-
    Point start_ = s_map.WorldToMap(start);
    int offset_dir = dir ? 1:-1;
    int offset = offset_dir * 3;
    int i = start_.x;

    int y_,y_u,y_d;
    y_ = start_.y + offset;
    y_u = start_.y + offset + 1;
    y_d = start_.y + offset - 1;
    while((s_map.RegionSegmentationMap.at<uint8_t >(y_,i) == box.id) &&
          ((s_map.NavMap.at<uint8_t >(y_,i) == NavFreeCell &&
            s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell) ||
           (s_map.NavMap.at<uint8_t >(y_u,i) == NavFreeCell &&
            s_map.ForbidenMap.at<uint8_t >(y_u,i) == FollowUnknowCell) ||
           (s_map.NavMap.at<uint8_t >(y_d,i) == NavFreeCell &&
            s_map.ForbidenMap.at<uint8_t >(y_d,i) == FollowUnknowCell))){
        if((s_map.CoverMap.at<uint8_t >(y_,i) == NavUnknowCell) ||
           (s_map.CoverMap.at<uint8_t >(y_u,i) == NavUnknowCell) ||
           (s_map.CoverMap.at<uint8_t >(y_d,i) == NavUnknowCell)){
            p_s = s_map.MapToWorld(Point(i,y_));
            p_e = p_s;
            p_s.x = s_map.origin.x + fabs(s_map.origin.x)*2;
            p_e.x = s_map.origin.x;
            return true;
        }
        i = i + offset_dir;
    }
    return false;
}
bool CleanPath::clean_right(BoxInfo box , Point2f start , Point2f & p_s , Point2f & p_e , int dir){//Y+
    Point start_ = s_map.WorldToMap(start);
    int offset_dir = dir ? 1:-1;
    int offset = offset_dir * 3;
    int i = start_.x;
    int y_,y_u,y_d;
    y_ = start_.y + offset;
    y_u = start_.y + offset + 1;
    y_d = start_.y + offset - 1;
    if(offset_dir > 0){
        while((i <= box.point_max.x && y_ >= box.point_min.y && y_ <= box.point_max.y) &&
              ((s_map.FollowMap.at<uint8_t >(y_,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell) ||
               (s_map.FollowMap.at<uint8_t >(y_u,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_u,i) == FollowUnknowCell) ||
               (s_map.FollowMap.at<uint8_t >(y_d,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_d,i) == FollowUnknowCell))){
            if(s_map.CoverMap.at<uint8_t >(y_,i) == NavUnknowCell ||
               s_map.CoverMap.at<uint8_t >(y_u,i) == NavUnknowCell ||
               s_map.CoverMap.at<uint8_t >(y_d,i) == NavUnknowCell){
                p_s = s_map.MapToWorld(Point(i,y_));
                p_e = p_s;
                p_s.x = s_map.origin.x + fabs(s_map.origin.x)*2;
                p_e.x = s_map.origin.x;
                return true;
            }
            i = i - offset_dir;
        }
    }else{
        while((i >= box.point_min.x && y_ >= box.point_min.y && y_ <= box.point_max.y) &&
              ((s_map.FollowMap.at<uint8_t >(y_,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell) ||
               (s_map.FollowMap.at<uint8_t >(y_u,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_u,i) == FollowUnknowCell) ||
               (s_map.FollowMap.at<uint8_t >(y_d,i) == FollowUnknowCell &&
                s_map.ForbidenMap.at<uint8_t >(y_d,i) == FollowUnknowCell))){
            if(s_map.CoverMap.at<uint8_t >(y_,i) == NavUnknowCell ||
               s_map.CoverMap.at<uint8_t >(y_u,i) == NavUnknowCell ||
               s_map.CoverMap.at<uint8_t >(y_d,i) == NavUnknowCell){
                p_s = s_map.MapToWorld(Point(i,y_));
                p_e = p_s;
                p_s.x = s_map.origin.x + fabs(s_map.origin.x)*2;
                p_e.x = s_map.origin.x;
                return true;
            }
            i = i - offset_dir;
        }
    }

    return false;
}
bool CleanPath::clean_rightAcute(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir){//Y+
    LOGPLANNING(INFO) << "rightAcute start: (" << start.point.x << ", " << start.point.y << ") , dir:"<< dir;

    int offset_dir = dir ? 1:-1;
    if((start.point.y + offset_dir * 3) < box.point_min.y || (start.point.y + offset_dir * 3) > box.point_max.y){
        LOGPLANNING(INFO) << "rightAcute start near box side";
        return false;
    }
    if((start.point.x - offset_dir * 3) > box.point_min.x && (start.point.x - offset_dir * 3) < box.point_max.x){

        if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x - offset_dir * 4) , (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "rightAcute 45 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }
    }
    return false;
}
bool CleanPath::clean_rightAcute_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir){//Y+
    LOGPLANNING(INFO) << "clean_rightAcute_mark start: (" << start.point.x << ", " << start.point.y << ") , dir:"<< dir;

    int offset_dir = dir ? 1:-1;
    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point.y + offset_dir * 3,start.point.x) != box.id){
        LOGPLANNING(INFO) << "clean_rightAcute_mark start near box side";
        return false;
    }
    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point.y , start.point.x - offset_dir * 3) == box.id){

        if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x - offset_dir * 4) , (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x - offset_dir * 2) , (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x - offset_dir * 3) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "clean_rightAcute_mark 45 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }
    }
    return false;
}

bool CleanPath::clean_rightObtuse(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir){
    LOGPLANNING(INFO) << "rightObtuse start: (" << start.point.x << ", " << start.point.y << ") , dir:" << dir << " box: " << box.point_min.x << " - " << box.point_max.x;

    int offset_dir = dir ? 1:-1;

    if((start.point.y + offset_dir * 3) < box.point_min.y || (start.point.y + offset_dir * 3) > box.point_max.y){
        LOGPLANNING(INFO) << "rightObtuse start near box side";
        return false;
    }

    if((start.point.x + offset_dir * 3) > box.point_min.x && (start.point.x + offset_dir * 3) < box.point_max.x){

        if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x + offset_dir * 4) , (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "rightObtuse 45 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }

        if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x), (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x  + offset_dir) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x + offset_dir), (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x  - offset_dir) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x - offset_dir), (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "rightObtuse 90 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }

    }else{
        if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x), (start.point.y + offset_dir * 3)))){

                    if(clean_isfit_NoObs(Point((start.point.x  + offset_dir) , (start.point.y + offset_dir)))){
                        if(clean_isfit_NoObs(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit_NoObs(Point((start.point.x + offset_dir), (start.point.y + offset_dir * 3)))){

                                if(clean_isfit_NoObs(Point((start.point.x  - offset_dir) , (start.point.y + offset_dir)))){
                                    if(clean_isfit_NoObs(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit_NoObs(Point((start.point.x - offset_dir), (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "rightObtuse 90 side p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }
    }
    return false;
}
bool CleanPath::clean_rightObtuse_mark(BoxInfo box , PoseStamped start , PoseStamped & p_s , PoseStamped & p_e , int dir){
    LOGPLANNING(INFO) << "clean_rightObtuse_mark start: (" << start.point.x << ", " << start.point.y << ") , dir:"<< dir;
    int offset_dir = dir ? 1:-1;

    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point.y + offset_dir * 3 , start.point.x) != box.id){
        LOGPLANNING(INFO) << "clean_rightObtuse_mark start near box side";
        return false;
    }
    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point) == box.id &&
       (s_map.RegionSegmentationMap.at<uint8_t >(start.point.y , start.point.x + 1) != box.id || s_map.RegionSegmentationMap.at<uint8_t >(start.point.y , start.point.x - 1) != box.id)){
        if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x), (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x  + offset_dir) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x + offset_dir), (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x  - offset_dir) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x - offset_dir), (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "clean_rightObtuse_mark 90 side p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }
    }
    if(s_map.RegionSegmentationMap.at<uint8_t >(start.point.y , start.point.x + offset_dir * 3) == box.id){

        if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x + offset_dir * 4) , (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x + offset_dir * 2) , (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x + offset_dir * 3) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "clean_rightObtuse_mark 45 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }

        if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir)))){
            if(clean_isfit(Point((start.point.x) , (start.point.y + offset_dir * 2)))){
                if(clean_isfit(Point((start.point.x), (start.point.y + offset_dir * 3)))){

                    if(clean_isfit(Point((start.point.x  + offset_dir) , (start.point.y + offset_dir)))){
                        if(clean_isfit(Point((start.point.x + offset_dir) , (start.point.y + offset_dir * 2)))){
                            if(clean_isfit(Point((start.point.x + offset_dir), (start.point.y + offset_dir * 3)))){

                                if(clean_isfit(Point((start.point.x  - offset_dir) , (start.point.y + offset_dir)))){
                                    if(clean_isfit(Point((start.point.x - offset_dir) , (start.point.y + offset_dir * 2)))){
                                        if(clean_isfit(Point((start.point.x - offset_dir), (start.point.y + offset_dir * 3)))){

                                            p_s.point = Point((start.point.x) , (start.point.y + offset_dir * 3));
                                            p_s.pose = s_map.MapToWorld(p_s.point);
                                            LOGPLANNING(INFO) << "clean_rightObtuse_mark 90 p_s:(" << p_s.point.x << " , " << p_s.point.y << ")";
                                            return true;

                                        }
                                    }
                                }

                            }
                        }
                    }

                }
            }
        }

    }
    return false;
}

bool CleanPath::clean_right_mark(BoxInfo box , Point2f start , Point2f & p_s , Point2f & p_e , int dir){//Y+
    Point start_ = s_map.WorldToMap(start);
    int offset_dir = dir ? 1:-1;
    int offset = offset_dir * 3;
    int i = start_.x;

    int y_,y_u,y_d;
    y_ = start_.y + offset;
    y_u = start_.y + offset + 1;
    y_d = start_.y + offset - 1;
    while((s_map.RegionSegmentationMap.at<uint8_t >(y_,i) == box.id) &&
          ((s_map.NavMap.at<uint8_t >(y_,i) == NavFreeCell &&
            s_map.ForbidenMap.at<uint8_t >(y_,i) == ForbiddenUnknowCell) ||
           (s_map.NavMap.at<uint8_t >(y_u,i) == NavFreeCell &&
            s_map.ForbidenMap.at<uint8_t >(y_u,i) == FollowUnknowCell) ||
           (s_map.NavMap.at<uint8_t >(y_d,i) == NavFreeCell &&
            s_map.ForbidenMap.at<uint8_t >(y_d,i) == FollowUnknowCell))){
        if((s_map.CoverMap.at<uint8_t >(y_,i) == NavUnknowCell) ||
           (s_map.CoverMap.at<uint8_t >(y_u,i) == NavUnknowCell) ||
           (s_map.CoverMap.at<uint8_t >(y_d,i) == NavUnknowCell)){
            p_s = s_map.MapToWorld(Point(i,y_));
            p_e = p_s;
            p_s.x = s_map.origin.x + fabs(s_map.origin.x)*2;
            p_e.x = s_map.origin.x;
            return true;
        }
        i = i - offset_dir;
    }
    return false;
}

bool CleanPath::clean_isfit(Point p){
    return ((s_map.NavMap.at<uint8_t >(p) == NavFreeCell || s_map.NavMap.at<uint8_t >(p) == NavUnknowCell) &&
            s_map.ForbidenMap.at<uint8_t >(p) == FollowUnknowCell && s_map.DeepLearningMap.at<uint8_t >(p) != ObsCell &&
            s_map.CoverMap.at<uint8_t >(p) != CoverCell);
}
bool CleanPath::clean_isfit_NoObs(Point p){
    return ((s_map.NavMap.at<uint8_t >(p) == NavFreeCell || s_map.NavMap.at<uint8_t >(p) == NavUnknowCell) &&
            s_map.ForbidenMap.at<uint8_t >(p) == FollowUnknowCell && s_map.DeepLearningMap.at<uint8_t >(p) != ObsCell);
}

int CleanPath::find_corner_point_0(const Mat & src , Point input , Point p_min , Point p_max , Point & output){
    output = input;

    if(src.empty()){
        LOGPLANNING(INFO) << "地图数据为空 !!!";
        return 0;
    }
    if(input.x < p_min.x || input.y < p_min.y || input.x > p_max.x || input.y > p_max.y){
        LOGPLANNING(INFO) << "起点超出范围: (" << input.x << ", " << input.y << ")";
        return 0;
    }
    LOGPLANNING(INFO) << "corner input: (" << input.x << ", " << input.y << ")";
    LOGPLANNING(INFO) << "corner p_min: (" << p_min.x << ", " << p_min.y << ") p_max: (" << p_max.x << ", " << p_max.y << ")";
    Point p_up = input,p_down = input,p_left = input,p_right = input;

    Point maprange_min = p_min;
    Point maprange_max = p_max;

    int x_,y_;

    if(Dir_Cleanning_Path == SPECIFIED_LOCK_DIE_X){
        LOGPLANNING(INFO) << "SPECIFIED_LOCK_DIE_X";
        x_ = p_right.x;
        y_ = p_right.y;
        while(true){
            if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    x_ ++;
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }else{
                if(x_ > p_right.x){
                    p_right.x = x_ - 1;
                }
                break;
            }
        }
        x_ = p_left.x;
        y_ = p_left.y;
        while(true){
            if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    x_ --;
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }else{
                if(x_ < p_left.x){
                    p_left.x = x_ + 1;
                }
                break;
            }
        }
        for(int i = p_left.x ; i <= p_right.x ; i ++){
            x_ = i;
            y_ = input.y;
            while(true) {
                if (y_ >= maprange_min.y && y_ <= maprange_max.y) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        y_++;
                    } else {
                        if (y_ > p_up.y) {
                            int X = i;
                            int Y = y_ - 1;
                            int X_min = 0;
                            int X_max = 0;
                            while(true){
                                if (X >= maprange_min.x && X <= maprange_max.x) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X --;
                                    }else{
                                        if(X < i){
                                            X_min = X ++;
                                        }else{
                                            X_min = X;
                                        }
                                        break;
                                    }
                                }else{
                                    X_min = maprange_min.x;
                                    break;
                                }
                            }
                            X = i;
                            Y = y_ - 1;
                            while(true){
                                if (X >= maprange_min.x && X <= maprange_max.x) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X ++;
                                    }else{
                                        if(X > i){
                                            X_max = X --;
                                        }else{
                                            X_max = X;
                                        }
                                        break;
                                    }
                                }else{
                                    X_max = maprange_max.x;
                                    break;
                                }
                            }
                            if(abs(X_max - X_min) > 5){
                                p_up.x = i;
                                p_up.y = y_ - 1;
                            }
                        }
                        break;
                    }
                }else{
                    if (y_ > p_up.y) {
                        int X = i;
                        int Y = y_ - 1;
                        int X_min = 0;
                        int X_max = 0;
                        while(true){
                            if (X >= maprange_min.x && X <= maprange_max.x) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X --;
                                }else{
                                    if(X < i){
                                        X_min = X ++;
                                    }else{
                                        X_min = X;
                                    }
                                    break;
                                }
                            }else{
                                X_min = maprange_min.x;
                                break;
                            }
                        }
                        X = i;
                        Y = y_ - 1;
                        while(true){
                            if (X >= maprange_min.x && X <= maprange_max.x) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X ++;
                                }else{
                                    if(X > i){
                                        X_max = X --;
                                    }else{
                                        X_max = X;
                                    }
                                    break;
                                }
                            }else{
                                X_max = maprange_max.x;
                                break;
                            }
                        }
                        if(abs(X_max - X_min) > 5){
                            p_up.x = i;
                            p_up.y = y_ - 1;
                        }
                    }
                    break;
                }
            }
            x_ = i;
            y_ = input.y;
            while(true) {
                if (y_ >= maprange_min.y && y_ <= maprange_max.y) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        y_--;
                    } else {
                        if (y_ < p_down.y) {
                            int X = i;
                            int Y = y_ + 1;
                            int X_min = 0;
                            int X_max = 0;
                            while(true){
                                if (X >= maprange_min.x && X <= maprange_max.x) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X --;
                                    }else{
                                        if(X < i){
                                            X_min = (X + 1);
                                        }else{
                                            X_min = X;
                                        }
                                        break;
                                    }
                                }else{
                                    X_min = maprange_min.x;
                                    break;
                                }
                            }
                            X = i;
                            Y = y_ + 1;
                            while(true){
                                if (X >= maprange_min.x && X <= maprange_max.x) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        X ++;
                                    }else{
                                        if(X > i){
                                            X_max = (X - 1);
                                        }else{
                                            X_max = X;
                                        }
                                        break;
                                    }
                                }else{
                                    X_max = maprange_max.x;
                                    break;
                                }
                            }
                            if(abs(X_max - X_min) > 5){
                                p_down.x = i;
                                p_down.y = y_ + 1;
                            }
                        }
                        break;
                    }
                }else{
                    if (y_ < p_down.y) {
                        int X = i;
                        int Y = y_ + 1;
                        int X_min = 0;
                        int X_max = 0;
                        while(true){
                            if (X >= maprange_min.x && X <= maprange_max.x) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X --;
                                }else{
                                    if(X < i){
                                        X_min = (X + 1);
                                    }else{
                                        X_min = X;
                                    }
                                    break;
                                }
                            }else{
                                X_min = maprange_min.x;
                                break;
                            }
                        }
                        X = i;
                        Y = y_ + 1;
                        while(true){
                            if (X >= maprange_min.x && X <= maprange_max.x) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    X ++;
                                }else{
                                    if(X > i){
                                        X_max = (X - 1);
                                    }else{
                                        X_max = X;
                                    }
                                    break;
                                }
                            }else{
                                X_max = maprange_max.x;
                                break;
                            }
                        }
                        if(abs(X_max - X_min) > 5){
                            p_down.x = i;
                            p_down.y = y_ + 1;
                        }
                    }
                    break;
                }
            }
        }
        LOGPLANNING(INFO) << "left: (" << p_left.x << ", " << p_left.y << ") right: (" << p_right.x << ", " << p_right.y << ") up: (" << p_up.x << ", " << p_up.y << ") down: (" << p_down.x << ", " << p_down.y << ")";
        if(abs(p_up.y - input.y) > abs(input.y - p_down.y)){
            dir_clean_lock = 1;//锁Y轴+
            x_ = p_down.x;
            y_ = p_down.y;
            p_right.x = x_;
            while(true){
                if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ ++;
                    }else{
                        if(x_ > p_right.x){
                            p_right.x = x_ - 1;
                        }
                        break;
                    }
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }
            x_ = p_down.x;
            y_ = p_down.y;
            p_left.x = x_;
            while(true){
                if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ --;
                    }else{
                        if(x_ < p_left.x){
                            p_left.x = x_ + 1;
                        }
                        break;
                    }
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "+ left: (" << p_left.x << ") right: (" << p_right.x << ")";
            if(abs(input.x - p_left.x) > abs(p_right.x - input.x)){
                dir_clean_path_first = 2;//X-方向
                output.x = p_right.x;
                output.y = p_down.y;
            }else{
                dir_clean_path_first = 1;//X+方向
                output.x = p_left.x;
                output.y = p_down.y;
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }else{
            dir_clean_lock = 2;//锁Y轴-
            x_ = p_up.x;
            y_ = p_up.y;
            p_right.x = x_;
            while(true){
                if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ ++;
                    }else{
                        if(x_ > p_right.x){
                            p_right.x = x_ - 1;
                        }
                        break;
                    }
                }else{
                    if(x_ > p_right.x){
                        p_right.x = x_ - 1;
                    }
                    break;
                }
            }
            x_ = p_up.x;
            y_ = p_up.y;
            p_left.x = x_;
            while(true){
                if(x_ >= maprange_min.x && x_ <= maprange_max.x){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        x_ --;
                    }else{
                        if(x_ < p_left.x){
                            p_left.x = x_ + 1;
                        }
                        break;
                    }
                }else{
                    if(x_ < p_left.x){
                        p_left.x = x_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "- left: (" << p_left.x << ") right: (" << p_right.x << ")";
            if(abs(input.x - p_left.x) > abs(p_right.x - input.x)){
                dir_clean_path_first = 2;//X-方向
                output.x = p_right.x;
                output.y = p_up.y;
            }else{
                dir_clean_path_first = 1;//X+方向
                output.x = p_left.x;
                output.y = p_up.y;
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }

    }else if(Dir_Cleanning_Path == SPECIFIED_LOCK_DIE_Y){
        LOGPLANNING(INFO) << "SPECIFIED_LOCK_DIE_Y";
        x_ = p_up.x;
        y_ = p_up.y;
        while(true){
            if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    y_ ++;
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }else{
                if(y_ > p_up.y){
                    p_up.y = y_ - 1;
                }
                break;
            }
        }
        x_ = p_down.x;
        y_ = p_down.y;
        while(true){
            if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                    y_ --;
                }else{
                    if(y_ < p_down.y){
                        p_down.x = y_ + 1;
                    }
                    break;
                }
            }else{
                if(y_ < p_down.y){
                    p_down.y = y_ + 1;
                }
                break;
            }
        }
        for(int i = p_down.y ; i <= p_up.y ; i ++){
            x_ = input.x;
            y_ = i;
            while(true) {
                if (x_ >= maprange_min.x && x_ <= maprange_max.x) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        x_++;
                    } else {
                        if (x_ > p_right.x) {
                            int X = x_ - 1;
                            int Y = i;
                            int Y_min = 0;
                            int Y_max = 0;
                            while(true){
                                if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y --;
                                    }else{
                                        if(Y < i){
                                            Y_min = (Y + 1);
                                        }else{
                                            Y_min = Y;
                                        }
                                        break;
                                    }
                                }else{
                                    Y_min = maprange_min.y;
                                    break;
                                }
                            }
                            X = x_ - 1;
                            Y = i;
                            while(true){
                                if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y ++;
                                    }else{
                                        if(Y > i){
                                            Y_max = (Y - 1);
                                        }else{
                                            Y_max = Y;
                                        }
                                        break;
                                    }
                                }else{
                                    Y_max = maprange_max.y;
                                    break;
                                }
                            }
                            if(abs(Y_max - Y_min) > 5){
                                p_right.x = x_ - 1;
                                p_right.y = i;
                            }
                        }
                        break;
                    }
                }else{
                    if (x_ > p_right.x) {
                        int X = x_ - 1;
                        int Y = i;
                        int Y_min = 0;
                        int Y_max = 0;
                        while(true){
                            if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y --;
                                }else{
                                    if(Y < i){
                                        Y_min = (Y + 1);
                                    }else{
                                        Y_min = Y;
                                    }
                                    break;
                                }
                            }else{
                                Y_min = maprange_min.y;
                                break;
                            }
                        }
                        X = x_ - 1;
                        Y = i;
                        while(true){
                            if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y ++;
                                }else{
                                    if(Y > i){
                                        Y_max = (Y - 1);
                                    }else{
                                        Y_max = Y;
                                    }
                                    break;
                                }
                            }else{
                                Y_max = maprange_max.y;
                                break;
                            }
                        }
                        if(abs(Y_max - Y_min) > 5){
                            p_right.x = x_ - 1;
                            p_right.y = i;
                        }
                    }
                    break;
                }
            }
            x_ = input.x;
            y_ = i;
            while(true) {
                if (x_ >= maprange_min.x && x_ <= maprange_max.x) {
                    if (src.at<uint8_t>(Point(x_, y_)) == FREE_AREA) {
                        x_--;
                    } else {
                        if (x_ < p_left.x) {
                            int X = x_ + 1;
                            int Y = i;
                            int Y_min = 0;
                            int Y_max = 0;
                            while(true){
                                if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y --;
                                    }else{
                                        if(Y < i){
                                            Y_min = (Y + 1);
                                        }else{
                                            Y_min = Y;
                                        }
                                        break;
                                    }
                                }else{
                                    Y_min = maprange_min.y;
                                    break;
                                }
                            }
                            X = x_ + 1;
                            Y = i;
                            while(true){
                                if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                    if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                        Y ++;
                                    }else{
                                        if(Y > i){
                                            Y_max = (Y - 1);
                                        }else{
                                            Y_max = Y;
                                        }
                                        break;
                                    }
                                }else{
                                    Y_max = maprange_max.y;
                                    break;
                                }
                            }
                            if(abs(Y_max - Y_min) > 5){
                                p_left.x = x_ + 1;
                                p_left.y = i;
                            }

                        }
                        break;
                    }
                }else{
                    if (y_ < p_left.y) {
                        int X = x_ + 1;
                        int Y = i;
                        int Y_min = 0;
                        int Y_max = 0;
                        while(true){
                            if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y --;
                                }else{
                                    if(Y < i){
                                        Y_min = (Y + 1);
                                    }else{
                                        Y_min = Y;
                                    }
                                    break;
                                }
                            }else{
                                Y_min = maprange_min.y;
                                break;
                            }
                        }
                        X = x_ + 1;
                        Y = i;
                        while(true){
                            if (Y >= maprange_min.y && Y <= maprange_max.y) {
                                if (src.at<uint8_t>(Point(X, Y)) == FREE_AREA) {
                                    Y ++;
                                }else{
                                    if(Y > i){
                                        Y_max = (Y - 1);
                                    }else{
                                        Y_max = Y;
                                    }
                                    break;
                                }
                            }else{
                                Y_max = maprange_max.y;
                                break;
                            }
                        }
                        if(abs(Y_max - Y_min) > 5){
                            p_left.x = x_ + 1;
                            p_left.y = i;
                        }
                    }
                    break;
                }
            }
        }
        LOGPLANNING(INFO) << "left: (" << p_left.x << ", " << p_left.y << ") right: (" << p_right.x << ", " << p_right.y << ") up: (" << p_up.x << ", " << p_up.y << ") down: (" << p_down.x << ", " << p_down.y << ")";
        if(abs(p_right.x - input.x) > abs(input.x - p_left.x)){
            dir_clean_lock = 3;//锁X轴+
            x_ = p_left.x;
            y_ = p_left.y;
            p_up.y = y_;
            while(true){
                if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ ++;
                    }else{
                        if(y_ > p_up.y){
                            p_up.y = y_ - 1;
                        }
                        break;
                    }
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }
            x_ = p_left.x;
            y_ = p_left.y;
            p_down.y = y_;
            while(true){
                if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ --;
                    }else{
                        if(y_ < p_down.y){
                            p_down.y = y_ + 1;
                        }
                        break;
                    }
                }else{
                    if(y_ < p_down.y){
                        p_down.y = y_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "+ down: (" << p_down.y << ") up: (" << p_up.y << ")";
            if(abs(input.y - p_down.y) > abs(p_up.y - input.y)){
                dir_clean_path_first = 2;//Y-方向
                output.x = p_left.x;
                output.y = p_up.y;
            }else{
                dir_clean_path_first = 1;//Y+方向
                output.x = p_left.x;
                output.y = p_down.y;
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }else{
            dir_clean_lock = 4;//锁X轴-
            x_ = p_right.x;
            y_ = p_right.y;
            p_up.y = y_;
            while(true){
                if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ ++;
                    }else{
                        if(y_ > p_up.y){
                            p_up.y = y_ - 1;
                        }
                        break;
                    }
                }else{
                    if(y_ > p_up.y){
                        p_up.y = y_ - 1;
                    }
                    break;
                }
            }
            x_ = p_right.x;
            y_ = p_right.y;
            p_down.y = y_;
            while(true){
                if(y_ >= maprange_min.y && y_ <= maprange_max.y){
                    if(src.at<uint8_t >(Point(x_,y_)) == FREE_AREA){
                        y_ --;
                    }else{
                        if(y_ < p_down.y){
                            p_down.y = y_ + 1;
                        }
                        break;
                    }
                }else{
                    if(y_ < p_down.y){
                        p_down.y = y_ + 1;
                    }
                    break;
                }
            }
            LOGPLANNING(INFO) << "- down: (" << p_down.y << ") up: (" << p_up.y << ")";
            if(abs(input.y - p_down.y) > abs(p_up.y - input.y)){
                dir_clean_path_first = 2;//Y-方向
                output.x = p_right.x;
                output.y = p_up.y;
            }else{
                dir_clean_path_first = 1;//Y+方向
                output.x = p_right.x;
                output.y = p_down.y;
            }
            LOGPLANNING(INFO) << "dir_clean_lock: " << dir_clean_lock << ", dir_clean_path_first: " << dir_clean_path_first;
        }
    }
    return 1;
}
int CleanPath::ProjectiveP2L(Point2f p , Point2f p1 , Point2f p2 , Point2f & target){
    if(p2.x == p1.x){
        target.x = p1.x;
        target.y = p.y;
    }else if(p2.y == p1.y){
        target.x = p.x;
        target.y = p1.y;
    }else{
        double k = (p2.y - p1.y)/(p2.x - p1.x);
        target.x = (float)((k * p1.x + p.x / k + p1.y - p.y) / (1 / k + k));
        target.y = (float)(-1 / k * (target.x - p.x) + p.y);
    }
    return 1;
}

