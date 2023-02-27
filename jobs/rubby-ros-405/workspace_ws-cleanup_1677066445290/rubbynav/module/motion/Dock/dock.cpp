#include "dock.h"

void Dock::DockInit()
{
	DockStep = STEP_IDLE;
	Kp = -0.05f;
	err = 0.0f;
	infraredGuidanceMode = false;
	LOGMOTION(INFO) << "[DOCK] DockStep is: " << DockStep;
	dockStep_bk = DockStep;
}

void Dock::Scan(const SensorInfo *sensor)
{
	static std::uint64_t startTime = nav::Now();
	static std::uint32_t noneSignalCount = 0;

	if ((sensor->Bms.Ir_leftB == BMS::C_region && sensor->Bms.Ir_rightB == BMS::B_region) || sensor->Bms.Ir_leftB == BMS::E_region || sensor->Bms.Ir_rightB == BMS::E_region)
	{
		noneSignalCount = 0;
		static std::uint32_t count = 0;
		count++;
		if (count > 1)
		{
			LOGMOTION(INFO) << "[DOCK] Locate Mid";
			count = 0;
			SetVel(0.0, 0.0);
			DockStep = STEP_ALIGNED;
		}
	}
	else if (sensor->Bms.Ir_left == BMS::A_region || sensor->Bms.Ir_leftB == BMS::A_region || sensor->Bms.Ir_right == BMS::A_region || sensor->Bms.Ir_rightB == BMS::A_region ||
			 sensor->Bms.Ir_left == BMS::B_region || sensor->Bms.Ir_leftB == BMS::B_region || sensor->Bms.Ir_right == BMS::B_region || sensor->Bms.Ir_rightB == BMS::B_region)
	{
		noneSignalCount = 0;
		dockDetector++;
	}
	else if (sensor->Bms.Ir_left == BMS::D_region || sensor->Bms.Ir_leftB == BMS::D_region || sensor->Bms.Ir_right == BMS::D_region || sensor->Bms.Ir_rightB == BMS::D_region ||
			 sensor->Bms.Ir_left == BMS::C_region || sensor->Bms.Ir_leftB == BMS::C_region || sensor->Bms.Ir_right == BMS::C_region || sensor->Bms.Ir_rightB == BMS::C_region)
	{
		noneSignalCount = 0;
		dockDetector--;
	}
	else if (dockDetector != 0 && nav::Now() - startTime > (1.0 * M_PI / 0.45) * 1e6)
	{
		noneSignalCount = 0;
		LOGMOTION(INFO) << "[DOCK] Ahead: " << dockDetector;
		DockStep = STEP_AHEAD_MID;
	}
	else if (sensor->Bms.Ir_leftB == BMS::Outside_region && sensor->Bms.Ir_rightB == BMS::Outside_region && sensor->Bms.Ir_left == BMS::Outside_region && sensor->Bms.Ir_right == BMS::Outside_region)
	{
		noneSignalCount++;
		if (noneSignalCount > 10 * 50)
		{
			LOGMOTION(INFO) << "[DOCK] Four ir: NONE_region, Turn";
			noneSignalCount = 0;
			DockStep = STEP_NONE_SIGNAL;
		}
	}
	else
	{
		LOGMOTION(INFO) << "[DOCK] Scan All conditions are not satisfy";
	}
}

