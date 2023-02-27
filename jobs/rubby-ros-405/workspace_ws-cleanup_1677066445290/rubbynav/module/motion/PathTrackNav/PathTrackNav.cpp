#include "PathTrackNav.h"

namespace PathTrackNav
{
	pathTrackNav::pathTrackNav() : KP(0.0), KI(0.0), KD(0.0),
																err(0.0), lastErr(0.0), sum(0.0), out(0.0),
																angleSpaceConfig(0.0), angleSpaceCalcul(0.0),
																pointSpace(0.0),
																isReached(false), isEndReached(false),
																m_cmd(0.0), maxSpeedVl(0.32), maxSpeedVa(0.8)
	{
		KP = s_config.readParam<float>("KPNav");
		KI = s_config.readParam<float>("KINav");
		KD = s_config.readParam<float>("KDNav");
		angleSpaceConfig = s_config.readParam<float>("angleLimitNav");
		pointSpace = s_config.readParam<float>("pointSpacingNav");
		maxSpeedVl = s_config.readParam<float>("maxSpeedVlNav");
		maxSpeedVa = s_config.readParam<float>("maxSpeedVaNav");

		angleSpaceCalcul = 0.017453 * angleSpaceConfig;
		init();
	}

	void pathTrackNav::init()
	{
		err = 0.0;
		lastErr = 0.0;
		sum = 0.0;
		m_cmd = 0.0;
		isReached = true;
		isEndReached = false;
		isReachedNextPath = true;
		step = STEP_INIT;
	}

	void pathTrackNav::insertPoint(const PoseStamped& point1, const PoseStamped& point2, std::vector<PoseStamped>& insertPath)
	{
		double tmp = pointSpace;
		std::vector<PoseStamped> vec_;
		PoseStamped point_;
		vec_.clear();
		vec_.emplace_back(point1);

		double deltax_ = point2.pose.x - point1.pose.x;
		double deltay_ = point2.pose.y - point1.pose.y;
		double theta_ = 0.0;
		double distance_ = hypot(deltax_, deltay_); 

		while(distance_ > tmp)
		{
			if(point2.pose.x > point1.pose.x && point2.pose.y > point1.pose.y)
			{
				deltax_ = point2.pose.x - vec_.back().pose.x;
				deltay_ = point2.pose.y - vec_.back().pose.y;
				theta_ = atan2(deltay_, deltax_);            
				point_.pose.x = vec_.back().pose.x + tmp * cos(theta_);
				point_.pose.y = vec_.back().pose.y + tmp * sin(theta_);           
			}
			else if(point2.pose.x > point1.pose.x && point2.pose.y < point1.pose.y)
			{
				deltax_ = point2.pose.x - vec_.back().pose.x;
				deltay_ = vec_.back().pose.y - point2.pose.y;
				theta_ = atan2(deltax_, deltay_);            
				point_.pose.x = vec_.back().pose.x + tmp * sin(theta_);
				point_.pose.y = vec_.back().pose.y - tmp * cos(theta_);
			}
			else if(point2.pose.x < point1.pose.x && point2.pose.y > point1.pose.y)
			{
				deltax_ = vec_.back().pose.x - point2.pose.x;
				deltay_ = point2.pose.y - vec_.back().pose.y;
				theta_ = atan2(deltay_, deltax_);
				point_.pose.x = vec_.back().pose.x - tmp * cos(theta_);
				point_.pose.y = vec_.back().pose.y + tmp * sin(theta_); 
			}
			else if(point2.pose.x < point1.pose.x && point2.pose.y < point1.pose.y)
			{
				deltax_ = vec_.back().pose.x - point2.pose.x;
				deltay_ = vec_.back().pose.y - point2.pose.y;
				theta_ = atan2(deltax_, deltay_);
				point_.pose.x = vec_.back().pose.x - tmp * sin(theta_);
				point_.pose.y = vec_.back().pose.y - tmp * cos(theta_);
			}
			else if(point2.pose.x > point1.pose.x && point2.pose.y == point1.pose.y)
			{
				point_.pose.x = vec_.back().pose.x + tmp;
				point_.pose.y = vec_.back().pose.y;            
			}
			else if(point2.pose.x < point1.pose.x && point2.pose.y == point1.pose.y)
			{
				point_.pose.x = vec_.back().pose.x - tmp;
				point_.pose.y = vec_.back().pose.y;
			}
			else if(point2.pose.x == point1.pose.x && point2.pose.y > point1.pose.y)
			{
				point_.pose.x = vec_.back().pose.x;
				point_.pose.y = vec_.back().pose.y + tmp;
			}
			else if(point2.pose.x == point1.pose.x && point2.pose.y < point1.pose.y)
			{
				point_.pose.x = vec_.back().pose.x;
				point_.pose.y = vec_.back().pose.y - tmp;            
			}
			else
			{
				return ;
			} 

			vec_.emplace_back(point_);

			deltax_ = point2.pose.x - vec_.back().pose.x;
			deltay_ = point2.pose.y - vec_.back().pose.y;
			distance_ = hypot(deltax_, deltay_); 
		}

		for(auto i : vec_)
		{
			insertPath.emplace_back(i);
		}
	}

