#include <utility>
#include <Map.h>

#include "motionsdk.h"

void MotionSdk::Destory()
{
#ifdef ROSDEBUG
    kill_thread_motion();
#endif
    if (sensor != nullptr)
    {
        sensor = nullptr;
    }
    if (datadown != nullptr)
    {
        datadown = nullptr;
    }
    if (!CleanBoxCur_history.empty())
    {
        CleanBoxCur_history.clear();
        std::vector<BoxInfo>().swap(CleanBoxCur_history);
    }
    if (!cleanpath.empty())
    {
        cleanpath.clear();
        vector<PoseStamped>().swap(cleanpath);
    }
    if (!navpath.empty())
    {
        navpath.clear();
        vector<PoseStamped>().swap(navpath);
    }
}

bool MotionSdk::motionInit(SensorInfo *SensorInfo_, DataDown *DataDown_)
{
    s_log.storageTypeInit(LOG_MOTION);

    if (SensorInfo_ != nullptr)
    {
        sensor = SensorInfo_;
    }
    else
    {
        LOGMOTION(INFO) << "SensorInfo_ is nullptr !!!";
        return false;
    }
    if (DataDown_ != nullptr)
    {
        datadown = DataDown_;
    }
    else
    {
        LOGMOTION(INFO) << "DataDown_ is nullptr !!!";
        return false;
    }
#ifdef ROSDEBUG
    creat_thread_motion();
#endif
    //    ControlInit();

    Relocation_go = s_config.readParam<float>("triangleSideLength");
    dis_overlook = s_config.readParam<float>("overLook");

    return true;
}

/*重定位*/
bool MotionSdk::reLocationInit()
{
    //    dlt_tmp = 0.0;
    //    robotpose_last = sensor->Posture;
    //    reLocation_step_cur = 1;
    LOGMOTION(INFO) << "reLocationInit";
    RelocationInit(Relocation_go);
    return true;
}

int MotionSdk::reLocationRun()
{
    datadown->MotionState = RELOCATION;
    datadown->FlgSlam = Reposition_RUNNING;

    int flg = RelocationRun(sensor, datadown->cmd);
    if (flg == 19)
    {
        datadown->MotionState = INIT;
        datadown->FlgSlam = Reposition_FINISHEND;
        return 2;
    }
    return 0;
}

/*建图*/
bool MotionSdk::firstbuildmapInit()
{
    datadown->MotionState = FIRSTBUILDMAP;
    FirstBuildInit(sensor->Posture.euler.yaw);
    return true;
}

bool MotionSdk::firstbuildmapRun()
{
    datadown->MotionState = FIRSTBUILDMAP;
    return FirstBuildRun(sensor->Posture.euler.yaw , datadown->cmd);
}

/*导航追踪*/
bool MotionSdk::followNavPathInit(vector<PoseStamped> &navPath_)
{
#ifdef ROSDEBUG
    navpath.clear();
    for (int i = 0; i < navPath_.size(); i++)
    {
        navpath.emplace_back(navPath_[i]);
        LOGMOTION(INFO) << "[NAV-BUG]: navpath " << i << ":(" << navpath[i].pose.x << "," << navpath[i].pose.y << ")";
    }
#endif
    return pathtrackInit(navPath_, sensor->Posture, insertPath, 1);
}