void Dock::AheadMid(const SensorInfo *sensor)
{
	if (dockDetector > 0)
	{
		if ((std::fabs((M_PI / 2.0) - rotateAngle - AngleModify(GetRobotPose().euler.yaw)) < 0.1))
		{
			LOGMOTION(INFO) << "[DOCK] disAngle[1]: " << std::fabs((M_PI / 2.0) - rotateAngle - AngleModify(GetRobotPose().euler.yaw));
			SetVel(0.03, 0.0);
			moveMidPositionX = sensor->Posture.pose.x;
		    moveMidPositionY = sensor->Posture.pose.y;
			DockStep = STEP_MOVE_TO_MID;
		}
		else
		{
			LOGMOTION(INFO) << std::fabs((M_PI / 2.0) - rotateAngle - AngleModify(GetRobotPose().euler.yaw));
		}
	}
	else if (dockDetector < 0)
	{
		if ((std::fabs((-M_PI / 2.0) - rotateAngle - AngleModify(GetRobotPose().euler.yaw)) < 0.1))
		{
			LOGMOTION(INFO) << "[DOCK] Locate Left ready to align: " << -M_PI / 2.0 << " vs " << AngleModify(GetRobotPose().euler.yaw)
							<< ": " << std::fabs((-M_PI / 2.0) - rotateAngle - AngleModify(GetRobotPose().euler.yaw));
			SetVel(0.03, 0.0);
			moveMidPositionX = sensor->Posture.pose.x;
		    moveMidPositionY = sensor->Posture.pose.y;
			DockStep = STEP_MOVE_TO_MID;
		}
	}
	else
	{
		LOGMOTION(INFO) << "[DOCK] ERROR: should not show this log";
	}
}

void Dock::Move2Mid(const SensorInfo *sensor)
{
	static std::uint16_t filter = 0;
	static std::uint16_t safetyCount = 0;
	static std::uint16_t infisibleCount = 0;
	if (sensor->Bms.Ir_left != BMS::Outside_region || sensor->Bms.Ir_right != BMS::Outside_region)
	{
		ariseSignal = true;
		signalMidPositionX = sensor->Posture.pose.x;
		signalMidPositionY = sensor->Posture.pose.y;
	}
	if (dockDetector > 0)
	{
		if (sensor->Bms.Ir_left == BMS::E_region)
		{
			LOGMOTION(INFO) << "[DOCK] ready to aligned";
			filter++;
			if (filter > 1)
			{
				filter = 0;
				SetVel(0.0, -0.3);
				DockStep = STEP_TURN;
			}
		}
		else if (sensor->Bms.Ir_left == BMS::C_region)
		{
			LOGMOTION(INFO) << "[DOCK] ready to aligned, but sensor->Bms.Ir_left sees C_region";
			SetVel(0.0, -0.3);
			gotSignal = true;
			DockStep = STEP_TURN;
		}
		else if (sensor->Bms.Ir_left == BMS::D_region)
		{
			safetyCount++;
			if (safetyCount > 2)
			{
				LOGMOTION(INFO) << "[DOCK] Moved too far so could not dock, to SCAN";
				safetyCount = 0;
				SetVel(0.0, 0.0);
				gotSignal = true;
				DockStep = STEP_IDLE;
			}
		}
	}
	else if (dockDetector < 0)
	{
		if (sensor->Bms.Ir_right == BMS::E_region)
		{
			LOGMOTION(INFO) << "[DOCK] ready to aligned";
			filter++;
			if (filter > 1)
			{
				filter = 0;
				SetVel(0.0, 0.3);
				DockStep = STEP_TURN;
			}
		}
		else if (sensor->Bms.Ir_right == BMS::B_region)
		{
			LOGMOTION(INFO) << "[DOCK] ready to aligned, but sensor->Bms.Ir_right sees B_region";
			SetVel(0.0, 0.3);
			gotSignal = true;
			DockStep = STEP_TURN;
		}
		else if (sensor->Bms.Ir_right == BMS::A_region)
		{
			safetyCount++;
			if (safetyCount > 2)
			{
				LOGMOTION(INFO) << "[DOCK] Moved too far so could not dock, to SCAN";
				safetyCount = 0;
				SetVel(0.0, 0.0);
				gotSignal = true;
				DockStep = STEP_IDLE;
			}
		}
	}
	else
	{
		LOGMOTION(INFO) << "[DOCK] dockDetector =0";
	}

	if (gotSignal && sensor->Bms.Ir_left == BMS::Outside_region && sensor->Bms.Ir_right == BMS::Outside_region)
	{
		infisibleCount++;
		if (infisibleCount > 5)
		{
			LOGMOTION(INFO) << "[DOCK] Outside Region. ";
			gotSignal = false;
			infisibleCount = 0;
			SetVel(0.0, 0.0);
			DockStep = STEP_IDLE;
		}
	}
}

