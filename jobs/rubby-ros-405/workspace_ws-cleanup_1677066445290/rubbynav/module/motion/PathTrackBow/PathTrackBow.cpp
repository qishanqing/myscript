#include "PathTrackBow.h"
namespace PathTrackBow
{
	pathTrackBow::pathTrackBow() : KP(0.0), KI(0.0), KD(0.0),
																err(0.0), lastErr(0.0), sum(0.0), out(0.0),
																angleSpaceConfig(0.0), angleSpaceCalcul(0.0),
																pointSpace(0.0),
																m_cmd(0.0),
																maxSpeedVl(0.32), maxSpeedVa(0.8),
																isReached(false), isEndReached(false), isReachedNextPath(false)
	{
		KP = s_config.readParam<float>("KPBow");
		KI = s_config.readParam<float>("KIBow");
		KD = s_config.readParam<float>("KDBow");
		angleSpaceConfig = s_config.readParam<float>("angleLimitBow");
		pointSpace = s_config.readParam<float>("pointSpacingBow");
		maxSpeedVl = s_config.readParam<float>("maxSpeedVlBow");
		maxSpeedVa = s_config.readParam<float>("maxSpeedVaBow");

		angleSpaceCalcul = 0.017453 * angleSpaceConfig;
		init();
	}

	void pathTrackBow::init()
	{
		err = 0.0;
		lastErr = 0.0;
		sum = 0.0;
		isReached = true;
		isEndReached = false;
		isReachedNextPath = true;
		SEG = PATHSEG_INIT;
	}

	void pathTrackBow::insertPoint(const PoseStamped& point1, const PoseStamped& point2, std::vector<PoseStamped>& insertPath, const double& pointSpace_, const bool& loop_)
	{
		double tmp = pointSpace_;
		std::vector<PoseStamped> vec_;
		PoseStamped point_;
		vec_.clear();
		vec_.emplace_back(point1);

		double deltax_ = point2.pose.x - point1.pose.x;
		double deltay_ = point2.pose.y - point1.pose.y;
		double theta_ = 0.0;
		double distance_ = hypot(deltax_, deltay_);

		if(loop_)
		{
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
		}
		else
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

	std::vector<PoseStamped> pathTrackBow::insertPathBow(const std::vector<PoseStamped>& originPath_)
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
			if(hypot(originPath_bk[0].pose.x - originPath_bk[1].pose.x, originPath_bk[0].pose.y - originPath_bk[1].pose.y) > 2 * pointSpace)
			{
				insertPoint(originPath_bk[0], originPath_bk[1], insertPath, pointSpace, true);
				insertPath.emplace_back(originPath_bk.back());
			}
			else
			{
				insertPath = originPath_bk;
			}

		}

		int length = insertPath.size();
		int slow = 0, fast = 1;
		while(fast < length)
		{
			if(insertPath[fast].pose.x != insertPath[slow].pose.x || insertPath[fast].pose.y != insertPath[slow].pose.y)
			{
				insertPath[++slow] = insertPath[fast];
			}
			fast++;
		}

		int diff = length - (slow + 1);
		while(diff--)
		{
			insertPath.pop_back();
		}