int MotionSdk::followNavPath()
{
    datadown->MotionState = NAVIGATION;
    Point point_overlook;
    point_overlook = s_map.WorldToMap(Point2f((float)(sensor->Posture.pose.x + dis_overlook * cos(sensor->Posture.euler.yaw)), (float)(sensor->Posture.pose.y + dis_overlook * sin(sensor->Posture.euler.yaw))));
    if (s_map.NavMap.at<uint8_t>(sensor->Posture.point) == NavFreeCell && s_map.NavMap.at<uint8_t>(point_overlook) != NavFreeCell && s_map.NavMap.at<uint8_t>(point_overlook) != NavUnknowCell && datadown->cmd.vl > 0.1f)
    {
        s_map.NavMap.at<uint8_t>(point_overlook) = NavFreeCell;
        datadown->cmd.vl = 0.0f;
        datadown->cmd.va = 0.0f;
        flg_navfollowobs = 0; //不沿边
        LOGMOTION(INFO) << "[NAV-BUG]: Nav Triggered !!!";
        return Nav_Follow_Fail;
    }
    if (s_map.DeepLearningMap.at<uint8_t>(sensor->Posture.point) == UnknowCell && s_map.DeepLearningMap.at<uint8_t>(point_overlook) == ObsCell && datadown->cmd.vl > 0.1f)
    {
        datadown->cmd.vl = 0.0f;
        datadown->cmd.va = 0.0f;
        flg_navfollowobs = 0; //不沿边
        LOGMOTION(INFO) << "[NAV-BUG]: DeepLearning Triggered !!!";
        return Nav_Follow_Fail;
    }

    if (sensor->Bumper.BumperTriggered || sensor->Cliff.CliffTriggered)
    {
        datadown->cmd.vl = 0.0f;
        datadown->cmd.va = 0.0f;
        if (sensor->Bumper.Bumper_Left)
        {
            flg_navfollowobs = 1; //左沿边
        }
        else
        {
            flg_navfollowobs = 2; //右沿边
        }
        LOGMOTION(INFO) << "[NAV-BUG]: Bumper Triggered!  l = " << sensor->Bumper.Bumper_Left << " ,f = " << sensor->Bumper.Bumper_Front << " ,r = " << sensor->Bumper.Bumper_Right;
        LOGMOTION(INFO) << "[NAV-BUG]: Cliff Triggered!  l = " << sensor->Cliff.Cliff_Left << " ,lf = " << sensor->Cliff.Cliff_LeftF << " ,rf = " << sensor->Cliff.Cliff_RightF << " ,r = " << sensor->Cliff.Cliff_Right;
        return Nav_Follow_Fail;
    }

    return pathtrackRun(sensor->Posture, datadown->cmd, 1);
}

/*弓形追踪*/
bool MotionSdk::followBowInit(vector<PoseStamped> &bowPath_)
{
#ifdef ROSDEBUG
    cleanpath.clear();
    for (int i = 0; i < bowPath_.size(); i++)
    {
        cleanpath.emplace_back(bowPath_[i]);
        LOGMOTION(INFO) << "[NAV-BUG]: CleanPath " << i << ":(" << cleanpath[i].pose.x << "," << cleanpath[i].pose.y << ")";
    }
#endif
    return pathtrackInit(bowPath_, sensor->Posture, insertPath, 2);
}

int MotionSdk::followBowPath()
{
    datadown->MotionState = BOW;
    fill_cover(sensor->Posture.point, boxinfo, 1);
    Point point_overlook;
    point_overlook = s_map.WorldToMap(Point2f((float)(sensor->Posture.pose.x + dis_overlook * cos(sensor->Posture.euler.yaw)), (float)(sensor->Posture.pose.y + dis_overlook * sin(sensor->Posture.euler.yaw))));
    if (s_map.NavMap.at<uint8_t>(point_overlook) != NavFreeCell && s_map.NavMap.at<uint8_t>(point_overlook) != NavUnknowCell && datadown->cmd.vl > 0.1f)
    {
        datadown->cmd.vl = 0.0;
        datadown->cmd.va = 0.0;
        flg_navfollowobs = 0; //不沿边
        LOGMOTION(INFO) << "[NAV-BUG]: Nav Triggered !!!";
        return Clean_Follow_Fail;
    }
    if (s_map.DeepLearningMap.at<uint8_t>(point_overlook) == ObsCell && datadown->cmd.vl > 0.1f)
    {
        datadown->cmd.vl = 0.0;
        datadown->cmd.va = 0.0;
        flg_navfollowobs = 0; //不沿边
        LOGMOTION(INFO) << "[NAV-BUG]: DeepLearning Triggered !!!";
        return Clean_Follow_Fail;
    }

    if (sensor->Bumper.BumperTriggered || sensor->Cliff.CliffTriggered)
    {
        datadown->cmd.vl = 0.0;
        datadown->cmd.va = 0.0;
        if (sensor->Bumper.Bumper_Left)
        {
            flg_navfollowobs = 1; //左沿边
        }
        else
        {
            flg_navfollowobs = 2; //右沿边
        }
        LOGMOTION(INFO) << "[NAV-BUG]: Bumper Triggered!  l = " << sensor->Bumper.Bumper_Left << " ,f = " << sensor->Bumper.Bumper_Front << " ,r = " << sensor->Bumper.Bumper_Right;
        LOGMOTION(INFO) << "[NAV-BUG]: Cliff Triggered!  l = " << sensor->Cliff.Cliff_Left << " ,lf = " << sensor->Cliff.Cliff_LeftF << " ,rf = " << sensor->Cliff.Cliff_RightF << " ,r = " << sensor->Cliff.Cliff_Right;

        return Clean_Follow_Fail;
    }

    return pathtrackRun(sensor->Posture, datadown->cmd, 2);
}