void Dock::Turn(const SensorInfo *sensor)
{
	static std::uint32_t turnNoSignalCount = 0;
	static std::uint16_t count = 0;
	if ((sensor->Bms.Ir_leftB == BMS::E_region && sensor->Bms.Ir_rightB == BMS::B_region) ||
		(sensor->Bms.Ir_rightB == BMS::E_region && sensor->Bms.Ir_leftB == BMS::C_region) ||
		(sensor->Bms.Ir_leftB == BMS::C_region && sensor->Bms.Ir_rightB == BMS::B_region) ||
		(sensor->Bms.Ir_leftB == BMS::E_region && sensor->Bms.Ir_rightB == BMS::E_region))
	{
		turnNoSignalCount = 0;
		count++;
		if (count > 2)
		{
			count = 0;
			LOGMOTION(INFO) << "[DOCK] Find mid region, ready to back move [1]";
			DockStep = STEP_ALIGNED;
		}
	}
	else if (sensor->Bms.Ir_leftB == BMS::E_region || sensor->Bms.Ir_rightB == BMS::E_region)
	{
		turnNoSignalCount = 0;
		count++;
		if (count > 3)
		{
			LOGMOTION(INFO) << "[DOCK] Find mid region, ready to back move [2]";
			DockStep = STEP_ALIGNED;
		}
	}
	else
	{
		turnNoSignalCount++;
		if (turnNoSignalCount > 10 * 50)
		{
			LOGMOTION(INFO) << "[DOCK] Turn NONE_region";
			turnNoSignalCount = 0;
			DockStep = STEP_IDLE;
		}
	}
}

