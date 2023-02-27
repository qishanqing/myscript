#ifndef PATHTRACKNAV_H
#define PATHTRACKNAV_H

#include "Common.h"
#include "config.h"
#include <vector>

namespace PathTrackNav
{
	enum STEP
	{
		STEP_INIT = 0,
		STEP_TRACK
	};

	class pathTrackNav
	{
	private:
		double KP;
		double KI;
		double KD;
		double err;
		double lastErr;
		double sum;
		double out;

		double angleSpaceConfig;
		double angleSpaceCalcul;
		double pointSpace;

		float m_cmd;
		float maxSpeedVl;
		float maxSpeedVa;

		STEP step;

		PoseStamped frontPathPoint;
		bool getFrontPathPoint();

		PoseStamped robotPose;
		PoseStamped getRobotPose()
		{
			return robotPose;
		}

		std::vector< std::vector<PoseStamped> > pathNav;
		std::vector<PoseStamped> pathNavRviz;


		PoseStamped endSegPathPoint;
		std::vector<PoseStamped> segmentNavPath;
		bool getSegmentNavPath();

		void insertPoint(const PoseStamped& point1, const PoseStamped& point2, std::vector<PoseStamped>& insertPath);
		std::vector<PoseStamped> insertPathNav(const std::vector<PoseStamped>& originPath_);
		void judgeStep();
		void rotate(const std::vector<PoseStamped>& segmentPath_, float& cmdVl_, float& cmdVa_);
		void pathTrack(float& cmdVl, float& cmdVa);

	public:
		pathTrackNav();
		void init();
		void setRobotPose(const PoseStamped& pose_)
		{
			robotPose = pose_;
		}
		bool isReached;
		bool isEndReached;
		bool isReachedNextPath;  /*当前分段路径是否走完*/
		void setPathNav(const std::vector<PoseStamped>& path_);

		std::vector<PoseStamped> getPathNav()
		{
			return pathNavRviz;
		}
		void update(float& cmdVl, float& cmdVa);
		void clear();
	};
}

#endif