		return insertPath;
	}

	void pathTrackBow::setPathBow(const std::vector<PoseStamped>& path_)
	{
		for(int i = 0; i < path_.size(); i++)
		{
			LOGMOTION(INFO) << "Received origin Bow path[" << i << "] is: (" << path_[i].pose.x << ", " << path_[i].pose.y << ")";
		}
		std::vector<PoseStamped> pathBK = path_;
		if(pathBK.front().pose.x < -30 || pathBK.front().pose.x > 30 || pathBK.front().pose.y < -30 || pathBK.front().pose.y > 30)
		{
			LOGMOTION(INFO) << "Origin Bow path front point is: (" << pathBK.front().pose.x << ", " << pathBK.front().pose.y << ")";
			pathBK.erase(pathBK.begin());
			LOGMOTION(INFO) << "Delete Bow path front point !!!";
		}

		pathBow.clear();
		if(pathBK.size() == 4)
		{
			std::vector< std::vector<PoseStamped> > pathBowCopy;

			for(unsigned int i = 0, j = i + 1; j < pathBK.size(); i++, j++)
			{
				std::vector<PoseStamped> point;
				point.clear();
				point.emplace_back(pathBK[i]);
				point.emplace_back(pathBK[j]);
				pathBowCopy.emplace_back(point);
			}

			for(unsigned int i = 0; i < pathBowCopy.size(); i++)
			{
				std::vector<PoseStamped> pointInsert;
				pointInsert.clear();
				pointInsert = insertPathBow(pathBowCopy[i]);
				pathBowCopy[i].clear();
				pathBowCopy[i] = pointInsert;
			}
			pathBow = pathBowCopy;
		}
		else if(pathBK.size() == 2)
		{
			pathBow.emplace_back(insertPathBow(pathBK));
		}
		else
		{
			LOGMOTION(INFO) << "[ERROR-BOW] Original path size is ERROR";
		}

		for(unsigned int i = 0; i < pathBow.size(); i++)
		{
			for(unsigned int j = 0; j < pathBow[i].size(); j++)
			{
				pathBowRviz.emplace_back(pathBow[i][j]);
			}
		}
	}

	bool pathTrackBow::getSegmentPath()
	{
		endSegPathPoint = PoseStamped();
		segmentPath.clear();
		if(pathBow.size() > 0)
		{
			if(!pathBow.front().empty())
			{
				segmentPath = pathBow.front();
				endSegPathPoint = segmentPath.back();
				return true;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool pathTrackBow::getFrontPathPoint()
	{
		frontPathPoint = PoseStamped();
		if(segmentPath.size() > 0)
		{
			frontPathPoint = segmentPath.front();
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

	void pathTrackBow::pathTrack1(float& cmdVl_, float& cmdVa_)
	{
		PoseStamped robotPose_ = getRobotPose();
		if(segmentPath.size() > 0 && isReached)
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
					m_cmd -= 0.013;
					m_cmd = (m_cmd < 0.1) ? 0.1 : m_cmd;
				}
				else
				{
					m_cmd = 0.1;
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
			// LOGMOTION(INFO) << "BOW -- m_cmd: " << m_cmd;
			cmdVl_ = m_cmd;
			cmdVa_ = out;
		}
		else
		{
			cmdVl_ = 0.0;
			m_cmd = 0.0;
			cmdVa_ = 0.8 * turnDirection(robotPose_.euler.yaw, theta);
		}

		if(disFrontToCur < 0.04 && segmentPath.size() > 0)
		{
			isReached = true;
			segmentPath.erase(segmentPath.begin());
		}

		if(disEndToCur < 0.02 && segmentPath.size() == 0)
		{
			pathBow.erase(pathBow.begin());
			if(pathBow.size() > 1)
			{
				isReachedNextPath = true;
				isReached = true;
				SEG = PATHSEG_TWO;
			}
			else
			{
				isReachedNextPath = true;
				isReached = true;
				isEndReached = true;
			}
		}
	}

	void pathTrackBow::pathTrack2(float& cmdVl_, float& cmdVa_)
	{
		PoseStamped robotPose_ = getRobotPose();
		if(segmentPath.size() > 0 && isReached)
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

			if(disEndToCur < 0.085)
			{
				if(m_cmd > 0.1)
				{
					m_cmd -= 0.00625;
					m_cmd = (m_cmd < 0.1) ? 0.1 : m_cmd;
				}
				else
				{
					m_cmd = 0.1;
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

			cmdVl_ = m_cmd;
			cmdVa_ = out;
		}
		else
		{
			cmdVl_ = 0.0;
			m_cmd = 0.0;
			cmdVa_ = 0.8 * turnDirection(robotPose_.euler.yaw, theta);
		}

		if(disFrontToCur < 0.04 && segmentPath.size() > 0)
		{
			segmentPath.erase(segmentPath.begin());
			isReached = true;
		}

		if(disEndToCur < 0.02 && segmentPath.size() == 0)
		{
			cmdVl_ = 0.0;
			cmdVa_ = 0.0;
			m_cmd = 0.0;
			pathBow.erase(pathBow.begin());
			isReachedNextPath = true;
			isReached = true;
			SEG = PATHSEG_THREE;
		}
	}

	void pathTrackBow::pathTrack3(float& cmdVl_, float& cmdVa_)
	{
		PoseStamped robotPose_ = getRobotPose();
		if(segmentPath.size() > 0 && isReached)
		{
			if(getFrontPathPoint())
			{
				segmentPath.erase(segmentPath.begin());
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

			cmdVl_ = m_cmd;
			cmdVa_ = out;
		}
		else
		{
			cmdVl_ = 0.0;
			m_cmd = 0.0;
			cmdVa_ = 0.8 * turnDirection(robotPose_.euler.yaw, theta);
		}

		if(disFrontToCur < 0.02 && segmentPath.size() > 0)
		{
			isReached = true;
		}

		if(segmentPath.size() == 0)
		{
			pathBow.erase(pathBow.begin());
			isReachedNextPath = true;
			isReached = true;
			isEndReached = true;
		}
	}

	void pathTrackBow::update(float& cmdVl, float& cmdVa)
	{
		if(pathBow.size() > 0 && isReachedNextPath)
		{
			if(getSegmentPath())
			{
				isReachedNextPath = false;
			}
		}

		switch (SEG)
		{
		case PATHSEG_INIT:
			SEG = PATHSEG_ONE;
			break;

		case PATHSEG_ONE:
			pathTrack1(cmdVl, cmdVa);
			break;

		case PATHSEG_TWO:
			pathTrack2(cmdVl, cmdVa);
			break;

		case PATHSEG_THREE:
			pathTrack3(cmdVl, cmdVa);
			break;

		default:
			LOGMOTION(INFO) << "[ERROR] segmentPathNum is wrong..";
			break;
		}
	}

	void pathTrackBow::clear()
	{
		pathBow.clear();
		pathBowRviz.clear();
		init();
	}
}