void Dock::Aligned(const SensorInfo *sensor)
{
	static std::uint16_t midLeftCount = 0;
	static std::uint16_t midRightCount = 0;
	static std::uint16_t leftCount = 0;
	static std::uint16_t rightCount = 0;

	PoseStamped robotPose = GetRobotPose();

	if (sensor->Bms.Ir_leftB == BMS::E_region || sensor->Bms.Ir_rightB == BMS::E_region || (sensor->Bms.Ir_leftB == BMS::C_region && sensor->Bms.Ir_rightB == BMS::B_region))
	{
		if (sensor->Bms.Ir_leftB == BMS::E_region)
		{
			// if (sensor->Bms.Ir_rightB != BMS::B_region || sensor->Bms.Ir_rightB != BMS::C_region || sensor->Bms.Ir_rightB != BMS::E_region)
			if (sensor->Bms.Ir_rightB != BMS::E_region)
			{
				leftCount++;
				if (leftCount > 1)
				{
					lastThetaFlag = true;
					leftCount = 0;
					SetVel(-0.04, -0.1);
				}
			}
			else
			{
				leftCount++;
				if (leftCount > 1)
				{
					lastThetaFlag = true;
					leftCount = 0;
					SetVel(-0.04, 0.0);
				}
			}
		}
		else if (sensor->Bms.Ir_rightB == BMS::E_region)
		{
			// if (sensor->Bms.Ir_leftB != BMS::B_region || sensor->Bms.Ir_leftB != BMS::C_region || sensor->Bms.Ir_leftB != BMS::E_region)
			if (sensor->Bms.Ir_leftB != BMS::E_region)
			{
				leftCount++;
				if (leftCount > 1)
				{
					lastThetaFlag = true;
					leftCount = 0;
					SetVel(-0.04, 0.1);
				}
			}
			else
			{
				leftCount++;
				if (leftCount > 1)
				{
					lastThetaFlag = true;
					leftCount = 0;
					SetVel(-0.04, 0.0);
				}
			}
		}
		else
		{
			if (lastThetaFlag)
			{
				lastTheta = robotPose.euler.yaw;
				lastThetaFlag = false;
			}

			currentTheta = robotPose.euler.yaw;
			if (std::fabs(currentTheta - lastTheta) > M_PI)
			{
				currentTheta = (currentTheta < 0) ? (currentTheta + 2 * M_PI) : currentTheta;
				lastTheta = (lastTheta < 0) ? (lastTheta + 2 * M_PI) : lastTheta;
			}

			double v = 0, w = 0;
			if (std::fabs(currentTheta - lastTheta) < 0.2)
			{
				err = currentTheta - lastTheta;
				w = Kp * err;
				w = (w < -0.02) ? -0.02 : w;
				w = (w > 0.02) ? 0.02 : w;
				v = -0.045;
			}
			else
			{
				int turnDir = TurnDirection(currentTheta, lastTheta);
				v = -0.01;
				w = turnDir;
				w = (w > 0) ? 0.4 : -0.4;
				LOGMOTION(INFO) << "[DOCK] TEST: " << w;
			}
			SetVel(v, w);
		}
	}
	else if (sensor->Bms.Ir_leftB == BMS::B_region)
	{
		midLeftCount++;
		if (midLeftCount > 1)
		{
			lastThetaFlag = true;
			midLeftCount = 0;
			SetVel(-0.02, -0.2);
		}
	}
	else if (sensor->Bms.Ir_rightB == BMS::C_region)
	{
		midRightCount++;
		if (midRightCount > 1)
		{
			lastThetaFlag = true;
			midRightCount = 0;
			SetVel(-0.02, 0.2);
		}
	}
	else if (sensor->Bms.Ir_leftB == BMS::C_region && sensor->Bms.Ir_rightB == BMS::Outside_region)
	{
		midLeftCount++;
		if (midLeftCount > 1)
		{
			lastThetaFlag = true;
			midLeftCount = 0;
			SetVel(-0.02, 0.2);
		}
	}
	else if (sensor->Bms.Ir_rightB == BMS::B_region && sensor->Bms.Ir_leftB == BMS::Outside_region)
	{
		midRightCount++;
		if (midRightCount > 1)
		{
			lastThetaFlag = true;
			midRightCount = 0;
			SetVel(-0.02, -0.2);
		}
	}
	else if (sensor->Bms.Ir_leftB == BMS::A_region || sensor->Bms.Ir_rightB == BMS::A_region)
	{
		leftCount++;
		if (leftCount > 1)
		{
			lastThetaFlag = true;
			leftCount = 0;
			SetVel(-0.02, -0.3);
		}
	}
	else if (sensor->Bms.Ir_leftB == BMS::D_region || sensor->Bms.Ir_rightB == BMS::D_region)
	{
		rightCount++;
		if (rightCount > 1)
		{
			lastThetaFlag = true;
			rightCount = 0;
			SetVel(-0.02, 0.3);
		}
	}
	else
	{
		LOGMOTION(INFO) << "[DOCK] Unsatisfied signal";
		SetVel(0.0, 0.45);
		DockStep = STEP_SCAN;
	}
}

int Dock::TurnDirection(double now, double des)
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

void Dock::FindSignal(int number, const SensorInfo *sensor)
{
	static std::uint32_t findTime = 0;
	float sigValue = (number % 2) ? -1 : 1;
	if (number > 4)
	{
		LOGMOTION(INFO) << "[DOCK] number more than 4 time try";
		DockStep = STEP_ERROR;
		return;
	}
	if (!infraredGuidanceMode)
	{
		if ((std::fabs((sigValue * M_PI / 2.0) - AngleModify(GetRobotPose().euler.yaw)) < 0.1))
		{
			LOGMOTION(INFO) << "[DOCK] Find Signal , SetVel";
			SetVel(0.03, 0.0);
			findTime++;
			NAVLOG_EVERY_SEC(INFO, 0.5, "[NAV] [MOTION] [DOCK] findTime = " << findTime);
			if (findTime > 5 * number * 25)
			{
				findTime = 0;
				SetVel(0.0, 0.0);
				LOGMOTION(INFO) << "[DOCK] Find Signal , to IDLE";
				DockStep = STEP_IDLE;
			}
		}
	}
	else
	{
		LOGMOTION(INFO) << "[DOCK] Infrared Guidance Find Signal , SetVel";
		SetVel(0.03, 0.0);
		findTime++;
		if (findTime > 5 * 50)
		{
			findTime = 0;
			SetVel(0.0, 0.0);
			LOGMOTION(INFO) << "[DOCK] Infrared Guidance Find Signal  to IDLE";
			DockStep = STEP_IDLE;
		}
	}
}