int MotionSdk::followBowPathDelimit()
{
    datadown->MotionState = BOW;
    fill_cover_delimit(sensor->Posture.point, s_map.RegionSegmentationMap, boxinfo, 1);
    Point point_overlook;
    point_overlook = s_map.WorldToMap(Point2f((float)(sensor->Posture.pose.x + dis_overlook * cos(sensor->Posture.euler.yaw)), (float)(sensor->Posture.pose.y + dis_overlook * sin(sensor->Posture.euler.yaw))));
    if (s_map.NavMap.at<uint8_t>(point_overlook) != NavFreeCell && s_map.NavMap.at<uint8_t>(point_overlook) != NavUnknowCell && datadown->cmd.vl > 0.1f)
    {
        datadown->cmd.vl = 0.0;
        datadown->cmd.va = 0.0;
        flg_navfollowobs = 0; //不沿边
        LOGMOTION(INFO) << "[NAV-BUG]: Nav Triggered !!!";
        return Clean_Follow_Fail;
    }
    if (s_map.DeepLearningMap.at<uint8_t>(point_overlook) == ObsCell && datadown->cmd.vl > 0.1f)
    {
        datadown->cmd.vl = 0.0;
        datadown->cmd.va = 0.0;
        flg_navfollowobs = 0; //不沿边
        LOGMOTION(INFO) << "[NAV-BUG]: DeepLearning Triggered !!!";
        return Clean_Follow_Fail;
    }

    if (sensor->Bumper.BumperTriggered || sensor->Cliff.CliffTriggered)
    {
        datadown->cmd.vl = 0.0;
        datadown->cmd.va = 0.0;
        if (sensor->Bumper.Bumper_Left)
        {
            flg_navfollowobs = 1; //左沿边
        }
        else
        {
            flg_navfollowobs = 2; //右沿边
        }
        LOGMOTION(INFO) << "[NAV-BUG]: Bumper Triggered!  l = " << sensor->Bumper.Bumper_Left << " ,f = " << sensor->Bumper.Bumper_Front << " ,r = " << sensor->Bumper.Bumper_Right;
        LOGMOTION(INFO) << "[NAV-BUG]: Cliff Triggered!  l = " << sensor->Cliff.Cliff_Left << " ,lf = " << sensor->Cliff.Cliff_LeftF << " ,rf = " << sensor->Cliff.Cliff_RightF << " ,r = " << sensor->Cliff.Cliff_Right;

        return Clean_Follow_Fail;
    }

    return pathtrackRun(sensor->Posture, datadown->cmd, 2);
}

/*沿边*/
bool MotionSdk::followObsInit(PoseStamped p_nav, PoseStamped p_obs)
{
    LOGMOTION(INFO) << "followObsInit: p_nav(" << p_nav.point.x << " , " << p_nav.point.y << ")-(" << p_nav.pose.x << ", " << p_nav.pose.y << ") , p_obs(" << p_obs.point.x << " , " << p_obs.point.y << ")-(" << p_obs.pose.x << " , " << p_obs.pose.y << ")";
    followObs_nav = p_nav;
    followObs_obs = p_obs;
    FollowObsInit(p_nav, p_obs);
    return true;
}

bool MotionSdk::followEdgeTypeInit()
{
    return true;
}

EdgeType MotionSdk::followEdgeTypeRun()
{
    return EdgeType::Edge_DEFAULT;
}

bool MotionSdk::followObsEdgeRightInit()
{
#if 0
    clean_startfollowobs = PoseStamped();
    Dir_360 = 0;
    s_map.FollowObSidId = s_map.FollowObSidId < FollowObsIdMaxCell ? (uint8_t)(s_map.FollowObSidId + 1) : (uint8_t)0;
#endif
//    FollowObsBumperRightInit();
    FollowObsIrRightInit();
	motionstate_last = 0;
	if(!followpath.empty()){
		followpath.clear();
	}
    LOGMOTION(INFO) << "followBowEdgeRightInit , sideId:" << (int)s_map.FollowObSidId;
    return true;
}

