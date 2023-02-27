//
// Created by lcy on 22-4-29.
//

#include <opencv2/imgproc.hpp>
#include "MotionMapManage.h"
#include "Map.h"

void MotionMapManage::fill_followmap(cv::Point p , int max){
    for(int n = 1 ; n < max ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){//mapexpand
            cv::Point ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.FollowMap.cols || ptmp.y >= s_map.FollowMap.rows){
                continue;
            }
            if(s_map.FollowMap.at<uint8_t >(ptmp) == FollowUnknowCell){
                s_map.FollowMap.at<uint8_t >(ptmp) = s_map.FollowObSidId;
            }
        }
    if(s_map.FollowMap.at<uint8_t >(p) == FollowUnknowCell){
        s_map.FollowMap.at<uint8_t >(p) = s_map.FollowObSidId;
    }
}

void MotionMapManage::fill_followmap_reset(cv::Point p , int id , cv::Point min , cv::Point max , int num){
    if(num < 1){
        LOGMOTION(INFO) << __LINE__ << ":num < 1 !!!";
        return;
    }
    for(int n = 1 ; n < num+1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){//mapexpand
            cv::Point ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < min.x || ptmp.y < min.y || ptmp.x > max.x || ptmp.y > max.y){
                continue;
            }
            s_map.FollowMap.at<uint8_t >(ptmp) = FollowUnknowCell;
        }
    s_map.FollowMap.at<uint8_t >(p) = FollowUnknowCell;
}

void MotionMapManage::fill_followmap_reset(cv::Point p , uint8_t cell , int num){
    if(num < 1){
        LOGMOTION(INFO) << __LINE__ << ":num < 1 !!!";
        return;
    }
    for(int n = 1 ; n < num+1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){//mapexpand
            cv::Point ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.FollowMap.cols || ptmp.y >= s_map.FollowMap.rows){
                continue;
            }
            s_map.FollowMap.at<uint8_t >(ptmp) = cell;
        }
    s_map.FollowMap.at<uint8_t >(p) = cell;
}

void MotionMapManage::fill_followmap_force(cv::Point p , uint8_t id , int num){
    if(num < 1){
        LOGMOTION(INFO) << __LINE__ << ":num < 1 !!!";
        return;
    }
    for(int n = 1 ; n < num+1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){//mapexpand
            cv::Point ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.FollowMap.cols || ptmp.y >= s_map.FollowMap.rows){
                continue;
            }
            s_map.FollowMap.at<uint8_t >(ptmp) = s_map.FollowObSidId;
        }
    s_map.FollowMap.at<uint8_t >(p) = s_map.FollowObSidId;
}

void MotionMapManage::fill_sensorobs(cv::Point2f p){
    cv::Point p_2 = s_map.WorldToMap(p);
    cv::Point ptmp;
        for(int n = 1 ; n < radius_obs + 1 ; n++)
            for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
                ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p_2.x,s_map.GetExpandSingleElement(n,i)->y + p_2.y);
                if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.width || ptmp.y >= s_map.height){
                    continue;
                }
                if(s_map.NavMap.at<uint8_t >(ptmp) == NavFreeCell){
                    s_map.NavMap.at<uint8_t >(ptmp) = NavObsExpandCell;
                }
            }
        s_map.NavMap.at<uint8_t >(p_2) = NavSensorObsCell;
}

void MotionMapManage::fill_bumperobs_init(const SensorInfo * dataup){
    p_bumper_last = dataup->Posture.pose;
}

void MotionMapManage::fill_bumperobs(const SensorInfo * dataup , int flg){
    double l = pow((dataup->Posture.pose.x - p_bumper_last.x),2) + pow((dataup->Posture.pose.y - p_bumper_last.y),2);
    if(l <= bumper_l_min){
        LOGMOTION(INFO) << "bumper 触发点相隔太近(" << l << ")";
    }else {
        LOGMOTION(INFO) << "相隔距离: " << sqrt(pow((dataup->Posture.pose.x - p_bumper_last.x),2) + pow((dataup->Posture.pose.y - p_bumper_last.y),2));
        cv::Point2f p = cv::Point2f((dataup->Posture.pose.x + p_bumper_last.x)/2,(dataup->Posture.pose.y + p_bumper_last.y)/2);
        double yam = 0;
        if(flg > 0){
            yam = atan2((dataup->Posture.pose.y - p_bumper_last.y),(dataup->Posture.pose.x - p_bumper_last.x)) + Pi_/2;//左沿边
            LOGMOTION(INFO) << "yam l:" << yam;
            yam = yam > (float)Pi_ ? (yam - 2 * (float)Pi_) : yam;
            p.x = p.x + (float)(ROBOTRADIUS * cos(yam));
            p.y = p.y + (float)(ROBOTRADIUS * sin(yam));
            fill_sensorobs(p);
        }else if(flg < 0){
            yam = atan2((dataup->Posture.pose.y - p_bumper_last.y),(dataup->Posture.pose.x - p_bumper_last.x)) - Pi_/2;//右沿边
            LOGMOTION(INFO) << "yam r:" << yam;
            yam = yam < -(float)Pi_ ? (yam + 2 * (float)Pi_) : yam;
            p.x = p.x + (float)(ROBOTRADIUS * cos(yam));
            p.y = p.y + (float)(ROBOTRADIUS * sin(yam));
            fill_sensorobs(p);
        }

    }
}

void MotionMapManage::fill_cover(cv::Point p , BoxInfo box , int num){
    cv::Point ptmp;
    for(int n = 1 ; n < num + 1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
            ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < box.point_min.x || ptmp.y < box.point_min.y || ptmp.x > box.point_max.x || ptmp.y > box.point_max.y){
                continue;
            }
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverCell;
        }