bool Dock::CollisionProcessing(const SensorInfo *sensor)
{
	if (BumperType == COLLISIONTYPE_FIND_SIGNAL)
	{
		LOGMOTION(INFO) << "[DOCK] COLLISIONTYPE_FIND_SIGNAL";
		if (sqrt(pow(sensor->Posture.pose.x - currentPositionX, 2) + pow(sensor->Posture.pose.y - currentPositionY, 2)) > 0.02)
		{
			SetVel(0.0, 0.0);
			BumperType = COLLISIONTYPE_IDLE;
			DockStep = STEP_IDLE;
		}
	}
	else if (BumperType == COLLISIONTYPE_ALIGNED)
	{
		LOGMOTION(INFO) << "[DOCK] COLLISIONTYPE_ALIGNED";
		if (sqrt(pow(sensor->Posture.pose.x - currentPositionX, 2) + pow(sensor->Posture.pose.y - currentPositionY, 2)) > 0.10)
		{
			SetVel(0.0, 0.0);
			alignedTestNum++;
			BumperType = COLLISIONTYPE_IDLE;
			DockStep = STEP_IDLE;
		}
		if (alignedTestNum > 3)
		{
			alignedTestNum = 0;
			DockStep = STEP_ERROR;
			LOGMOTION(INFO) << "[DOCK] COLLISIONTYPE_ALIGNED try 3 ,but failure";
		}
	}
	else if (BumperType == COLLISIONTYPE_MOVE_TO_MID)
	{
		LOGMOTION(INFO) << "[DOCK] COLLISIONTYPE_MOVE_TO_MID";
		if (sqrt(pow(sensor->Posture.pose.x - currentPositionX, 2) + pow(sensor->Posture.pose.y - currentPositionY, 2)) > 0.05)
		{
			SetVel(0.0, 0.45);
			if ((std::fabs((M_PI)-rotateAngle - AngleModify(GetRobotPose().euler.yaw)) < 0.1))
			{
				SetVel(0.04, 0.0);
				if (sqrt(pow(sensor->Posture.pose.x - currentPositionX, 2) + pow(sensor->Posture.pose.y - currentPositionY, 2)) > 0.1)
				{
					BumperType = COLLISIONTYPE_IDLE;
					DockStep = STEP_IDLE;
					LOGMOTION(INFO) << "[DOCK] COLLISIONTYPE_MOVE_TO_MID to STEP_IDLE";
				}
			}
		}
		// BumperType ==COLLISIONTYPE_IDLE;
	}
	else if (BumperType == COLLISIONTYPE_MOVE_TO_MID_NOSIGNAL)
	{
		infraredGuidanceMode = true;
		DockStep = STEP_INFRARED_GUIDANCE;
	}
	else
	{
		LOGMOTION(INFO) << "[DOCK]  COLLISIONTYPE_IDLE";
	}
}