int MotionSdk::followObsEdgeRight()
{
    datadown->MotionState = FEBCLEANRIGHT;
#if 0
    if (!sensor->Bumper.BumperTriggered && clean_startfollowobs.point != Point(0, 0))
    {
        if (s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != s_map.FollowObSidId &&
            s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != FollowUnknowCell &&
            s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != FollowIgnoreCell)
        {
            LOGMOTION(INFO) << "follow obs step history path , stepId:" << s_map.FollowMap.at<uint8_t>(sensor->Posture.point) << " , sideId:" << (int)s_map.FollowObSidId << "point:(" << sensor->Posture.point.x << " , " << sensor->Posture.point.y << ")";
            BasicAction::RunParking(datadown->cmd);
            return NodeStatus::FAILURE;
        }
        int id = (int)floor((sensor->Posture.euler.yaw + Pi_) / 0.1047); // 0.104719755 == 6/180*Pi_
        if (Dir_360 == 0xFFFFFFFFFFFFFFF)
        {
            //            LOGMOTION(INFO) << "follow obs , Dir_360:" << Dir_360 << " , clean_startfollowobs:" << clean_startfollowobs.point.x << " , " <<clean_startfollowobs.point.y << "), pose(" <<sensor->Posture.point.x<<" , "<<sensor->Posture.point.y<<")";
            if (SquareDis_p2p(clean_startfollowobs.pose, sensor->Posture.pose) <= 0.05 * 0.05)
            {
                LOGMOTION(INFO) << "follow obs end at start , Dir_360:" << Dir_360 << " , sideId:" << (int)s_map.FollowObSidId;
                BasicAction::RunParking(datadown->cmd);
                return NodeStatus::SUCCESS; //闭合
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
    }
#endif

//    int flg = FollowObsBumperRight(sensor, datadown->cmd);
    int flg = FollowObsIrRight(sensor, datadown->cmd);
    if(flg < 0){
        LOGMOTION(INFO) << "can not touch Obs";
        return NodeStatus::SUCCESS;
    }else if (flg == 9 && motionstate_last == 4)
    {
		followObs_start = sensor->Posture;
        followInit(sensor->Posture , true);
    }
	motionstate_last = flg;
    if (flg > 4)
    {
#if 0
		if(followpath.empty()){
			followpath.emplace_front(sensor->Posture);
		}else if(sensor->Posture.point != followpath.front().point){
			if(followpath.size() >= 10){
				fill_followmap(followpath.back().point, 2);
				followpath.pop_back();
				followpath.emplace_front(sensor->Posture);
			}else{
				followpath.emplace_front(sensor->Posture);
			}
		}
#endif
        if(followpath.empty()){
            followpath.emplace_front(sensor->Posture.point);
        }else {
            list<Point>::iterator it = find(followpath.begin(), followpath.end(), sensor->Posture.point);
            if(it == followpath.end()){
                if(followpath.size() >= 10){
                    fill_followmap(followpath.back(), 2);
                    followpath.pop_back();
                    followpath.emplace_front(sensor->Posture.point);
                }else{
                    followpath.emplace_front(sensor->Posture.point);
                }
            }
		}

        //fill_followmap(sensor->Posture.point, 2);
        flg = followState(sensor);
        if(flg > 0){
            BasicAction::RunParking(datadown->cmd);
			while(!followpath.empty()){
				fill_followmap(followpath.back(), 2);
				followpath.pop_back();
			}
            if(flg == 1){//超过分区范围,沿边到历史路径,切左沿边
                return NodeStatus::FAILURE;
            }else if(flg == 2){//起点闭合
				line(s_map.FollowMap,followObs_start.point,sensor->Posture.point,Scalar(s_map.FollowObSidId),2,LINE_4);
                return NodeStatus::SUCCESS;
            }else if(flg == 3){//中途闭合,切左沿边
                return NodeStatus::FAILURE;
			}
        }
    }

    fill_cover_followobs(sensor->Posture.point, 1);

    return NodeStatus::RUNNING;
}

bool MotionSdk::getObsEdgeLeftStart(PoseStamped &PoseStamped_)
{
    PoseStamped_ = followObs_nav;
    LOGMOTION(INFO) << "getObsEdgeLeftStart , followObs_nav:(" << PoseStamped_.point.x << " , " << PoseStamped_.point.y << ")-(" << PoseStamped_.pose.x << " , " << PoseStamped_.pose.y << ")";
    return true;
}

bool MotionSdk::followObsEdgeLeftInit()
{
#if 0
    clean_startfollowobs = PoseStamped();
    Dir_360 = 0;
    s_map.FollowObSidId = s_map.FollowObSidId < FollowObsIdMaxCell ? (uint8_t)(s_map.FollowObSidId + 1) : (uint8_t)0;
#endif
    //FollowObsBumperLeftInit();
	FollowObsIrLeftInit();
	motionstate_last = 0;
	if(!followpath.empty()){
		followpath.clear();
	}
	leftFollowResetPose.clear();
    LOGMOTION(INFO) << "followBowEdgeLeftInit , sideId:" << (int)s_map.FollowObSidId;
    return true;
}

int MotionSdk::followObsEdgeLeft()
{
    datadown->MotionState = FEBCLEANLEFT;
#if 0
    if (!sensor->Bumper.BumperTriggered && clean_startfollowobs.point != Point(0, 0))
    {
        if (s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != s_map.FollowObSidId &&
            s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != FollowUnknowCell &&
            s_map.FollowMap.at<uint8_t>(sensor->Posture.point) != FollowIgnoreCell)
        {
            LOGMOTION(INFO) << "follow obs step history path , stepId:" << s_map.FollowMap.at<uint8_t>(sensor->Posture.point) << " , sideId:" << (int)s_map.FollowObSidId << "point:(" << sensor->Posture.point.x << " , " << sensor->Posture.point.y << ")";
            BasicAction::RunParking(datadown->cmd);
            return NodeStatus::FAILURE;
        }
        int id = (int)floor((sensor->Posture.euler.yaw + Pi_) / 0.1047); // 0.104719755 == 6/180*Pi_
        if (Dir_360 == 0xFFFFFFFFFFFFFFF)
        {
            //            LOGMOTION(INFO) << "follow obs , Dir_360:" << Dir_360 << " , clean_startfollowobs:" << clean_startfollowobs.point.x << " , " <<clean_startfollowobs.point.y << "), pose(" <<sensor->Posture.point.x<<" , "<<sensor->Posture.point.y<<")";
            if (SquareDis_p2p(clean_startfollowobs.pose, sensor->Posture.pose) <= 0.05 * 0.05)
            {
                LOGMOTION(INFO) << "follow obs end at start , Dir_360:" << Dir_360 << " , sideId:" << (int)s_map.FollowObSidId;
                BasicAction::RunParking(datadown->cmd);
                return NodeStatus::SUCCESS; //闭合
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
    }
#endif
    //int flg = FollowObsBumperLeft(sensor, datadown->cmd);
	int flg = FollowObsIrLeft(sensor, datadown->cmd);
    if(flg < 0){
        LOGMOTION(INFO) << "can not touch Obs";
        return NodeStatus::SUCCESS;
    }else if (flg == 9 && motionstate_last == 4)
    {
		//s_map.FollowMap.at<uint8_t >(ptmp) = s_map.FollowObSidId;
		line(s_map.FollowMap,followObs_start.point,sensor->Posture.point,Scalar(s_map.FollowObSidId),2,LINE_4);
        followInit(sensor->Posture , false);

		cv::Point ptmp;
		for(int n = 1 ; n < 4 ; n++)
			for(int i = 0 ; i < s_map.GetExpandSingleElement(n)->size() ; i++){
				ptmp = cv::Point(s_map.GetExpandSingleElement(n,i)->x + sensor->Posture.point.x,s_map.GetExpandSingleElement(n,i)->y + sensor->Posture.point.y);
				if(s_map.FollowMap.at<uint8_t >(ptmp) == s_map.FollowObSidId){
					s_map.FollowMap.at<uint8_t >(ptmp) = FollowUnknowCell;
					leftFollowResetPose.emplace_back(ptmp);
				}
			}
		if(s_map.FollowMap.at<uint8_t >(sensor->Posture.point) == s_map.FollowObSidId){
			s_map.FollowMap.at<uint8_t >(sensor->Posture.point) = FollowUnknowCell;
			leftFollowResetPose.emplace_back(sensor->Posture.point);
		}
		LOGMOTION(INFO) << "leftFollowResetPose size:" << leftFollowResetPose.size();
    }
	motionstate_last = flg;
    if (flg > 4)
    {
#if 0
		if(followpath.empty()){
			followpath.emplace_front(sensor->Posture);
		}else{
			bool sig = false;
			list<PoseStamped>::iterator iter;
			for(iter = followpath.begin() ; iter != followpath.end() ; iter++){
				if(iter->point == sensor->Posture.point){
					sig = true;
					break;
				}
			}
			if(!sig){
				if(followpath.size() >= 10){
					fill_followmap(followpath.back().point, 2);
					followpath.pop_back();
					followpath.emplace_front(sensor->Posture);
				}else{
					followpath.emplace_front(sensor->Posture);
				}
			}
		}
#endif
        if(followpath.empty()){
            followpath.emplace_front(sensor->Posture.point);
        }else {
            list<Point>::iterator it = find(followpath.begin(), followpath.end(), sensor->Posture.point);
            if(it == followpath.end()){
                if(followpath.size() >= 10){
                    fill_followmap(followpath.back(), 2);
                    followpath.pop_back();
                    followpath.emplace_front(sensor->Posture.point);
                }else{
                    followpath.emplace_front(sensor->Posture.point);
                }
            }
        }

        //fill_followmap(sensor->Posture.point, 2);
        flg = followState(sensor);
        if(flg > 0){
            BasicAction::RunParking(datadown->cmd);
			while(!followpath.empty()){
				fill_followmap(followpath.back(), 2);
				followpath.pop_back();
			}
			LOGMOTION(INFO) << "leftFollowResetPose reset size:" << leftFollowResetPose.size();
			while(!leftFollowResetPose.empty()){
				s_map.FollowMap.at<uint8_t >(leftFollowResetPose.back()) = s_map.FollowObSidId;
				leftFollowResetPose.pop_back();
			}
			LOGMOTION(INFO) << "followObsEdgeLeft ，flg:" << flg;
            if(flg == 1){
                return NodeStatus::SUCCESS;//踩非本次沿边历史路径
            }else if(flg == 2){
                return NodeStatus::SUCCESS; //闭合
            }else if(flg == 3){
				return NodeStatus::SUCCESS; //闭合
			}
        }
    }

    fill_cover_followobs(sensor->Posture.point, 1);

    return NodeStatus::RUNNING;
}

bool MotionSdk::followVirtualEdgeInit()
{
    return true;
}

NodeStatus MotionSdk::followVirtualEdgeRun()
{
    return NodeStatus::FAILURE;
}

/*避障*/
int MotionSdk::ObstacleAvoidanceInit()
{
    return ObstacleAvoid::ObstacleAvoidanceInit(sensor->Posture, flg_navfollowobs , datadown->MotionState);
}

int MotionSdk::ObstacleAvoidance()
{
    return ObstacleAvoid::ObstacleAvoidance(sensor, datadown->cmd);
}


/*上桩*/
bool MotionSdk::dockInit()
{
    DockInit();
    return true;
}

int MotionSdk::dockRun()
{
#ifdef RUBBYSIMULATION
    return true;
#endif
    int flg = DockRun(sensor , datadown->cmd.vl, datadown->cmd.va);
    if (flg != 0)
    {
        datadown->MotionState = motionState::INIT;
    }
    return flg;
}


/*下桩*/
bool MotionSdk::outDockInit()
{
    return OutDock::outDockInit(sensor, datadown);
}

bool MotionSdk::outDockRun()
{
    return OutDock::outDockRun(sensor, datadown, 0.1f);
}

void MotionSdk::completeTask()
{
    RunParking();
    LOGMOTION(INFO) << "send FINISHEDTASK succeed";
    datadown->MotionState = FINISHEDTASK;
    return;
}

bool MotionSdk::cleanBoxInit(BoxInfo BoxInfo_)
{
    boxinfo = std::move(BoxInfo_);
    return true;
}

bool MotionSdk::cleanBoxInitDelimit(BoxInfo BoxInfo_)
{
    boxinfo = std::move(BoxInfo_);
    return true;
}

/*停车*/
int MotionSdk::RunParking()
{
    return BasicAction::RunParking(datadown->cmd);
}

bool MotionSdk::setRotateAngle(double angle_)
{
    rotateAngle = angle_;
    return true;
}

/*脱困*/

bool MotionSdk::recoveryInit(const RecoveryStyle& recoveryStyle)
{
    return Recovery::RecoveryInit(recoveryStyle);
}

NodeStatus MotionSdk::recoveryRun(const RecoveryStyle& recoveryStyle)
{
    switch (recoveryStyle)
    {
    case RecoveryStyle::Recovery_Forbiden:
        return Recovery::RecoveryForbidenRun(datadown);
        break;
    case RecoveryStyle::Recovery_Obs:
        return Recovery::RecoveryObsRun(sensor, datadown);
        break;
    case RecoveryStyle::Recovery_Surrounded:
        return Recovery::RecoverySurroundedRun(sensor, datadown);
        break;
    case RecoveryStyle::Recovery_Physics:
        return Recovery::RecoveryPhysicsRun(datadown);
        break;
    
    default:
        break;
    }

    return NodeStatus::RUNNING;
}

#ifdef ROSDEBUG

#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <nav_msgs/OccupancyGrid.h>
#include <visualization_msgs/MarkerArray.h>
#include <sys/syscall.h>

int MotionSdk::creat_thread_motion()
{
    if (T_motion != nullptr)
    {
        LOGMOTION(INFO) << "T_motion is nullptr !!!";
        return 0;
    }

    T_motion = new thread([this]()
                          {
        //        int argc;
        //        char **argv;
        //        ros::init (argc, argv, "motionpublish");
        LOGMOTION(INFO) << "Enter T_motion , pid: " << getpid() << " , tid: " << syscall(SYS_gettid);
        LOGMOTION(INFO) << "this thread id is " << std::this_thread::get_id();
        ros::NodeHandle nh;
#ifndef SLAMTEST
        ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 10);
        ros::Publisher cleanpath_publisher_ = nh.advertise<nav_msgs::Path>("cleanpath", 25);
#endif
#ifdef NEWCONTROL
#ifndef SLAMTEST
        ros::Publisher insertPath_pub = nh.advertise<visualization_msgs::Marker>("insertPath_marker", 10);
#endif
#endif
        nav_msgs::Path mypath;
        mypath.header.frame_id = "odom";
        mypath.header.stamp = ros::Time(0);

        geometry_msgs::PoseStamped this_pose_stamped;
        geometry_msgs::Point p_marker;

        visualization_msgs::Marker points, line_strip, line_list;
        points.header.frame_id = line_strip.header.frame_id = line_list.header.frame_id = "/odom";
        points.header.stamp = line_strip.header.stamp = line_list.header.stamp = ros::Time::now();
        points.ns = line_strip.ns = line_list.ns = "navpoints_and_lines";
        points.action = line_strip.action = line_list.action = visualization_msgs::Marker::ADD;
        points.pose.orientation.w = line_strip.pose.orientation.w = line_list.pose.orientation.w = 1.0;

        points.id = 0;
        line_strip.id = 1;
        line_list.id = 2;

        points.type = visualization_msgs::Marker::POINTS;
        line_strip.type = visualization_msgs::Marker::LINE_STRIP;
        line_list.type = visualization_msgs::Marker::LINE_LIST;

        // POINTS markers use x and y scale for width/height respectively
        points.scale.x = 0.02;
        points.scale.y = 0.02;

        // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
        line_strip.scale.x = 0.01;
        line_list.scale.x = 0.01;

        // Points are green
        points.color.g = 1.0f;
        points.color.a = 1.0;

        // Line strip is blue
        line_strip.color.b = 1.0;
        line_strip.color.a = 1.0;

        // Line list is red
        line_list.color.r = 1.0;
        line_list.color.a = 1.0;

        visualization_msgs::Marker insertpoints, insertline_strip, insertline_list;
        insertpoints.header.frame_id = insertline_strip.header.frame_id = insertline_list.header.frame_id = "/odom";
        insertpoints.header.stamp = insertline_strip.header.stamp = insertline_list.header.stamp = ros::Time::now();
        insertpoints.ns = insertline_strip.ns = insertline_list.ns = "insert_points_and_lines";
        insertpoints.action = insertline_strip.action = insertline_list.action = visualization_msgs::Marker::ADD;
        insertpoints.pose.orientation.w = insertline_strip.pose.orientation.w = insertline_list.pose.orientation.w = 1.0;

        insertpoints.id = 0;
        insertline_strip.id = 1;
        insertline_list.id = 2;

        insertpoints.type = visualization_msgs::Marker::POINTS;
        insertline_strip.type = visualization_msgs::Marker::LINE_STRIP;
        insertline_list.type = visualization_msgs::Marker::LINE_LIST;

        // POINTS markers use x and y scale for width/height respectively
        insertpoints.scale.x = 0.02;
        insertpoints.scale.y = 0.02;

        // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
        insertline_strip.scale.x = 0.01;
        insertline_list.scale.x = 0.01;

        // Points are red
        insertpoints.color.r = 1.0f;
        insertpoints.color.a = 1.0;

        // Line strip is blue
        insertline_strip.color.b = 1.0;
        insertline_strip.color.a = 0.5;

        // Line list is red
        insertline_list.color.r = 1.0;
        insertline_list.color.a = 0.1;

        int num_count = 0;
        flg_thread_motion = true;
        while(flg_thread_motion) {
            if(!navpath.empty()){
                if(!points.points.empty()){
                    points.points.clear();
                }
                if(!line_strip.points.empty()){
                    line_strip.points.clear();
                }
                if(!line_list.points.empty()){
                    line_list.points.clear();
                }

                for(int i = 0 ; i < navpath.size() ; i++){
                    geometry_msgs::Point p;
                    p.x = navpath[i].pose.x;
                    p.y = navpath[i].pose.y;
                    p.z = 0.0;
                    points.points.push_back(p);
                    line_strip.points.push_back(p);

                    // The line list needs two points for each line
                    if(i != navpath.size() - 1){
                        line_list.points.push_back(p);
                        p.x = navpath[i + 1].pose.x;
                        p.y = navpath[i + 1].pose.y;
                        p.z = 0.0;
                        line_list.points.push_back(p);
                    }
                }
#ifndef SLAMTEST
                marker_pub.publish(points);
                marker_pub.publish(line_strip);
                marker_pub.publish(line_list);
#endif
            }

#ifdef NEWCONTROL
           if(!insertPath.empty()){
               if(!insertpoints.points.empty()){
                   insertpoints.points.clear();
               }
               if(!insertline_strip.points.empty()){
                   insertline_strip.points.clear();
               }
               if(!insertline_list.points.empty()){
                   insertline_list.points.clear();
               }

                for(int i = 0 ; i < insertPath.size() ; i++){
                    geometry_msgs::Point p;
                    p.x = insertPath[i].pose.x;
                    p.y = insertPath[i].pose.y;
                    p.z = 0.0;
                    insertpoints.points.push_back(p);
                    insertline_strip.points.push_back(p);

                    // The line list needs two points for each line
                    if(i != insertPath.size() - 1){
                        insertline_list.points.push_back(p);
                        p.x = insertPath[i + 1].pose.x;
                        p.y = insertPath[i + 1].pose.y;
                        p.z = 0.0;
                        insertline_list.points.push_back(p);
                    }
                }
#ifndef SLAMTEST
                insertPath_pub.publish(insertpoints);
                insertPath_pub.publish(insertline_strip);
                insertPath_pub.publish(insertline_list);
#endif
            }
#endif
            if(!cleanpath.empty()){
                if(!mypath.poses.empty()){
                    mypath.poses.clear();
                }
                for(int i = 0 ; i < cleanpath.size() ; i++){
                    this_pose_stamped.pose.position.x = cleanpath[i].pose.x;
                    this_pose_stamped.pose.position.y = cleanpath[i].pose.y;
                    this_pose_stamped.pose.orientation.x = 0;//goal_quat.x;
                    this_pose_stamped.pose.orientation.y = 0;//goal_quat.y;
                    this_pose_stamped.pose.orientation.z = 0;//goal_quat.z;
                    this_pose_stamped.pose.orientation.w = 0;//goal_quat.w;
                    this_pose_stamped.header.frame_id="odom";
                    this_pose_stamped.header.stamp = ros::Time(0);
                    mypath.poses.emplace_back(this_pose_stamped);
                }
#ifndef SLAMTEST
                cleanpath_publisher_.publish(mypath);
#endif
            }
            num_count ++;
            if(num_count % 50 == 0){
            }

            this_thread::sleep_for(chrono::microseconds(20000));//50HZ
        }
#ifndef SLAMTEST
        marker_pub.shutdown();
        cleanpath_publisher_.shutdown();
#endif
#ifdef NEWCONTROL
#ifndef SLAMTEST
        insertPath_pub.shutdown();
#endif
#endif
        if(!points.points.empty()){
            points.points.clear();
        }
        if(!line_strip.points.empty()){
            line_strip.points.clear();
        }
        if(!line_list.points.empty()){
            line_list.points.clear();
        }
        if(!insertpoints.points.empty()){
            insertpoints.points.clear();
        }
        if(!insertline_strip.points.empty()){
            insertline_strip.points.clear();
        }
        if(!insertline_list.points.empty()){
            insertline_list.points.clear();
        }
        if(!mypath.poses.empty()){
            mypath.poses.clear();
        } });
    return 0;
}

int MotionSdk::kill_thread_motion()
{
    flg_thread_motion = false;
    if (T_motion != nullptr)
    {
        T_motion->join();
        delete T_motion;
        T_motion = nullptr;
    }
    return 0;
}

#endif