	std::vector<PoseStamped> pathTrackNav::insertPathNav(const std::vector<PoseStamped>& originPath_)
	{
		std::vector<PoseStamped> originPath_bk = originPath_;
		std::vector<PoseStamped> insertPath;
		insertPath.clear();

		if(originPath_bk.size() < 2)
		{
			return originPath_bk;
		}
		else
		{
			for(int i = 1; i < originPath_bk.size(); i++)
			{
				if(hypot(originPath_bk[i-1].pose.x - originPath_bk[i].pose.x, originPath_bk[i-1].pose.y - originPath_bk[i].pose.y) > 2 * pointSpace)
				{
					insertPoint(originPath_bk[i-1], originPath_bk[i], insertPath);
				}
				else
				{
					insertPath.emplace_back(originPath_bk[i-1]);
				}
			}
			insertPath.emplace_back(originPath_bk.back());
		}

		int fast = 0, slow = 0;
		for(fast, slow; fast < insertPath.size(); fast++)
		{
			if((insertPath[slow].pose.x != insertPath[fast].pose.x) || (insertPath[slow].pose.y != insertPath[fast].pose.y))
			{
				insertPath[++slow] = insertPath[fast];
			}
		}

		for(int i = slow + 1; i < insertPath.size(); i++)
		{
			insertPath.pop_back();
		}

		return insertPath;
	}

	void pathTrackNav::setPathNav(const std::vector<PoseStamped>& path_)
	{
		for(int i = 0; i < path_.size(); i++)
		{
			LOGMOTION(INFO) << "Received origin Nav path[" << i << "] is: (" << path_[i].pose.x << ", " << path_[i].pose.y << ")";
		}
		std::vector<PoseStamped> pathBK = path_;
		if(pathBK.front().pose.x < -30 || pathBK.front().pose.x > 30 || pathBK.front().pose.y < -30 || pathBK.front().pose.y > 30)
		{
			LOGMOTION(INFO) << "Origin Nav path front point is: (" << pathBK.front().pose.x << ", " << pathBK.front().pose.y << ")";
			pathBK.erase(pathBK.begin());
			LOGMOTION(INFO) << "Delete Nav path front point !!!";
		}

		pathNav.clear();
		if(pathBK.size() < 2)
		{
			LOGMOTION(INFO) << "[ERROR-NAV] Received Nav path size is: " <<  pathBK.size();
			exit(-1);
		}
		else if(pathBK.size() == 2)
		{
			pathNav.emplace_back(insertPathNav(pathBK));
		}
		else
		{
			int size = pathBK.size();

			std::vector< std::vector<PoseStamped> > pathNavCopy;
			for(unsigned int i = 0, j = i + 1; j < pathBK.size(); i++, j++)
			{
				std::vector<PoseStamped> point;
				point.clear();
				point.emplace_back(pathBK[i]);
				point.emplace_back(pathBK[j]);
				pathNavCopy.emplace_back(point);
			}

			for(unsigned int i = 0; i < pathNavCopy.size(); i++)
			{
				std::vector<PoseStamped> pointInsert;
				pointInsert.clear();
				pointInsert = insertPathNav(pathNavCopy[i]);
				pathNavCopy[i].clear();
				pathNavCopy[i] = pointInsert;
			}

			pathNav = pathNavCopy;
		}

		for(unsigned int i = 0; i < pathNav.size(); i++)
		{
			for(unsigned int j = 0; j < pathNav[i].size(); j++)
			{
				pathNavRviz.emplace_back(pathNav[i][j]);
			}
		}
	}