void Dock::InfraredGuidance(const SensorInfo *sensor)
{
	static std::uint32_t noSignalCount = 0;
	if (sensor->Bms.Ir_right == BMS::D_region || sensor->Bms.Ir_right == BMS::C_region)
		reversalSignal = true;
	else if(sensor->Bms.Ir_right == BMS::A_region || sensor->Bms.Ir_right == BMS::B_region)
		reversalSignal = false;

	if (sensor->Bms.Ir_left == BMS::A_region || sensor->Bms.Ir_right == BMS::D_region)
	{
		SetVel(0.2, 0.0);
		noSignalCount = 0;
	}
	else if (sensor->Bms.Ir_left == BMS::B_region || sensor->Bms.Ir_right == BMS::C_region)
	{
		SetVel(0.05, 0.0);
		noSignalCount = 0;
	}
		else if (sensor->Bms.Ir_left == BMS::C_region || sensor->Bms.Ir_right == BMS::B_region)
	{
		noSignalCount = 0;
	}
		else if (sensor->Bms.Ir_left == BMS::D_region || sensor->Bms.Ir_right == BMS::A_region)
	{
		noSignalCount = 0;
	}
	else if (sensor->Bms.Ir_left == BMS::E_region)
	{
		noSignalCount = 0;
		LOGMOTION(INFO) << "[DOCK]InfraredGuidance Into Turn";
		SetVel(0.0, -0.3);
		DockStep = STEP_TURN;
	}
	else if (sensor->Bms.Ir_right == BMS::E_region)
	{
		noSignalCount = 0;
		LOGMOTION(INFO) << "[DOCK] InfraredGuidance Into Turn";
		SetVel(0.0, 0.3);
		DockStep = STEP_TURN;
	}
	else if (sensor->Bms.Ir_leftB == BMS::E_region || sensor->Bms.Ir_rightB == BMS::E_region)
	{
		noSignalCount = 0;
		DockStep = STEP_ALIGNED;
		LOGMOTION(INFO) << "[DOCK] InfraredGuidance Into STEP_ALIGNED ";
	}
	else
	{
		if (reversalSignal)
			SetVel(0.0, -0.3);
		else
			SetVel(0.0, 0.3);
		noSignalCount++;
		if (noSignalCount > 13 * 50)
		{
			LOGMOTION(INFO) << "[DOCK] InfraredGuidance NONE_singal time=5s";
			noSignalCount = 0;
			DockStep = STEP_NONE_SIGNAL;
		}
	}
}