#if 1
    int num_out = num + 1;
    for(int i = 0 ; i < s_map.GetExpandSingleElement(num_out)->size() ; i++){
        ptmp = cv::Point(s_map.GetExpandSingleElement(num_out,i)->x + p.x,s_map.GetExpandSingleElement(num_out,i)->y + p.y);
        if(ptmp.x < box.point_min.x || ptmp.y < box.point_min.y || ptmp.x > box.point_max.x || ptmp.y > box.point_max.y){
            continue;
        }
        if(s_map.CoverMap.at<uint8_t >(ptmp) == CoverUnknowCell){
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverIgnoreCell;
        }
    }
#endif
}
void MotionMapManage::fill_cover_followobs(cv::Point p , int num){
    cv::Point ptmp;
    for(int n = 1 ; n < num + 1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
            ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.CoverMap.cols || ptmp.y >= s_map.CoverMap.rows){
                continue;
            }
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverCell;
        }
#if 1
    int num_out = num + 1;
    for(int i = 0 ; i < s_map.GetExpandSingleElement(num_out)->size() ; i++){
        ptmp = cv::Point(s_map.GetExpandSingleElement(num_out,i)->x + p.x,s_map.GetExpandSingleElement(num_out,i)->y + p.y);
        if(ptmp.x < 0 || ptmp.y < 0 || ptmp.x >= s_map.CoverMap.cols || ptmp.y >= s_map.CoverMap.rows){
            continue;
        }
        if(s_map.CoverMap.at<uint8_t >(ptmp) == CoverUnknowCell){
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverIgnoreCell;
        }
    }
#endif
}
void MotionMapManage::fill_cover_delimit(cv::Point p , const cv::Mat & mark ,BoxInfo box , int num){
    cv::Point ptmp;
    for(int n = 1 ; n < num + 1 ; n++)
        for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
            ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + p.x,s_map.GetExpandSingleElement(n,i)->y + p.y);
            if(mark.at<uint8_t >(ptmp) != box.id){
                continue;
            }
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverCell;
        }
#if 1
    int num_out = num + 1;
    for(int i = 0 ; i < s_map.GetExpandSingleElement(num_out)->size() ; i++){
        ptmp = cv::Point(s_map.GetExpandSingleElement(num_out,i)->x + p.x,s_map.GetExpandSingleElement(num_out,i)->y + p.y);
        if(mark.at<uint8_t >(ptmp) != box.id){
            continue;
        }
        if(s_map.CoverMap.at<uint8_t >(ptmp) == CoverUnknowCell){
            s_map.CoverMap.at<uint8_t >(ptmp) = CoverIgnoreCell;
        }
    }
#endif
}

void MotionMapManage::followInit(const PoseStamped & p , bool flg){
    clean_startfollowobs = p;
    Dir_360 = 0;
    if(flg){
        s_map.FollowObSidId = s_map.FollowObSidId < FollowObsIdMaxCell ? (uint8_t)(s_map.FollowObSidId + 1) : (uint8_t)0;
    }
}
int MotionMapManage::followState(const SensorInfo * sensor){
//    if (!sensor->Bumper.BumperTriggered && clean_startfollowobs.point != cv::Point(0, 0))
//	{
        if(sensor->workMode.workMode == workingMode_::WORK_DELIMITCLEAN){
            if(s_map.RegionSegmentationMap.at<uint8_t>(sensor->Posture.point) == CellType::RegionUnknowCell){
                LOGMOTION(INFO) << "follow obs out region";
                return 1;
            }
        }
		if (s_map.FollowMap.at<uint8_t>(sensor->Posture.point) == s_map.FollowObSidId){
			LOGMOTION(INFO) << "follow obs end at pass , pose:(" << sensor->Posture.point.x << " , " << sensor->Posture.point.y << ") , sideId:" << (int)s_map.FollowObSidId;
			return 3; //中途闭合
		}
        if (s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != s_map.FollowObSidId &&
            s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != FollowUnknowCell &&
            s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != FollowIgnoreCell)
        {
            LOGMOTION(INFO) << "follow obs step history path , stepId:" << (int)s_map.FollowMap.at<uint8_t>(sensor->Posture.point) << " , sideId:" << (int)s_map.FollowObSidId << "point:(" << sensor->Posture.point.x << " , " << sensor->Posture.point.y << ")";
            return 1;
        }
        int id = (int)floor((sensor->Posture.euler.yaw + Pi_) / 0.1047); // 0.104719755 == 6/180*Pi_
        if (Dir_360 == 0xFFFFFFFFFFFFFFF)
        {
            //            LOGMOTION(INFO) << "follow obs , Dir_360:" << Dir_360 << " , clean_startfollowobs:" << clean_startfollowobs.point.x << " , " <<clean_startfollowobs.point.y << "), pose(" <<sensor->Posture.point.x<<" , "<<sensor->Posture.point.y<<")";
            if (SquareDis_p2p(clean_startfollowobs.pose, sensor->Posture.pose) <= pow(ROBOTRADIUS , 2))
            {
                LOGMOTION(INFO) << "follow obs end at start , Dir_360:" << Dir_360 << " , sideId:" << (int)s_map.FollowObSidId;
                return 2; //闭合
            }
        }
        else
        {
            if (id < 60)
            {
                unsigned long long d = 0x01;
                Dir_360 |= (d << id);
            }
            //            LOGMOTION(INFO) << "follow obs , Dir_360:" << Dir_360 << " , sideId:" << (int)s_map.FollowObSidId;
        }
//    }
    return 0;
}