	bool pathTrackNav::getSegmentNavPath()
	{
		endSegPathPoint = PoseStamped();
		segmentNavPath.clear();
		if(pathNav.size() > 0)
		{
			if(!pathNav.front().empty())
			{
				segmentNavPath = pathNav.front();
				endSegPathPoint = segmentNavPath.back();
				return true;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool pathTrackNav::getFrontPathPoint()
	{
		if(segmentNavPath.size() > 0)
		{
			frontPathPoint = segmentNavPath.front();
			return true;
		}
		else
		{
			return false;
		}
	}

	int turnDirection(double now, double des)
	{
		int turn_direction = 1;
		if (std::fabs(des - now) > M_PI)
		{
			turn_direction = des - now > 0 ? -1 : 1;
		}
		else
		{
			turn_direction = des - now > 0 ? 1 : -1;
		}
		return turn_direction;
	}

	void pathTrackNav::pathTrack(float& cmdVl_, float& cmdVa_)
	{
	PoseStamped robotPose_ = getRobotPose();
		if(segmentNavPath.size() > 0 && isReached)
		{
			if(getFrontPathPoint())
			{
				isReached = false;
			}
		}

		double delta_x = frontPathPoint.pose.x - robotPose_.pose.x;
		double delta_y = frontPathPoint.pose.y - robotPose_.pose.y;
		double theta = atan2(delta_y, delta_x);
		double disFrontToCur = hypot(delta_x, delta_y);
		double disEndToCur = hypot(endSegPathPoint.pose.x - robotPose_.pose.x, endSegPathPoint.pose.y - robotPose_.pose.y);

		if(std::fabs(robotPose_.euler.yaw - theta) > Pi_)
		{
			if(robotPose_.euler.yaw < 0) robotPose_.euler.yaw += 2 * Pi_;
			if(theta < 0) theta += 2* Pi_;
		}

		if(((robotPose_.euler.yaw - theta) > (-1 * angleSpaceCalcul)) && ((robotPose_.euler.yaw - theta) < angleSpaceCalcul))
		{
			lastErr = err;
			err = robotPose_.euler.yaw - theta;
			sum += err;

			sum = (sum < (-1.6 * maxSpeedVa)) ? (-1.6 * maxSpeedVa) : sum;
			sum = (sum > (1.6 * maxSpeedVa)) ? (1.6 * maxSpeedVa) : sum;

			out = KP * err + KI * sum + KD * (err - lastErr);

			out = (out < (-1.6 * maxSpeedVa)) ? (-1.6 * maxSpeedVa) : out;
			out = (out > (1.6 * maxSpeedVa)) ? (1.6 * maxSpeedVa) : out;

			if(disEndToCur < 0.1)
			{
				if(m_cmd > 0.1)
				{
					m_cmd -= 0.01;
					m_cmd = (m_cmd < 0.1) ? 0.1 : m_cmd;
				}
				else
				{
					m_cmd = (m_cmd < 0.1) ? (m_cmd + 0.01) : 0.1;
				}
			}
			else
			{
				float desCmdAcc = maxSpeedVl * (1 - fabs(err) / (angleSpaceCalcul * 1.375));
				if(m_cmd < desCmdAcc)
				{
					m_cmd += 0.005;
					m_cmd = (m_cmd > desCmdAcc) ? desCmdAcc : m_cmd;
				}
				else
				{
					m_cmd = desCmdAcc;
				}
			}
			// LOGMOTION(INFO) << "NAV -- m_cmd: " << m_cmd;
			cmdVl_ = m_cmd;
			cmdVa_ = out;
		}
		else
		{
			cmdVl_ = 0.0;
			m_cmd = 0.0;
			cmdVa_ = 0.8 * turnDirection(robotPose_.euler.yaw, theta);
		}

		if(disFrontToCur < 0.04 && segmentNavPath.size() > 0)
		{
			isReached = true;
			segmentNavPath.erase(segmentNavPath.begin());
		}

		if(disEndToCur < 0.02 && segmentNavPath.size() == 0)
		{
			cmdVl_ = 0.0;
			m_cmd = 0.0;
			cmdVa_ = 0.0;
			pathNav.erase(pathNav.begin());

			if(pathNav.size() > 0)
			{
				isReachedNextPath = true;
				isReached = true;
				step = STEP_TRACK;
			}
			else
			{
				isReachedNextPath = true;
				isReached = true;
				isEndReached = true;
			}
		}
	}

	void pathTrackNav::update(float& cmdVl, float& cmdVa)
	{
		if(pathNav.size() > 0 && isReachedNextPath)
		{
			if(getSegmentNavPath())
			{
				isReachedNextPath = false;
			}
		}

		switch (step)
		{
		case STEP::STEP_INIT:
			step = STEP::STEP_TRACK;
			break;

		case STEP::STEP_TRACK:
			pathTrack(cmdVl, cmdVa);
			break;

		default:
			break;
		}
	}

	void pathTrackNav::clear()
	{
		pathNav.clear();
		pathNavRviz.clear();
		init();
	}
}