int Dock::DockRun(const SensorInfo *sensor, float &v, float &w)
{
	NAVLOG_EVERY_SEC(INFO, 0.5, "[NAV] [MOTION] [Dock] BMS(L,R,LB,RB): (" << sensor->Bms.Ir_left << ", " << sensor->Bms.Ir_right << ", " << sensor->Bms.Ir_leftB << ", " << sensor->Bms.Ir_rightB << ")");
	NAVLOG_EVERY_SEC(INFO, 0.5, "[NAV] [MOTION] [Dock] Pose(x,y,yaw): (" << sensor->Posture.pose.x << ", " << sensor->Posture.pose.y << ", " << sensor->Posture.euler.yaw << ")");
	rPose.pose.x = sensor->Posture.pose.x;
	rPose.pose.y = sensor->Posture.pose.y;
	rPose.euler.yaw = sensor->Posture.euler.yaw;

	if (sensor->Bms.isInPile)
	{
		LOGMOTION(INFO) << "[DOCK] Res: Charge SUCCESS";
		v = 0.0;
		w = 0.0;
		return NodeStatus::SUCCESS;
	}

	if (dockStep_bk != DockStep)
	{
		LOGMOTION(INFO) << "[DOCK] DockStep is: " << DockStep;
		dockStep_bk = DockStep;
	}

	switch (DockStep)
	{
	case STEP_TEST_MANUAL_START:
		break;

	case STEP_IDLE:
		if (!infraredGuidanceMode)
		{
			dockDetector = 0;
			ariseSignal = false;
			static std::uint32_t limitCount = 0;
			if ((sensor->Bms.Ir_leftB == BMS::C_region && sensor->Bms.Ir_rightB == BMS::B_region) || sensor->Bms.Ir_leftB == BMS::E_region || sensor->Bms.Ir_rightB == BMS::E_region)
			{
				static std::uint32_t count = 0;
				count++;
				if (count > 5)
				{
					limitCount = 0;
					count = 0;
					SetVel(0.0, 0.0);
					DockStep = STEP_ALIGNED;
				}
			}
			else if (limitCount > 35)
			{
				limitCount = 0;
				SetVel(0.0, 0.45);
				DockStep = STEP_SCAN;
			}
			limitCount++;
		}
		else
		{
			DockStep = STEP_INFRARED_GUIDANCE;
		}

		break;

	case STEP_SCAN:
		Scan(sensor);
		break;

	case STEP_AHEAD_MID:
		AheadMid(sensor);
		break;

	case STEP_MOVE_TO_MID:
		if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right || sensor->Bumper.Bumper_Front) // move to mid collision
		{
			LOGMOTION(INFO) << "[DOCK] STEP_MOVE_TO_MID Tigger Bumper";
			currentPositionX = sensor->Posture.pose.x;
			currentPositionY = sensor->Posture.pose.y;
			SetVel(-0.03, 0.0);
			BumperType = COLLISIONTYPE_MOVE_TO_MID;
			DockStep = STEP_TRIGGER_BUMPER;
		}
		else if (ariseSignal && sensor->Bms.Ir_left == BMS::Outside_region && sensor->Bms.Ir_right == BMS::Outside_region)
		{
			if (sqrt(pow(sensor->Posture.pose.x - signalMidPositionX, 2) + pow(sensor->Posture.pose.y - signalMidPositionY, 2)) > 0.2)
			{
				LOGMOTION(INFO) << "[DOCK] COLLISIONTYPE_MOVE_TO_MID_NO_SIGNAL, yaw PIAN";
				BumperType = COLLISIONTYPE_MOVE_TO_MID_NOSIGNAL;
				DockStep = STEP_TRIGGER_BUMPER;
			}
		}
		else if (!ariseSignal && sensor->Bms.Ir_left == BMS::Outside_region && sensor->Bms.Ir_right == BMS::Outside_region)
		{
			if (sqrt(pow(sensor->Posture.pose.x - moveMidPositionX, 2) + pow(sensor->Posture.pose.y - moveMidPositionY, 2)) > 0.7)
			{
				LOGMOTION(INFO) << "[DOCK] MOVE_TO_MID_ALWAYS_1m_NO SIGNAL";
				BumperType = COLLISIONTYPE_MOVE_TO_MID_NOSIGNAL;
				DockStep = STEP_TRIGGER_BUMPER;
			}
		}
		else
			Move2Mid(sensor);
		break;

	case STEP_TURN:
		Turn(sensor);
		break;

	case STEP_ALIGNED:
		static std::uint32_t collsionStopTime = 0;
		if ((sensor->motorCur.leftWheelMotorsCurrent > 0.08) || (sensor->motorCur.rightWheelMotorsCurrent) > 0.08) // aligned back collision
		{
			collsionStopTime++;
			if (collsionStopTime > 1 * 50)
			{
				collsionStopTime = 0;
				LOGMOTION(INFO) << "[DOCK]  STEP_ALIGNED COLLISION";
				currentPositionX = sensor->Posture.pose.x;
				currentPositionY = sensor->Posture.pose.y;
				SetVel(0.03, 0.0);
				BumperType = COLLISIONTYPE_ALIGNED;
				DockStep = STEP_TRIGGER_BUMPER;
			}
		}
		else if (sensor->Bms.Ir_leftB == BMS::Outside_region && sensor->Bms.Ir_rightB == BMS::Outside_region)
		{
			SetVel(0.0, 0.0);
			LOGMOTION(INFO) << "[DOCK]  MID line have obstacle";
			DockStep = STEP_IDLE;
		}
		else
			Aligned(sensor);
		break;

	case STEP_NONE_SIGNAL:
		number++;
		DockStep = STEP_FINDSIGNAL;
		break;

	case STEP_FINDSIGNAL:
		if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right || sensor->Bumper.Bumper_Front) // find signal collsion
		{
			LOGMOTION(INFO) << "[DOCK] Find Signal Tigger Bumper";
			currentPositionX = sensor->Posture.pose.x;
			currentPositionY = sensor->Posture.pose.y;
			SetVel(-0.03, 0.0);
			BumperType = COLLISIONTYPE_FIND_SIGNAL;
			DockStep = STEP_TRIGGER_BUMPER;
		}
		else
			FindSignal(number, sensor);
		break;

	case STEP_TRIGGER_BUMPER:
		CollisionProcessing(sensor);
		break;

	case STEP_INFRARED_GUIDANCE:
		InfraredGuidance(sensor);
		break;

	case STEP_ERROR:
		SetVel(0.0, 0.0);
		return NodeStatus::FAILURE;
		break;

	default:
		LOGMOTION(INFO) << "[DOCK] Dock default step: " << (int)DockStep;
		break;
	}

	v = (float)vx;
	w = (float)wz;
	return NodeStatus::RUNNING;
}
