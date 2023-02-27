#ifndef PATHTRACKBOW_H
#define PATHTRACKBOW_H

#include "Common.h"
#include "config.h"
#include <vector>

namespace PathTrackBow
{
	enum PATHSEG
	{
		PATHSEG_INIT = 0,
		PATHSEG_ONE = 1,
		PATHSEG_TWO = 2,
		PATHSEG_THREE = 3
	};
	class pathTrackBow
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

		PoseStamped frontPathPoint;
		bool getFrontPathPoint();

		PATHSEG SEG;

		PoseStamped endSegPathPoint;

		std::vector<PoseStamped> segmentPath;
		bool getSegmentPath();

		PoseStamped robotPose;
		PoseStamped getRobotPose()
		{
			return robotPose;
		}

		std::vector< std::vector<PoseStamped> > pathBow;
		std::vector<PoseStamped> pathBowRviz;
		void insertPoint(const PoseStamped& point1, const PoseStamped& point2, std::vector<PoseStamped>& insertPath, const double& pointSpace, const bool& loop);
		std::vector<PoseStamped> insertPathBow(const std::vector<PoseStamped>& originPath_);
		void pathTrack1(float& cmdVl, float& cmdVa);
		void pathTrack2(float& cmdVl, float& cmdVa);
		void pathTrack3(float& cmdVl, float& cmdVa);

	public:
		pathTrackBow();
		void init();
		void setRobotPose(const PoseStamped& pose_)
		{
			robotPose = pose_;
		}
		bool isReached;  /*是否到达当前路径上的中间点*/
		bool isEndReached;   /*当前整条弓字是否走完*/
		bool isReachedNextPath;  /*当前弓字中的分段路径是否走完*/
		void setPathBow(const std::vector<PoseStamped>& path_);
		std::vector<PoseStamped> getPathBow()
		{
			return pathBowRviz;
		}
		void update(float& cmdVl, float& cmdVa);
		void clear();
	};
}

#endif