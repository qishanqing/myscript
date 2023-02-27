#include "rubbyHsm.h"
#include <unistd.h>
#include <iostream>
using std::endl;

namespace rubbyHSM
{
	/********************** class rubbyOwner ***********************/

	rubbyOwner::rubbyOwner(struct SensorInfo* SensorInfo_, struct DataDown* DataDown_)  : _SensorInfo(SensorInfo_), _DataDown(DataDown_), 
																																												_hsmMode(rubbyHSM::workingModeName::NO_WORK),
																																												_hsmIsMotionInit(false), _hsmIsPlanningInit(false)
	{
		HSMMODE = rubbyHSM::workingModeName::NO_WORK;
		_hsmIsInPileRecharge = false;
		countHSM = false;
		count2HSM = false;
		count3HSM = false;
		count4HSM = false;

		_hsmTellSLAMStartFlag = false;
		boxflag = 0;
		_hsmIsStartPile = false;
		_hsmIsStartRelocation = false;
		_hsmIsEnd = false;
		_hsmIsNeedVoice = false;

		_hsmIsKidnapFlag = false;
		_hsmIsKidnapUp = false;
		_hsmIsMergeMapEnd = false;

		_hsmReLocationInit_NoKidnap = false;
		_hsmReLocation_NoKidnap = NodeStatus::DEFAULT;
		_hsmIsExecReLocation_NoKidnap = false;
		_hsmRelocationMapInit_NoKidnap = false;
		_hsmExecRelocationMapResult_NoKidnap = false;
		_hsmRelocationMapResult_NoKidnap = false;

		_hsmReLocationInit_Kidnap = false;
		_hsmReLocation_Kidnap = NodeStatus::DEFAULT;
		_hsmIsExecReLocation_Kidnap = false;
		_hsmRelocationMapInit_Kidnap = false;
		_hsmExecRelocationMapResult_Kidnap = false;
		_hsmRelocationMapResult_Kidnap = false;

		_hsmIsExecMotionAndPlanningInit = false;

		_hsmRoutineInitFlag_ROU = false;
		_hsmIsRoutineInit_ROU  =false;
		_hsmIsExecGetCleanBox_ROU = false;
		_hsmIsExecGetCleanStartPoint_ROU = false;
		_hsmIsGetCleanStartPoint_ROU = SiggetCleanStart::FindUncleanPoint_Null;
		_hsmIsExecNav_ROU = false;
		_hsmIsSucceedNav_ROU = SigNav::Nav_fail;
		_hsmIsMotionNavPathInit_ROU = false;
		_hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_ROU = false;
		_hsmIsGetBowPath_ROU = false;
		_hsmIsExecGetBowPath_ROU = false;
		_hsmIsGetBowShapeInit_ROU = false;
		_hsmIsMotionBowInit_ROU = false;
		_hsmIsMotionBow_ROU = NodeStatus::DEFAULT;
		_hsmIsExecMotionBow_ROU = false;
		_hsmIsGetSlamPath_ROU = false;
		_hsmIsExecGetSlamPath_ROU = false;
		_hsmIsMergeSlamPath_ROU = false;
		_hsmIsExecMergeSlamPath_ROU = false;
		_hsmIsObstacleAvoidInit_ROU = false;
		_hsmIsExecObstacleAvoid_ROU = false;
		_hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;
		_hsmIsExecGetAndMergeSlamPath_ROU = false;
		_hsmIsUpperPileInit_ROU = false;
		_hsmIsExecUpperPile_ROU = false;
		_hsmIsUpperPile_ROU = NodeStatus::DEFAULT;
		_hsmIsUnderPileInit_ROU = false;
		_hsmIsExecUnderPile_ROU = false;
		_hsmIsUnderPile_ROU = false;
		_hsmIsExecSearchObsPoint_ROU = false;
		_hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;
		_hsmIsFollowEdgeRightInit_ROU = false;
		_hsmIsExecFollowEdgeRight_ROU = false;
		_hsmIsFollowEdgeRight_ROU = NodeStatus::DEFAULT;
		_hsmIsExecGetObsLeftStart_ROU = false;
		_hsmIsGetObsLeftStart_ROU = false;
		_hsmIsFollowEdgeLeftInit_ROU = false;
		_hsmIsExecFollowEdgeLeft_ROU = false;
		_hsmIsFollowEdgeLeft_ROU = NodeStatus::DEFAULT;

		_hsmDelimitInitFlag_DEL = false;
		_hsmIsDelimitInit_DEL  =false;
		_hsmIsExecGetCleanBox_DEL = false;
		_hsmIsExecGetCleanStartPoint_DEL = false;
		_hsmIsGetCleanStartPoint_DEL = SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;
		_hsmIsExecNav_DEL = false;
		_hsmIsSucceedNav_DEL = SigNav::Nav_fail;
		_hsmIsMotionNavPathInit_DEL = false;
		_hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_DEL = false;
		_hsmIsGetBowPath_DEL = false;
		_hsmIsExecGetBowPath_DEL = false;
		_hsmIsGetBowShapeInit_DEL = false;
		_hsmIsMotionBowInit_DEL = false;
		_hsmIsMotionBow_DEL = NodeStatus::DEFAULT;
		_hsmIsExecMotionBow_DEL = false;
		_hsmIsGetSlamPath_DEL = false;
		_hsmIsExecGetSlamPath_DEL = false;
		_hsmIsMergeSlamPath_DEL = false;
		_hsmIsExecMergeSlamPath_DEL = false;
		_hsmIsObstacleAvoidInit_DEL = false;
		_hsmIsExecObstacleAvoid_DEL = false;
		_hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;
		_hsmIsExecGetAndMergeSlamPath_DEL = false;
		_hsmIsUpperPileInit_DEL = false;
		_hsmIsExecUpperPile_DEL = false;
		_hsmIsUpperPile_DEL = NodeStatus::DEFAULT;
		_hsmIsUnderPileInit_DEL = false;
		_hsmIsExecUnderPile_DEL = false;
		_hsmIsUnderPile_DEL = false;
		_hsmIsExecSearchObsPoint_DEL = false;
		_hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;
		_hsmIsFollowEdgeRightInit_DEL = false;
		_hsmIsExecFollowEdgeRight_DEL = false;
		_hsmIsFollowEdgeRight_DEL = NodeStatus::DEFAULT;
		_hsmIsExecGetObsLeftStart_DEL = false;
		_hsmIsGetObsLeftStart_DEL = false;
		_hsmIsFollowEdgeLeftInit_DEL = false;
		_hsmIsExecFollowEdgeLeft_DEL = false;
		_hsmIsFollowEdgeLeft_DEL = NodeStatus::DEFAULT;

		_hsmIsExecNav_REC = false;
		_hsmIsSucceedNav_REC = SigNav::Nav_fail;
		_hsmIsMotionNavPathInit_REC = false;
		_hsmIsMotionNavPath_REC = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_REC = false;
		_hsmIsObstacleAvoidInit_REC = false;
		_hsmIsExecObstacleAvoid_REC = false;
		_hsmIsObstacleAvoid_REC = NodeStatus::DEFAULT;
		_hsmIsUpperPileInit_REC = false;
		_hsmIsExecUpperPile_REC = false;
		_hsmIsUpperPile_REC = NodeStatus::DEFAULT;

		_hsmIsRecoveryInit = false;
		_hsmIsExecRecovery = false;
		_hsmIsRecovery = NodeStatus::DEFAULT;

		_commonSdk = new CommonSdk;
		_motionSdk = new MotionSdk;
		_planningSdk = new PlanningSdk;

		#ifdef RUBBYSIMULATION
		google::InitGoogleLogging("NAV-HSM");
		FLAGS_alsologtostderr = true;
		FLAGS_colorlogtostderr = true;
		google::SetLogDestination(google::INFO, "../src/log/");
		#endif
		
		boxflag = s_config.readParam<int>("boxNumber");
		
		rubbyStateMachine.Initialize<rubbyStates::interactive>(this);
		rubbyStateMachine.SetDebugInfo("TestHsm", TraceLevel::Basic);
	}

	rubbyOwner::~rubbyOwner()
	{
		#ifdef RUBBYSIMULATION
		google::ShutdownGoogleLogging();
		#endif

		destoryHsm();
	}

	void rubbyOwner::updateStateMachine(std::fstream& FILE_)
	{
		_FILE = &FILE_;
		rubbyStateMachine.ProcessStateTransitions();
		rubbyStateMachine.UpdateStates();
	}

	void rubbyOwner::updateMap()
	{
		_planningSdk->updateMap();
	}

	void rubbyOwner::resetAllParams()
	{
		/*class rubbySTATICLOG*/
		countHSM = false;
		count2HSM = false;
		count3HSM = false;
		count4HSM = false;

		/*rubbyArgs*/
		_startPoseStamped_ROU = PoseStamped();
		_BoxInfo_ROU.resetBox();
		_navPath_ROU.clear();
		_bowPath_ROU.clear();
		_slamPath_ROU.clear();
		_PoseStamped_ROU = PoseStamped();
		_start_ROU = PoseStamped();
		_end_ROU = PoseStamped();
		_poseRecord_ROU = PoseStamped();
		_slamPose_ROU = PoseStamped();
		_searchNavPoint_ROU = PoseStamped();
		_searchObsPoint_ROU = PoseStamped();
		_obsLeftStart_ROU = PoseStamped();

		_startPoseStamped_DEL = PoseStamped();
		_BoxInfo_DEL.resetBox();
		_navPath_DEL.clear();
		_bowPath_DEL.clear();
		_slamPath_DEL.clear();
		_PoseStamped_DEL = PoseStamped();
		_start_DEL = PoseStamped();
		_end_DEL = PoseStamped();
		_poseRecord_DEL = PoseStamped();
		_slamPose_DEL = PoseStamped();
		_searchNavPoint_DEL = PoseStamped();
		_searchObsPoint_DEL = PoseStamped();
		_obsLeftStart_DEL = PoseStamped();

		_startPoseStamped_REC = PoseStamped();
		_navPath_REC.clear();
		_start_REC = PoseStamped();
		_end_REC = PoseStamped();

		/*rubbyHSMArgs*/
		HSMMODE = rubbyHSM::workingModeName::NO_WORK;

		_hsmIsInPileRecharge = false;
		_hsmTellSLAMStartFlag = false;
		_hsmIsStartRelocation = false;

		_hsmIsEnd = false;
		_hsmIsNeedVoice = false;

		_hsmIsMergeMapEnd = false;
		_hsmIsStartPile = false;

		/* --- */
		_hsmIsKidnapFlag = false;
		_hsmIsKidnapUp = false;

		_hsmReLocationInit_NoKidnap = false;
		_hsmReLocation_NoKidnap = NodeStatus::DEFAULT;
		_hsmIsExecReLocation_NoKidnap = false;

		_hsmRelocationMapInit_NoKidnap = false;
		_hsmExecRelocationMapResult_NoKidnap = false;
		_hsmRelocationMapResult_NoKidnap = false;

		_hsmReLocationInit_Kidnap = false;
		_hsmReLocation_Kidnap = NodeStatus::DEFAULT;
		_hsmIsExecReLocation_Kidnap = false;

		_hsmRelocationMapInit_Kidnap = false;
		_hsmExecRelocationMapResult_Kidnap = false;
		_hsmRelocationMapResult_Kidnap = false;

		/* --- */
		_hsmIsRoutineInit_ROU = false;
		_hsmRoutineInitFlag_ROU = false;
		_hsmIsExecGetCleanBox_ROU = false;
		_hsmIsExecGetAndMergeSlamPath_ROU = false;

		_hsmIsDelimitInit_DEL = false;
		_hsmDelimitInitFlag_DEL = false;
		_hsmIsExecGetCleanBox_DEL = false;
		_hsmIsExecGetAndMergeSlamPath_DEL = false;

		_hsmIsExecMotionAndPlanningInit = false;

		/* --- */
		_hsmIsExecGetCleanStartPoint_ROU = false;
		_hsmIsGetCleanStartPoint_ROU = SiggetCleanStart::FindUncleanPoint_Null;
		_hsmIsExecNav_ROU = false;
		_hsmIsSucceedNav_ROU = SigNav::Nav_fail;
		_hsmIsMotionNavPathInit_ROU = false;
		_hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_ROU = false;
		_hsmIsGetBowPath_ROU = false;
		_hsmIsExecGetBowPath_ROU = false;
		_hsmIsGetBowShapeInit_ROU = false;
		_hsmIsMotionBowInit_ROU = false;
		_hsmIsMotionBow_ROU = NodeStatus::DEFAULT;
		_hsmIsExecMotionBow_ROU = false;
		_hsmIsGetSlamPath_ROU = false;
		_hsmIsExecGetSlamPath_ROU = false;
		_hsmIsMergeSlamPath_ROU = false;
		_hsmIsExecMergeSlamPath_ROU = false;
		_hsmIsObstacleAvoidInit_ROU = false;
		_hsmIsExecObstacleAvoid_ROU = false;
		_hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;
		_hsmIsUpperPileInit_ROU = false;
		_hsmIsExecUpperPile_ROU = false;
		_hsmIsUpperPile_ROU = NodeStatus::DEFAULT;
		_hsmIsUnderPileInit_ROU = false;
		_hsmIsExecUnderPile_ROU = false;
		_hsmIsUnderPile_ROU = false;
		_hsmIsExecSearchObsPoint_ROU = false;
		_hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;
		_hsmIsFollowEdgeRightInit_ROU = false;
		_hsmIsExecFollowEdgeRight_ROU = false;
		_hsmIsFollowEdgeRight_ROU = NodeStatus::DEFAULT;
		_hsmIsExecGetObsLeftStart_ROU = false;
		_hsmIsGetObsLeftStart_ROU = false;
		_hsmIsFollowEdgeLeftInit_ROU = false;
		_hsmIsExecFollowEdgeLeft_ROU = false;
		_hsmIsFollowEdgeLeft_ROU = NodeStatus::DEFAULT;

		/* --- */
		_hsmIsExecGetCleanStartPoint_DEL = false;
		_hsmIsGetCleanStartPoint_DEL = SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;
		_hsmIsExecNav_DEL = false;
		_hsmIsSucceedNav_DEL = SigNav::Nav_fail;
		_hsmIsMotionNavPathInit_DEL = false;
		_hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_DEL = false;
		_hsmIsGetBowPath_DEL = false;
		_hsmIsExecGetBowPath_DEL = false;
		_hsmIsGetBowShapeInit_DEL = false;
		_hsmIsMotionBowInit_DEL = false;
		_hsmIsMotionBow_DEL = NodeStatus::DEFAULT;
		_hsmIsExecMotionBow_DEL = false;
		_hsmIsGetSlamPath_DEL = false;
		_hsmIsExecGetSlamPath_DEL = false;
		_hsmIsMergeSlamPath_DEL = false;
		_hsmIsExecMergeSlamPath_DEL = false;
		_hsmIsObstacleAvoidInit_DEL = false;
		_hsmIsExecObstacleAvoid_DEL = false;
		_hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;
		_hsmIsUpperPileInit_DEL = false;
		_hsmIsExecUpperPile_DEL = false;
		_hsmIsUpperPile_DEL = NodeStatus::DEFAULT;
		_hsmIsUnderPileInit_DEL = false;
		_hsmIsExecUnderPile_DEL = false;
		_hsmIsUnderPile_DEL = false;
		_hsmIsExecSearchObsPoint_DEL = false;
		_hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;
		_hsmIsFollowEdgeRightInit_DEL = false;
		_hsmIsExecFollowEdgeRight_DEL = false;
		_hsmIsFollowEdgeRight_DEL = NodeStatus::DEFAULT;
		_hsmIsExecGetObsLeftStart_DEL = false;
		_hsmIsGetObsLeftStart_DEL = false;
		_hsmIsFollowEdgeLeftInit_DEL = false;
		_hsmIsExecFollowEdgeLeft_DEL = false;
		_hsmIsFollowEdgeLeft_DEL = NodeStatus::DEFAULT;

		/* --- */
		_hsmIsExecNav_REC = false;
		_hsmIsSucceedNav_REC = SigNav::Nav_fail;
		_hsmIsMotionNavPathInit_REC = false;
		_hsmIsMotionNavPath_REC = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_REC = false;
		_hsmIsObstacleAvoidInit_REC = false;
		_hsmIsExecObstacleAvoid_REC = false;
		_hsmIsObstacleAvoid_REC = NodeStatus::DEFAULT;
		_hsmIsUpperPileInit_REC = false;
		_hsmIsExecUpperPile_REC = false;
		_hsmIsUpperPile_REC = NodeStatus::DEFAULT;

		/* --- */
		_hsmIsRecoveryInit = false;
		_hsmIsExecRecovery = false;
		_hsmIsRecovery = NodeStatus::DEFAULT;

		/*rubbyOwner*/
		_hsmMode = workingModeName::NO_WORK;
		_hsmIsMotionInit = false;
		_hsmIsPlanningInit = false;
		boxflag = 0;
	}

	void rubbyOwner::resetParams_ROU()
	{
		_navPath_ROU.clear();
		_bowPath_ROU.clear();
		_slamPath_ROU.clear();
		_PoseStamped_ROU = PoseStamped();
		_start_ROU = PoseStamped();
		_end_ROU = PoseStamped();
		_poseRecord_ROU = PoseStamped();
		_slamPose_ROU = PoseStamped();
		_searchNavPoint_ROU = PoseStamped();
		_searchObsPoint_ROU = PoseStamped();
		_obsLeftStart_ROU = PoseStamped();

		_hsmReLocationInit_Kidnap = false;
		_hsmReLocation_Kidnap = NodeStatus::DEFAULT;
		_hsmIsExecReLocation_Kidnap = false;

		_hsmRelocationMapInit_Kidnap = false;
		_hsmExecRelocationMapResult_Kidnap = false;
		_hsmRelocationMapResult_Kidnap = false;

		_hsmIsExecGetCleanStartPoint_ROU = false;
		_hsmIsGetCleanStartPoint_ROU = SiggetCleanStart::FindUncleanPoint_Null;

		_hsmIsExecNav_ROU = false;
		_hsmIsSucceedNav_ROU = SigNav::Nav_fail;

		_hsmIsMotionNavPathInit_ROU = false;
		_hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_ROU = false;

		_hsmIsGetBowShapeInit_ROU = false;
		_hsmIsGetBowPath_ROU = false;
		_hsmIsExecGetBowPath_ROU = false;

		_hsmIsMotionBowInit_ROU = false;
		_hsmIsMotionBow_ROU = NodeStatus::DEFAULT;
		_hsmIsExecMotionBow_ROU = false;

		_hsmIsGetSlamPath_ROU = false;
		_hsmIsExecGetSlamPath_ROU = false;

		_hsmIsMergeSlamPath_ROU = false;
		_hsmIsExecMergeSlamPath_ROU = false;

		_hsmIsObstacleAvoidInit_ROU = false;
		_hsmIsExecObstacleAvoid_ROU = false;
		_hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;

		_hsmIsUpperPileInit_ROU = false;
		_hsmIsExecUpperPile_ROU = false;
		_hsmIsUpperPile_ROU = NodeStatus::DEFAULT;

		_hsmIsExecSearchObsPoint_ROU = false;
		_hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;

		_hsmIsFollowEdgeRightInit_ROU = false;
		_hsmIsExecFollowEdgeRight_ROU = false;
		_hsmIsFollowEdgeRight_ROU = NodeStatus::DEFAULT;

		_hsmIsExecGetObsLeftStart_ROU = false;
		_hsmIsGetObsLeftStart_ROU = false;

		_hsmIsFollowEdgeLeftInit_ROU = false;
		_hsmIsExecFollowEdgeLeft_ROU = false;
		_hsmIsFollowEdgeLeft_ROU = NodeStatus::DEFAULT;
	}

	void rubbyOwner::resetParams_DEL()
	{
		_navPath_DEL.clear();
		_bowPath_DEL.clear();
		_slamPath_DEL.clear();
		_PoseStamped_DEL = PoseStamped();
		_start_DEL = PoseStamped();
		_end_DEL = PoseStamped();
		_poseRecord_DEL = PoseStamped();
		_slamPose_DEL = PoseStamped();
		_searchNavPoint_DEL = PoseStamped();
		_searchObsPoint_DEL = PoseStamped();
		_obsLeftStart_DEL = PoseStamped();

		_hsmReLocationInit_Kidnap = false;
		_hsmReLocation_Kidnap = NodeStatus::DEFAULT;
		_hsmIsExecReLocation_Kidnap = false;

		_hsmRelocationMapInit_Kidnap = false;
		_hsmExecRelocationMapResult_Kidnap = false;
		_hsmRelocationMapResult_Kidnap = false;

		_hsmIsExecGetCleanStartPoint_DEL = false;
		_hsmIsGetCleanStartPoint_DEL = SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;

		_hsmIsExecNav_DEL = false;
		_hsmIsSucceedNav_DEL = SigNav::Nav_fail;

		_hsmIsMotionNavPathInit_DEL = false;
		_hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_DEL = false;

		_hsmIsGetBowShapeInit_DEL = false;
		_hsmIsGetBowPath_DEL = false;
		_hsmIsExecGetBowPath_DEL = false;

		_hsmIsMotionBowInit_DEL = false;
		_hsmIsMotionBow_DEL = NodeStatus::DEFAULT;
		_hsmIsExecMotionBow_DEL = false;

		_hsmIsGetSlamPath_DEL = false;
		_hsmIsExecGetSlamPath_DEL = false;

		_hsmIsMergeSlamPath_DEL = false;
		_hsmIsExecMergeSlamPath_DEL = false;

		_hsmIsObstacleAvoidInit_DEL = false;
		_hsmIsExecObstacleAvoid_DEL = false;
		_hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;

		_hsmIsUpperPileInit_DEL = false;
		_hsmIsExecUpperPile_DEL = false;
		_hsmIsUpperPile_DEL = NodeStatus::DEFAULT;

		_hsmIsExecSearchObsPoint_DEL = false;
		_hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;

		_hsmIsFollowEdgeRightInit_DEL = false;
		_hsmIsExecFollowEdgeRight_DEL = false;
		_hsmIsFollowEdgeRight_DEL = NodeStatus::DEFAULT;

		_hsmIsExecGetObsLeftStart_DEL = false;
		_hsmIsGetObsLeftStart_DEL = false;

		_hsmIsFollowEdgeLeftInit_DEL = false;
		_hsmIsExecFollowEdgeLeft_DEL = false;
		_hsmIsFollowEdgeLeft_DEL = NodeStatus::DEFAULT;
	}

	void rubbyOwner::resetParams_REC()
	{
		_navPath_REC.clear();
		_start_REC = PoseStamped();
		_end_REC = PoseStamped();
		_hsmIsExecNav_REC = false;
		_hsmIsSucceedNav_REC = SigNav::Nav_fail;
		_hsmIsMotionNavPathInit_REC = false;
		_hsmIsMotionNavPath_REC = NodeStatus::DEFAULT;
		_hsmIsExecMotionNavPath_REC = false;
		_hsmIsObstacleAvoidInit_REC = false;
		_hsmIsExecObstacleAvoid_REC = false;
		_hsmIsObstacleAvoid_REC = NodeStatus::DEFAULT;
		_hsmIsUpperPileInit_REC = false;
		_hsmIsExecUpperPile_REC = false;
		_hsmIsUpperPile_REC = NodeStatus::DEFAULT;
	}

	void rubbyOwner::resetParams_RECOVERY()
	{
		_hsmIsRecoveryInit = false;
		_hsmIsExecRecovery = false;
		_hsmIsRecovery = NodeStatus::DEFAULT;
	}

	void rubbyOwner::destoryHsm()
	{
		if(_commonSdk != nullptr)
		{
			_commonSdk->Destory();
			delete _commonSdk;
			_commonSdk = nullptr;
		}

		if(_motionSdk != nullptr)
		{
			_motionSdk->Destory();
			delete _motionSdk;
			_motionSdk = nullptr;
		}

		if(_planningSdk != nullptr)
		{
			_planningSdk->Destroy();
			delete _planningSdk;
			_planningSdk = nullptr;
		}
	}

	void rubbyOwner::setRotateAngle(double angle)
	{
		_motionSdk->setRotateAngle(angle);
	}

	bool rubbyOwner::setAutoSegment()
	{
		_planningSdk->autoSegment();
	}

	bool rubbyOwner::setAutoSegmentOperation()
	{
		_planningSdk->autoSegmentOperation();
	}

	/********************** struct rubbyStates ***********************/

	/********************** Lev1-interactive ***********************/

	void rubbyStates::interactive::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev1-interactive" << endl;
		mode_interactive_Lev1 = rubbyHSM::workingModeName::NO_WORK;
		Owner()._hsmIsInPileRecharge = false;

#ifdef SLAMTEST
		LOGHSM(INFO) << "SLAM 测试程序: 无下桩, 非绑架重定位默认失败(不走三角动作), 绑架重定位不受影响, 仅支持在常规清扫模式下进行测试";
#endif

		Owner()._hsmIsStartPile = Owner()._SensorInfo->Bms.isInPile ? true : false;
		if(Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_ROUTINECLEAN ||
			Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_DELIMITCLEAN)
		{
			if(Owner()._hsmIsStartPile)
			{
				LOGHSM(INFO) << "桩上启动!!!";
			}
			else
			{
				LOGHSM(INFO) << "非桩上启动!!!";
			}
		}

		if(Owner()._SensorInfo->Bms.isInPile && Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_RECHARGE)
		{
			LOGHSM(INFO) << "检测到在桩上使用回充功能, 动作无意义, 任务结束直接待机......";
			Owner()._DataDown->MotionState = motionState::FINISHEDTASK;
			Owner()._DataDown->cmd.vl = 0.0f;
			Owner()._DataDown->cmd.va = 0.0f;
			Owner()._hsmIsInPileRecharge = true;
		}
	}

	Transition rubbyStates::interactive::GetTransition()
	{
		switch (Owner()._SensorInfo->workMode.workMode)
		{
		case rubbyHSM::workingModeName::WORK_INIT:
			Owner()._hsmMode = rubbyHSM::workingModeName::WORK_INIT;
			break;
		case rubbyHSM::workingModeName::WORK_ROUTINECLEAN:
			Owner()._hsmMode = rubbyHSM::workingModeName::WORK_ROUTINECLEAN;
			break;
		case rubbyHSM::workingModeName::WORK_DELIMITCLEAN:
			Owner()._hsmMode = rubbyHSM::workingModeName::WORK_DELIMITCLEAN;
			break;
		case rubbyHSM::workingModeName::WORK_RECHARGE:
			Owner()._hsmMode = rubbyHSM::workingModeName::WORK_RECHARGE;
			break;
		case rubbyHSM::workingModeName::WORK_RECOVERY:
			Owner()._hsmMode = rubbyHSM::workingModeName::WORK_RECOVERY;
			break;

		default:
			Owner()._hsmMode = rubbyHSM::workingModeName::NO_WORK;
			break;
		}
		mode_interactive_Lev1 = Owner()._hsmMode;

		if(Owner()._hsmIsInPileRecharge)
		{
			return InnerTransition<standByLev2>();
		}

		if(Owner()._SensorInfo->Slamflg.state == SigLocation::Location_SLAM_start)
		{
			Owner()._SensorInfo->Slamflg.state = SigLocation::Location_init;
			LOGHSM(INFO) << "received slam state : no-map relocation start";
			Owner()._hsmIsStartRelocation = true;
		}

		if(Owner()._SensorInfo->Slamflg.state == SigLocation::Location_map_relocation_start)
		{
			Owner()._SensorInfo->Slamflg.state = SigLocation::Location_init;
			LOGHSM(INFO) << "received slam state : map relocation start";
			Owner()._hsmIsStartRelocation = true;
		}

#ifdef RUBBYSIMULATION
		Owner()._hsmIsStartRelocation = true;
		Owner()._SensorInfo->mapload = MapLoadState::MapLoad_Fail;
		Owner()._hsmIsStartPile = false;
		Owner()._hsmIsMergeMapEnd = true;
#endif

		if(Owner()._SensorInfo->Slamflg.state == SigLocation::Location_kinnap_relocation_start)
		{
			Owner()._SensorInfo->Slamflg.state = SigLocation::Location_init;
			LOGHSM(INFO) << "received slam state : Kidnap relocation start";
			Owner()._hsmIsKidnapFlag = true;
		}

		if(Owner()._SensorInfo->Slamflg.state == SigLocation::Location_KINNAPPED)
		{
			LOGHSM(INFO) << "received slam state : Location_KINNAPPED LIFT";
			Owner()._hsmIsKidnapUp = true;
		}
		else
		{
			Owner()._hsmIsKidnapUp = false;
		}

		if(Owner()._SensorInfo->Slamflg.state == SigLocation::Location_MERGE_MAP_END)
		{
			Owner()._hsmIsMergeMapEnd = true;
		}

		/*****常规清扫模式下所有需要语音播报或者待机的状态都在此检测*****/
		if(mode_interactive_Lev1 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmIsEnd)
		{
			if(IsInInnerState<routineCleanLev4>() && !IsInInnerState<selectStateLev5>())  /*常规清扫模式初始化未完成*/
			{
				return InnerTransition<standByLev2>();
			}
			else if(IsInInnerState<getCleanBoxLev6>())   /*获取清扫区域失败*/
			{
				return InnerTransition<standByLev2>();
			}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Null && IsInInnerState<navigationLev6>())  /*搜索起始点返回0时导航失败*/
			{
				return InnerTransition<standByLev2>();
			}
			else if(IsInInnerState<upperPileLev6>() && Owner()._hsmIsMotionNavPath_ROU == NodeStatus::SUCCESS && (Owner()._hsmIsUpperPile_ROU == NodeStatus::SUCCESS || Owner()._hsmIsUpperPile_ROU == NodeStatus::FAILURE))  /*上桩成功或失败*/
			{
				return InnerTransition<standByLev2>();
			}
		}

		/*****划区清扫模式下所有需要语音播报或者待机的状态都在此检测*****/
		if(mode_interactive_Lev1 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmIsEnd)
		{
			if(IsInInnerState<delimitCleanLev4>() && !IsInInnerState<selectStateLev5>())  /*常规清扫模式初始化未完成*/
			{
				return InnerTransition<standByLev2>();
			}
			else if(IsInInnerState<getCleanBoxLev6>())   /*获取清扫区域失败*/
			{
				return InnerTransition<standByLev2>();
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Null && IsInInnerState<navigationLev6>())  /*搜索起始点返回0时导航失败*/
			{
				return InnerTransition<standByLev2>();
			}
			else if(IsInInnerState<upperPileLev6>() && Owner()._hsmIsMotionNavPath_DEL == NodeStatus::SUCCESS && (Owner()._hsmIsUpperPile_DEL == NodeStatus::SUCCESS || Owner()._hsmIsUpperPile_DEL == NodeStatus::FAILURE))  /*上桩成功或失败*/
			{
				return InnerTransition<standByLev2>();
			}
		}

		/*****回充清扫模式下所有需要语音播报或者待机的状态都在此检测*****/
		if(mode_interactive_Lev1 == rubbyHSM::workingModeName::WORK_RECHARGE && Owner()._hsmIsEnd)
		{
			if(IsInInnerState<upperPileLev6>() && (Owner()._hsmIsUpperPile_REC == NodeStatus::SUCCESS || Owner()._hsmIsUpperPile_REC == NodeStatus::FAILURE))
			{
				return InnerTransition<standByLev2>();
			}
		}

		/*****脱困模式下所有需要语音播报或者待机的状态都在此检测*****/
		if((mode_interactive_Lev1 == rubbyHSM::workingModeName::WORK_RECOVERY || Owner()._SensorInfo->recoveryStyle != RecoveryStyle::Recovery_INIT) && Owner()._hsmIsEnd)
		{
			return InnerTransition<standByLev2>();
		}

		if(mode_interactive_Lev1 != rubbyHSM::workingModeName::WORK_INIT && mode_interactive_Lev1 != rubbyHSM::workingModeName::NO_WORK)
		{
			if(mode_interactive_Lev1 != Owner().HSMMODE)
			{
				return SiblingTransition<TRANS>();
			}
			else
			{
				return InnerEntryTransition<informationPubLev2>(informationPubLev2::Args(mode_interactive_Lev1));
			}
		}
		else
		{
			return NoTransition();
		}

	}

	void rubbyStates::interactive::OnExit()
	{
		mode_interactive_Lev1 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev1-interactive" << endl;
	}

	/********************** Lev1-TRANS ***********************/

	void rubbyStates::TRANS::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev1-TRANS" << endl;
		Owner().HSMMODE = Owner()._hsmMode;
	}

	Transition rubbyStates::TRANS::GetTransition()
	{
		return SiblingTransition<interactive>();
	}

	void rubbyStates::TRANS::OnExit()
	{
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev1-TRANS" << endl;
	}

	/********************** Lev2-standByLev2 ***********************/

	void rubbyStates::standByLev2::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev2-standByLev2" << endl;

		if(Owner()._hsmIsEnd && Owner()._hsmIsNeedVoice)
		{
		*(Owner()._FILE) << s_log.logNotes() + "语音播报开始" << endl;
			LOGHSM(INFO) << "语音播报开始";
			Owner()._commonSdk->voiceAnnounce();
			*(Owner()._FILE) << s_log.logNotes() + "语音播报结束" << endl;
			LOGHSM(INFO) << "语音播报结束";
		}

		Owner().resetAllParams();
		*(Owner()._FILE) << s_log.logNotes() + "初始化HSM所有参数" << endl;
		LOGHSM(INFO) << "初始化HSM所有参数";

		*(Owner()._FILE) << s_log.logNotes() + "进入待机状态......" << endl;
		LOGHSM(INFO) << "进入待机状态......";
	}

	Transition rubbyStates::standByLev2::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::standByLev2::Update()
	{
		Owner()._commonSdk->standBy();
	}

	void rubbyStates::standByLev2::OnExit()
	{
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev2-standByLev2" << endl;
	}

	/********************** Lev2-informationPubLev2 ***********************/

	void rubbyStates::informationPubLev2::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev2-informationPubLev2" << endl;
		mode_infor_Lev2 = args.mode_infor_Args_Lev2;
		if(!Owner()._hsmIsExecMotionAndPlanningInit)
		{
			*(Owner()._FILE) << s_log.logNotes() + "motion初始化开始" << endl;
			LOGHSM(INFO) << "motion初始化开始";

			Owner()._hsmIsMotionInit = Owner()._motionSdk->motionInit(Owner()._SensorInfo, Owner()._DataDown);
			// Owner()._hsmIsMotionInit = false;  // 测试
			// Owner()._hsmIsMotionInit = true;  // 测试

			*(Owner()._FILE) << s_log.logNotes() + "planning初始化开始" << endl;
			LOGHSM(INFO) << "planning初始化开始";

			Owner()._hsmIsPlanningInit = Owner()._planningSdk->planningInit(Owner()._SensorInfo, Owner()._DataDown);
			// Owner()._hsmIsPlanningInit = false;  // 测试
			// Owner()._hsmIsPlanningInit = true;  // 测试

			if(Owner()._hsmIsMotionInit && Owner()._hsmIsPlanningInit)
			{
				if(mode_infor_Lev2 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN || mode_infor_Lev2 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
				{
					*(Owner()._FILE) << s_log.logNotes() + "motion 与 planning 初始化完成, 将进行(非绑架)重定位初始化" << endl;
					LOGHSM(INFO) << "motion 与 planning 初始化完成, 将进行(非绑架)重定位初始化";
				}
				else if(mode_infor_Lev2 == rubbyHSM::workingModeName::WORK_RECHARGE || mode_infor_Lev2 == rubbyHSM::workingModeName::WORK_RECOVERY)
				{
					*(Owner()._FILE) << s_log.logNotes() + "motion 与 planning 初始化完成" << endl;
					LOGHSM(INFO) << "motion 与 planning 初始化完成";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "informationPubLev2 --> OnEnter WRONG" << endl;
					LOGHSM(INFO) << "informationPubLev2 --> OnEnter WRONG";
				}
			}

			if(!Owner()._hsmIsMotionInit)
			{
				*(Owner()._FILE) << s_log.logNotes() + "motion 初始化未完成, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "motion 初始化未完成, 状态机阻塞...";
			}

			if(!Owner()._hsmIsPlanningInit)
			{
				*(Owner()._FILE) << s_log.logNotes() + "planning 初始化未完成, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "planning 初始化未完成, 状态机阻塞...";
			}

			Owner()._hsmIsExecMotionAndPlanningInit = true;
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "motion 与 planning 已经初始化, 不需要再次初始化" << endl;
			LOGHSM(INFO) << "motion 与 planning 已经初始化, 不需要再次初始化";
		}
	}

	Transition rubbyStates::informationPubLev2::GetTransition()
	{
		if(Owner()._hsmIsMotionInit && Owner()._hsmIsPlanningInit)
		{
			return SiblingTransition<selectStateLev2>(selectStateLev2::Args(mode_infor_Lev2));
		}
		else
		{
			return NoTransition();
		}
	}

	void rubbyStates::informationPubLev2::Update()
	{
	}

	void rubbyStates::informationPubLev2::OnExit()
	{
		mode_infor_Lev2 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev2-informationPubLev2" << endl;
	}

	/********************** Lev2-selectStateLev2 ***********************/

	void rubbyStates::selectStateLev2::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev2-selectStateLev2" << endl;
		mode_selec_Lev2 = args.mode_selec_Args_Lev2;
	}

	Transition rubbyStates::selectStateLev2::GetTransition()
	{
		return SiblingTransition<workingModeLev2>(workingModeLev2::Args(mode_selec_Lev2));
	}

	void rubbyStates::selectStateLev2::OnExit()
	{
		mode_selec_Lev2 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev2-selectStateLev2" << endl;
	}

	/********************** Lev2-workingModeLev2 ***********************/

	void rubbyStates::workingModeLev2::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev2-workingModeLev2" << endl;
		mode_work_Lev2 = args.mode_work_Args_Lev2;
	}

	Transition rubbyStates::workingModeLev2::GetTransition()
	{
		if(mode_work_Lev2 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			if(!Owner()._hsmIsStartPile)  /*非桩上启动*/
			{
				if(mode_work_Lev2 != rubbyHSM::workingModeName::WORK_INIT && !Owner()._hsmTellSLAMStartFlag)
				{
					Owner()._DataDown->FlgSlam = SigRepositioning::Reposition_STARTSUCCEED;
					Owner()._hsmTellSLAMStartFlag = true;
					LOGHSM(INFO) << "send message HSM STARTSUCCEED to slam succeed (ROU)";
				}
				
				if(Owner()._hsmIsStartRelocation)/*接收到SLAM有无历史地图启动信号*/
				{
					if(!Owner()._hsmIsExecReLocation_NoKidnap)  /*进入重定位*/
					{
						return InnerTransition<reLocation_NoKidnapLev6>(reLocation_NoKidnapLev6::Args(mode_work_Lev2));
					}

					if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && Owner()._hsmIsMergeMapEnd)  /*重定位成功且收到位姿更新信号*/
					{
						if(Owner()._hsmIsExecReLocation_NoKidnap && (Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE) &&  Owner()._SensorInfo->mapload == MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<EMPTYLev6>(EMPTYLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && (Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE) && Owner()._SensorInfo->mapload != MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<mappingAfterRelocation_NoKidnapLev6>(mappingAfterRelocation_NoKidnapLev6::Args(mode_work_Lev2));
						}

						if(IsInInnerState<mappingAfterRelocation_NoKidnapLev6>() && Owner()._hsmExecRelocationMapResult_NoKidnap)
						{
							return InnerTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
						}
					}
					else if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && !Owner()._hsmIsMergeMapEnd) /*重定位成功但没有收到位姿更新信号*/
					{
						return InnerTransition<SPEEDZEROLev6>(SPEEDZEROLev6::Args(mode_work_Lev2));
					}
					else if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE)   /*重定位失败*/
					{
						if(Owner()._hsmIsExecReLocation_NoKidnap && (Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE) &&  Owner()._SensorInfo->mapload == MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<EMPTYLev6>(EMPTYLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && (Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE) && Owner()._SensorInfo->mapload != MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<mappingAfterRelocation_NoKidnapLev6>(mappingAfterRelocation_NoKidnapLev6::Args(mode_work_Lev2));
						}

						if(IsInInnerState<mappingAfterRelocation_NoKidnapLev6>() && Owner()._hsmExecRelocationMapResult_NoKidnap)
						{
							return InnerTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
						}
					}
					else
					{
						return NoTransition();
					}
				}
				else
				{
					return NoTransition();
				}
			}
			else  /*桩上启动*/
			{
				if(mode_work_Lev2 != rubbyHSM::workingModeName::WORK_INIT && !Owner()._hsmTellSLAMStartFlag)
				{
					Owner()._DataDown->FlgSlam = SigRepositioning::Reposition_STARTSUCCEED;
					Owner()._hsmTellSLAMStartFlag = true;
					LOGHSM(INFO) << "send message HSM STARTSUCCEED to slam succeed (ROU)";
				}

				if(!Owner()._hsmIsExecUnderPile_ROU)  /*下桩*/
				{
					return InnerTransition<underPileLev6>(underPileLev6::Args(mode_work_Lev2));
				}

				if(Owner()._hsmIsExecUnderPile_ROU && Owner()._hsmIsUnderPile_ROU && Owner()._hsmIsStartRelocation)/*接收到SLAM有无历史地图启动信号*/
				{
					if(!Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmIsExecUnderPile_ROU)  /*进入重定位*/
					{
						return InnerTransition<reLocation_NoKidnapLev6>(reLocation_NoKidnapLev6::Args(mode_work_Lev2));
					}

					if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && Owner()._hsmIsMergeMapEnd)  /*重定位成功且收到位姿更新信号*/
					{
						if(Owner()._hsmIsExecReLocation_NoKidnap && ( Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE) &&  Owner()._SensorInfo->mapload == MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<EMPTYLev6>(EMPTYLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && (Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE) && Owner()._SensorInfo->mapload != MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<mappingAfterRelocation_NoKidnapLev6>(mappingAfterRelocation_NoKidnapLev6::Args(mode_work_Lev2));
						}

						if(IsInInnerState<mappingAfterRelocation_NoKidnapLev6>() && Owner()._hsmExecRelocationMapResult_NoKidnap)
						{
							return InnerTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
						}
					}
					else if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && !Owner()._hsmIsMergeMapEnd)  /*重定位成功但没有收到位姿更新信号*/
					{
						return InnerTransition<SPEEDZEROLev6>(SPEEDZEROLev6::Args(mode_work_Lev2));
					}
					else if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE)  /*重定位失败*/
					{
						if(Owner()._hsmIsExecReLocation_NoKidnap && ( Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE) &&  Owner()._SensorInfo->mapload == MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<EMPTYLev6>(EMPTYLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && (Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE) && Owner()._SensorInfo->mapload != MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<mappingAfterRelocation_NoKidnapLev6>(mappingAfterRelocation_NoKidnapLev6::Args(mode_work_Lev2));
						}

						if(IsInInnerState<mappingAfterRelocation_NoKidnapLev6>() && Owner()._hsmExecRelocationMapResult_NoKidnap)
						{
							return InnerTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
						}
					}
					else
					{
						return NoTransition();
					}
				}
				else
				{
					return NoTransition();
				}
			}
		}
		else if(mode_work_Lev2 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			if(!Owner()._hsmIsStartPile)  /*非桩上启动*/
			{
				if(mode_work_Lev2 != rubbyHSM::workingModeName::WORK_INIT && !Owner()._hsmTellSLAMStartFlag)
				{
					Owner()._DataDown->FlgSlam = SigRepositioning::Reposition_STARTSUCCEED;
					Owner()._hsmTellSLAMStartFlag = true;
					LOGHSM(INFO) << "send message HSM STARTSUCCEED to slam succeed (DEL)";
				}

				if(Owner()._hsmIsStartRelocation)/*接收到SLAM有无历史地图启动信号*/
				{
					if(!Owner()._hsmIsExecReLocation_NoKidnap)  /*进入重定位*/
					{
						return InnerTransition<reLocation_NoKidnapLev6>(reLocation_NoKidnapLev6::Args(mode_work_Lev2));
					}

					if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && Owner()._hsmIsMergeMapEnd)  /*重定位成功且收到位姿更新信号*/
					{
						if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS &&  Owner()._SensorInfo->mapload == MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<EMPTYLev6>(EMPTYLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && Owner()._SensorInfo->mapload != MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<mappingAfterRelocation_NoKidnapLev6>(mappingAfterRelocation_NoKidnapLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE)
						{
							return NoTransition();
						}

						if(IsInInnerState<mappingAfterRelocation_NoKidnapLev6>() && Owner()._hsmExecRelocationMapResult_NoKidnap)
						{
							return InnerTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
						}
					}
					else if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && !Owner()._hsmIsMergeMapEnd)  /*重定位成功但没有收到位姿更新信号*/
					{
						return InnerTransition<SPEEDZEROLev6>(SPEEDZEROLev6::Args(mode_work_Lev2));
					}
					else if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE)
					{
						if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && Owner()._SensorInfo->mapload == MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<EMPTYLev6>(EMPTYLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && Owner()._SensorInfo->mapload != MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<mappingAfterRelocation_NoKidnapLev6>(mappingAfterRelocation_NoKidnapLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE)
						{
							return NoTransition();
						}

						if(IsInInnerState<mappingAfterRelocation_NoKidnapLev6>() && Owner()._hsmExecRelocationMapResult_NoKidnap)
						{
							return InnerTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
						}
					}
					else
					{
						return NoTransition();
					}
				}
				else
				{
					return NoTransition();
				}
			}
			else /*桩上启动*/
			{
				if(mode_work_Lev2 != rubbyHSM::workingModeName::WORK_INIT && !Owner()._hsmTellSLAMStartFlag)
				{
					Owner()._DataDown->FlgSlam = SigRepositioning::Reposition_STARTSUCCEED;
					Owner()._hsmTellSLAMStartFlag = true;
					LOGHSM(INFO) << "send message HSM STARTSUCCEED to slam succeed (ROU)";
				}

				if(!Owner()._hsmIsExecUnderPile_DEL)
				{
					return InnerTransition<underPileLev6>(underPileLev6::Args(mode_work_Lev2));
				}

				if(Owner()._hsmIsExecUnderPile_DEL && Owner()._hsmIsUnderPile_DEL && Owner()._hsmIsStartRelocation)/*接收到SLAM有无历史地图启动信号*/
				{
					if(Owner()._hsmIsExecUnderPile_DEL && !Owner()._hsmIsExecReLocation_NoKidnap)  /*进入重定位*/
					{
						return InnerTransition<reLocation_NoKidnapLev6>(reLocation_NoKidnapLev6::Args(mode_work_Lev2));
					}

					if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && Owner()._hsmIsMergeMapEnd)  /*重定位成功且收到位姿更新信号*/
					{
						if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS &&  Owner()._SensorInfo->mapload == MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<EMPTYLev6>(EMPTYLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS  && Owner()._SensorInfo->mapload != MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<mappingAfterRelocation_NoKidnapLev6>(mappingAfterRelocation_NoKidnapLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE)
						{
							return NoTransition();
						}

						if(IsInInnerState<mappingAfterRelocation_NoKidnapLev6>() && Owner()._hsmExecRelocationMapResult_NoKidnap)
						{
							return InnerTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
						}
					}
					else if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS && !Owner()._hsmIsMergeMapEnd)
					{
						return InnerTransition<SPEEDZEROLev6>(SPEEDZEROLev6::Args(mode_work_Lev2));
					}
					else if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE)
					{
						if(Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS &&  Owner()._SensorInfo->mapload == MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<EMPTYLev6>(EMPTYLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap && Owner()._hsmReLocation_NoKidnap == NodeStatus::SUCCESS  && Owner()._SensorInfo->mapload != MapLoadState::MapLoad_INIT)
						{
							return InnerTransition<mappingAfterRelocation_NoKidnapLev6>(mappingAfterRelocation_NoKidnapLev6::Args(mode_work_Lev2));
						}

						if(!Owner()._hsmExecRelocationMapResult_NoKidnap && Owner()._hsmIsExecReLocation_NoKidnap  && Owner()._hsmReLocation_NoKidnap == NodeStatus::FAILURE)
						{
							return NoTransition();
						}

						if(IsInInnerState<mappingAfterRelocation_NoKidnapLev6>() && Owner()._hsmExecRelocationMapResult_NoKidnap)
						{
							return InnerTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
						}
					}
					else
					{
						return NoTransition();
					}
				}
				else
				{
					return NoTransition();
				}
			}
		}
		else if(mode_work_Lev2 == rubbyHSM::workingModeName::WORK_RECHARGE)
		{
			return InnerEntryTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
		}
		else if(mode_work_Lev2 == rubbyHSM::workingModeName::WORK_RECOVERY)
		{
			return InnerEntryTransition<informationPubLev3>(informationPubLev3::Args(mode_work_Lev2));
		}
		else
		{
			return NoTransition();
		}
	}

	void rubbyStates::workingModeLev2::OnExit()
	{ 
		mode_work_Lev2 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev2-workingModeLev2" << endl;
	}

	/********************** Lev3-informationPubLev3 ***********************/

	void rubbyStates::informationPubLev3::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev3-informationPubLev3" << endl;
		mode_infor_Lev3 = args.mode_infor_Args_Lev3;
	}

	Transition rubbyStates::informationPubLev3::GetTransition()
	{
		return SiblingTransition<selectStateLev3>(selectStateLev3::Args(mode_infor_Lev3));
	}

	void rubbyStates::informationPubLev3::OnExit()
	{
		mode_infor_Lev3 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev3-informationPubLev3" << endl;
	}

	/********************** Lev3-selectStateLev3 ***********************/

	void rubbyStates::selectStateLev3::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev3-selectStateLev3" << endl;
		mode_selec_Lev3 = args.mode_selec_Args_Lev3;
	}

	Transition rubbyStates::selectStateLev3::GetTransition()
	{
		if(mode_selec_Lev3 == rubbyHSM::workingModeName::WORK_RECOVERY || Owner()._SensorInfo->recoveryStyle != RecoveryStyle::Recovery_INIT)
		{
			return InnerTransition<recoveryLev4>();
		}
		else if(mode_selec_Lev3 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			return InnerTransition<routineCleanLev4>(routineCleanLev4::Args(mode_selec_Lev3));
		}
		else if(mode_selec_Lev3 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			return InnerTransition<delimitCleanLev4>(delimitCleanLev4::Args(mode_selec_Lev3));
		}
		else if(mode_selec_Lev3 == rubbyHSM::workingModeName::WORK_RECHARGE)
		{
			return InnerTransition<reChargeLev4>(reChargeLev4::Args(mode_selec_Lev3));
		}
		else
		{
			return NoTransition();
		}
	}

	void rubbyStates::selectStateLev3::OnExit()
	{
		mode_selec_Lev3 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev3-selectStateLev3" << endl;
	}

	/********************** Lev4-routineCleanLev4 ***********************/

	void rubbyStates::routineCleanLev4::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev4-routineCleanLev4" << endl;
		mode_routi_Lev4 = args.mode_routi_Args_Lev4;
		if(mode_routi_Lev4 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			Owner()._BoxInfo_ROU.resetBox();
			if(Owner()._hsmIsExecRecovery)
			{
				Owner()._hsmRoutineInitFlag_ROU = true;
			}
			else
			{
				Owner()._hsmRoutineInitFlag_ROU = false;
			}
			*(Owner()._FILE) << s_log.logNotes() + "进入常规清扫模式" << endl;
			LOGHSM(INFO) << "进入常规清扫模式";
		}
	}

	Transition rubbyStates::routineCleanLev4::GetTransition()
	{
		if(mode_routi_Lev4 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			if(!Owner()._hsmRoutineInitFlag_ROU)
			{
				Owner()._hsmRoutineInitFlag_ROU = true;
				if(mode_routi_Lev4 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
				{
					*(Owner()._FILE) << s_log.logNotes() + "当前工作模式: 常规清扫模式" << endl;
					LOGHSM(INFO) << "当前工作模式: 常规清扫模式";

					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 常规清扫模式初始化开始" << endl;
					LOGHSM(INFO) << "常规清扫模式: 常规清扫模式初始化开始";
					Owner().resetParams_ROU();
					Owner()._hsmIsRoutineInit_ROU = false;
					Owner()._hsmIsExecGetCleanBox_ROU = false;
					Owner()._hsmIsKidnapFlag = false;

					Owner()._hsmIsRoutineInit_ROU = Owner()._planningSdk->routineCleanInit();
					// Owner()._hsmIsRoutineInit_ROU = false; // 测试
					// Owner()._hsmIsRoutineInit_ROU = true; // 测试

					Owner()._startPoseStamped_ROU = Owner()._SensorInfo->Posture;

					if(Owner()._hsmIsRoutineInit_ROU)
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 常规清扫模式初始化完成，将进行清扫区域的获取" << endl;
						LOGHSM(INFO) << "常规清扫模式: 常规清扫模式初始化完成，将进行清扫区域的获取";
					}
					else
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 常规清扫模式初始化失败, 将直接进入待机状态" << endl;
						LOGHSM(INFO) << "常规清扫模式: 常规清扫模式初始化失败, 将直接进入待机状态";
					}
				}
			}

			if(Owner()._hsmIsRoutineInit_ROU)
			{
				return InnerEntryTransition<informationPubLev5>(informationPubLev5::Args(mode_routi_Lev4));
			}
			else
			{
				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				return NoTransition();
			}
		}
		else
		{
			return NoTransition();
		}
		
	}

	void rubbyStates::routineCleanLev4::OnExit()
	{
		mode_routi_Lev4 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev4-routineCleanLev4" << endl;
	}

	/********************** Lev4-delimitCleanLev4 ***********************/

	void rubbyStates::delimitCleanLev4::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev4-delimitCleanLev4" << endl;
		mode_delimit_Lev4 = args.mode_delimit_Args_Lev4;
		if(mode_delimit_Lev4 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			Owner()._BoxInfo_DEL.resetBox();
			if(Owner()._hsmIsExecRecovery)
			{
				Owner()._hsmDelimitInitFlag_DEL = true;
			}
			else
			{
				Owner()._hsmDelimitInitFlag_DEL = false;
			}
			*(Owner()._FILE) << s_log.logNotes() + "进入划区清扫模式" << endl;
			LOGHSM(INFO) << "进入划区清扫模式";
		}
	}

	Transition rubbyStates::delimitCleanLev4::GetTransition()
	{
		if(mode_delimit_Lev4 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			if(!Owner()._hsmDelimitInitFlag_DEL)
			{
				Owner()._hsmDelimitInitFlag_DEL = true;
				if(mode_delimit_Lev4 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
				{
					*(Owner()._FILE) << s_log.logNotes() + "当前工作模式: 划区清扫模式" << endl;
					LOGHSM(INFO) << "当前工作模式: 划区清扫模式";

					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 划区清扫模式初始化开始" << endl;
					LOGHSM(INFO) << "划区清扫模式: 划区清扫模式初始化开始";
					Owner().resetParams_DEL();
					Owner()._hsmIsDelimitInit_DEL = false;
					Owner()._hsmIsExecGetCleanBox_DEL = false;
					Owner()._hsmIsKidnapFlag = false;

					Owner()._hsmIsDelimitInit_DEL = Owner()._planningSdk->DelimitCleanInit();
					// Owner()._hsmIsDelimitInit_DEL = false; // 测试
					// Owner()._hsmIsDelimitInit_DEL = true; // 测试

					Owner()._startPoseStamped_DEL = Owner()._SensorInfo->Posture;

					if(Owner()._hsmIsDelimitInit_DEL)
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 划区清扫模式初始化完成，将进行清扫区域的获取" << endl;
						LOGHSM(INFO) << "划区清扫模式: 划区清扫模式初始化完成，将进行清扫区域的获取";
					}
					else
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 划区清扫模式初始化失败, 将直接进入待机状态" << endl;
						LOGHSM(INFO) << "划区清扫模式: 划区清扫模式初始化失败, 将直接进入待机状态";
					}
				}
			}

			if(Owner()._hsmIsDelimitInit_DEL)
			{
				return InnerEntryTransition<informationPubLev5>(informationPubLev5::Args(mode_delimit_Lev4));
			}
			else
			{
				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				return NoTransition();
			}
		}
		else
		{
			return NoTransition();
		}
	}

	void rubbyStates::delimitCleanLev4::OnExit()
	{
		mode_delimit_Lev4 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev4-delimitCleanLev4" << endl;
	}

	/********************** Lev4-reChargeLev4 ***********************/

	void rubbyStates::reChargeLev4::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev4-reChargeLev4" << endl;
		mode_recharge_Lev4 = args.mode_recharge_Args_Lev4;
		if(mode_recharge_Lev4 == rubbyHSM::workingModeName::WORK_RECHARGE)
		{
			Owner().resetParams_REC();
			*(Owner()._FILE) << s_log.logNotes() + "进入回充模式" << endl;
			LOGHSM(INFO) << "进入回充模式";
		}
	}

	Transition rubbyStates::reChargeLev4::GetTransition()
	{
		return InnerEntryTransition<informationPubLev5>(informationPubLev5::Args(mode_recharge_Lev4));
	}

	void rubbyStates::reChargeLev4::OnExit()
	{
		mode_recharge_Lev4 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev4-reChargeLev4" << endl;
	}

	/********************** Lev4-recoveryLev4 ***********************/

	void rubbyStates::recoveryLev4::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev4-recoveryLev4" << endl;

		*(Owner()._FILE) << s_log.logNotes() + "进入脱困模式" << endl;
		LOGHSM(INFO) << "进入脱困模式";

		Owner().resetParams_RECOVERY();
		*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 进入脱困模式前初始化脱困模式下所有参数" << endl;
		LOGHSM(INFO) << "脱困模式: 进入脱困模式前初始化脱困模式下所有参数";
	}

	Transition rubbyStates::recoveryLev4::GetTransition()
	{
		return InnerEntryTransition<recoverySelectStateLev5>();
	}

	void rubbyStates::recoveryLev4::OnExit()
	{
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev4-recoveryLev4" << endl;
	}

	/********************** Lev5-informationPubLev5 ***********************/

	void rubbyStates::informationPubLev5::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev5-informationPubLev5" << endl;
		mode_infor_Lev5 = args.mode_infor_Args_Lev5;
	}

	Transition rubbyStates::informationPubLev5::GetTransition()
	{
		return SiblingTransition<selectStateLev5>(selectStateLev5::Args(mode_infor_Lev5));
	}

	void rubbyStates::informationPubLev5::OnExit()
	{
		mode_infor_Lev5 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev5-informationPubLev5" << endl;
	}

	/********************** Lev5-selectStateLev5 ***********************/

	void rubbyStates::selectStateLev5::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev5-selectStateLev5" << endl;
		mode_selec_Lev5 = args.mode_selec_Args_Lev5;
	}

	Transition rubbyStates::selectStateLev5::GetTransition()
	{
		if(mode_selec_Lev5 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			if(!Owner()._hsmIsExecGetCleanBox_ROU)  /*获取清扫区域*/
			{
				return InnerTransition<getCleanBoxLev6>(getCleanBoxLev6::Args(mode_selec_Lev5));
			}
			
			if(Owner()._hsmIsKidnapUp)  /*检测到绑架抬起事件后下发0速度*/
			{
				return InnerTransition<SPEEDZEROLev6>(SPEEDZEROLev6::Args(mode_selec_Lev5));
			}

			if(Owner()._hsmIsKidnapFlag)   /*检测机器人被绑架后放在地上事件*/
			{
				if(!Owner()._hsmIsExecReLocation_Kidnap)
				{
					return InnerTransition<reLocation_KidnapLev6>(reLocation_KidnapLev6::Args(mode_selec_Lev5));
				}

				if(!Owner()._hsmExecRelocationMapResult_Kidnap && Owner()._hsmIsExecReLocation_Kidnap && (Owner()._hsmReLocation_Kidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_Kidnap == NodeStatus::FAILURE))
				{
					return InnerTransition<mappingAfterRelocation_KidnapLev6>(mappingAfterRelocation_KidnapLev6::Args(mode_selec_Lev5));
				}
			}

			if(!Owner()._hsmIsExecGetCleanStartPoint_ROU)   /*获得清扫起始点*/
			{
				return InnerTransition<getCleanStartPointLev6>(getCleanStartPointLev6::Args(mode_selec_Lev5));
			}
			else
			{
				switch(Owner()._hsmIsGetCleanStartPoint_ROU)
				{
					case SiggetCleanStart::FindUncleanPoint_Null:  /*搜索清扫起始点返回0*/
						if(!Owner()._hsmIsExecSearchObsPoint_ROU)
						{
							return InnerTransition<searchObsPointLev6>(searchObsPointLev6::Args(mode_selec_Lev5));
						}

						if(Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_Null)  /*没有搜索到障碍物点*/
						{
							if(!Owner()._hsmIsExecNav_ROU)
							{
								Owner()._start_ROU = Owner()._SensorInfo->Posture;   // 当前最新位姿
								Owner()._end_ROU = Owner()._startPoseStamped_ROU;  // 目标点就是进入常规清扫时保存的清扫起始点
								return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_ROU, Owner()._end_ROU));
							}

							if(!Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsExecNav_ROU && Owner()._hsmIsSucceedNav_ROU == SigNav::Nav_success)
							{
								return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecUpperPile_ROU && Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsMotionNavPath_ROU == NodeStatus::SUCCESS)
							{
								return InnerTransition<upperPileLev6>(upperPileLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecObstacleAvoid_ROU && Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsMotionNavPath_ROU == NodeStatus::FAILURE)
							{
								return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
							}
						}

						if(Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_success)  /*搜索到障碍物点*/
						{
							if(!Owner()._hsmIsExecNav_ROU)
							{
								if(Owner()._hsmIsExecGetObsLeftStart_ROU && Owner()._hsmIsGetObsLeftStart_ROU)
								{
									Owner()._start_ROU = Owner()._SensorInfo->Posture;   // 当前最新位姿
									Owner()._end_ROU = Owner()._obsLeftStart_ROU;  // 目标点就是左沿边起始点
								}
								else
								{
									Owner()._start_ROU = Owner()._SensorInfo->Posture;   // 当前最新位姿
									Owner()._end_ROU = Owner()._searchNavPoint_ROU;  // 目标点就是搜索到的障碍物附近的导航点
								}
								return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_ROU, Owner()._end_ROU));
							}

							if(!Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsExecNav_ROU && Owner()._hsmIsSucceedNav_ROU == SigNav::Nav_success)
							{
								return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecObstacleAvoid_ROU && Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsMotionNavPath_ROU == NodeStatus::FAILURE)
							{
								return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecFollowEdgeRight_ROU && Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsMotionNavPath_ROU == NodeStatus::SUCCESS)
							{
								return InnerTransition<followEdgeRightLev6>(followEdgeRightLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecGetObsLeftStart_ROU && Owner()._hsmIsExecFollowEdgeRight_ROU && Owner()._hsmIsFollowEdgeRight_ROU == NodeStatus::FAILURE)
							{
								return InnerTransition<getObsLeftStartLev6>(getObsLeftStartLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecFollowEdgeLeft_ROU && Owner()._hsmIsExecGetObsLeftStart_ROU && Owner()._hsmIsGetObsLeftStart_ROU && Owner()._hsmIsMotionNavPath_ROU == NodeStatus::SUCCESS)
							{
								return InnerTransition<followEdgeLeftLev6>(followEdgeLeftLev6::Args(mode_selec_Lev5));
							}
						}
						
						if(Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_InObs)
						{
							return NoTransition();
						}
						
						break;

					case SiggetCleanStart::FindUncleanPoint_Out:  /*搜索清扫起始点返回1*/
						if(Owner()._hsmIsExecGetAndMergeSlamPath_ROU)
						{
							if(!Owner()._hsmIsExecNav_ROU)
							{
								Owner()._start_ROU = Owner()._SensorInfo->Posture;       /*从传入的数据中拿到当前最新位姿*/
								Owner()._end_ROU = Owner()._poseRecord_ROU;      /*目标点就是上一步搜索到的清扫起始点*/
								return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_ROU, Owner()._end_ROU));
							}

							if(Owner()._hsmIsExecNav_ROU && (Owner()._hsmIsSucceedNav_ROU == SigNav::Nav_success) && !Owner()._hsmIsExecMotionNavPath_ROU)   /*导航成功，接下来进入motion路径追踪*/
							{
								return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecObstacleAvoid_ROU && Owner()._hsmIsExecMotionNavPath_ROU && 	Owner()._hsmIsMotionNavPath_ROU == NodeStatus::FAILURE)
							{
								return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
							}

						}
						else
						{
							if(!Owner()._hsmIsExecGetSlamPath_ROU)
							{
								return InnerTransition<getSlamPathLev6>(getSlamPathLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecNav_ROU && Owner()._hsmIsExecGetSlamPath_ROU && Owner()._hsmIsGetSlamPath_ROU)
							{
								Owner()._start_ROU = Owner()._SensorInfo->Posture;       /*从传入的数据中拿到当前最新位姿*/
								Owner()._end_ROU = Owner()._slamPose_ROU;      /*目标点就是路径端点*/
								return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_ROU, Owner()._end_ROU));
							}

							if(!Owner()._hsmIsExecMergeSlamPath_ROU && Owner()._hsmIsExecGetSlamPath_ROU && Owner()._hsmIsExecNav_ROU && Owner()._hsmIsSucceedNav_ROU == SigNav::Nav_success && Owner()._hsmIsGetSlamPath_ROU)
							{
								return InnerTransition<mergeSlamPathLev6>(mergeSlamPathLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsExecMergeSlamPath_ROU && Owner()._hsmIsMergeSlamPath_ROU)
							{
								return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecObstacleAvoid_ROU && 	Owner()._hsmIsExecMotionNavPath_ROU && 	Owner()._hsmIsMotionNavPath_ROU == NodeStatus::FAILURE)
							{
								return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
							}
						}

						break;

					case SiggetCleanStart::FindUncleanPoint_Inside:
						if(!Owner()._hsmIsExecNav_ROU)
						{
							Owner()._start_ROU = Owner()._SensorInfo->Posture;   /*从传入的数据中拿到当前最新位姿*/
							Owner()._end_ROU = Owner()._PoseStamped_ROU;  /*目标点就是上一步搜索到的清扫起始点*/
							return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_ROU, Owner()._end_ROU));
						}

						if(!Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsExecNav_ROU && Owner()._hsmIsSucceedNav_ROU == SigNav::Nav_success)   // motion追踪导航路径
						{
							return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
						}

						if(!Owner()._hsmIsExecObstacleAvoid_ROU && Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsMotionNavPath_ROU == NodeStatus::FAILURE)
						{
							return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
						}

						if(!Owner()._hsmIsExecGetBowPath_ROU && Owner()._hsmIsExecMotionNavPath_ROU && Owner()._hsmIsMotionNavPath_ROU == NodeStatus::SUCCESS)  // 获取弓字形路径
						{
							return InnerTransition<bowShapeLev6>(bowShapeLev6::Args(mode_selec_Lev5));
						}

						if(!Owner()._hsmIsExecMotionBow_ROU && Owner()._hsmIsExecGetBowPath_ROU && Owner()._hsmIsGetBowPath_ROU)   // 追踪弓字形路径
						{
							return InnerTransition<motionBowLev6>(motionBowLev6::Args(mode_selec_Lev5));
						}

						if(!Owner()._hsmIsExecObstacleAvoid_ROU && Owner()._hsmIsExecMotionBow_ROU && Owner()._hsmIsMotionBow_ROU == NodeStatus::FAILURE)
						{
							return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
						}

						break;

					case SiggetCleanStart::FindUncleanPoint_InObs:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索清扫起始点返回FindUncleanPoint_InObs, 进入脱困模式" << endl;
						LOGHSM(INFO) << "常规清扫模式: 搜索清扫起始点返回FindUncleanPoint_InObs, 进入脱困模式";
						return NoTransition();
						break;

					default:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索清扫起始点结果异常, 返回异常值" << endl;
						LOGHSM(INFO) << "常规清扫模式: 搜索清扫起始点结果异常, 返回异常值";
						break;
				}
			}
		}
		else if(mode_selec_Lev5 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			if(!Owner()._hsmIsExecGetCleanBox_DEL)  /*获取清扫区域*/
			{
				return InnerTransition<getCleanBoxLev6>(getCleanBoxLev6::Args(mode_selec_Lev5));
			}

			if(Owner()._hsmIsKidnapUp)  /*检测到绑架抬起事件后下发0速度*/
			{
				return InnerTransition<SPEEDZEROLev6>(SPEEDZEROLev6::Args(mode_selec_Lev5));
			}

			if(Owner()._hsmIsKidnapFlag)
			{
				if(!Owner()._hsmIsExecReLocation_Kidnap)
				{
					return InnerTransition<reLocation_KidnapLev6>(reLocation_KidnapLev6::Args(mode_selec_Lev5));
				}

				if(!Owner()._hsmExecRelocationMapResult_Kidnap && Owner()._hsmIsExecReLocation_Kidnap && (Owner()._hsmReLocation_Kidnap == NodeStatus::SUCCESS || Owner()._hsmReLocation_Kidnap == NodeStatus::FAILURE))
				{
					return InnerTransition<mappingAfterRelocation_KidnapLev6>(mappingAfterRelocation_KidnapLev6::Args(mode_selec_Lev5));
				}
			}

			if(!Owner()._hsmIsExecGetCleanStartPoint_DEL)   /*获得清扫起始点*/
			{
				return InnerTransition<getCleanStartPointLev6>(getCleanStartPointLev6::Args(mode_selec_Lev5));
			}
			else
			{
				switch(Owner()._hsmIsGetCleanStartPoint_DEL)
				{
					case SiggetCleanStartDelimit::FindUncleanPointDelimit_Null:
						if(!Owner()._hsmIsExecSearchObsPoint_DEL)
						{
							return InnerTransition<searchObsPointLev6>(searchObsPointLev6::Args(mode_selec_Lev5));
						}
						
						if(Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_Null)
						{
							if(!Owner()._hsmIsExecNav_DEL)
							{
								Owner()._start_DEL = Owner()._SensorInfo->Posture;   // 当前最新位姿
								Owner()._end_DEL = Owner()._startPoseStamped_DEL;  // 目标点就是进入划区清扫时保存的清扫起始点
								return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_DEL, Owner()._end_DEL));
							}

							if(!Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsExecNav_DEL && Owner()._hsmIsSucceedNav_DEL == SigNav::Nav_success)
							{
								return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecUpperPile_DEL && Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsMotionNavPath_DEL == NodeStatus::SUCCESS)
							{
								return InnerTransition<upperPileLev6>(upperPileLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecObstacleAvoid_DEL && Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsMotionNavPath_DEL == NodeStatus::FAILURE)
							{
								return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
							}
						}

						if(Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_success)
						{
							if(!Owner()._hsmIsExecNav_DEL)
							{
								if(Owner()._hsmIsExecGetObsLeftStart_DEL && Owner()._hsmIsGetObsLeftStart_DEL)
								{
									Owner()._start_DEL = Owner()._SensorInfo->Posture;   // 当前最新位姿
									Owner()._end_DEL = Owner()._obsLeftStart_DEL;  // 目标点就是左沿边起始点
								}
								else
								{
									Owner()._start_DEL = Owner()._SensorInfo->Posture;   // 当前最新位姿
									Owner()._end_DEL = Owner()._searchNavPoint_DEL;  // 目标点就是搜索到的障碍物附近的导航点
								}
								return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_DEL, Owner()._end_DEL));
							}

							if(!Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsExecNav_DEL && Owner()._hsmIsSucceedNav_DEL == SigNav::Nav_success)
							{
								return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecObstacleAvoid_DEL && Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsMotionNavPath_DEL == NodeStatus::FAILURE)
							{
								return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecFollowEdgeRight_DEL && Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsMotionNavPath_DEL == NodeStatus::SUCCESS)
							{
								return InnerTransition<followEdgeRightLev6>(followEdgeRightLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecGetObsLeftStart_DEL && Owner()._hsmIsExecFollowEdgeRight_DEL && Owner()._hsmIsFollowEdgeRight_DEL == NodeStatus::FAILURE)
							{
								return InnerTransition<getObsLeftStartLev6>(getObsLeftStartLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecFollowEdgeLeft_DEL && Owner()._hsmIsExecGetObsLeftStart_DEL && Owner()._hsmIsGetObsLeftStart_DEL && Owner()._hsmIsMotionNavPath_DEL == NodeStatus::SUCCESS)
							{
								return InnerTransition<followEdgeLeftLev6>(followEdgeLeftLev6::Args(mode_selec_Lev5));
							}
						}
						
						if(Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_InObs)
						{
							return NoTransition();
						}
						
						break;

					case SiggetCleanStartDelimit::FindUncleanPointDelimit_Out:  /*搜索清扫起始点返回1*/
						if(Owner()._hsmIsExecGetAndMergeSlamPath_DEL)
						{
							if(!Owner()._hsmIsExecNav_DEL)
							{
								Owner()._start_DEL = Owner()._SensorInfo->Posture;       /*从传入的数据中拿到当前最新位姿*/
								Owner()._end_DEL = Owner()._poseRecord_DEL;      /*目标点就是上一步搜索到的清扫起始点*/
								return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_DEL, Owner()._end_DEL));
							}

							if(Owner()._hsmIsExecNav_DEL && Owner()._hsmIsSucceedNav_DEL == SigNav::Nav_success && !Owner()._hsmIsExecMotionNavPath_DEL)   /*导航成功，接下来进入motion路径追踪*/
							{
								return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecObstacleAvoid_DEL && Owner()._hsmIsExecMotionNavPath_DEL && 	Owner()._hsmIsMotionNavPath_DEL == NodeStatus::FAILURE)
							{
								return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
							}
						}
						else
						{
							if(!Owner()._hsmIsExecGetSlamPath_DEL)
							{
								return InnerTransition<getSlamPathLev6>(getSlamPathLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecNav_DEL && Owner()._hsmIsExecGetSlamPath_DEL && Owner()._hsmIsGetSlamPath_DEL)
							{
								Owner()._start_DEL = Owner()._SensorInfo->Posture;       /*从传入的数据中拿到当前最新位姿*/
								Owner()._end_DEL = Owner()._slamPose_DEL;      /*目标点就是路径端点*/
								return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_DEL, Owner()._end_DEL));
							}

							if(!Owner()._hsmIsExecMergeSlamPath_DEL && Owner()._hsmIsExecGetSlamPath_DEL && Owner()._hsmIsExecNav_DEL && Owner()._hsmIsSucceedNav_DEL == SigNav::Nav_success && Owner()._hsmIsGetSlamPath_DEL)
							{
								return InnerTransition<mergeSlamPathLev6>(mergeSlamPathLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsExecMergeSlamPath_DEL && Owner()._hsmIsMergeSlamPath_DEL)
							{
								return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
							}

							if(!Owner()._hsmIsExecObstacleAvoid_DEL && 	Owner()._hsmIsExecMotionNavPath_DEL && 	Owner()._hsmIsMotionNavPath_DEL == NodeStatus::FAILURE)
							{
								return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
							}
						}

						break;

					case SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside:
						if(!Owner()._hsmIsExecNav_DEL)
						{
							Owner()._start_DEL = Owner()._SensorInfo->Posture;   /*从传入的数据中拿到当前最新位姿*/
							Owner()._end_DEL = Owner()._PoseStamped_DEL;  /*目标点就是上一步搜索到的清扫起始点*/
							return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_DEL, Owner()._end_DEL));
						}

						if(!Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsExecNav_DEL && Owner()._hsmIsSucceedNav_DEL == SigNav::Nav_success)   // motion追踪导航路径
						{
							return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
						}

						if(!Owner()._hsmIsExecObstacleAvoid_DEL && Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsMotionNavPath_DEL == NodeStatus::FAILURE)
						{
							return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
						}

						if(!Owner()._hsmIsExecGetBowPath_DEL && Owner()._hsmIsExecMotionNavPath_DEL && Owner()._hsmIsMotionNavPath_DEL == NodeStatus::SUCCESS)  // 获取弓字形路径
						{
							return InnerTransition<bowShapeLev6>(bowShapeLev6::Args(mode_selec_Lev5));
						}

						if(!Owner()._hsmIsExecMotionBow_DEL && Owner()._hsmIsExecGetBowPath_DEL && Owner()._hsmIsGetBowPath_DEL)   // 追踪弓字形路径
						{
							return InnerTransition<motionBowLev6>(motionBowLev6::Args(mode_selec_Lev5));
						}

						if(!Owner()._hsmIsExecObstacleAvoid_DEL && Owner()._hsmIsExecMotionBow_DEL && 	Owner()._hsmIsMotionBow_DEL == NodeStatus::FAILURE)
						{
							return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
						}

						break;

					case SiggetCleanStartDelimit::FindUncleanPointDelimit_InObs:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 搜索清扫起始点返回FindUncleanPointDelimit_InObs, 进入脱困模式" << endl;
						LOGHSM(INFO) << "划区清扫模式: 搜索清扫起始点返回FindUncleanPointDelimit_InObs, 进入脱困模式";
						return NoTransition();
						break;

					default:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 搜索清扫起始点结果异常, 返回异常值" << endl;
						LOGHSM(INFO) << "划区清扫模式: 搜索清扫起始点结果异常, 返回异常值";
						break;
				}
			}
		}
		else if(mode_selec_Lev5 == rubbyHSM::workingModeName::WORK_RECHARGE)
		{
			if(!Owner()._hsmIsExecReLocation_NoKidnap)   /*重定位动作未执行完成*/
			{
				if(!Owner()._hsmIsExecUpperPile_REC)
				{
					return InnerTransition<upperPileLev6>(upperPileLev6::Args(mode_selec_Lev5));
				}
			}
			else  /*重定位失败(动作执行完成)或重定位成功*/
			{
				// PoseStamped Point_REC = PoseStamped();
				// Owner()._planningSdk->getSearchChargingPoint(Point_REC);  /*探桩*/

				if(!Owner()._hsmIsExecNav_REC)
				{
					Owner()._start_REC = Owner()._SensorInfo->Posture;
					Owner()._end_REC = Owner()._startPoseStamped_REC;
					return InnerTransition<navigationLev6>(navigationLev6::Args(mode_selec_Lev5, Owner()._start_REC, Owner()._end_REC));
				}

				switch (Owner()._hsmIsSucceedNav_REC)
				{
				case SigNav::Nav_success:
					if(!Owner()._hsmIsExecMotionNavPath_REC && Owner()._hsmIsExecNav_REC)
					{
						return InnerTransition<motionNavLev6>(motionNavLev6::Args(mode_selec_Lev5));
					}

					if(!Owner()._hsmIsExecUpperPile_REC && Owner()._hsmIsExecMotionNavPath_REC && Owner()._hsmIsMotionNavPath_REC == NodeStatus::SUCCESS)
					{
						return InnerTransition<upperPileLev6>(upperPileLev6::Args(mode_selec_Lev5));
					}

					if(!Owner()._hsmIsExecObstacleAvoid_REC && Owner()._hsmIsExecMotionNavPath_REC && Owner()._hsmIsMotionNavPath_REC == NodeStatus::FAILURE)
					{
						return InnerTransition<obstacleAvoidLev6>(obstacleAvoidLev6::Args(mode_selec_Lev5));
					}
					break;
				case SigNav::Nav_fail:
					if(!Owner()._hsmIsExecUpperPile_REC && Owner()._hsmIsExecNav_REC)
					{
						return InnerTransition<upperPileLev6>(upperPileLev6::Args(mode_selec_Lev5));
					}
					break;
				case SigNav::Nav_InObs:
					*(Owner()._FILE) << s_log.logNotes() + "回充模式: 导航接口返回SigNav::Nav_InObs, 进入脱困模式" << endl;
					LOGHSM(INFO) << "回充模式: 导航接口返回SigNav::Nav_InObs, 进入脱困模式";
					return NoTransition();
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "回充模式: 导航接口返回异常值, 状态机阻塞..." << endl;
					LOGHSM(INFO) << "回充模式: 导航接口返回异常值, 状态机阻塞...";
					break;
				}
			}
		}
		else
		{
			return NoTransition();
		}
	}

	void rubbyStates::selectStateLev5::OnExit()
	{
		mode_selec_Lev5 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev5-selectStateLev5" << endl;
	}

	/********************** Lev5-recoverySelectStateLev5 ***********************/

	void rubbyStates::recoverySelectStateLev5::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev5-recoverySelectStateLev5" << endl;
	}

	Transition rubbyStates::recoverySelectStateLev5::GetTransition()
	{
		switch (Owner()._SensorInfo->recoveryStyle)
		{
		case RecoveryStyle::Recovery_Forbiden:
			return InnerTransition<recoveryForbidenLev6>();
			break;
		case RecoveryStyle::Recovery_Obs:
			return InnerTransition<recoveryObsLev6>();
			break;
		case RecoveryStyle::Recovery_Surrounded:
			return InnerTransition<recoverySurroundedLev6>();
			break;
		case RecoveryStyle::Recovery_Physics:
			return InnerTransition<recoveryPhysicsLev6>();
			break;
		default:
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 脱困类型异常, 返回异常值" << endl;
			LOGHSM(INFO) << "脱困模式: 脱困类型异常, 返回异常值: " << (int)Owner()._SensorInfo->recoveryStyle;
			break;
		}
	}

	void rubbyStates::recoverySelectStateLev5::OnExit()
	{
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev5-recoverySelectStateLev5" << endl;
	}

	/********************** Lev6-navigationLev6 ***********************/

	void rubbyStates::navigationLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-navigationLev6" << endl;
		mode_nav_Lev6 = args.mode_nav_Args_Lev6;
		_start_ = args.start_Args_nav_Lev6;
		_end_ = args.end_Args_nav_Lev6;
	}

	Transition rubbyStates::navigationLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::navigationLev6::Update()
	{
		if(mode_nav_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Null && Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_Null)
			{
				PoseStamped pointEnd_;
				Owner()._navPath_ROU.clear();
				Owner()._planningSdk->getNearSafePoint(_end_, pointEnd_);
				Owner()._hsmIsSucceedNav_ROU = Owner()._planningSdk->nav(_start_, pointEnd_, Owner()._navPath_ROU);
				Owner()._hsmIsExecNav_ROU = true;
			}
			else
			{
				Owner()._navPath_ROU.clear();
				Owner()._hsmIsSucceedNav_ROU = Owner()._planningSdk->nav(_start_, _end_, Owner()._navPath_ROU);
				Owner()._hsmIsExecNav_ROU = true;
			}

			// Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;  // 测试
			// Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_success;  // 测试

			switch (Owner()._hsmIsGetCleanStartPoint_ROU)
			{
			case SiggetCleanStart::FindUncleanPoint_Null:
				switch (Owner()._hsmIsSearchObsPoint_ROU)
				{
				case SigSearchObsPoint::FindObs_success:
					if(Owner()._hsmIsExecGetObsLeftStart_ROU)  /*获取到左沿边起始点时调用导航*/
					{
						switch (Owner()._hsmIsSucceedNav_ROU)
						{
						case SigNav::Nav_success:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到左沿边起始点的路径成功, 将调用motion对导航路径进行追踪" << endl;
							LOGHSM(INFO) << "常规清扫模式: 导航规划到左沿边起始点的路径成功, 将调用motion对导航路径进行追踪";
							Owner()._hsmIsExecMotionNavPath_ROU = false;
							break;
						case SigNav::Nav_fail:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到左沿边起始点的路径失败, 将重新搜索障碍物点" << endl;
							LOGHSM(INFO) << "常规清扫模式: 导航规划到左沿边起始点的路径失败, 将重新搜索障碍物点";

							Owner()._hsmIsExecSearchObsPoint_ROU = false;
							Owner()._hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;

							Owner()._hsmIsExecNav_ROU = false;
							Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;

							Owner()._hsmIsMotionNavPathInit_ROU = false;
							Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
							Owner()._hsmIsExecMotionNavPath_ROU = false;

							Owner()._hsmIsObstacleAvoidInit_ROU = false;
							Owner()._hsmIsExecObstacleAvoid_ROU = false;
							Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;

							Owner()._hsmIsFollowEdgeRightInit_ROU = false;
							Owner()._hsmIsFollowEdgeRight_ROU = NodeStatus::DEFAULT;
							Owner()._hsmIsExecFollowEdgeRight_ROU = false;

							Owner()._hsmIsGetObsLeftStart_ROU = false;
							Owner()._hsmIsExecGetObsLeftStart_ROU = false;							
							break;
						case SigNav::Nav_InObs:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到左沿边起始点的路径时返回Nav_InObs, 将进入脱困模式" << endl;
							LOGHSM(INFO) << "常规清扫模式: 导航规划到左沿边起始点的路径时返回Nav_InObs, 将进入脱困模式";
							break;
						
						default:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划接口到左沿边起始点的路径时返回异常值, 状态机阻塞..." << endl;
							LOGHSM(INFO) << "常规清扫模式: 导航规划接口到左沿边起始点的路径时返回异常值, 状态机阻塞...";							
							break;
						}
					}
					else  /*导航到障碍物点时调用导航*/
					{
						switch (Owner()._hsmIsSucceedNav_ROU)
						{
						case SigNav::Nav_success:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到障碍物附近点的路径成功, 将调用motion对导航路径进行追踪" << endl;
							LOGHSM(INFO) << "常规清扫模式: 导航规划到障碍物附近点的路径成功, 将调用motion对导航路径进行追踪";
							break;
						case SigNav::Nav_fail:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到障碍物附近点的路径失败, 将重新搜索障碍物点" << endl;
							LOGHSM(INFO) << "常规清扫模式: 导航规划到障碍物附近点的路径失败, 将重新搜索障碍物点";

							Owner()._hsmIsExecSearchObsPoint_ROU = false;
							Owner()._hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;

							Owner()._hsmIsExecNav_ROU = false;
							Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;						
							break;
						case SigNav::Nav_InObs:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到障碍物附近点的路径时返回Nav_InObs, 将进入脱困模式" << endl;
							LOGHSM(INFO) << "常规清扫模式: 导航规划到障碍物附近点的路径时返回Nav_InObs, 将进入脱困模式";
							break;
						
						default:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到障碍物附近点的路径时接口返回异常值, 状态机阻塞..." << endl;
							LOGHSM(INFO) << "常规清扫模式: 导航规划到障碍物附近点的路径时接口返回异常值, 状态机阻塞...";							
							break;
						}
					}
					break;
				case SigSearchObsPoint::FindObs_Null:
					switch (Owner()._hsmIsSucceedNav_ROU)
					{
					case SigNav::Nav_success:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到回原点的路径成功, 将调用motion对导航路径进行追踪" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到回原点的路径成功, 将调用motion对导航路径进行追踪";
						break;
					case SigNav::Nav_fail:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到回原点的路径失败, 将进行语音播报并进入待机状态" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到回原点的路径失败, 将进行语音播报并进入待机状态";

						Owner()._planningSdk->completeTask();
						Owner()._motionSdk->completeTask();

						Owner()._hsmIsEnd = true;
						Owner()._hsmIsNeedVoice = true;
						break;
					case SigNav::Nav_InObs:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到回原点的路径时返回Nav_InObs, 将进入脱困模式" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到回原点的路径时返回Nav_InObs, 将进入脱困模式";
						break;
					
					default:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到回原点的路径时接口返回异常值, 状态机阻塞..." << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到回原点的路径时接口返回异常值, 状态机阻塞...";							
						break;
					}
					break;
				case SigSearchObsPoint::FindObs_InObs:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索障碍物点时返回FindObs_InObs, 将进入脱困模式" << endl;
					LOGHSM(INFO) << "常规清扫模式: 搜索障碍物点时返回FindObs_InObs, 将进入脱困模式";
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索障碍物点时返回异常值, 状态机阻塞..." << endl;
					LOGHSM(INFO) << "常规清扫模式: 搜索障碍物点时返回异常值, 状态机阻塞...";
					break;
				}
				break;
			case SiggetCleanStart::FindUncleanPoint_Out:
				if(!Owner()._hsmIsExecGetAndMergeSlamPath_ROU && Owner()._hsmIsExecGetSlamPath_ROU && Owner()._hsmIsGetSlamPath_ROU)
				{
					switch (Owner()._hsmIsSucceedNav_ROU)
					{
					case SigNav::Nav_success:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到SLAM路径端点的路径成功, 将合并SLAM路径" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到SLAM路径端点的路径成功, 将合并SLAM路径";
						break;
					case SigNav::Nav_fail:
						Owner()._hsmIsExecGetAndMergeSlamPath_ROU = true;
						Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;
						Owner()._hsmIsExecNav_ROU = false;
						Owner()._hsmIsExecGetSlamPath_ROU = false;
						Owner()._hsmIsGetSlamPath_ROU = false;
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到SLAM路径端点的路径失败, 将导航到区域外目标点" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到SLAM路径端点的路径失败, 将导航到区域外目标点";
						break;		
					case SigNav::Nav_InObs:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到SLAM路径端点的路径时返回Nav_InObs, 将进入脱困模式" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到SLAM路径端点的路径时返回Nav_InObs, 将进入脱困模式";
						break;
					
					default:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到SLAM路径端点的路径时接口返回异常值, 状态机阻塞..." << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到SLAM路径端点的路径时接口返回异常值, 状态机阻塞...";							
						break;
					}
				}
				else
				{
					switch (Owner()._hsmIsSucceedNav_ROU)
					{
					case SigNav::Nav_success:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到区域外清扫起始点的路径成功, 将调用motion对导航路径进行追踪" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到区域外清扫起始点的路径成功, 将调用motion对导航路径进行追踪";
						break;
					case SigNav::Nav_fail:
						Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到区域外目标点的路径失败, 将重新搜索清扫起始点" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到区域外目标点的路径失败, 将重新搜索清扫起始点";
						break;
					case SigNav::Nav_InObs:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到区域外目标点的路径时返回Nav_InObs, 将进入脱困模式" << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到区域外目标点的路径时返回Nav_InObs, 将进入脱困模式";
						break;
					
					default:
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到区域外目标点的路径时接口返回异常值, 状态机阻塞..." << endl;
						LOGHSM(INFO) << "常规清扫模式: 导航规划到区域外目标点的路径时接口返回异常值, 状态机阻塞...";							
						break;
					}
				}
				break;
			case SiggetCleanStart::FindUncleanPoint_Inside:
				switch (Owner()._hsmIsSucceedNav_ROU)
				{
				case SigNav::Nav_success:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到区域内清扫起始点的路径成功, 将调用motion对导航路径进行追踪" << endl;
					LOGHSM(INFO) << "常规清扫模式: 导航规划到区域内清扫起始点的路径成功, 将调用motion对导航路径进行追踪";
					break;
				case SigNav::Nav_fail:
					Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到区域内清扫起始点的路径失败, 将重新搜索清扫起始点" << endl;
					LOGHSM(INFO) << "常规清扫模式: 导航规划到区域内清扫起始点的路径失败, 将重新搜索清扫起始点";
					break;
				case SigNav::Nav_InObs:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到区域内清扫起始点的路径时返回Nav_InObs, 将进入脱困模式" << endl;
					LOGHSM(INFO) << "常规清扫模式: 导航规划到区域内清扫起始点的路径时返回Nav_InObs, 将进入脱困模式";
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 导航规划到区域内清扫起始点的路径时接口返回异常值, 状态机阻塞..." << endl;
					LOGHSM(INFO) << "常规清扫模式: 导航规划到区域内清扫起始点的路径时接口返回异常值, 状态机阻塞...";							
					break;
				}
				break;
			case SiggetCleanStart::FindUncleanPoint_InObs:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: navigationLev6返回FindUncleanPoint_InObs, 将进入脱困模式" << endl;
				LOGHSM(INFO) << "常规清扫模式: navigationLev6返回FindUncleanPoint_InObs, 将进入脱困模式";
				break;

			default:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: navigationLev6返回值异常, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "常规清扫模式: navigationLev6返回值异常, 状态机阻塞...";
				break;
			}
		}
		else if(mode_nav_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Null && Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_Null)
			{
				PoseStamped pointEnd_;
				Owner()._navPath_DEL.clear();
				Owner()._planningSdk->getNearSafePoint(_end_, pointEnd_);
				Owner()._hsmIsSucceedNav_DEL = Owner()._planningSdk->nav(_start_, pointEnd_, Owner()._navPath_DEL);
				Owner()._hsmIsExecNav_DEL = true;
			}
			else
			{
				Owner()._navPath_DEL.clear();
				Owner()._hsmIsSucceedNav_DEL = Owner()._planningSdk->nav(_start_, _end_, Owner()._navPath_DEL);
				Owner()._hsmIsExecNav_DEL = true;
			}

			// Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;  // 测试
			// Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_success;  // 测试

			switch (Owner()._hsmIsGetCleanStartPoint_DEL)
			{
			case SiggetCleanStartDelimit::FindUncleanPointDelimit_Null:
				switch (Owner()._hsmIsSearchObsPoint_DEL)
				{
				case SigSearchObsPoint::FindObs_success:
					if(Owner()._hsmIsExecGetObsLeftStart_DEL)
					{
						switch (Owner()._hsmIsSucceedNav_DEL)
						{
						case SigNav::Nav_success:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到左沿边起始点的路径成功, 将调用motion对导航路径进行追踪" << endl;
							LOGHSM(INFO) << "划区清扫模式: 导航规划到左沿边起始点的路径成功, 将调用motion对导航路径进行追踪";
							Owner()._hsmIsExecMotionNavPath_DEL = false;
							break;
						case SigNav::Nav_fail:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到左沿边起始点的路径失败, 将重新搜索障碍物点" << endl;
							LOGHSM(INFO) << "划区清扫模式: 导航规划到左沿边起始点的路径失败, 将重新搜索障碍物点";

							Owner()._hsmIsExecSearchObsPoint_DEL = false;
							Owner()._hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;

							Owner()._hsmIsExecNav_DEL = false;
							Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;

							Owner()._hsmIsMotionNavPathInit_DEL = false;
							Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
							Owner()._hsmIsExecMotionNavPath_DEL = false;

							Owner()._hsmIsObstacleAvoidInit_DEL = false;
							Owner()._hsmIsExecObstacleAvoid_DEL = false;
							Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;

							Owner()._hsmIsFollowEdgeRightInit_DEL = false;
							Owner()._hsmIsFollowEdgeRight_DEL = NodeStatus::DEFAULT;
							Owner()._hsmIsExecFollowEdgeRight_DEL = false;

							Owner()._hsmIsGetObsLeftStart_DEL = false;
							Owner()._hsmIsExecGetObsLeftStart_DEL = false;
							break;
						case SigNav::Nav_InObs:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到左沿边起始点的路径时返回SigNav::Nav_InObs, 将进入脱困模式" << endl;
							LOGHSM(INFO) << "划区清扫模式: 导航规划到左沿边起始点的路径时返回SigNav::Nav_InObs, 将进入脱困模式";
							break;
						
						default:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到左沿边起始点的路径时返回值异常, 状态机阻塞..." << endl;
							LOGHSM(INFO) << "划区清扫模式: 导航规划到左沿边起始点的路径时返回值异常, 状态机阻塞...";
							break;
						}
					}
					else
					{
						switch (Owner()._hsmIsSucceedNav_DEL)
						{
						case SigNav::Nav_success:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到障碍物附近点的路径成功, 将调用motion对导航路径进行追踪" << endl;
							LOGHSM(INFO) << "划区清扫模式: 导航规划到障碍物附近点的路径成功, 将调用motion对导航路径进行追踪";
							break;
						case SigNav::Nav_fail:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到障碍物附近点的路径失败, 将重新搜索障碍物点" << endl;
							LOGHSM(INFO) << "划区清扫模式: 导航规划到障碍物附近点的路径失败, 将重新搜索障碍物点";

							Owner()._hsmIsExecSearchObsPoint_DEL = false;
							Owner()._hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;

							Owner()._hsmIsExecNav_DEL = false;
							Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;
							break;
						case SigNav::Nav_InObs:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到障碍物附近点的路径时返回SigNav::Nav_InObs, 将进入脱困模式" << endl;
							LOGHSM(INFO) << "划区清扫模式: 导航规划到障碍物附近点的路径时返回SigNav::Nav_InObs, 将进入脱困模式";
							break;
						
						default:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到障碍物附近点的路径时返回值异常, 状态机阻塞..." << endl;
							LOGHSM(INFO) << "划区清扫模式: 导航规划到障碍物附近点的路径时返回值异常, 状态机阻塞...";
							break;
						}
					}
					break;
				case SigSearchObsPoint::FindObs_Null:
					switch (Owner()._hsmIsSucceedNav_DEL)
					{
					case SigNav::Nav_success:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到回原点的路径成功, 将调用motion对导航路径进行追踪" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到回原点的路径成功, 将调用motion对导航路径进行追踪";
						break;
					case SigNav::Nav_fail:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到回原点的路径失败, 将进行语音播报并进入待机状态" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到回原点的路径失败, 将进行语音播报并进入待机状态";

						Owner()._planningSdk->completeTask();
						Owner()._motionSdk->completeTask();

						Owner()._hsmIsEnd = true;
						Owner()._hsmIsNeedVoice = true;
						break;
					case SigNav::Nav_InObs:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到回原点的路径时返回SigNav::Nav_InObs, 将进入脱困模式" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到回原点的路径时返回SigNav::Nav_InObs, 将进入脱困模式";
						break;
					
					default:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到回原点的路径时返回值异常, 状态机阻塞..." << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到回原点的路径时返回值异常, 状态机阻塞...";
						break;
					}
					break;
				case SigSearchObsPoint::FindObs_InObs:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: navigationLev6搜索障碍物点返回FindObs_InObs, 进入脱困模式" << endl;
					LOGHSM(INFO) << "划区清扫模式: navigationLev6搜索障碍物点返回FindObs_InObs, 进入脱困模式";
					break;

				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: navigationLev6搜索障碍物点返回异常值, 状态机阻塞..." << endl;
					LOGHSM(INFO) << "划区清扫模式: navigationLev6搜索障碍物点返回异常值, 状态机阻塞...";
					break;
				}
				break;
			case SiggetCleanStartDelimit::FindUncleanPointDelimit_Out:
				if(!Owner()._hsmIsExecGetAndMergeSlamPath_DEL && Owner()._hsmIsExecGetSlamPath_DEL && Owner()._hsmIsGetSlamPath_DEL)
				{
					switch (Owner()._hsmIsSucceedNav_DEL)
					{
					case SigNav::Nav_success:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到SLAM路径端点的路径成功, 将合并SLAM路径" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到SLAM路径端点的路径成功, 将合并SLAM路径";
						break;
					case SigNav::Nav_fail:
						Owner()._hsmIsExecGetAndMergeSlamPath_DEL = true;
						Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;
						Owner()._hsmIsExecNav_DEL = false;
						Owner()._hsmIsExecGetSlamPath_DEL = false;
						Owner()._hsmIsGetSlamPath_DEL = false;
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到SLAM路径端点的路径失败, 将导航到区域外目标点" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到SLAM路径端点的路径失败, 将导航到区域外目标点";
						break;
					case SigNav::Nav_InObs:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到SLAM路径端点的路径时返回SigNav::Nav_InObs, 将进入脱困模式" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到SLAM路径端点的路径时返回SigNav::Nav_InObs, 将进入脱困模式";
						break;
					
					default:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到SLAM路径端点的路径时返回值异常, 状态机阻塞..." << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到SLAM路径端点的路径时返回值异常, 状态机阻塞...";
						break;
					}
				}
				else
				{
					switch (Owner()._hsmIsSucceedNav_DEL)
					{
					case SigNav::Nav_success:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到区域外清扫起始点的路径成功, 将调用motion对导航路径进行追踪" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到区域外清扫起始点的路径成功, 将调用motion对导航路径进行追踪";
						break;
					case SigNav::Nav_fail:
						Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到区域外目标点的路径失败, 将重新搜索清扫起始点" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到区域外目标点的路径失败, 将重新搜索清扫起始点";
						break;
					case SigNav::Nav_InObs:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到区域外目标点的路径时返回SigNav::Nav_InObs, 将进入脱困模式" << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到区域外目标点的路径时返回SigNav::Nav_InObs, 将进入脱困模式";
						break;
					
					default:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到区域外目标点的路径时返回值异常, 状态机阻塞..." << endl;
						LOGHSM(INFO) << "划区清扫模式: 导航规划到区域外目标点的路径时返回值异常, 状态机阻塞...";
						break;
					}
				}
				break;
			case SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside:
				switch (Owner()._hsmIsSucceedNav_DEL)
				{
				case SigNav::Nav_success:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到区域内清扫起始点的路径成功, 将调用motion对导航路径进行追踪" << endl;
					LOGHSM(INFO) << "划区清扫模式: 导航规划到区域内清扫起始点的路径成功, 将调用motion对导航路径进行追踪";
					break;
				case SigNav::Nav_fail:
					Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到区域内清扫起始点的路径失败, 将重新搜索清扫起始点" << endl;
					LOGHSM(INFO) << "划区清扫模式: 导航规划到区域内清扫起始点的路径失败, 将重新搜索清扫起始点";
					break;
				case SigNav::Nav_InObs:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到区域内清扫起始点的路径时返回SigNav::Nav_InObs, 将进入脱困模式" << endl;
					LOGHSM(INFO) << "划区清扫模式: 导航规划到区域内清扫起始点的路径时返回SigNav::Nav_InObs, 将进入脱困模式";
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 导航规划到区域内清扫起始点的路径时返回值异常, 状态机阻塞..." << endl;
					LOGHSM(INFO) << "划区清扫模式: 导航规划到区域内清扫起始点的路径时返回值异常, 状态机阻塞...";
					break;
				}
				break;
			case SiggetCleanStartDelimit::FindUncleanPointDelimit_InObs:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: navigationLev6返回FindUncleanPointDelimit_InObs, 将进入脱困模式" << endl;
				LOGHSM(INFO) << "划区清扫模式: navigationLev6返回FindUncleanPointDelimit_InObs, 将进入脱困模式";
				break;

			default:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: navigationLev6返回值异常, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "划区清扫模式: navigationLev6返回值异常, 状态机阻塞...";
			}
		}
		else if(mode_nav_Lev6 == rubbyHSM::workingModeName::WORK_RECHARGE)
		{
			PoseStamped pointEnd_;
			Owner()._navPath_REC.clear();
			Owner()._planningSdk->getNearSafePoint(_end_, pointEnd_);
			Owner()._hsmIsSucceedNav_REC = Owner()._planningSdk->nav(_start_, pointEnd_, Owner()._navPath_REC);
			Owner()._hsmIsExecNav_REC = true;
			
			switch (Owner()._hsmIsSucceedNav_REC)
			{
			case SigNav::Nav_success:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 导航规划到回重定位动作结束点的路径成功, 将调用motion对导航路径进行追踪" << endl;
				LOGHSM(INFO) << "回充模式: 导航规划到回重定位动作结束点的路径成功, 将调用motion对导航路径进行追踪";
				break;
			case SigNav::Nav_fail:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 导航规划到回重定位动作结束点的路径失败, 将直接调用上桩功能进行上桩" << endl;
				LOGHSM(INFO) << "回充模式: 导航规划到回重定位动作结束点的路径失败, 将直接调用上桩功能进行上桩";
				break;
			case SigNav::Nav_InObs:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 导航规划到回重定位动作结束点的路径时返回SigNav::Nav_InObs, 将进入脱困模式" << endl;
				LOGHSM(INFO) << "回充模式: 导航规划到回重定位动作结束点的路径时返回SigNav::Nav_InObs, 将进入脱困模式";
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 导航规划到回重定位动作结束点的路径时返回异常值, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "回充模式: 导航规划到回重定位动作结束点的路径时返回异常值, 状态机阻塞...";
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "navigationLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "navigationLev6 --> Update WRONG";

			Owner()._planningSdk->completeTask();
			Owner()._motionSdk->completeTask();

			Owner()._hsmIsEnd = true;
			Owner()._hsmIsNeedVoice = true;
		}
	}

	void rubbyStates::navigationLev6::OnExit()
	{
		mode_nav_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-navigationLev6" << endl;
	}

	/********************** Lev6-bowShapeLev6 ***********************/

	void rubbyStates::bowShapeLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-bowShapeLev6" << endl;
		mode_bow_Lev6 = args.mode_nav_Args_Lev6;

		if(mode_bow_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			if(!Owner()._hsmIsGetBowShapeInit_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 开始弓字形初始化" << endl;
				LOGHSM(INFO) << "常规清扫模式: 开始弓字形初始化";
				Owner()._hsmIsGetBowShapeInit_ROU = Owner()._planningSdk->bowInit(Owner()._PoseStamped_ROU, Owner()._BoxInfo_ROU);
				// Owner()._hsmIsGetBowShapeInit_ROU = false;  // 测试
				// Owner()._hsmIsGetBowShapeInit_ROU = true;  // 测试

				if(Owner()._hsmIsGetBowShapeInit_ROU)
				{
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 弓字形初始化完成" << endl;
					LOGHSM(INFO) << "常规清扫模式: 弓字形初始化完成";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 弓字形初始化失败, 状态机阻塞" << endl;
					LOGHSM(INFO) << "常规清扫模式: 弓字形初始化失败, 状态机阻塞";
				}
			}
		}
		else if(mode_bow_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			if(!Owner()._hsmIsGetBowShapeInit_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 开始弓字形初始化" << endl;
				LOGHSM(INFO) << "划区清扫模式: 开始弓字形初始化";
				Owner()._hsmIsGetBowShapeInit_DEL = Owner()._planningSdk->bowInitDelimit(Owner()._PoseStamped_DEL, Owner()._BoxInfo_DEL);
				// Owner()._hsmIsGetBowShapeInit_DEL = false;  // 测试
				// Owner()._hsmIsGetBowShapeInit_DEL = true;  // 测试

				if(Owner()._hsmIsGetBowShapeInit_DEL)
				{
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 弓字形初始化完成" << endl;
					LOGHSM(INFO) << "划区清扫模式: 弓字形初始化完成";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 弓字形初始化失败, 状态机阻塞" << endl;
					LOGHSM(INFO) << "划区清扫模式: 弓字形初始化失败, 状态机阻塞";
				}
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "bowShapeLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "bowShapeLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::bowShapeLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::bowShapeLev6::Update()
	{
		if(mode_bow_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmIsGetBowShapeInit_ROU)
		{
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 开始获取弓字形路径" << endl;
			LOGHSM(INFO) << "常规清扫模式: 开始获取弓字形路径";
			Owner()._bowPath_ROU.clear();
			Owner()._hsmIsGetBowPath_ROU = Owner()._planningSdk->getBowPath(Owner()._PoseStamped_ROU, Owner()._BoxInfo_ROU, Owner()._bowPath_ROU);
			Owner()._hsmIsExecGetBowPath_ROU = true;

			// Owner()._hsmIsGetBowPath_ROU = false;  // 测试
			// Owner()._hsmIsGetBowPath_ROU = true;  // 测试

			if(Owner()._hsmIsGetBowPath_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 获得弓字形路径成功, 将调用motion追踪弓字形路径" << endl;
				LOGHSM(INFO) << "常规清扫模式: 获得弓字形路径成功, 将调用motion追踪弓字形路径";
			}
			else
			{
				Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 获得弓字形路径失败, 将重新搜索清扫起始点" << endl;
				LOGHSM(INFO) << "常规清扫模式: 获得弓字形路径失败, 将重新搜索清扫起始点";
			}
		}
		else if(mode_bow_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmIsGetBowShapeInit_DEL)
		{
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 开始获取弓字形路径" << endl;
			LOGHSM(INFO) << "划区清扫模式: 开始获取弓字形路径";
			Owner()._bowPath_DEL.clear();
			Owner()._hsmIsGetBowPath_DEL = Owner()._planningSdk->getBowPathDelimit(Owner()._PoseStamped_DEL, Owner()._BoxInfo_DEL, Owner()._bowPath_DEL);
			Owner()._hsmIsExecGetBowPath_DEL = true;

			// Owner()._hsmIsGetBowPath_DEL = false;  // 测试
			// Owner()._hsmIsGetBowPath_DEL = true;  // 测试

			if(Owner()._hsmIsGetBowPath_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 获得弓字形路径成功, 将调用motion追踪弓字形路径" << endl;
				LOGHSM(INFO) << "划区清扫模式: 获得弓字形路径成功, 将调用motion追踪弓字形路径";
			}
			else
			{
				Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 获得弓字形路径失败, 将重新搜索清扫起始点" << endl;
				LOGHSM(INFO) << "划区清扫模式: 获得弓字形路径失败, 将重新搜索清扫起始点";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "bowShapeLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "bowShapeLev6 --> Update WRONG";
		}
	}

	void rubbyStates::bowShapeLev6::OnExit()
	{
		mode_bow_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-bowShapeLev6" << endl;
	}

	/********************** Lev6-reLocation_NoKidnapLev6 ***********************/

	void rubbyStates::reLocation_NoKidnapLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-reLocation_NoKidnapLev6" << endl;
		mode_reLocat_NoKidnap_Lev6 = args.mode_reLocat_NoKidnap_Args_Lev6;

		if(!Owner()._hsmReLocationInit_NoKidnap)
		{
			*(Owner()._FILE) << s_log.logNotes() + "(非绑架)重定位初始化开始" << endl;
			LOGHSM(INFO) << "(非绑架)重定位初始化开始";

#ifdef SLAMTEST
			Owner()._hsmReLocationInit_NoKidnap = true;
#else
			Owner()._hsmReLocationInit_NoKidnap = Owner()._motionSdk->reLocationInit();
#endif
			
			// Owner()._hsmReLocationInit_NoKidnap = true;  // 测试

			if(Owner()._hsmReLocationInit_NoKidnap)
			{
				*(Owner()._FILE) << s_log.logNotes() + "(非绑架)重定位初始化完成, (非绑架)重定位动作执行中......" << endl;
				LOGHSM(INFO) << "(非绑架)重定位初始化完成, (非绑架)重定位动作执行中......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "(非绑架)重定位初始化未完成, 状态机阻塞" << endl;
				LOGHSM(INFO) << "(非绑架)重定位初始化未完成, 状态机阻塞";
			}
		}
	}

	Transition rubbyStates::reLocation_NoKidnapLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::reLocation_NoKidnapLev6::Update()
	{
#ifdef SLAMTEST
		Owner()._hsmReLocation_NoKidnap = 2;
#else
		if(Owner()._SensorInfo->Slamflg.state == Location_map_relocation_ok)
		{
			LOGHSM(INFO) << "reLocation_NoKidnapLev6: received slam state is: MAP_RELOCATION_OK -- (NAV: Location_map_relocation_ok)";

			Owner()._motionSdk->RunParking();
			Owner()._hsmReLocation_NoKidnap = NodeStatus::SUCCESS;
		}
		else
		{
			Owner()._hsmReLocation_NoKidnap = Owner()._motionSdk->reLocationRun();
		}
#endif

		Owner()._hsmIsExecReLocation_NoKidnap = true;

		// Owner()._hsmReLocation_NoKidnap = 0;  // 测试
		// Owner()._hsmReLocation_NoKidnap = 2;  // 测试
		// Owner()._hsmReLocation_NoKidnap = 1;  // 测试

		switch (Owner()._hsmReLocation_NoKidnap)
		{
		case NodeStatus::RUNNING:
			Owner()._hsmIsExecReLocation_NoKidnap = false;
			break;

		case NodeStatus::SUCCESS:
			Owner()._DataDown->FlgSlam = Reposition_NOFINISHEND;
			Owner()._SensorInfo->Slamflg.state = Location_init;
			*(Owner()._FILE) << s_log.logNotes() + "(非绑架)重定位动作执行成功, 将进入(非绑架)建图动作" << endl;
			LOGHSM(INFO) << "(非绑架)重定位动作执行成功, 将进入(非绑架)建图动作";

			Owner()._startPoseStamped_REC = Owner()._SensorInfo->Posture;  /*回充模式记录位姿*/
			break;

		case NodeStatus::FAILURE:
			Owner()._DataDown->FlgSlam = Reposition_FINISHEND;
			Owner()._SensorInfo->Slamflg.state = Location_init;

			Owner()._startPoseStamped_REC = Owner()._SensorInfo->Posture;  /*回充模式记录位姿*/

			if(mode_reLocat_NoKidnap_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
			{
				*(Owner()._FILE) << s_log.logNotes() + "(非绑架)重定位动作执行失败, 将进入(非绑架)建图动作(ROU)" << endl;
				LOGHSM(INFO) << "(非绑架)重定位动作执行失败, 将进入(非绑架)建图动作(ROU)";
			}
			else if(mode_reLocat_NoKidnap_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
			{
				if(!Owner().count2HSM)
				{
					*(Owner()._FILE) << s_log.logNotes() + "(非绑架)重定位动作执行失败, 状态机阻塞(DEL)" << endl;
					LOGHSM(INFO) << "(非绑架)重定位动作执行失败, 状态机阻塞(DEL)";
					Owner().count2HSM = true;
				}

#ifdef RUBBYSIMULATION
				Owner()._hsmReLocation_NoKidnap = NodeStatus::SUCCESS;
				LOGHSM(INFO) << "!!!仿真测试, 划区清扫模式中(非绑架)重定位动作认为成功!!!";
#endif
			}
			break;
				
		default:
			*(Owner()._FILE) << s_log.logNotes() + "(非绑架)重定位异常, 返回非法值" << endl;
			LOGHSM(INFO) << "(非绑架)重定位异常, 返回非法值";
			break;
		}
	}

	void rubbyStates::reLocation_NoKidnapLev6::OnExit()
	{
		mode_reLocat_NoKidnap_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-reLocation_NoKidnapLev6" << endl;
	}

	/********************** Lev6-getCleanBoxLev6 ***********************/

	void rubbyStates::getCleanBoxLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-getCleanBoxLev6" << endl;
		mode_getCleanBox_Lev6 = args.mode_getCleanBox_Args_Lev6;
		if(mode_getCleanBox_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			this->isGetCleanBox = false;
		}
		else if(mode_getCleanBox_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			this->isGetCleanBox = false;
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "getCleanBoxLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "getCleanBoxLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::getCleanBoxLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::getCleanBoxLev6::Update()
	{
		if(mode_getCleanBox_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			this->isGetCleanBox = Owner()._planningSdk->getCleanBox(Owner()._BoxInfo_ROU);
			Owner()._hsmIsExecGetCleanBox_ROU = true;

			// this->isGetCleanBox = false;  // 测试
			// this->isGetCleanBox = true;  // 测试

			if(this->isGetCleanBox)
			{
				Owner()._motionSdk->cleanBoxInit(Owner()._BoxInfo_ROU);
				Owner()._hsmIsExecGetAndMergeSlamPath_ROU = false;
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 获取清扫区域成功, 开始搜索清扫起始点" << endl;
				LOGHSM(INFO) << "常规清扫模式: 获取清扫区域成功, 开始搜索清扫起始点";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 获取清扫区域失败, 将进行语音播报并进入待机状态" << endl;
				LOGHSM(INFO) << "常规清扫模式: 获取清扫区域失败, 将进行语音播报并进入待机状态";

				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = true;
			}
		}
		else if(mode_getCleanBox_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			this->isGetCleanBox = Owner()._planningSdk->getCleanDelimit(Owner()._BoxInfo_DEL);
			Owner()._hsmIsExecGetCleanBox_DEL = true;

			// this->isGetCleanBox = false;  // 测试
			// this->isGetCleanBox = true;  // 测试

			if(this->isGetCleanBox)
			{
				Owner()._motionSdk->cleanBoxInitDelimit(Owner()._BoxInfo_DEL);
				Owner()._hsmIsExecGetAndMergeSlamPath_DEL = false;
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 获取清扫区域成功, 开始搜索清扫起始点" << endl;
				LOGHSM(INFO) << "划区清扫模式: 获取清扫区域成功, 开始搜索清扫起始点";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 获取清扫区域失败, 将进行语音播报并进入待机状态" << endl;
				LOGHSM(INFO) << "划区清扫模式: 获取清扫区域失败, 将进行语音播报并进入待机状态";

				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = true;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "getCleanBoxLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "getCleanBoxLev6 --> Update WRONG";
		}
	}

	void rubbyStates::getCleanBoxLev6::OnExit()
	{
		mode_getCleanBox_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		this->isGetCleanBox = false;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-getCleanBoxLev6" << endl;
	}

	/********************** Lev6-getCleanStartPointLev6 ***********************/

	void rubbyStates::getCleanStartPointLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-getCleanStartPointLev6" << endl;
		mode_getCleanStartPoint_Lev6 = args.mode_getCleanStartPoint_Args_Lev6;
	}

	Transition rubbyStates::getCleanStartPointLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::getCleanStartPointLev6::Update()
	{
		if(mode_getCleanStartPoint_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索清扫起始点之前初始化所有变量, 开始获取清扫起始点" << endl;
			LOGHSM(INFO) << "常规清扫模式: 搜索清扫起始点之前初始化所有变量, 开始获取清扫起始点";
			Owner().resetParams_ROU();

			Owner()._hsmIsGetCleanStartPoint_ROU = Owner()._planningSdk->getCleanStart(Owner()._PoseStamped_ROU);
			Owner()._hsmIsExecGetCleanStartPoint_ROU = true;

			if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Out && Owner().boxflag == 1)
			{
				Owner()._hsmIsGetCleanStartPoint_ROU = SiggetCleanStart::FindUncleanPoint_Null;
			}

			// Owner()._hsmIsGetCleanStartPoint_ROU = SiggetCleanStart::FindUncleanPoint_Null;  // 测试
			// Owner()._hsmIsGetCleanStartPoint_ROU = SiggetCleanStart::FindUncleanPoint_Inside;  // 测试
			// Owner()._hsmIsGetCleanStartPoint_ROU = SiggetCleanStart::FindUncleanPoint_Out;  // 测试

			switch (Owner()._hsmIsGetCleanStartPoint_ROU)
			{
			case SiggetCleanStart::FindUncleanPoint_Null:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 未搜索到清扫起始点, 即表示所有区域已经清扫完成, 开始进行沿边，搜索障碍物点" << endl;
				LOGHSM(INFO) << "常规清扫模式: 未搜索到清扫起始点, 即表示所有区域已经清扫完成, 开始进行沿边，搜索障碍物点";
				break;

			case SiggetCleanStart::FindUncleanPoint_Out:
				Owner()._poseRecord_ROU = Owner()._PoseStamped_ROU;  /*记录的清扫起始点*/
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索到区域外清扫起始点, 即表示当前区域已经清扫完成" << endl;
				LOGHSM(INFO) << "常规清扫模式: 搜索到区域外清扫起始点, 即表示当前区域已经清扫完成";
				break;

			case SiggetCleanStart::FindUncleanPoint_Inside:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索到区域内清扫起始点, 开始导航规划到区域内清扫起始点的路径" << endl;
				LOGHSM(INFO) << "常规清扫模式: 搜索到区域内清扫起始点, 开始导航规划到区域内清扫起始点的路径";
				break;

			case SiggetCleanStart::FindUncleanPoint_InObs:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索清扫起始点时返回FindUncleanPoint_InObs, 进入脱困模式" << endl;
				LOGHSM(INFO) << "常规清扫模式: 搜索清扫起始点时返回FindUncleanPoint_InObs, 进入脱困模式";
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索清扫起始点返回异常值, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "常规清扫模式: 搜索清扫起始点返回异常值, 状态机阻塞...";
				break;
			}
		}
		else if(mode_getCleanStartPoint_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 搜索清扫起始点之前初始化所有变量, 开始获取清扫起始点" << endl;
			LOGHSM(INFO) << "划区清扫模式: 搜索清扫起始点之前初始化所有变量, 开始获取清扫起始点";
			Owner().resetParams_DEL();

			Owner()._hsmIsGetCleanStartPoint_DEL = Owner()._planningSdk->getCleanStartDelimit(Owner()._PoseStamped_DEL);
			Owner()._hsmIsExecGetCleanStartPoint_DEL = true;

			// if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside && Owner().boxflag == 1)
			// {
			// 	Owner()._hsmIsGetCleanStartPoint_DEL = SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;
			// }

			// Owner()._hsmIsGetCleanStartPoint_DEL = SiggetCleanStartDelimit::FindUncleanPointDelimit_Null;  // 测试
			// Owner()._hsmIsGetCleanStartPoint_DEL = SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside;  // 测试
			// Owner()._hsmIsGetCleanStartPoint_DEL = SiggetCleanStartDelimit::FindUncleanPointDelimit_Out;  // 测试

			switch (Owner()._hsmIsGetCleanStartPoint_DEL)
			{
			case SiggetCleanStartDelimit::FindUncleanPointDelimit_Null:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 未搜索到清扫起始点, 即表示所有区域已经清扫完成, 开始进行沿边，搜索障碍物点" << endl;
				LOGHSM(INFO) << "划区清扫模式: 未搜索到清扫起始点, 即表示所有区域已经清扫完成, 开始进行沿边，搜索障碍物点";
				break;

			case SiggetCleanStartDelimit::FindUncleanPointDelimit_Out:
				Owner()._poseRecord_DEL = Owner()._PoseStamped_DEL;  /*记录的清扫起始点*/
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 搜索到区域外清扫起始点, 即表示当前区域已经清扫完成" << endl;
				LOGHSM(INFO) << "划区清扫模式: 搜索到区域外清扫起始点, 即表示当前区域已经清扫完成";
				break;

			case SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 搜索到区域内清扫起始点, 开始导航规划到区域内清扫起始点的路径" << endl;
				LOGHSM(INFO) << "划区清扫模式: 搜索到区域内清扫起始点, 开始导航规划到区域内清扫起始点的路径";
				break;

			case SiggetCleanStartDelimit::FindUncleanPointDelimit_InObs:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 搜索清扫起始点时返回FindUncleanPointDelimit_InObs, 进入脱困模式" << endl;
				LOGHSM(INFO) << "划区清扫模式: 搜索清扫起始点时返回FindUncleanPointDelimit_InObs, 进入脱困模式";
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 搜索清扫起始点返回异常值, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "划区清扫模式: 搜索清扫起始点返回异常值, 状态机阻塞...";
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "getCleanStartPointLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "getCleanStartPointLev6 --> Update WRONG";
		}
	}

	void rubbyStates::getCleanStartPointLev6::OnExit()
	{
		mode_getCleanStartPoint_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-getCleanStartPointLev6" << endl;
	}

	/********************** Lev6-motionNavLev6 ***********************/

	void rubbyStates::motionNavLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-motionNavLev6" << endl;
		mode_motionNav_Lev6 = args.mode_motionNav_Args_Lev6;

		if(mode_motionNav_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			if(!Owner()._hsmIsExecGetAndMergeSlamPath_ROU && Owner()._hsmIsMergeSlamPath_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion SLAM合并路径开始初始化" << endl;
				LOGHSM(INFO) << "常规清扫模式: motion SLAM合并路径开始初始化";

				Owner()._hsmIsMotionNavPathInit_ROU = Owner()._motionSdk->followNavPathInit(Owner()._slamPath_ROU);

				// Owner()._hsmIsMotionNavPathInit_ROU = true;  // 测试
				// Owner()._hsmIsMotionNavPathInit_ROU = false;  // 测试

				if(Owner()._hsmIsMotionNavPathInit_ROU)
				{
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion SLAM合并路径初始化完成, 开始追踪合并的SLAM路径......" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion SLAM合并路径初始化完成, 开始追踪合并的SLAM路径......";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion SLAM合并路径初始化失败, 状态机阻塞" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion SLAM合并路径初始化失败, 状态机阻塞";
				}
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion导航路径开始初始化" << endl;
				LOGHSM(INFO) << "常规清扫模式: motion导航路径开始初始化";

				Owner()._hsmIsMotionNavPathInit_ROU = Owner()._motionSdk->followNavPathInit(Owner()._navPath_ROU);

				// Owner()._hsmIsMotionNavPathInit_ROU = false;  // 测试
				// Owner()._hsmIsMotionNavPathInit_ROU = true;  // 测试

				if(Owner()._hsmIsMotionNavPathInit_ROU)
				{
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion路径初始化成功, 开始追踪路径......" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion路径初始化成功, 开始追踪路径......";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion导航路径初始化失败, 状态机阻塞" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion导航路径初始化失败, 状态机阻塞";
				}
			}
		}
		else if(mode_motionNav_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			if(!Owner()._hsmIsExecGetAndMergeSlamPath_DEL && Owner()._hsmIsMergeSlamPath_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion SLAM合并路径开始初始化" << endl;
				LOGHSM(INFO) << "划区清扫模式: motion SLAM合并路径开始初始化";

				Owner()._hsmIsMotionNavPathInit_DEL = Owner()._motionSdk->followNavPathInit(Owner()._slamPath_DEL);
				// Owner()._hsmIsMotionNavPathInit_DEL = true;  // 测试
				// Owner()._hsmIsMotionNavPathInit_DEL = false;  // 测试
				if(Owner()._hsmIsMotionNavPathInit_DEL)
				{
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion SLAM合并路径初始化完成, 开始追踪合并的SLAM路径......" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion SLAM合并路径初始化完成, 开始追踪合并的SLAM路径......";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion SLAM合并路径初始化失败, 状态机阻塞" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion SLAM合并路径初始化失败, 状态机阻塞";
				}
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion导航路径开始初始化" << endl;
				LOGHSM(INFO) << "划区清扫模式: motion导航路径开始初始化";

				Owner()._hsmIsMotionNavPathInit_DEL = Owner()._motionSdk->followNavPathInit(Owner()._navPath_DEL);
				// Owner()._hsmIsMotionNavPathInit_DEL = false;  // 测试
				// Owner()._hsmIsMotionNavPathInit_DEL = true;  // 测试

				if(Owner()._hsmIsMotionNavPathInit_DEL)
				{
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion路径初始化成功, 开始追踪路径......" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion路径初始化成功, 开始追踪路径......";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion导航路径初始化失败, 状态机阻塞" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion导航路径初始化失败, 状态机阻塞";
				}
			}
		}
		else if(mode_motionNav_Lev6 == rubbyHSM::workingModeName::WORK_RECHARGE)
		{
			*(Owner()._FILE) << s_log.logNotes() + "回充模式: motion导航路径开始初始化" << endl;
			LOGHSM(INFO) << "回充模式: motion导航路径开始初始化";

			Owner()._hsmIsMotionNavPathInit_REC = Owner()._motionSdk->followNavPathInit(Owner()._navPath_REC);
			// Owner()._hsmIsMotionNavPathInit_REC = false;  // 测试
			// Owner()._hsmIsMotionNavPathInit_REC = true;  // 测试

			if(Owner()._hsmIsMotionNavPathInit_REC)
			{
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: motion路径初始化成功, 开始追踪路径......" << endl;
				LOGHSM(INFO) << "回充模式: motion路径初始化成功, 开始追踪路径......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: motion导航路径初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "回充模式: motion导航路径初始化失败, 状态机阻塞";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "motionNavLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "motionNavLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::motionNavLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::motionNavLev6::Update()
	{
		if(mode_motionNav_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmIsMotionNavPathInit_ROU)
		{
			Owner()._hsmIsMotionNavPath_ROU = Owner()._motionSdk->followNavPath();
			Owner()._hsmIsExecMotionNavPath_ROU = true;

			if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Null && Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_Null)   /*获取清扫起始点返回值为0时且没有搜索到障碍物点*/
			{
				switch (Owner()._hsmIsMotionNavPath_ROU)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到原点的路径成功, 已经返回清扫起始点, 接下来进行上桩, 首先进行上桩初始化" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion追踪导航到原点的路径成功, 已经返回清扫起始点, 接下来进行上桩, 首先进行上桩初始化";
					break;

				case NodeStatus::FAILURE:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到原点的路径失败, 将进行避障动作" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion追踪导航到原点的路径失败, 将进行避障动作";
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到原点的路径返回值出现异常" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion追踪导航到原点的路径出现异常";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Null && Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_success)   /*获取清扫起始点返回值为0时且搜索到障碍物点*/
			{
				switch (Owner()._hsmIsMotionNavPath_ROU)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					if(Owner()._hsmIsExecGetObsLeftStart_ROU)  /*获取到左沿边起始点*/
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到左沿边起始点的路径成功, 接下来进行左沿边初始化" << endl;
						LOGHSM(INFO) << "常规清扫模式: motion追踪导航到左沿边起始点的路径成功, 接下来进行左沿边初始化";
					}
					else
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到障碍物点的路径成功, 接下来进行右沿边初始化" << endl;
						LOGHSM(INFO) << "常规清扫模式: motion追踪导航到障碍物点的路径成功, 接下来进行右沿边初始化";
						Owner()._motionSdk->followObsInit(Owner()._searchNavPoint_ROU, Owner()._searchObsPoint_ROU);  /*设置沿边参考点*/
					}
					break;

				case NodeStatus::FAILURE:
					if(Owner()._hsmIsExecGetObsLeftStart_ROU)
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到左沿边起始点的路径失败, 将进行避障动作" << endl;
						LOGHSM(INFO) << "常规清扫模式: motion追踪导航到左沿边起始点的路径失败, 将进行避障动作";
					}
					else
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到障碍物点的路径失败, 将进行避障动作" << endl;
						LOGHSM(INFO) << "常规清扫模式: motion追踪导航到障碍物点的路径失败, 将进行避障动作";	
					}
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航路径返回值出现异常" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion追踪导航路径出现异常";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Null && Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_InObs)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motionNavLev6时搜索障碍物点为FindObs_InObs, 进入脱困模式" << endl;
				LOGHSM(INFO) << "常规清扫模式: motionNavLev6时搜索障碍物点为FindObs_InObs, 进入脱困模式";
			}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Out)  /*获取清扫起始点返回值为1时的分支*/
				{
					if(!Owner()._hsmIsExecGetAndMergeSlamPath_ROU)
					{
							switch (Owner()._hsmIsMotionNavPath_ROU)
							{
							case NodeStatus::RUNNING:
								break;

							case NodeStatus::SUCCESS:
								Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
								Owner()._hsmIsExecGetAndMergeSlamPath_ROU = true;
								*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪合并的SLAM路径成功, 将重新搜索清扫起始点" << endl;
								LOGHSM(INFO) << "常规清扫模式: motion追踪合并的SLAM路径成功, 将重新搜索清扫起始点";
								break;
							
							case NodeStatus::FAILURE:
								*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪合并的SLAM路径失败, 开始进行避障动作" << endl;
								LOGHSM(INFO) << "常规清扫模式: motion追踪合并的SLAM路径失败, 开始进行避障动作";
								break;
							
							default:
								*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到区域外清扫起始点的路径出现异常, 返回异常值" << endl;
								LOGHSM(INFO) << "常规清扫模式: motion追踪导航到区域外清扫起始点的路径出现异常, 返回异常值";
								break;
							}
					}
					else
					{
						switch (Owner()._hsmIsMotionNavPath_ROU)
						{
						case NodeStatus::RUNNING:
							break;

						case NodeStatus::SUCCESS:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到区域外清扫起始点的路径成功, 将重新开始获取清扫区域" << endl;
							LOGHSM(INFO) << "常规清扫模式: motion追踪导航到区域外清扫起始点的路径成功, 将重新开始获取清扫区域";
							Owner()._hsmIsExecGetCleanBox_ROU = false;
							Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
							break;
						
						case NodeStatus::FAILURE:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到区域外清扫起始点的路径失败, 将进行避障动作" << endl;
							LOGHSM(INFO) << "常规清扫模式: motion追踪导航到区域外清扫起始点的路径失败, 将进行避障动作";
							break;
						
						default:
							*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到区域外清扫起始点的路径出现异常, 返回非法值" << endl;
							LOGHSM(INFO) << "常规清扫模式: motion追踪导航到区域外清扫起始点的路径出现异常, 返回非法值";
							break;
						}
					}
				}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Inside)
			{
				switch (Owner()._hsmIsMotionNavPath_ROU)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到区域内清扫起始点的路径成功, 接下来进行弓字形路径的获取" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion追踪导航到区域内清扫起始点的路径成功, 接下来进行弓字形路径的获取";
					break;

				case NodeStatus::FAILURE:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到区域内清扫起始点的路径失败, 将进行避障动作" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion追踪导航到区域内清扫起始点的路径失败, 将进行避障动作";
					break;
					
				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪导航到区域内清扫起始点的路径出现异常, 返回异常值" << endl;
					LOGHSM(INFO) << "常规清扫模式: motion追踪导航到区域内清扫起始点的路径出现异常, 返回异常值";
					break;
				}
			}
		}
		else if(mode_motionNav_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmIsMotionNavPathInit_DEL)
		{
			Owner()._hsmIsMotionNavPath_DEL = Owner()._motionSdk->followNavPath();
			Owner()._hsmIsExecMotionNavPath_DEL = true;

			// Owner()._hsmIsMotionNavPath_DEL = 0;  // 测试
			// Owner()._hsmIsMotionNavPath_DEL = 1;  // 测试
			// Owner()._hsmIsMotionNavPath_DEL = 2;  // 测试

			if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Null && Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_Null)   /*获取清扫起始点返回值为0时且没有搜索到障碍物点*/
			{
				switch (Owner()._hsmIsMotionNavPath_DEL)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到原点的路径成功, 已经返回清扫起始点, 接下来进行上桩, 首先进行上桩初始化" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion追踪导航到原点的路径成功, 已经返回清扫起始点, 接下来进行上桩, 首先进行上桩初始化";
					break;

				case NodeStatus::FAILURE:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到原点的路径失败, 将进行避障动作" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion追踪导航到原点的路径失败, 将进行避障动作";
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到原点的路径返回值出现异常" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion追踪导航到原点的路径出现异常";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Null && Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_success)
			{
				switch (Owner()._hsmIsMotionNavPath_DEL)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					if(Owner()._hsmIsExecGetObsLeftStart_DEL)
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到左沿边起始点的路径成功, 接下来进行左沿边初始化" << endl;
						LOGHSM(INFO) << "划区清扫模式: motion追踪导航到左沿边起始点的路径成功, 接下来进行左沿边初始化";
					}
					else
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到障碍物点的路径成功, 接下来进行右沿边初始化" << endl;
						LOGHSM(INFO) << "划区清扫模式: motion追踪导航到障碍物点的路径成功, 接下来进行右沿边初始化";
						Owner()._motionSdk->followObsInit(Owner()._searchNavPoint_DEL, Owner()._searchObsPoint_DEL);
					}
					break;

				case NodeStatus::FAILURE:
					if(Owner()._hsmIsExecGetObsLeftStart_DEL)
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到左沿边起始点的路径失败, 将进行避障动作" << endl;
						LOGHSM(INFO) << "划区清扫模式: motion追踪导航到左沿边起始点的路径失败, 将进行避障动作";
					}
					else
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到原点的路径失败, 将进行避障动作" << endl;
						LOGHSM(INFO) << "划区清扫模式: motion追踪导航到原点的路径失败, 将进行避障动作";
					}
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航路径返回值出现异常" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion追踪导航路径出现异常";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Null && Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_InObs)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motionNavLev6时搜索障碍物点为FindObs_InObs, 进入脱困模式" << endl;
				LOGHSM(INFO) << "划区清扫模式: motionNavLev6时搜索障碍物点为FindObs_InObs, 进入脱困模式";
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Out)  /*获取清扫起始点返回值为1时的分支*/
			{
				if(!Owner()._hsmIsExecGetAndMergeSlamPath_DEL)
				{
						switch (Owner()._hsmIsMotionNavPath_DEL)
						{
						case NodeStatus::RUNNING:
							break;

						case NodeStatus::SUCCESS:
							Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
							Owner()._hsmIsExecGetAndMergeSlamPath_DEL = true;
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪合并的SLAM路径成功, 将重新搜索清扫起始点" << endl;
							LOGHSM(INFO) << "划区清扫模式: motion追踪合并的SLAM路径成功, 将重新搜索清扫起始点";
							break;
						
						case NodeStatus::FAILURE:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪合并的SLAM路径失败, 开始进行避障动作" << endl;
							LOGHSM(INFO) << "划区清扫模式: motion追踪合并的SLAM路径失败, 开始进行避障动作";
							break;
						
						default:
							*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到区域外清扫起始点的路径出现异常, 返回异常值" << endl;
							LOGHSM(INFO) << "划区清扫模式: motion追踪导航到区域外清扫起始点的路径出现异常, 返回异常值";
							break;
						}
				}
				else
				{
					switch (Owner()._hsmIsMotionNavPath_DEL)
					{
					case NodeStatus::RUNNING:
						break;

					case NodeStatus::SUCCESS:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到区域外清扫起始点的路径成功, 将重新开始获取清扫区域" << endl;
						LOGHSM(INFO) << "划区清扫模式: motion追踪导航到区域外清扫起始点的路径成功, 将重新开始获取清扫区域";
						Owner()._hsmIsExecGetCleanBox_DEL = false;
						Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
						break;
					
					case NodeStatus::FAILURE:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到区域外清扫起始点的路径失败, 将进行避障动作" << endl;
						LOGHSM(INFO) << "划区清扫模式: motion追踪导航到区域外清扫起始点的路径失败, 将进行避障动作";
						break;
					
					default:
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到区域外清扫起始点的路径出现异常, 返回非法值" << endl;
						LOGHSM(INFO) << "划区清扫模式: motion追踪导航到区域外清扫起始点的路径出现异常, 返回非法值";
						break;
					}
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside)
			{
				switch (Owner()._hsmIsMotionNavPath_DEL)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到区域内清扫起始点的路径成功, 接下来进行弓字形路径的获取" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion追踪导航到区域内清扫起始点的路径成功, 接下来进行弓字形路径的获取";
					break;

				case NodeStatus::FAILURE:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到区域内清扫起始点的路径失败, 将进行避障动作" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion追踪导航到区域内清扫起始点的路径失败, 将进行避障动作";
					break;
					
				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪导航到区域内清扫起始点的路径出现异常, 返回异常值" << endl;
					LOGHSM(INFO) << "划区清扫模式: motion追踪导航到区域内清扫起始点的路径出现异常, 返回异常值";
					break;
				}
			}
		}
		else if(mode_motionNav_Lev6 == rubbyHSM::workingModeName::WORK_RECHARGE && Owner()._hsmIsMotionNavPathInit_REC)
		{
			Owner()._hsmIsMotionNavPath_REC = Owner()._motionSdk->followNavPath();
			Owner()._hsmIsExecMotionNavPath_REC = true;
			
			// Owner()._hsmIsMotionNavPath_REC = 0;  // 测试
			// Owner()._hsmIsMotionNavPath_REC = 1;  // 测试
			// Owner()._hsmIsMotionNavPath_REC = 2;  // 测试

			switch (Owner()._hsmIsMotionNavPath_REC)
			{
			case NodeStatus::RUNNING:
				break;

			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: motion追踪导航到重定位动作结束点的路径成功, 接下来进行上桩, 首先进行上桩初始化" << endl;
				LOGHSM(INFO) << "回充模式: motion追踪导航到重定位动作结束点的路径成功, 接下来进行上桩, 首先进行上桩初始化";
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: motion追踪导航到重定位动作结束点的路径失败, 将进行避障动作" << endl;
				LOGHSM(INFO) << "回充模式: motion追踪导航到重定位动作结束点的路径失败, 将进行避障动作";
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: motion追踪导航到重定位动作结束点的路径返回值出现异常" << endl;
				LOGHSM(INFO) << "回充模式: motion追踪导航到重定位动作结束点的路径出现异常";
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "motionNavLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "motionNavLev6 --> Update WRONG";
		}
	}

	void rubbyStates::motionNavLev6::OnExit()
	{
		mode_motionNav_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-motionNavLev6" << endl;
	}

	/********************** Lev6-motionBowLev6 ***********************/

	void rubbyStates::motionBowLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-motionBowLev6" << endl;
		mode_motionBow_Lev6 = args.mode_motionBow_Args_Lev6;

		if(mode_motionBow_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪弓字形路径开始初始化" << endl;
			LOGHSM(INFO) << "常规清扫模式: motion追踪弓字形路径开始初始化";

			Owner()._hsmIsMotionBowInit_ROU = Owner()._motionSdk->followBowInit(Owner()._bowPath_ROU);
			// Owner()._hsmIsMotionBowInit_ROU = false;  // 测试
			// Owner()._hsmIsMotionBowInit_ROU = true;  // 测试

			if(Owner()._hsmIsMotionBowInit_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪弓字形路径初始化完成, 开始motion追踪弓字形路径......" << endl;
				LOGHSM(INFO) << "常规清扫模式: motion追踪弓字形路径初始化完成, 开始motion追踪弓字形路径......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪弓字形路径初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "常规清扫模式: motion追踪弓字形路径初始化失败, 状态机阻塞";
			}
		}
		else if(mode_motionBow_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪弓字形路径开始初始化" << endl;
			LOGHSM(INFO) << "划区清扫模式: motion追踪弓字形路径开始初始化";

			Owner()._hsmIsMotionBowInit_DEL = Owner()._motionSdk->followBowInit(Owner()._bowPath_DEL);
			// Owner()._hsmIsMotionBowInit_DEL = false;  // 测试
			// Owner()._hsmIsMotionBowInit_DEL = true;  // 测试

			if(Owner()._hsmIsMotionBowInit_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪弓字形路径初始化完成, 开始motion追踪弓字形路径......" << endl;
				LOGHSM(INFO) << "划区清扫模式: motion追踪弓字形路径初始化完成, 开始motion追踪弓字形路径......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪弓字形路径初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "划区清扫模式: motion追踪弓字形路径初始化失败, 状态机阻塞";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "motionBowLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "motionBowLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::motionBowLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::motionBowLev6::Update()
	{
		if(mode_motionBow_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmIsMotionBowInit_ROU)
		{
			Owner()._hsmIsMotionBow_ROU = Owner()._motionSdk->followBowPath();
			Owner()._hsmIsExecMotionBow_ROU = true;

			// Owner()._hsmIsMotionBow_ROU = 0; // 测试
			// Owner()._hsmIsMotionBow_ROU = 1; // 测试
			// Owner()._hsmIsMotionBow_ROU = 2; // 测试

			switch (Owner()._hsmIsMotionBow_ROU)
			{
			case NodeStatus::RUNNING:
				break;

			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪弓字形路径成功, 重新获取弓字形路径" << endl;
				LOGHSM(INFO) << "常规清扫模式: motion追踪弓字形路径成功, 重新获取弓字形路径";

				Owner()._hsmIsExecGetBowPath_ROU = false;
				Owner()._hsmIsGetBowPath_ROU = false;
				Owner()._hsmIsMotionBowInit_ROU = false;
				Owner()._hsmIsMotionBow_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionBow_ROU = false;

				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪弓字形路径失败, 将进行避障动作" << endl;
				LOGHSM(INFO) << "常规清扫模式: motion追踪弓字形路径失败, 将进行避障动作";
				Owner()._hsmIsExecObstacleAvoid_ROU = false;
				break;

			default:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: motion追踪弓字形路径出现异常, 请进入 Lev6-motionBowLev6 进行排查" << endl;
				LOGHSM(INFO) << "常规清扫模式: motion追踪弓字形路径出现异常, 请进入 Lev6-motionBowLev6 进行排查";
				break;
			}
		}
		else if(mode_motionBow_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmIsMotionBowInit_DEL)
		{
			Owner()._hsmIsMotionBow_DEL = Owner()._motionSdk->followBowPathDelimit();
			Owner()._hsmIsExecMotionBow_DEL = true;

			// Owner()._hsmIsMotionBow_DEL = 0; // 测试
			// Owner()._hsmIsMotionBow_DEL = 1; // 测试
			// Owner()._hsmIsMotionBow_DEL = 2; // 测试

			switch (Owner()._hsmIsMotionBow_DEL)
			{
			case NodeStatus::RUNNING:
				break;

			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪弓字形路径成功, 重新获取弓字形路径" << endl;
				LOGHSM(INFO) << "划区清扫模式: motion追踪弓字形路径成功, 重新获取弓字形路径";

				Owner()._hsmIsExecGetBowPath_DEL = false;
				Owner()._hsmIsGetBowPath_DEL = false;
				Owner()._hsmIsMotionBowInit_DEL = false;
				Owner()._hsmIsMotionBow_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionBow_DEL = false;

				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪弓字形路径失败, 将进行避障动作" << endl;
				LOGHSM(INFO) << "划区清扫模式: motion追踪弓字形路径失败, 将进行避障动作";
				Owner()._hsmIsExecObstacleAvoid_DEL = false;
				break;

			default:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: motion追踪弓字形路径出现异常, 请进入 Lev6-motionBowLev6 进行排查" << endl;
				LOGHSM(INFO) << "划区清扫模式: motion追踪弓字形路径出现异常, 请进入 Lev6-motionBowLev6 进行排查";
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "motionBowLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "motionBowLev6 --> Update WRONG";
		}
	}

	void rubbyStates::motionBowLev6::OnExit()
	{
		mode_motionBow_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-motionBowLev6" << endl;
	}

	/********************** Lev6-getSlamPathLev6 ***********************/

	void rubbyStates::getSlamPathLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-getSlamPathLev6" << endl;
		mode_getSlamPath_Lev6 = args.mode_getSlamPath_Args_Lev6;

		if(mode_getSlamPath_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 开始获取SLAM路径" << endl;
			LOGHSM(INFO) << "常规清扫模式: 开始获取SLAM路径";			
		}
		else if(mode_getSlamPath_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 开始获取SLAM路径" << endl;
			LOGHSM(INFO) << "划区清扫模式: 开始获取SLAM路径";
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "getSlamPathLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "getSlamPathLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::getSlamPathLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::getSlamPathLev6::Update()
	{
		if(mode_getSlamPath_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			Owner()._hsmIsGetSlamPath_ROU = Owner()._planningSdk->getSlamPath(Owner()._BoxInfo_ROU, Owner()._slamPose_ROU);
			Owner()._hsmIsExecGetSlamPath_ROU = true;

			// Owner()._hsmIsGetSlamPath_ROU = false;  // 测试
			// Owner()._hsmIsGetSlamPath_ROU = true;  // 测试

			if(Owner()._hsmIsGetSlamPath_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 获取SLAM路径成功, 将调用导航规划到SLAM路径端点的路径" << endl;
				LOGHSM(INFO) << "常规清扫模式: 获取SLAM路径成功, 将调用导航规划到SLAM路径端点的路径";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 获取SLAM路径失败, 将调用导航规划到区域外起始点的路径" << endl;
				Owner()._hsmIsExecGetAndMergeSlamPath_ROU = true;
				LOGHSM(INFO) << "常规清扫模式: 获取SLAM路径失败, 将调用导航规划到区域外起始点的路径";
			}
		}
		else if(mode_getSlamPath_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			Owner()._hsmIsGetSlamPath_DEL = Owner()._planningSdk->getSlamPathDelimit(Owner()._BoxInfo_DEL, Owner()._slamPose_DEL);
			Owner()._hsmIsExecGetSlamPath_DEL = true;

			// Owner()._hsmIsGetSlamPath_DEL = false;  // 测试
			// Owner()._hsmIsGetSlamPath_DEL = true;  // 测试

			if(Owner()._hsmIsGetSlamPath_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 获取SLAM路径成功, 将调用导航规划到SLAM路径端点的路径" << endl;
				LOGHSM(INFO) << "划区清扫模式: 获取SLAM路径成功, 将调用导航规划到SLAM路径端点的路径";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 获取SLAM路径失败, 将调用导航规划到区域外起始点的路径" << endl;
				Owner()._hsmIsExecGetAndMergeSlamPath_DEL = true;
				LOGHSM(INFO) << "划区清扫模式: 获取SLAM路径失败, 将调用导航规划到区域外起始点的路径";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "getSlamPathLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "getSlamPathLev6 --> Update WRONG";
		}
	}

	void rubbyStates::getSlamPathLev6::OnExit()
	{
		mode_getSlamPath_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-getSlamPathLev6" << endl;
	}

	/********************** Lev6-mergeSlamPathLev6 ***********************/

	void rubbyStates::mergeSlamPathLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-mergeSlamPathLev6" << endl;
		mode_mergeSlamPath_Lev6 = args.mode_mergeSlamPath_Args_Lev6;
		if(mode_mergeSlamPath_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 开始合并SLAM路径" << endl;
			LOGHSM(INFO) << "常规清扫模式: 开始合并SLAM路径";
		}
		else if(mode_mergeSlamPath_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 开始合并SLAM路径" << endl;
			LOGHSM(INFO) << "划区清扫模式: 开始合并SLAM路径";
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "getSlamPathLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "getSlamPathLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::mergeSlamPathLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::mergeSlamPathLev6::Update()
	{
		if(mode_mergeSlamPath_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			Owner()._hsmIsMergeSlamPath_ROU = Owner()._planningSdk->mergeSlamPath(Owner()._navPath_ROU, Owner()._slamPath_ROU);
			Owner()._hsmIsExecMergeSlamPath_ROU = true;

			// Owner()._hsmIsMergeSlamPath_ROU = true;  // 测试
			// Owner()._hsmIsMergeSlamPath_ROU = false;  // 测试

			if(Owner()._hsmIsMergeSlamPath_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 合并SLAM路径成功, 将调用motion追踪路径合并的SLAM路径" << endl;
				LOGHSM(INFO) << "常规清扫模式: 合并SLAM路径成功, 将调用motion追踪路径合并的SLAM路径";
			}
			else
			{
				Owner()._hsmIsExecGetAndMergeSlamPath_ROU = true;

				Owner()._hsmIsGetSlamPath_ROU = false;
				Owner()._hsmIsExecGetSlamPath_ROU = false;
				Owner()._hsmIsExecNav_ROU = false;
				Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;
				Owner()._hsmIsExecMergeSlamPath_ROU = false;
				Owner()._hsmIsMergeSlamPath_ROU = false;
				
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 合并SLAM路径失败, 将调用导航规划到区域外起始点的路径" << endl;
				LOGHSM(INFO) << "常规清扫模式: 合并SLAM路径失败, 将调用导航规划到区域外起始点的路径";
			}
		}
		else if(mode_mergeSlamPath_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			Owner()._hsmIsMergeSlamPath_DEL = Owner()._planningSdk->mergeSlamPath(Owner()._navPath_DEL, Owner()._slamPath_DEL);
			Owner()._hsmIsExecMergeSlamPath_DEL = true;

			// Owner()._hsmIsMergeSlamPath_DEL = true;  // 测试
			// Owner()._hsmIsMergeSlamPath_DEL = false;  // 测试

			if(Owner()._hsmIsMergeSlamPath_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 合并SLAM路径成功, 将调用motion追踪路径合并的SLAM路径" << endl;
				LOGHSM(INFO) << "划区清扫模式: 合并SLAM路径成功, 将调用motion追踪路径合并的SLAM路径";
			}
			else
			{
				Owner()._hsmIsExecGetAndMergeSlamPath_DEL = true;

				Owner()._hsmIsGetSlamPath_DEL = false;
				Owner()._hsmIsExecGetSlamPath_DEL = false;
				Owner()._hsmIsExecNav_DEL = false;
				Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;
				Owner()._hsmIsExecMergeSlamPath_DEL = false;
				Owner()._hsmIsMergeSlamPath_DEL = false;
				
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 合并SLAM路径失败, 将调用导航规划到区域外起始点的路径" << endl;
				LOGHSM(INFO) << "划区清扫模式: 合并SLAM路径失败, 将调用导航规划到区域外起始点的路径";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "getSlamPathLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "getSlamPathLev6 --> Update WRONG";
		}
	}

	void rubbyStates::mergeSlamPathLev6::OnExit()
	{
		mode_mergeSlamPath_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-mergeSlamPathLev6" << endl;
	}

	/********************** Lev6-obstacleAvoidLev6 ***********************/

	void rubbyStates::obstacleAvoidLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-obstacleAvoidLev6" << endl;
		mode_obstacleAvoid_Lev6 = args.mode_obstacleAvoid_Args_Lev6;

		if(mode_obstacleAvoid_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作开始初始化" << endl;
			LOGHSM(INFO) << "常规清扫模式: 避障动作开始初始化";

			Owner()._hsmIsObstacleAvoidInit_ROU = Owner()._motionSdk->ObstacleAvoidanceInit();
			// Owner()._hsmIsObstacleAvoidInit_ROU = true;  //测试
			// Owner()._hsmIsObstacleAvoidInit_ROU = false;  //测试

			if(Owner()._hsmIsObstacleAvoidInit_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作初始化成功, 开始进行避障动作......" << endl;
				LOGHSM(INFO) << "常规清扫模式: 避障动作初始化成功, 开始进行避障动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "常规清扫模式: 避障动作初始化失败, 状态机阻塞";
			}
		}
		else if(mode_obstacleAvoid_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作开始初始化" << endl;
			LOGHSM(INFO) << "划区清扫模式: 避障动作开始初始化";

			Owner()._hsmIsObstacleAvoidInit_DEL = Owner()._motionSdk->ObstacleAvoidanceInit();
			// Owner()._hsmIsObstacleAvoidInit_DEL = true;  //测试
			// Owner()._hsmIsObstacleAvoidInit_DEL = false;  //测试

			if(Owner()._hsmIsObstacleAvoidInit_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作初始化成功, 开始进行避障动作......" << endl;
				LOGHSM(INFO) << "划区清扫模式: 避障动作初始化成功, 开始进行避障动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "划区清扫模式: 避障动作初始化失败, 状态机阻塞";
			}
		}
		else if(mode_obstacleAvoid_Lev6 == rubbyHSM::workingModeName::WORK_RECHARGE)
		{
			*(Owner()._FILE) << s_log.logNotes() + "回充模式: 避障动作开始初始化" << endl;
			LOGHSM(INFO) << "回充模式: 避障动作开始初始化";

			Owner()._hsmIsObstacleAvoidInit_REC = Owner()._motionSdk->ObstacleAvoidanceInit();
			// Owner()._hsmIsObstacleAvoidInit_REC = true;  //测试
			// Owner()._hsmIsObstacleAvoidInit_REC = false;  //测试

			if(Owner()._hsmIsObstacleAvoidInit_REC)
			{
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 避障动作初始化成功, 开始进行避障动作......" << endl;
				LOGHSM(INFO) << "回充模式: 避障动作初始化成功, 开始进行避障动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 避障动作初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "回充模式: 避障动作初始化失败, 状态机阻塞";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "obstacleAvoidLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "obstacleAvoidLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::obstacleAvoidLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::obstacleAvoidLev6::Update()
	{
		if(mode_obstacleAvoid_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmIsObstacleAvoidInit_ROU)
		{
			Owner()._hsmIsObstacleAvoid_ROU = Owner()._motionSdk->ObstacleAvoidance();
			Owner()._hsmIsExecObstacleAvoid_ROU = true;

			// Owner()._hsmIsObstacleAvoid_ROU = 0; //测试
			// Owner()._hsmIsObstacleAvoid_ROU = 1; //测试

			if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Null && Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_Null)
			{
				switch (Owner()._hsmIsObstacleAvoid_ROU)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作完成, 重新导航规划回原点的路径" << endl;
					LOGHSM(INFO) << "常规清扫模式: 避障动作完成, 重新导航规划回原点的路径";
					Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail; 
					Owner()._hsmIsExecNav_ROU = false;

					Owner()._hsmIsMotionNavPathInit_ROU = false;
					Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
					Owner()._hsmIsExecMotionNavPath_ROU = false;

					Owner()._hsmIsObstacleAvoidInit_ROU = false;
					Owner()._hsmIsExecObstacleAvoid_ROU = false;
					Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作返回异常值" << endl;
					LOGHSM(INFO) << "常规清扫模式: 避障动作返回异常值";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Null && Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_success)
			{
				switch (Owner()._hsmIsObstacleAvoid_ROU)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					if(Owner()._hsmIsExecGetObsLeftStart_ROU)  /*执行了获取左沿边起始点*/
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作完成, 重新导航规划到左沿边起始点的路径" << endl;
						LOGHSM(INFO) << "常规清扫模式: 避障动作完成, 重新导航规划到左沿边起始点的路径";
						Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail; 
						Owner()._hsmIsExecNav_ROU = false;

						Owner()._hsmIsMotionNavPathInit_ROU = false;
						Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
						Owner()._hsmIsExecMotionNavPath_ROU = false;

						Owner()._hsmIsObstacleAvoidInit_ROU = false;
						Owner()._hsmIsExecObstacleAvoid_ROU = false;
						Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;
					}
					else
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作完成, 重新导航规划障碍物点的路径" << endl;
						LOGHSM(INFO) << "常规清扫模式: 避障动作完成, 重新导航规划障碍物点的路径";
						Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail; 
						Owner()._hsmIsExecNav_ROU = false;

						Owner()._hsmIsMotionNavPathInit_ROU = false;
						Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
						Owner()._hsmIsExecMotionNavPath_ROU = false;

						Owner()._hsmIsObstacleAvoidInit_ROU = false;
						Owner()._hsmIsExecObstacleAvoid_ROU = false;
						Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;
					}
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作返回异常值" << endl;
					LOGHSM(INFO) << "常规清扫模式: 避障动作返回异常值";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Null && Owner()._hsmIsSearchObsPoint_ROU == SigSearchObsPoint::FindObs_InObs)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: obstacleAvoidLev6时搜索障碍物点为FindObs_InObs, 进入脱困模式" << endl;
				LOGHSM(INFO) << "常规清扫模式: obstacleAvoidLev6时搜索障碍物点为FindObs_InObs, 进入脱困模式";
			}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Out)
			{
				switch (Owner()._hsmIsObstacleAvoid_ROU)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					if(!Owner()._hsmIsExecGetAndMergeSlamPath_ROU)
					{
						Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
						Owner()._hsmIsExecGetAndMergeSlamPath_ROU = true;
						Owner()._hsmIsExecObstacleAvoid_ROU = false;
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作完成, 重新搜索清扫起始点" << endl;
						LOGHSM(INFO) << "常规清扫模式: 避障动作完成, 重新搜索清扫起始点";
					}
					else
					{
						Owner()._hsmIsExecNav_ROU = false;
						Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;
						Owner()._hsmIsMotionNavPathInit_ROU = false;
						Owner()._hsmIsExecMotionNavPath_ROU  =false;
						Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
						Owner()._hsmIsObstacleAvoidInit_ROU = false;
						Owner()._hsmIsExecObstacleAvoid_ROU = false;
						Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作完成, 将重新导航规划到区域外清扫起始点路径" << endl;
						LOGHSM(INFO) << "常规清扫模式: 避障动作完成, 将重新导航规划到区域外清扫起始点路径";
					}

					break;

				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作返回异常值" << endl;
					LOGHSM(INFO) << "常规清扫模式: 避障动作返回异常值";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_ROU == SiggetCleanStart::FindUncleanPoint_Inside)
			{
				switch (Owner()._hsmIsObstacleAvoid_ROU)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					if(Owner()._hsmIsExecGetBowPath_ROU && Owner()._hsmIsGetBowPath_ROU && Owner()._hsmIsMotionBow_ROU == NodeStatus::FAILURE)
					{
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作完成, 重新搜索清扫起始点" << endl;
						LOGHSM(INFO) << "常规清扫模式: 避障动作完成, 重新搜索清扫起始点";
						Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
					}
					else
					{
						Owner()._hsmIsExecNav_ROU = false;
						Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;
						Owner()._hsmIsMotionNavPathInit_ROU = false;
						Owner()._hsmIsExecMotionNavPath_ROU  =false;
						Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
						Owner()._hsmIsObstacleAvoidInit_ROU = false;
						Owner()._hsmIsExecObstacleAvoid_ROU = false;
						Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;
						*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作完成, 重新导航规划到区域内清扫起始点的路径" << endl;
						LOGHSM(INFO) << "常规清扫模式: 避障动作完成, 重新导航规划到区域内清扫起始点的路径";
					}
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 避障动作返回异常值" << endl;
					LOGHSM(INFO) << "常规清扫模式: 避障动作返回异常值";
					break;
				}
			}
		}
		else if(mode_obstacleAvoid_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmIsObstacleAvoidInit_DEL)
		{
			Owner()._hsmIsObstacleAvoid_DEL = Owner()._motionSdk->ObstacleAvoidance();
			Owner()._hsmIsExecObstacleAvoid_DEL = true;

			// Owner()._hsmIsObstacleAvoid_DEL = 0; //测试
			// Owner()._hsmIsObstacleAvoid_DEL = 1; //测试

			if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Null && Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_Null)
			{
				switch (Owner()._hsmIsObstacleAvoid_DEL)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作完成, 重新导航规划回原点的路径" << endl;
					LOGHSM(INFO) << "划区清扫模式: 避障动作完成, 重新导航规划回原点的路径";
					Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail; 
					Owner()._hsmIsExecNav_DEL = false;

					Owner()._hsmIsMotionNavPathInit_DEL = false;
					Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
					Owner()._hsmIsExecMotionNavPath_DEL = false;

					Owner()._hsmIsObstacleAvoidInit_DEL = false;
					Owner()._hsmIsExecObstacleAvoid_DEL = false;
					Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作返回异常值" << endl;
					LOGHSM(INFO) << "划区清扫模式: 避障动作返回异常值";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Null && Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_success)
			{
				switch (Owner()._hsmIsObstacleAvoid_DEL)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					if(Owner()._hsmIsExecGetObsLeftStart_DEL)
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作完成, 重新导航规划到左沿边起始点的路径" << endl;
						LOGHSM(INFO) << "划区清扫模式: 避障动作完成, 重新导航规划到左沿边起始点的路径";
						Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail; 
						Owner()._hsmIsExecNav_DEL = false;

						Owner()._hsmIsMotionNavPathInit_DEL = false;
						Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
						Owner()._hsmIsExecMotionNavPath_DEL = false;

						Owner()._hsmIsObstacleAvoidInit_DEL = false;
						Owner()._hsmIsExecObstacleAvoid_DEL = false;
						Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;
					}
					else
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作完成, 重新导航规划回障碍物点的路径" << endl;
						LOGHSM(INFO) << "划区清扫模式: 避障动作完成, 重新导航规划回障碍物点的路径";
						Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail; 
						Owner()._hsmIsExecNav_DEL = false;

						Owner()._hsmIsMotionNavPathInit_DEL = false;
						Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
						Owner()._hsmIsExecMotionNavPath_DEL = false;

						Owner()._hsmIsObstacleAvoidInit_DEL = false;
						Owner()._hsmIsExecObstacleAvoid_DEL = false;
						Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;
					}
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作返回异常值" << endl;
					LOGHSM(INFO) << "划区清扫模式: 避障动作返回异常值";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Null && Owner()._hsmIsSearchObsPoint_DEL == SigSearchObsPoint::FindObs_InObs)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: obstacleAvoidLev6时搜索障碍物点为FindObs_InObs, 进入脱困模式" << endl;
				LOGHSM(INFO) << "划区清扫模式: obstacleAvoidLev6时搜索障碍物点为FindObs_InObs, 进入脱困模式";
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Out)
			{
				switch (Owner()._hsmIsObstacleAvoid_DEL)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					if(!Owner()._hsmIsExecGetAndMergeSlamPath_DEL)
					{
						Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
						Owner()._hsmIsExecGetAndMergeSlamPath_DEL = true;
						Owner()._hsmIsExecObstacleAvoid_DEL = false;
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作完成, 重新搜索清扫起始点" << endl;
						LOGHSM(INFO) << "划区清扫模式: 避障动作完成, 重新搜索清扫起始点";
					}
					else
					{
						Owner()._hsmIsExecNav_DEL = false;
						Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;
						Owner()._hsmIsMotionNavPathInit_DEL = false;
						Owner()._hsmIsExecMotionNavPath_DEL  =false;
						Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
						Owner()._hsmIsObstacleAvoidInit_DEL = false;
						Owner()._hsmIsExecObstacleAvoid_DEL = false;
						Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作完成, 将重新导航规划到区域外清扫起始点路径" << endl;
						LOGHSM(INFO) << "划区清扫模式: 避障动作完成, 将重新导航规划到区域外清扫起始点路径";
					}

					break;

				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作返回异常值" << endl;
					LOGHSM(INFO) << "划区清扫模式: 避障动作返回异常值";
					break;
				}
			}
			else if(Owner()._hsmIsGetCleanStartPoint_DEL == SiggetCleanStartDelimit::FindUncleanPointDelimit_Inside)
			{
				switch (Owner()._hsmIsObstacleAvoid_DEL)
				{
				case NodeStatus::RUNNING:
					break;

				case NodeStatus::SUCCESS:
					if(Owner()._hsmIsExecGetBowPath_DEL && Owner()._hsmIsGetBowPath_DEL && Owner()._hsmIsMotionBow_DEL == NodeStatus::FAILURE)
					{
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作完成, 重新搜索清扫起始点" << endl;
						LOGHSM(INFO) << "划区清扫模式: 避障动作完成, 重新搜索清扫起始点";
						Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
					}
					else
					{
						Owner()._hsmIsExecNav_DEL = false;
						Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;
						Owner()._hsmIsMotionNavPathInit_DEL = false;
						Owner()._hsmIsExecMotionNavPath_DEL  =false;
						Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
						Owner()._hsmIsObstacleAvoidInit_DEL = false;
						Owner()._hsmIsExecObstacleAvoid_DEL = false;
						Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;
						*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作完成, 重新导航规划到区域内清扫起始点的路径" << endl;
						LOGHSM(INFO) << "划区清扫模式: 避障动作完成, 重新导航规划到区域内清扫起始点的路径";
					}
					break;
				
				default:
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 避障动作返回异常值" << endl;
					LOGHSM(INFO) << "划区清扫模式: 避障动作返回异常值";
					break;
				}
			}
		}
		else if(mode_obstacleAvoid_Lev6 == rubbyHSM::workingModeName::WORK_RECHARGE && Owner()._hsmIsObstacleAvoidInit_REC)
		{
			Owner()._hsmIsObstacleAvoid_REC = Owner()._motionSdk->ObstacleAvoidance();
			Owner()._hsmIsExecObstacleAvoid_REC = true;

			// Owner()._hsmIsObstacleAvoid_REC = 0; //测试
			// Owner()._hsmIsObstacleAvoid_REC = 1; //测试

			switch (Owner()._hsmIsObstacleAvoid_REC)
			{
			case NodeStatus::RUNNING:
				break;

			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 避障动作完成, 重新导航规划回重定位动作结束点的路径" << endl;
				LOGHSM(INFO) << "回充模式: 避障动作完成, 重新导航规划回重定位动作结束点的路径";
				Owner()._hsmIsSucceedNav_REC = SigNav::Nav_fail; 
				Owner()._hsmIsExecNav_REC = false;

				Owner()._hsmIsMotionNavPathInit_REC = false;
				Owner()._hsmIsMotionNavPath_REC = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_REC = false;

				Owner()._hsmIsObstacleAvoidInit_REC = false;
				Owner()._hsmIsExecObstacleAvoid_REC = false;
				Owner()._hsmIsObstacleAvoid_REC = NodeStatus::DEFAULT;
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 避障动作返回异常值" << endl;
				LOGHSM(INFO) << "回充模式: 避障动作返回异常值";
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "obstacleAvoidLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "obstacleAvoidLev6 --> Update WRONG";
		}
	}

	void rubbyStates::obstacleAvoidLev6::OnExit()
	{
		mode_obstacleAvoid_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-obstacleAvoidLev6" << endl;
	}

	/********************** Lev6-mappingAfterRelocation_NoKidnapLev6 ***********************/

	void rubbyStates::mappingAfterRelocation_NoKidnapLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-mappingAfterRelocationNoKidnapLev6" << endl;
		mode_mappingAfterRelocation_NoKidnap_Lev6 = args.mode_mappingAfterRelocation_NoKidnap_Args_Lev6;

		*(Owner()._FILE) << s_log.logNotes() + "(非绑架)建图动作开始初始化" << endl;
		LOGHSM(INFO) << "(非绑架)建图动作开始初始化";

		Owner()._hsmRelocationMapInit_NoKidnap = Owner()._motionSdk->firstbuildmapInit();

		// Owner()._hsmRelocationMapInit_NoKidnap = true;  // 测试
		// Owner()._hsmRelocationMapInit_NoKidnap = false;  // 测试

		if(Owner()._hsmRelocationMapInit_NoKidnap)
		{
			*(Owner()._FILE) << s_log.logNotes() + "(非绑架)建图动作初始化成功, 开始进行(非绑架)建图......" << endl;
			LOGHSM(INFO) << "(非绑架)建图动作初始化成功, 开始进行(非绑架)建图......";
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "(非绑架)建图动作初始化失败, 状态机阻塞..." << endl;
			LOGHSM(INFO) << "(非绑架)建图动作初始化失败, 状态机阻塞...";
		}
	}

	Transition rubbyStates::mappingAfterRelocation_NoKidnapLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::mappingAfterRelocation_NoKidnapLev6::Update()
	{
		if(Owner()._hsmRelocationMapInit_NoKidnap)
		{
#ifdef SLAMTEST
			Owner()._hsmRelocationMapResult_NoKidnap = false;
#else
			Owner()._hsmRelocationMapResult_NoKidnap = Owner()._motionSdk->firstbuildmapRun();
#endif
			
			Owner()._hsmExecRelocationMapResult_NoKidnap = true;

			// Owner()._hsmRelocationMapResult_NoKidnap = true; // 测试
			// Owner()._hsmRelocationMapResult_NoKidnap = false; // 测试

			if(!Owner()._hsmRelocationMapResult_NoKidnap)
			{
				Owner()._hsmExecRelocationMapResult_NoKidnap = false;
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "(非绑架) 建图动作完成, 将选择清扫模式" << endl;
				LOGHSM(INFO) << "(非绑架) 建图动作完成, 将选择清扫模式";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "mappingAfterRelocation_NoKidnapLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "mappingAfterRelocation_NoKidnapLev6 --> Update WRONG";
		}
	}

	void rubbyStates::mappingAfterRelocation_NoKidnapLev6::OnExit()
	{
		mode_mappingAfterRelocation_NoKidnap_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-mappingAfterRelocation_NoKidnapLev6" << endl;
	}

	/********************** Lev6-reLocation_KidnapLev6 ***********************/

	void rubbyStates::reLocation_KidnapLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-reLocation_KidnapLev6" << endl;
		mode_reLocat_Kidnap_Lev6 = args.mode_reLocat_Kidnap_Args_Lev6;

		if(mode_reLocat_Kidnap_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			if(!Owner()._hsmReLocationInit_Kidnap)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 收到机器人被绑架信号, 开始(绑架)重定位初始化" << endl;
				LOGHSM(INFO) << "常规清扫模式: 收到机器人被绑架信号, 开始(绑架)重定位初始化";

				Owner()._hsmReLocationInit_Kidnap = Owner()._motionSdk->reLocationInit();

				// Owner()._hsmReLocationInit_Kidnap = true;  // 测试
				// Owner()._hsmReLocationInit_Kidnap = false;  // 测试

				if(Owner()._hsmReLocationInit_Kidnap)
				{
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)重定位初始化完成, (绑架)重定位动作执行中......" << endl;
					LOGHSM(INFO) << "常规清扫模式: (绑架)重定位初始化完成, (绑架)重定位动作执行中......";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)重定位初始化未完成, 状态机阻塞" << endl;
					LOGHSM(INFO) << "常规清扫模式: (绑架)重定位初始化未完成, 状态机阻塞";
				}
			}
		}
		else if(mode_reLocat_Kidnap_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			if(!Owner()._hsmReLocationInit_Kidnap)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 收到机器人被绑架信号, 开始(绑架)重定位初始化" << endl;
				LOGHSM(INFO) << "划区清扫模式: 收到机器人被绑架信号, 开始(绑架)重定位初始化";

				Owner()._hsmReLocationInit_Kidnap = Owner()._motionSdk->reLocationInit();

				// Owner()._hsmReLocationInit_Kidnap = true;  // 测试
				// Owner()._hsmReLocationInit_Kidnap = false;  // 测试

				if(Owner()._hsmReLocationInit_Kidnap)
				{
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)重定位初始化完成, (绑架)重定位动作执行中......" << endl;
					LOGHSM(INFO) << "划区清扫模式: (绑架)重定位初始化完成, (绑架)重定位动作执行中......";
				}
				else
				{
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)重定位初始化未完成, 状态机阻塞" << endl;
					LOGHSM(INFO) << "划区清扫模式: (绑架)重定位初始化未完成, 状态机阻塞";
				}
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "reLocation_KidnapLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "reLocation_KidnapLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::reLocation_KidnapLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::reLocation_KidnapLev6::Update()
	{
		if(mode_reLocat_Kidnap_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmReLocationInit_Kidnap)
		{
			if(Owner()._SensorInfo->Slamflg.state == Location_map_relocation_ok || Owner()._SensorInfo->Slamflg.state == Location_kinnap_relocation_ok)
			{
				if(Owner()._SensorInfo->Slamflg.state == Location_map_relocation_ok)
				{
					LOGHSM(INFO) << "reLocation_KidnapLev6(ROU): received slam state is: MAP_RELOCATION_OK -- (NAV: Location_map_relocation_ok)";
				}

				if(Owner()._SensorInfo->Slamflg.state == Location_kinnap_relocation_ok)
				{
					LOGHSM(INFO) << "reLocation_KidnapLev6(ROU): received slam state is: KINNAP_RELOCATION_OK -- (NAV: Location_kinnap_relocation_ok)";
				}
				Owner()._motionSdk->RunParking();
				Owner()._hsmReLocation_Kidnap = 1;
			}
			else
			{
				Owner()._hsmReLocation_Kidnap = Owner()._motionSdk->reLocationRun();
			}

			Owner()._hsmIsExecReLocation_Kidnap = true;

			// Owner()._hsmReLocation_Kidnap = 0;  // 测试
			// Owner()._hsmReLocation_Kidnap = 2;  // 测试
			// Owner()._hsmReLocation_Kidnap = 1;  // 测试

			switch (Owner()._hsmReLocation_Kidnap)
			{
			case NodeStatus::RUNNING:
				Owner()._hsmIsExecReLocation_Kidnap = false;
				break;

			case NodeStatus::SUCCESS:
				Owner()._DataDown->FlgSlam = Reposition_NOFINISHEND;
				Owner()._SensorInfo->Slamflg.state = Location_init;
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)重定位动作执行成功, 将进入(绑架)建图动作" << endl;
				LOGHSM(INFO) << "常规清扫模式: (绑架)重定位动作执行成功, 将进入(绑架)建图动作";
				break;

			case NodeStatus::FAILURE:
				Owner()._DataDown->FlgSlam = Reposition_FINISHEND;
				Owner()._SensorInfo->Slamflg.state = Location_init;
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)重定位动作执行失败, 将进入(绑架)建图动作" << endl;
				LOGHSM(INFO) << "常规清扫模式: (绑架)重定位动作执行失败, 将进入(绑架)建图动作";
				Owner()._planningSdk->CleanDataRese();
				break;
					
			default:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)重定位异常, 返回非法值" << endl;
				LOGHSM(INFO) << "常规清扫模式: (绑架)重定位异常, 返回非法值";
				break;
			}
		}
		else if(mode_reLocat_Kidnap_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmReLocationInit_Kidnap)
		{
			if(Owner()._SensorInfo->Slamflg.state == Location_map_relocation_ok || Owner()._SensorInfo->Slamflg.state == Location_kinnap_relocation_ok)
			{
				if(Owner()._SensorInfo->Slamflg.state == Location_map_relocation_ok)
				{
					LOGHSM(INFO) << "reLocation_KidnapLev6(DEL): received slam state is: MAP_RELOCATION_OK -- (NAV: Location_map_relocation_ok)";
				}

				if(Owner()._SensorInfo->Slamflg.state == Location_kinnap_relocation_ok)
				{
					LOGHSM(INFO) << "reLocation_KidnapLev6(DEL): received slam state is: KINNAP_RELOCATION_OK -- (NAV: Location_kinnap_relocation_ok)";
				}

				Owner()._motionSdk->RunParking();
				Owner()._hsmReLocation_Kidnap = 1;
			}
			else
			{
				Owner()._hsmReLocation_Kidnap = Owner()._motionSdk->reLocationRun();
			}

			Owner()._hsmIsExecReLocation_Kidnap = true;

			// Owner()._hsmReLocation_Kidnap = 0;  // 测试
			// Owner()._hsmReLocation_Kidnap = 2;  // 测试
			// Owner()._hsmReLocation_Kidnap = 1;  // 测试

			switch (Owner()._hsmReLocation_Kidnap)
			{
			case NodeStatus::RUNNING:
				Owner()._hsmIsExecReLocation_Kidnap = false;
				break;

			case NodeStatus::SUCCESS:
				Owner()._DataDown->FlgSlam = Reposition_NOFINISHEND;
				Owner()._SensorInfo->Slamflg.state = Location_init;
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)重定位动作执行成功, 将进入(绑架)建图动作" << endl;
				LOGHSM(INFO) << "划区清扫模式: (绑架)重定位动作执行成功, 将进入(绑架)建图动作";
				break;

			case NodeStatus::FAILURE:
				Owner()._DataDown->FlgSlam = Reposition_FINISHEND;
				Owner()._SensorInfo->Slamflg.state = Location_init;
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)重定位动作执行失败, 将进入(绑架)建图动作" << endl;
				LOGHSM(INFO) << "划区清扫模式: (绑架)重定位动作执行失败, 将进入(绑架)建图动作";
				Owner()._planningSdk->CleanDataRese();
				break;
					
			default:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)重定位异常, 返回非法值" << endl;
				LOGHSM(INFO) << "划区清扫模式: (绑架)重定位异常, 返回非法值";
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "reLocation_KidnapLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "reLocation_KidnapLev6 --> Update WRONG";
		}
	}

	void rubbyStates::reLocation_KidnapLev6::OnExit()
	{
		mode_reLocat_Kidnap_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-reLocation_KidnapLev6" << endl;
	}

	/********************** Lev6-mappingAfterRelocation_KidnapLev6 ***********************/

	void rubbyStates::mappingAfterRelocation_KidnapLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-mappingAfterRelocation_KidnapLev6" << endl;
		mode_mappingAfterRelocation_Kidnap_Lev6 = args.mode_mappingAfterRelocation_Kidnap_Args_Lev6;

		if(mode_mappingAfterRelocation_Kidnap_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)建图动作开始初始化" << endl;
			LOGHSM(INFO) << "常规清扫模式: (绑架)建图动作开始初始化";

			Owner()._hsmRelocationMapInit_Kidnap = Owner()._motionSdk->firstbuildmapInit();

			// Owner()._hsmRelocationMapInit_Kidnap = true;  // 测试
			// Owner()._hsmRelocationMapInit_Kidnap = false;  // 测试

			if(Owner()._hsmRelocationMapInit_Kidnap)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)建图动作初始化成功, 开始进行(绑架)建图......" << endl;
				LOGHSM(INFO) << "常规清扫模式: (绑架)建图动作初始化成功, 开始进行(绑架)建图......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)建图动作初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "常规清扫模式: (绑架)建图动作初始化失败, 状态机阻塞";
			}
		}
		else if(mode_mappingAfterRelocation_Kidnap_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)建图动作开始初始化" << endl;
			LOGHSM(INFO) << "划区常规清扫模式: (绑架)建图动作开始初始化";

			Owner()._hsmRelocationMapInit_Kidnap = Owner()._motionSdk->firstbuildmapInit();

			// Owner()._hsmRelocationMapInit_Kidnap = true;  // 测试
			// Owner()._hsmRelocationMapInit_Kidnap = false;  // 测试

			if(Owner()._hsmRelocationMapInit_Kidnap)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)建图动作初始化成功, 开始进行(绑架)建图......" << endl;
				LOGHSM(INFO) << "划区清扫模式: (绑架)建图动作初始化成功, 开始进行(绑架)建图......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)建图动作初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "划区清扫模式: (绑架)建图动作初始化失败, 状态机阻塞";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "mappingAfterRelocation_KidnapLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "mappingAfterRelocation_KidnapLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::mappingAfterRelocation_KidnapLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::mappingAfterRelocation_KidnapLev6::Update()
	{
		if(mode_mappingAfterRelocation_Kidnap_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmRelocationMapInit_Kidnap)
		{
			Owner()._hsmRelocationMapResult_Kidnap = Owner()._motionSdk->firstbuildmapRun();
			Owner()._hsmExecRelocationMapResult_Kidnap = true;

			// Owner()._hsmRelocationMapResult_Kidnap = true; // 测试
			// Owner()._hsmRelocationMapResult_Kidnap = false; // 测试

			if(Owner()._hsmReLocation_Kidnap == NodeStatus::SUCCESS)  /*绑架重定位成功*/
			{
				if(!Owner()._hsmRelocationMapResult_Kidnap)
				{
					Owner()._hsmExecRelocationMapResult_Kidnap = false;
				}
				else
				{
					Owner()._hsmIsKidnapFlag = false;

					Owner()._hsmIsExecReLocation_Kidnap = false;
					Owner()._hsmReLocationInit_Kidnap = false;
					Owner()._hsmReLocation_Kidnap = NodeStatus::DEFAULT;
					Owner()._hsmRelocationMapInit_Kidnap = false;
					Owner()._hsmExecRelocationMapResult_Kidnap = false;
					Owner()._hsmRelocationMapResult_Kidnap = false;
					Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)建图动作完成, 开始获取清扫起始点" << endl;
					LOGHSM(INFO) << "常规清扫模式: (绑架)建图动作完成, 开始获取清扫起始点";
				}
			}
			else if(Owner()._hsmReLocation_Kidnap == NodeStatus::FAILURE)  /*绑架重定位失败*/
			{
				if(!Owner()._hsmRelocationMapResult_Kidnap)
				{
					Owner()._hsmExecRelocationMapResult_Kidnap = false;
				}
				else
				{
					Owner()._hsmIsKidnapFlag = false;
					Owner()._hsmRoutineInitFlag_ROU = false;
					*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: (绑架)建图动作完成, 重新开始常规清扫初始化" << endl;
					LOGHSM(INFO) << "常规清扫模式: (绑架)建图动作完成, 重新开始常规清扫初始化";
				}
			}
		}
		else if(mode_mappingAfterRelocation_Kidnap_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmRelocationMapInit_Kidnap)
		{
			Owner()._hsmRelocationMapResult_Kidnap = Owner()._motionSdk->firstbuildmapRun();
			Owner()._hsmExecRelocationMapResult_Kidnap = true;

			// Owner()._hsmRelocationMapResult_Kidnap = true; // 测试
			// Owner()._hsmRelocationMapResult_Kidnap = false; // 测试

			if(!Owner()._hsmRelocationMapResult_Kidnap)
			{
				Owner()._hsmExecRelocationMapResult_Kidnap = false;
			}
			else
			{
				if(Owner()._hsmReLocation_Kidnap == NodeStatus::SUCCESS)
				{
					Owner()._hsmIsKidnapFlag = false;

					Owner()._hsmIsExecReLocation_Kidnap = false;
					Owner()._hsmReLocationInit_Kidnap = false;
					Owner()._hsmReLocation_Kidnap = NodeStatus::DEFAULT;
					Owner()._hsmRelocationMapInit_Kidnap = false;
					Owner()._hsmExecRelocationMapResult_Kidnap = false;
					Owner()._hsmRelocationMapResult_Kidnap = false;
					Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)建图动作完成, 开始获取清扫起始点" << endl;
					LOGHSM(INFO) << "划区清扫模式: (绑架)建图动作完成, 开始获取清扫起始点";
				}
				else if(Owner()._hsmReLocation_Kidnap == NodeStatus::FAILURE)
				{
					Owner()._hsmIsKidnapFlag = false;
					Owner()._hsmDelimitInitFlag_DEL = false;
					*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: (绑架)建图动作失败, 重新开始划区清扫初始化" << endl;
					LOGHSM(INFO) << "划区清扫模式: (绑架)建图动作失败, 重新开始划区清扫初始化";
				}
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "mappingAfterRelocation_KidnapLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "mappingAfterRelocation_KidnapLev6 --> Update WRONG";
		}
	}

	void rubbyStates::mappingAfterRelocation_KidnapLev6::OnExit()
	{
		mode_mappingAfterRelocation_Kidnap_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-mappingAfterRelocation_KidnapLev6" << endl;
	}

	/********************** Lev6-upperPileLev6 ***********************/

	void rubbyStates::upperPileLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-upperPileLev6" << endl;
		mode_upperPile_Lev6 = args.mode_upperPile_Args_Lev6;

		if(mode_upperPile_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 上桩动作开始初始化" << endl;
			LOGHSM(INFO) << "常规清扫模式: 上桩动作开始初始化";

			Owner()._hsmIsUpperPileInit_ROU = Owner()._motionSdk->dockInit();

			// Owner()._hsmIsUpperPileInit_ROU = true;  // 测试
			// Owner()._hsmIsUpperPileInit_ROU = false;  // 测试

			if(Owner()._hsmIsUpperPileInit_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 上桩动作初始化成功, 开始进行上桩动作......" << endl;
				LOGHSM(INFO) << "常规清扫模式: 上桩动作初始化成功, 开始进行上桩动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 上桩动作初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "常规清扫模式: 上桩动作初始化失败, 状态机阻塞";
			}
		}
		else if(mode_upperPile_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 上桩动作开始初始化" << endl;
			LOGHSM(INFO) << "划区清扫模式: 上桩动作开始初始化";

			Owner()._hsmIsUpperPileInit_DEL = Owner()._motionSdk->dockInit();

			// Owner()._hsmIsUpperPileInit_DEL = true;  // 测试
			// Owner()._hsmIsUpperPileInit_DEL = false;  // 测试

			if(Owner()._hsmIsUpperPileInit_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 上桩动作初始化成功, 开始进行上桩动作......" << endl;
				LOGHSM(INFO) << "划区清扫模式: 上桩动作初始化成功, 开始进行上桩动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 上桩动作初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "划区清扫模式: 上桩动作初始化失败, 状态机阻塞";
			}
		}
		else if(mode_upperPile_Lev6 == rubbyHSM::workingModeName::WORK_RECHARGE)
		{
			*(Owner()._FILE) << s_log.logNotes() + "回充模式: 上桩动作开始初始化" << endl;
			LOGHSM(INFO) << "回充模式: 上桩动作开始初始化";

			Owner()._hsmIsUpperPileInit_REC = Owner()._motionSdk->dockInit();

			// Owner()._hsmIsUpperPileInit_REC = true;  // 测试
			// Owner()._hsmIsUpperPileInit_REC = false;  // 测试

			if(Owner()._hsmIsUpperPileInit_REC)
			{
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 上桩动作初始化成功, 开始进行上桩动作......" << endl;
				LOGHSM(INFO) << "回充模式: 上桩动作初始化成功, 开始进行上桩动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 上桩动作初始化失败, 状态机阻塞" << endl;
				LOGHSM(INFO) << "回充模式: 上桩动作初始化失败, 状态机阻塞";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "upperPileLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "upperPileLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::upperPileLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::upperPileLev6::Update()
	{
		if(mode_upperPile_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmIsUpperPileInit_ROU)
		{
			Owner()._hsmIsUpperPile_ROU = Owner()._motionSdk->dockRun();
			Owner()._hsmIsExecUpperPile_ROU = true;

			// Owner()._hsmIsUpperPile_ROU = true; // 测试
			// Owner()._hsmIsUpperPile_ROU = false; // 测试

			switch (Owner()._hsmIsUpperPile_ROU)
			{
			case NodeStatus::RUNNING:
				Owner()._hsmIsExecUpperPile_ROU = false;
				break;

			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 上桩成功, 动作完成, 进入待机状态" << endl;
				LOGHSM(INFO) << "常规清扫模式: 上桩成功, 动作完成, 进入待机状态";

				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 上桩失败, 直接进入待机状态" << endl;
				LOGHSM(INFO) << "常规清扫模式: 上桩失败, 直接进入待机状态";

				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;
			
			default:
				break;
			}
		}
		else if(mode_upperPile_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmIsUpperPileInit_DEL)
		{
			Owner()._hsmIsUpperPile_DEL = Owner()._motionSdk->dockRun();
			Owner()._hsmIsExecUpperPile_DEL = true;

			// Owner()._hsmIsUpperPile_DEL = true; // 测试
			// Owner()._hsmIsUpperPile_DEL = false; // 测试

			switch (Owner()._hsmIsUpperPile_DEL)
			{
			case NodeStatus::RUNNING:
				Owner()._hsmIsExecUpperPile_DEL = false;
				break;
			
			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 上桩成功, 动作完成, 进入待机状态" << endl;
				LOGHSM(INFO) << "划区清扫模式: 上桩成功, 动作完成, 进入待机状态";
				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 上桩失败, 直接进入待机状态" << endl;
				LOGHSM(INFO) << "划区清扫模式: 上桩失败, 直接进入待机状态";
				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;

			default:
				break;
			}
		}
		else if(mode_upperPile_Lev6 == rubbyHSM::workingModeName::WORK_RECHARGE && Owner()._hsmIsUpperPileInit_REC)
		{
			Owner()._hsmIsUpperPile_REC = Owner()._motionSdk->dockRun();
			Owner()._hsmIsExecUpperPile_REC = true;

			// Owner()._hsmIsUpperPile_REC = true; // 测试
			// Owner()._hsmIsUpperPile_REC = false; // 测试

			switch (Owner()._hsmIsUpperPile_REC)
			{
			case NodeStatus::RUNNING:
				Owner()._hsmIsExecUpperPile_REC = false;
				break;
			
			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 上桩成功, 动作完成, 进入待机状态" << endl;
				LOGHSM(INFO) << "回充模式: 上桩成功, 动作完成, 进入待机状态";
				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "回充模式: 上桩失败, 直接进入待机状态" << endl;
				LOGHSM(INFO) << "回充模式: 上桩失败, 直接进入待机状态";
				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;

			default:
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "upperPileLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "upperPileLev6 --> Update WRONG";
		}
	}

	void rubbyStates::upperPileLev6::OnExit()
	{
		mode_upperPile_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-upperPileLev6" << endl;
	}

	/********************** Lev6-underPileLev6 ***********************/

	void rubbyStates::underPileLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-upperPileLev6" << endl;
		mode_underPile_Lev6 = args.mode_underPile_Args_Lev6;

		if(mode_underPile_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "下桩动作开始初始化(ROU)" << endl;
			LOGHSM(INFO) << "下桩动作开始初始化(ROU)";

#ifdef SLAMTEST
			Owner()._hsmIsUnderPileInit_ROU = true;
#else
			Owner()._hsmIsUnderPileInit_ROU = Owner()._motionSdk->outDockInit();
#endif
			// Owner()._hsmIsUnderPileInit_ROU = true;  // 测试
			// Owner()._hsmIsUnderPileInit_ROU = false;  // 测试

			if(Owner()._hsmIsUnderPileInit_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "下桩动作初始化成功, 开始进行下桩动作......(ROU)" << endl;
				LOGHSM(INFO) << "下桩动作初始化成功, 开始进行下桩动作......(ROU)";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "下桩动作初始化失败, 状态机阻塞(ROU)" << endl;
				LOGHSM(INFO) << "下桩动作初始化失败, 状态机阻塞(ROU)";
			}
		}
		else if(mode_underPile_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			*(Owner()._FILE) << s_log.logNotes() + "下桩动作开始初始化(DEL)" << endl;
			LOGHSM(INFO) << "下桩动作开始初始化(DEL)";

#ifdef SLAMTEST
			Owner()._hsmIsUnderPileInit_ROU = true;
#else
			Owner()._hsmIsUnderPileInit_DEL = Owner()._motionSdk->outDockInit();
#endif
			// Owner()._hsmIsUnderPileInit_DEL = true;  // 测试
			// Owner()._hsmIsUnderPileInit_DEL = false;  // 测试

			if(Owner()._hsmIsUnderPileInit_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "下桩动作初始化成功, 开始进行下桩动作......(DEL)" << endl;
				LOGHSM(INFO) << "下桩动作初始化成功, 开始进行下桩动作......(DEL)";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "下桩动作初始化失败, 状态机阻塞(DEL)" << endl;
				LOGHSM(INFO) << "下桩动作初始化失败, 状态机阻塞(DEL)";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "underPileLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "underPileLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::underPileLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::underPileLev6::Update()
	{
		if(mode_underPile_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
#ifdef SLAMTEST
			Owner()._hsmIsUnderPile_ROU = true;
#else
			Owner()._hsmIsUnderPile_ROU = Owner()._motionSdk->outDockRun();
#endif

			Owner()._hsmIsExecUnderPile_ROU = true;

			// Owner()._hsmIsUnderPile_ROU = true;  // 测试
			// Owner()._hsmIsUnderPile_ROU = false;  // 测试

			if(!Owner()._hsmIsUnderPile_ROU)
			{
				Owner()._hsmIsExecUnderPile_ROU = false;
			}
			else
			{
				if(!Owner().countHSM)
				{
					*(Owner()._FILE) << s_log.logNotes() + "下桩动作完成, 开始进入重定位(ROU)" << endl;
					LOGHSM(INFO) << "下桩动作完成, 开始进入重定位(ROU)";
					Owner().countHSM = true;
				}
			}
		}
		else if(mode_underPile_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
#ifdef SLAMTEST
			Owner()._hsmIsUnderPile_ROU = true;
#else
			Owner()._hsmIsUnderPile_DEL = Owner()._motionSdk->outDockRun();
#endif
			
			Owner()._hsmIsExecUnderPile_DEL = true;

			// Owner()._hsmIsUnderPile_DEL = true;  // 测试
			// Owner()._hsmIsUnderPile_DEL = false;  // 测试

			if(!Owner()._hsmIsUnderPile_DEL)
			{
				Owner()._hsmIsExecUnderPile_DEL = false;
			}
			else
			{
				if(!Owner().count4HSM)
				{
					*(Owner()._FILE) << s_log.logNotes() + "下桩动作完成, 进入重定位初始化(DEL)" << endl;
					LOGHSM(INFO) << "下桩动作完成, 进入重定位初始化(DEL)";
					Owner().count4HSM = true;
				}
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "underPileLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "underPileLev6 --> Update WRONG";
		}
	}

	void rubbyStates::underPileLev6::OnExit()
	{
		Owner().countHSM = false;
		Owner().count4HSM = false;
		mode_underPile_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-underPileLev6" << endl;
	}

	/********************** Lev6-EMPTYLev6 ***********************/

	void rubbyStates::EMPTYLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-EMPTYLev6" << endl;
		mode_EMPTY_Lev6 = args.mode_EMPTY_Args_Lev6;
	}

	Transition rubbyStates::EMPTYLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::EMPTYLev6::Update()
	{
		if(!Owner().count3HSM)
		{
			LOGHSM(INFO) << "等待 Owner()._SensorInfo->mapload 结果......";
			Owner().count3HSM = true;
		}
	}

	void rubbyStates::EMPTYLev6::OnExit()
	{
		Owner().count3HSM = false;
		mode_EMPTY_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-EMPTYLev6" << endl;
		LOGHSM(INFO) << "Owner()._SensorInfo->mapload 加载动作完成......";
	}

	/********************** Lev6-SPEEDZEROLev6 ***********************/

	void rubbyStates::SPEEDZEROLev6::OnEnter(const Args& args)
	{
		LOGHSM(INFO) << "ENTER SPEEDZERO......";
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-SPEEDZEROLev6" << endl;
		mode_SPEEDZERO_Lev6 = args.mode_SPEEDZERO_Args_Lev6;
	}

	Transition rubbyStates::SPEEDZEROLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::SPEEDZEROLev6::Update()
	{
		Owner()._motionSdk->RunParking();
	}

	void rubbyStates::SPEEDZEROLev6::OnExit()
	{
		LOGHSM(INFO) << "EXIT SPEEDZERO......";
		mode_SPEEDZERO_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-SPEEDZEROLev6" << endl;
	}

	/********************** Lev6-searchObsPointLev6 ***********************/

	void rubbyStates::searchObsPointLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-searchObsPointLev6" << endl;
		mode_searchObsPoint_Lev6 = args.mode_searchObsPoint_Args_Lev6;

		if(mode_searchObsPoint_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			LOGHSM(INFO) << "常规清扫模式: 开始搜索障碍物点......";
			*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 开始搜索障碍物点......" << endl;
		}
		else if(mode_searchObsPoint_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			LOGHSM(INFO) << "划区清扫模式: 开始搜索障碍物点......";
			*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 开始搜索障碍物点......" << endl;
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "searchObsPointLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "searchObsPointLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::searchObsPointLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::searchObsPointLev6::Update()
	{
		// PoseStamped Point_ROU = PoseStamped();
		// PoseStamped Point_DEL = PoseStamped();
		if(mode_searchObsPoint_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			Owner()._hsmIsSearchObsPoint_ROU = Owner()._planningSdk->getFollowObsPoint(Owner()._searchNavPoint_ROU, Owner()._searchObsPoint_ROU);
			Owner()._hsmIsExecSearchObsPoint_ROU = true;

			// Owner()._hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_success;  // 测试
			// Owner()._hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;  // 测试

			switch (Owner()._hsmIsSearchObsPoint_ROU)
			{
			case SigSearchObsPoint::FindObs_success:
				Owner()._hsmIsExecNav_ROU = false;
				LOGHSM(INFO) << "常规清扫模式: 成功搜索到障碍物点，开始的导航规划到障碍物点的路径";
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 成功搜索到障碍物点，开始的导航规划到障碍物点的路径" << endl;
				break;
			case SigSearchObsPoint::FindObs_Null:
				Owner()._hsmIsExecNav_ROU = false;
				LOGHSM(INFO) << "常规清扫模式: 未搜索到障碍物点，开始搜索充电桩的位置";
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 未搜索到障碍物点，开始搜索充电桩的位置" << endl;

				// Owner()._planningSdk->getSearchChargingPoint(Point_ROU);
				LOGHSM(INFO) << "常规清扫模式: 搜索完成, 开始的导航规划到回原点的路径";
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索完成, 开始的导航规划到回原点的路径" << endl;
				break;
			case SigSearchObsPoint::FindObs_InObs:
				LOGHSM(INFO) << "常规清扫模式: 搜索障碍物点时返回FindObs_InObs, 将进入脱困模式";
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索障碍物点时返回FindObs_InObs, 将进入脱困模式" << endl;
				break;
			
			default:
				LOGHSM(INFO) << "常规清扫模式: 搜索障碍物点时返回异常值, 状态机阻塞...";
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 搜索障碍物点时返回异常值, 状态机阻塞..." << endl;
				break;
			}
		}
		else if(mode_searchObsPoint_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			Owner()._hsmIsSearchObsPoint_DEL = Owner()._planningSdk->getFollowObsPoint(Owner()._searchNavPoint_DEL, Owner()._searchObsPoint_DEL);
			Owner()._hsmIsExecSearchObsPoint_DEL = true;

			// Owner()._hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_success;  // 测试
			// Owner()._hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;  // 测试

			switch (Owner()._hsmIsSearchObsPoint_DEL)
			{
			case SigSearchObsPoint::FindObs_success:
				Owner()._hsmIsExecNav_DEL = false;
				LOGHSM(INFO) << "划区清扫模式: 成功搜索到障碍物点，开始的导航规划到障碍物点的路径";
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 成功搜索到障碍物点，开始的导航规划到障碍物点的路径" << endl;
				break;
			case SigSearchObsPoint::FindObs_Null:
				Owner()._hsmIsExecNav_DEL = false;
				LOGHSM(INFO) << "划区清扫模式: 未搜索到障碍物点，开始搜索充电桩的位置";
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 未搜索到障碍物点，开始搜索充电桩的位置" << endl;

				// Owner()._planningSdk->getSearchChargingPoint(Point_DEL);
				LOGHSM(INFO) << "划区清扫模式: 搜索完成, 开始的导航规划到回原点的路径";
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 搜索完成, 开始的导航规划到回原点的路径" << endl;
				break;
			case SigSearchObsPoint::FindObs_InObs:
				LOGHSM(INFO) << "划区清扫模式: searchObsPointLev6搜索障碍物点时返回FindObs_InObs, 进入脱困模式";
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: searchObsPointLev6搜索障碍物点时返回FindObs_InObs, 进入脱困模式" << endl;
				break;
			
			default:
				LOGHSM(INFO) << "划区清扫模式: searchObsPointLev6搜索障碍物点时返回异常值, 状态机阻塞...";
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: searchObsPointLev6搜索障碍物点时返回异常值, 状态机阻塞..." << endl;
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "searchObsPointLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "searchObsPointLev6 --> Update WRONG";
		}
	}

	void rubbyStates::searchObsPointLev6::OnExit()
	{
		mode_searchObsPoint_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-searchObsPointLev6" << endl;
	}

	/********************** Lev6-followEdgeRightLev6 ***********************/

	void rubbyStates::followEdgeRightLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-followEdgeRightLev6" << endl;
		mode_followEdgeRight_Lev6 = args.mode_followEdgeRight_Args_Lev6;

		if(mode_followEdgeRight_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			Owner()._hsmIsFollowEdgeRightInit_ROU = Owner()._motionSdk->followObsEdgeRightInit();

			// Owner()._hsmIsFollowEdgeRightInit_ROU = true;  // 测试
			// Owner()._hsmIsFollowEdgeRightInit_ROU = false;  // 测试

			if(Owner()._hsmIsFollowEdgeRightInit_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 右沿边初始化成功，开始进行右沿边动作......" << endl;
				LOGHSM(INFO) << "常规清扫模式: 右沿边初始化成功，开始进行右沿边动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 右沿边初始化失败，状态机阻塞" << endl;
				LOGHSM(INFO) << "常规清扫模式: 右沿边初始化失败，状态机阻塞";
			}
		}
		else if(mode_followEdgeRight_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			Owner()._hsmIsFollowEdgeRightInit_DEL = Owner()._motionSdk->followObsEdgeRightInit();

			// Owner()._hsmIsFollowEdgeRightInit_DEL = true;  // 测试
			// Owner()._hsmIsFollowEdgeRightInit_DEL = false;  // 测试

			if(Owner()._hsmIsFollowEdgeRightInit_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 右沿边初始化成功，开始进行右沿边动作......" << endl;
				LOGHSM(INFO) << "划区清扫模式: 右沿边初始化成功，开始进行右沿边动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 右沿边初始化失败，状态机阻塞" << endl;
				LOGHSM(INFO) << "划区清扫模式: 右沿边初始化失败，状态机阻塞";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "followEdgeRightLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "followEdgeRightLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::followEdgeRightLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::followEdgeRightLev6::Update()
	{
		if(mode_followEdgeRight_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmIsFollowEdgeRightInit_ROU)
		{
			Owner()._hsmIsFollowEdgeRight_ROU = Owner()._motionSdk->followObsEdgeRight();
			Owner()._hsmIsExecFollowEdgeRight_ROU = true;

			// Owner()._hsmIsFollowEdgeRight_ROU = 0;  // 测试
			// Owner()._hsmIsFollowEdgeRight_ROU = 1; // 测试
			// Owner()._hsmIsFollowEdgeRight_ROU = 2; // 测试

			switch (Owner()._hsmIsFollowEdgeRight_ROU)
			{
			case NodeStatus::RUNNING:
				break;
			
			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 右沿边成功，重新搜索障碍物点" << endl;
				LOGHSM(INFO) << "常规清扫模式: 右沿边成功，重新搜索障碍物点";

				Owner()._hsmIsExecSearchObsPoint_ROU = false;
				Owner()._hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;

				Owner()._hsmIsExecNav_ROU = false;
				Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;

				Owner()._hsmIsMotionNavPathInit_ROU = false;
				Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_ROU = false;

				Owner()._hsmIsObstacleAvoidInit_ROU = false;
				Owner()._hsmIsExecObstacleAvoid_ROU = false;
				Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;

				Owner()._hsmIsFollowEdgeRightInit_ROU = false;
				Owner()._hsmIsFollowEdgeRight_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeRight_ROU = false;
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 右沿边失败，开始获取左沿边起始点" << endl;
				LOGHSM(INFO) << "常规清扫模式: 右沿边失败，开始获取左沿边起始点";
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: followEdgeRightLev6 异常" << endl;
				LOGHSM(INFO) << "常规清扫模式: followEdgeRightLev6 异常";
				break;
			}
		}
		else if(mode_followEdgeRight_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmIsFollowEdgeRightInit_DEL)
		{
			Owner()._hsmIsFollowEdgeRight_DEL = Owner()._motionSdk->followObsEdgeRight();
			Owner()._hsmIsExecFollowEdgeRight_DEL = true;

			switch (Owner()._hsmIsFollowEdgeRight_DEL)
			{
			case NodeStatus::RUNNING:
				break;
			
			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 右沿边成功，重新搜索障碍物点" << endl;
				LOGHSM(INFO) << "划区清扫模式: 右沿边成功，重新搜索障碍物点";

				Owner()._hsmIsExecSearchObsPoint_DEL = false;
				Owner()._hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;

				Owner()._hsmIsExecNav_DEL = false;
				Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;

				Owner()._hsmIsMotionNavPathInit_DEL = false;
				Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_DEL = false;

				Owner()._hsmIsObstacleAvoidInit_DEL = false;
				Owner()._hsmIsExecObstacleAvoid_DEL = false;
				Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;

				Owner()._hsmIsFollowEdgeRightInit_DEL = false;
				Owner()._hsmIsFollowEdgeRight_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeRight_DEL = false;
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 右沿边失败，开始获取左沿边起始点" << endl;
				LOGHSM(INFO) << "划区清扫模式: 右沿边失败，开始获取左沿边起始点";
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: followEdgeRightLev6 异常" << endl;
				LOGHSM(INFO) << "划区清扫模式: followEdgeRightLev6 异常";
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "followEdgeRightLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "followEdgeRightLev6 --> Update WRONG";
		}
	}

	void rubbyStates::followEdgeRightLev6::OnExit()
	{
		mode_followEdgeRight_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-followEdgeRightLev6" << endl;
	}

	/********************** Lev6-getObsLeftStartLev6 ***********************/

	void rubbyStates::getObsLeftStartLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-getObsLeftStartLev6" << endl;
		mode_getObsLeftStart_Lev6 = args.mode_getObsLeftStart_Args_Lev6;
	}

	Transition rubbyStates::getObsLeftStartLev6::GetTransition() 
	{
		return NoTransition();
	}

	void rubbyStates::getObsLeftStartLev6::Update()
	{
		if(mode_getObsLeftStart_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			Owner()._hsmIsGetObsLeftStart_ROU = Owner()._motionSdk->getObsEdgeLeftStart(Owner()._obsLeftStart_ROU);
			Owner()._hsmIsExecGetObsLeftStart_ROU = true;

			// Owner()._hsmIsGetObsLeftStart_ROU = true;  // 测试
			// Owner()._hsmIsGetObsLeftStart_ROU = false; // 测试

			if(Owner()._hsmIsGetObsLeftStart_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 左沿边起始点获取成功，导航将规划到左沿边起始点的路径" << endl;
				LOGHSM(INFO) << "常规清扫模式: 左沿边起始点获取成功，导航将规划到左沿边起始点的路径";
				Owner()._hsmIsExecNav_ROU = false;
				Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 左沿边起始点获取失败，将重新搜索障碍物点" << endl;
				LOGHSM(INFO) << "常规清扫模式: 左沿边起始点获取失败，将重新搜索障碍物点";

				Owner()._hsmIsExecSearchObsPoint_ROU = false;
				Owner()._hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;

				Owner()._hsmIsExecNav_ROU = false;
				Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;

				Owner()._hsmIsMotionNavPathInit_ROU = false;
				Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_ROU = false;

				Owner()._hsmIsObstacleAvoidInit_ROU = false;
				Owner()._hsmIsExecObstacleAvoid_ROU = false;
				Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;

				Owner()._hsmIsFollowEdgeRightInit_ROU = false;
				Owner()._hsmIsFollowEdgeRight_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeRight_ROU = false;

				Owner()._hsmIsGetObsLeftStart_ROU = false;
				Owner()._hsmIsExecGetObsLeftStart_ROU = false;
				Owner()._obsLeftStart_ROU = PoseStamped();
			}
		}
		else if(mode_getObsLeftStart_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			Owner()._hsmIsGetObsLeftStart_DEL = Owner()._motionSdk->getObsEdgeLeftStart(Owner()._obsLeftStart_DEL);
			Owner()._hsmIsExecGetObsLeftStart_DEL = true;

			if(Owner()._hsmIsGetObsLeftStart_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 左沿边起始点获取成功，导航将规划到左沿边起始点的路径" << endl;
				LOGHSM(INFO) << "划区清扫模式: 左沿边起始点获取成功，导航将规划到左沿边起始点的路径";
				Owner()._hsmIsExecNav_DEL = false;
				Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 左沿边起始点获取失败，将重新搜索障碍物点" << endl;
				LOGHSM(INFO) << "划区清扫模式: 左沿边起始点获取失败，将重新搜索障碍物点";

				Owner()._hsmIsExecSearchObsPoint_DEL = false;
				Owner()._hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;

				Owner()._hsmIsExecNav_DEL = false;
				Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;

				Owner()._hsmIsMotionNavPathInit_DEL = false;
				Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_DEL = false;

				Owner()._hsmIsObstacleAvoidInit_DEL = false;
				Owner()._hsmIsExecObstacleAvoid_DEL = false;
				Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;

				Owner()._hsmIsFollowEdgeRightInit_DEL = false;
				Owner()._hsmIsFollowEdgeRight_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeRight_DEL = false;

				Owner()._hsmIsGetObsLeftStart_DEL = false;
				Owner()._hsmIsExecGetObsLeftStart_DEL = false;
				Owner()._obsLeftStart_DEL = PoseStamped();
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "getObsLeftStartLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "getObsLeftStartLev6 --> Update WRONG";
		}
	}

	void rubbyStates::getObsLeftStartLev6::OnExit()
	{
		mode_getObsLeftStart_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-getObsLeftStartLev6" << endl;
	}

	/********************** Lev6-followEdgeLeftLev6 ***********************/

	void rubbyStates::followEdgeLeftLev6::OnEnter(const Args& args)
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-followEdgeLeftLev6" << endl;
		mode_followEdgeLeft_Lev6 = args.mode_followEdgeLeft_Args_Lev6;

		if(mode_followEdgeLeft_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN)
		{
			Owner()._hsmIsFollowEdgeLeftInit_ROU = Owner()._motionSdk->followObsEdgeLeftInit();

			// Owner()._hsmIsFollowEdgeLeftInit_ROU = true;  // 测试
			// Owner()._hsmIsFollowEdgeLeftInit_ROU = false;  // 测试

			if(Owner()._hsmIsFollowEdgeLeftInit_ROU)
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 左沿边初始化成功，开始进行左沿边动作......" << endl;
				LOGHSM(INFO) << "常规清扫模式: 左沿边初始化成功，开始进行左沿边动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 左沿边初始化失败，状态机阻塞" << endl;
				LOGHSM(INFO) << "常规清扫模式: 左沿边初始化失败，状态机阻塞";
			}
		}
		else if(mode_followEdgeLeft_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN)
		{
			Owner()._hsmIsFollowEdgeLeftInit_DEL = Owner()._motionSdk->followObsEdgeLeftInit();

			// Owner()._hsmIsFollowEdgeLeftInit_DEL = true;  // 测试
			// Owner()._hsmIsFollowEdgeLeftInit_DEL = false;  // 测试

			if(Owner()._hsmIsFollowEdgeLeftInit_DEL)
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 左沿边初始化成功，开始进行左沿边动作......" << endl;
				LOGHSM(INFO) << "划区清扫模式: 左沿边初始化成功，开始进行左沿边动作......";
			}
			else
			{
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 左沿边初始化失败，状态机阻塞" << endl;
				LOGHSM(INFO) << "划区清扫模式: 左沿边初始化失败，状态机阻塞";
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "followEdgeLeftLev6 --> OnEnter WRONG" << endl;
			LOGHSM(INFO) << "followEdgeLeftLev6 --> OnEnter WRONG";
		}
	}

	Transition rubbyStates::followEdgeLeftLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::followEdgeLeftLev6::Update()
	{
		if(mode_followEdgeLeft_Lev6 == rubbyHSM::workingModeName::WORK_ROUTINECLEAN && Owner()._hsmIsFollowEdgeLeftInit_ROU)
		{
			Owner()._hsmIsFollowEdgeLeft_ROU = Owner()._motionSdk->followObsEdgeLeft();
			Owner()._hsmIsExecFollowEdgeLeft_ROU = true;

			// Owner()._hsmIsFollowEdgeLeft_ROU = 0;  // 测试
			// Owner()._hsmIsFollowEdgeLeft_ROU = 1;  // 测试
			// Owner()._hsmIsFollowEdgeLeft_ROU = 2;  // 测试

			switch (Owner()._hsmIsFollowEdgeLeft_ROU)
			{
			case NodeStatus::RUNNING:
				break;

			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 左沿边成功，重新搜索障碍物点" << endl;
				LOGHSM(INFO) << "常规清扫模式: 左沿边成功，重新搜索障碍物点";
				Owner()._hsmIsExecSearchObsPoint_ROU = false;
				Owner()._hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;

				Owner()._hsmIsExecNav_ROU = false;
				Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;

				Owner()._hsmIsMotionNavPathInit_ROU = false;
				Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_ROU = false;

				Owner()._hsmIsObstacleAvoidInit_ROU = false;
				Owner()._hsmIsExecObstacleAvoid_ROU = false;
				Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;

				Owner()._hsmIsFollowEdgeRightInit_ROU = false;
				Owner()._hsmIsFollowEdgeRight_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeRight_ROU = false;

				Owner()._hsmIsGetObsLeftStart_ROU = false;
				Owner()._hsmIsExecGetObsLeftStart_ROU = false;
				Owner()._obsLeftStart_ROU = PoseStamped();

				Owner()._hsmIsFollowEdgeLeftInit_ROU = false;
				Owner()._hsmIsFollowEdgeLeft_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeLeft_ROU = false;
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 左沿边失败，重新搜索障碍物点" << endl;
				LOGHSM(INFO) << "常规清扫模式: 左沿边失败，重新搜索障碍物点";
				Owner()._hsmIsExecSearchObsPoint_ROU = false;
				Owner()._hsmIsSearchObsPoint_ROU = SigSearchObsPoint::FindObs_Null;

				Owner()._hsmIsExecNav_ROU = false;
				Owner()._hsmIsSucceedNav_ROU = SigNav::Nav_fail;

				Owner()._hsmIsMotionNavPathInit_ROU = false;
				Owner()._hsmIsMotionNavPath_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_ROU = false;

				Owner()._hsmIsObstacleAvoidInit_ROU = false;
				Owner()._hsmIsExecObstacleAvoid_ROU = false;
				Owner()._hsmIsObstacleAvoid_ROU = NodeStatus::DEFAULT;

				Owner()._hsmIsFollowEdgeRightInit_ROU = false;
				Owner()._hsmIsFollowEdgeRight_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeRight_ROU = false;

				Owner()._hsmIsGetObsLeftStart_ROU = false;
				Owner()._hsmIsExecGetObsLeftStart_ROU = false;
				Owner()._obsLeftStart_ROU = PoseStamped();

				Owner()._hsmIsFollowEdgeLeftInit_ROU = false;
				Owner()._hsmIsFollowEdgeLeft_ROU = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeLeft_ROU = false;
				break;

			default:
				*(Owner()._FILE) << s_log.logNotes() + "常规清扫模式: 左沿边状态返回值异常" << endl;
				LOGHSM(INFO) << "常规清扫模式: 左沿边状态返回值异常";
				break;
			}
		}
		else if(mode_followEdgeLeft_Lev6 == rubbyHSM::workingModeName::WORK_DELIMITCLEAN && Owner()._hsmIsFollowEdgeLeftInit_DEL)
		{
			Owner()._hsmIsFollowEdgeLeft_DEL = Owner()._motionSdk->followObsEdgeLeft();
			Owner()._hsmIsExecFollowEdgeLeft_DEL = true;

			switch (Owner()._hsmIsFollowEdgeLeft_DEL)
			{
			case NodeStatus::RUNNING:
				break;

			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 左沿边成功，重新搜索障碍物点" << endl;
				LOGHSM(INFO) << "划区清扫模式: 左沿边成功，重新搜索障碍物点";
				Owner()._hsmIsExecSearchObsPoint_DEL = false;
				Owner()._hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;

				Owner()._hsmIsExecNav_DEL = false;
				Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;

				Owner()._hsmIsMotionNavPathInit_DEL = false;
				Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_DEL = false;

				Owner()._hsmIsObstacleAvoidInit_DEL = false;
				Owner()._hsmIsExecObstacleAvoid_DEL = false;
				Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;

				Owner()._hsmIsFollowEdgeRightInit_DEL = false;
				Owner()._hsmIsFollowEdgeRight_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeRight_DEL = false;

				Owner()._hsmIsGetObsLeftStart_DEL = false;
				Owner()._hsmIsExecGetObsLeftStart_DEL = false;
				Owner()._obsLeftStart_DEL = PoseStamped();

				Owner()._hsmIsFollowEdgeLeftInit_DEL = false;
				Owner()._hsmIsFollowEdgeLeft_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeLeft_DEL = false;
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 左沿边失败，重新搜索障碍物点" << endl;
				LOGHSM(INFO) << "划区清扫模式: 左沿边失败，重新搜索障碍物点";
				Owner()._hsmIsExecSearchObsPoint_DEL = false;
				Owner()._hsmIsSearchObsPoint_DEL = SigSearchObsPoint::FindObs_Null;

				Owner()._hsmIsExecNav_DEL = false;
				Owner()._hsmIsSucceedNav_DEL = SigNav::Nav_fail;

				Owner()._hsmIsMotionNavPathInit_DEL = false;
				Owner()._hsmIsMotionNavPath_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecMotionNavPath_DEL = false;

				Owner()._hsmIsObstacleAvoidInit_DEL = false;
				Owner()._hsmIsExecObstacleAvoid_DEL = false;
				Owner()._hsmIsObstacleAvoid_DEL = NodeStatus::DEFAULT;

				Owner()._hsmIsFollowEdgeRightInit_DEL = false;
				Owner()._hsmIsFollowEdgeRight_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeRight_DEL = false;

				Owner()._hsmIsGetObsLeftStart_DEL = false;
				Owner()._hsmIsExecGetObsLeftStart_DEL = false;
				Owner()._obsLeftStart_DEL = PoseStamped();

				Owner()._hsmIsFollowEdgeLeftInit_DEL = false;
				Owner()._hsmIsFollowEdgeLeft_DEL = NodeStatus::DEFAULT;
				Owner()._hsmIsExecFollowEdgeLeft_DEL = false;
				break;

			default:
				*(Owner()._FILE) << s_log.logNotes() + "划区清扫模式: 左沿边状态返回值异常" << endl;
				LOGHSM(INFO) << "划区清扫模式: 左沿边状态返回值异常";
				break;
			}
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "followEdgeLeftLev6 --> Update WRONG" << endl;
			LOGHSM(INFO) << "followEdgeLeftLev6 --> Update WRONG";
		}
	}

	void rubbyStates::followEdgeLeftLev6::OnExit()
	{
		mode_followEdgeLeft_Lev6 = rubbyHSM::workingModeName::NO_WORK;
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-followEdgeLeftLev6" << endl;
	}

	/********************** Lev6-recoveryForbidenLev6 ***********************/

	void rubbyStates::recoveryForbidenLev6::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-recoveryForbidenLev6" << endl;

		*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 规划层判定当前属于禁区脱困, 脱困动作开始初始化" << endl;
		LOGHSM(INFO) << "脱困模式: 规划层判定当前属于禁区脱困, 脱困动作开始初始化";

		Owner()._hsmIsRecoveryInit = Owner()._motionSdk->recoveryInit(RecoveryStyle::Recovery_Forbiden);

		if(Owner()._hsmIsRecoveryInit)
		{
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 禁区脱困模式初始化成功, 开始进行脱困动作..." << endl;
			LOGHSM(INFO) << "脱困模式: 禁区脱困模式初始化成功, 开始进行脱困动作...";
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 禁区脱困模式初始化失败, 直接进入待机状态" << endl;
			LOGHSM(INFO) << "脱困模式: 禁区脱困模式初始化失败, 直接进入待机状态";
		}
	}

	Transition rubbyStates::recoveryForbidenLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::recoveryForbidenLev6::Update()
	{
		if(Owner()._hsmIsRecoveryInit)
		{
			Owner()._hsmIsRecovery = Owner()._motionSdk->recoveryRun(RecoveryStyle::Recovery_Forbiden);
			Owner()._hsmIsExecRecovery = true;

			switch (Owner()._hsmIsRecovery)
			{
			case NodeStatus::RUNNING:
				break;

			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 禁区脱困动作失败, 直接进入待机状态" << endl;
				LOGHSM(INFO) << "脱困模式: 禁区脱困动作失败, 直接进入待机状态";

				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;
			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 禁区脱困动作成功, 返回值错误, 直接进入待机状态" << endl;
				LOGHSM(INFO) << "脱困模式: 禁区脱困动作成功, 返回值错误, 直接进入待机状态";
				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 禁区脱困动作返回异常值, 状态机阻塞" << endl;
				LOGHSM(INFO) << "脱困模式: 禁区脱困动作返回异常值, 状态机阻塞";
				break;
			}
		}
	}

	void rubbyStates::recoveryForbidenLev6::OnExit()
	{
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-recoveryForbidenLev6" << endl;
	}

	/********************** Lev6-recoveryObsLev6 ***********************/

	void rubbyStates::recoveryObsLev6::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-recoveryObsLev6" << endl;

		*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 规划层判定当前属于障碍物膨胀层脱困, 脱困动作开始初始化" << endl;
		LOGHSM(INFO) << "脱困模式: 规划层判定当前属于障碍物膨胀层脱困, 脱困动作开始初始化";

		Owner()._hsmIsRecoveryInit = Owner()._motionSdk->recoveryInit(RecoveryStyle::Recovery_Obs);

		if(Owner()._hsmIsRecoveryInit)
		{
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 障碍物膨胀层脱困模式初始化成功, 开始进行脱困动作..." << endl;
			LOGHSM(INFO) << "脱困模式: 障碍物膨胀层脱困模式初始化成功, 开始进行脱困动作...";
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 障碍物膨胀层脱困模式初始化失败, 直接进入待机状态" << endl;
			LOGHSM(INFO) << "脱困模式: 障碍物膨胀层脱困模式初始化失败, 直接进入待机状态";
		}
	}

	Transition rubbyStates::recoveryObsLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::recoveryObsLev6::Update()
	{
		if(Owner()._hsmIsRecoveryInit)
		{
			Owner()._hsmIsRecovery = Owner()._motionSdk->recoveryRun(RecoveryStyle::Recovery_Obs);
			Owner()._hsmIsExecRecovery = true;

			switch (Owner()._hsmIsRecovery)
			{
			case NodeStatus::RUNNING:
				break;
			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 障碍物膨胀层脱困动作失败, 直接进入待机状态" << endl;
				LOGHSM(INFO) << "脱困模式: 障碍物膨胀层脱困动作失败, 直接进入待机状态";

				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;
			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 障碍物膨胀层脱困动作成功, 返回原清扫模式, 重新搜索清扫起始点" << endl;
				LOGHSM(INFO) << "脱困模式: 障碍物膨胀层脱困动作成功, 返回原清扫模式, 重新搜索清扫起始点";

				Owner()._hsmIsRecoveryInit = false;    // 初始化参数
				Owner()._hsmIsRecovery = NodeStatus::DEFAULT;    // 初始化参数
				Owner()._SensorInfo->recoveryStyle = RecoveryStyle::Recovery_INIT;

				if(Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_ROUTINECLEAN)
				{
					Owner()._hsmRoutineInitFlag_ROU = true;
					Owner()._hsmIsRoutineInit_ROU = true;
					Owner()._hsmIsExecGetCleanBox_ROU = true;
					Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
				}

				if(Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_DELIMITCLEAN)
				{
					Owner()._hsmDelimitInitFlag_DEL = true;
					Owner()._hsmIsDelimitInit_DEL = true;
					Owner()._hsmIsExecGetCleanBox_DEL = true;
					Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
				}
				
				if(Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_RECHARGE)
				{
					Owner()._hsmIsExecRecovery = false;
				}
				break;

			default:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 障碍物膨胀层脱困动作返回异常值, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "脱困模式: 障碍物膨胀层脱困动作返回异常值, 状态机阻塞...";
				break;
			}			
		}
	}

	void rubbyStates::recoveryObsLev6::OnExit()
	{
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-recoveryObsLev6" << endl;
	}

	/********************** Lev6-recoverySurroundedLev6 ***********************/

	void rubbyStates::recoverySurroundedLev6::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-recoverySurroundedLev6" << endl;

		*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 规划层判定当前属于包络区域脱困, 脱困动作开始初始化" << endl;
		LOGHSM(INFO) << "脱困模式: 规划层判定当前属于包络区域脱困, 脱困动作开始初始化";

		Owner()._hsmIsRecoveryInit = Owner()._motionSdk->recoveryInit(RecoveryStyle::Recovery_Surrounded);

		if(Owner()._hsmIsRecoveryInit)
		{
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 包络区域脱困模式初始化成功, 开始进行脱困动作..." << endl;
			LOGHSM(INFO) << "脱困模式: 包络区域脱困模式初始化成功, 开始进行脱困动作...";
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 包络区域脱困模式初始化失败, 直接进入待机状态" << endl;
			LOGHSM(INFO) << "脱困模式: 包络区域脱困模式初始化失败, 直接进入待机状态";
		}
	}

	Transition rubbyStates::recoverySurroundedLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::recoverySurroundedLev6::Update()
	{
		if(Owner()._hsmIsRecoveryInit)
		{
			Owner()._hsmIsRecovery = Owner()._motionSdk->recoveryRun(RecoveryStyle::Recovery_Surrounded);
			Owner()._hsmIsExecRecovery = true;

			switch (Owner()._hsmIsRecovery)
			{
			case NodeStatus::RUNNING:
				break;
			case NodeStatus::SUCCESS:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 障碍物包络区域脱困动作成功, 返回原清扫模式, 重新搜索清扫起始点" << endl;
				LOGHSM(INFO) << "脱困模式: 障碍物包络区域脱困动作成功, 返回原清扫模式, 重新搜索清扫起始点";

				Owner()._hsmIsRecoveryInit = false;    // 初始化参数
				Owner()._hsmIsRecovery = NodeStatus::DEFAULT;    // 初始化参数
				Owner()._SensorInfo->recoveryStyle = RecoveryStyle::Recovery_INIT;
				
				if(Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_ROUTINECLEAN)
				{
					Owner()._hsmRoutineInitFlag_ROU = true;
					Owner()._hsmIsRoutineInit_ROU = true;
					Owner()._hsmIsExecGetCleanBox_ROU = true;
					Owner()._hsmIsExecGetCleanStartPoint_ROU = false;
				}

				if(Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_DELIMITCLEAN)
				{
					Owner()._hsmDelimitInitFlag_DEL = true;
					Owner()._hsmIsDelimitInit_DEL = true;
					Owner()._hsmIsExecGetCleanBox_DEL = true;
					Owner()._hsmIsExecGetCleanStartPoint_DEL = false;
				}
				
				if(Owner()._SensorInfo->workMode.workMode == workingMode_::WORK_RECHARGE)
				{
					Owner()._hsmIsExecRecovery = false;
				}
				break;
			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 包络区域脱困动作失败, 直接进入待机状态" << endl;
				LOGHSM(INFO) << "脱困模式: 包络区域脱困动作失败, 直接进入待机状态";

				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 包络区域脱困动作返回异常值, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "脱困模式: 包络区域脱困动作返回异常值, 状态机阻塞...";
				break;
			}
		}
	}

	void rubbyStates::recoverySurroundedLev6::OnExit()
	{
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-recoverySurroundedLev6" << endl;
	}

	/********************** Lev6-recoveryPhysicsLev6 ***********************/

	void rubbyStates::recoveryPhysicsLev6::OnEnter()
	{
		*(Owner()._FILE) << s_log.logNotes() + "进入 Lev6-recoveryPhysicsLev6" << endl;

		*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 规划层判定当前属于物理被困, 脱困动作开始初始化" << endl;
		LOGHSM(INFO) << "脱困模式: 规划层判定当前属于物理被困, 脱困动作开始初始化";

		Owner()._hsmIsRecoveryInit = Owner()._motionSdk->recoveryInit(RecoveryStyle::Recovery_Physics);

		if(Owner()._hsmIsRecoveryInit)
		{
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 物理被困模式初始化成功, 开始进行脱困动作..." << endl;
			LOGHSM(INFO) << "脱困模式: 物理被困模式初始化成功, 开始进行脱困动作...";
		}
		else
		{
			*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 物理被困模式初始化失败, 直接进入待机状态" << endl;
			LOGHSM(INFO) << "脱困模式: 物理被困模式初始化失败, 直接进入待机状态";
		}
	}

	Transition rubbyStates::recoveryPhysicsLev6::GetTransition()
	{
		return NoTransition();
	}

	void rubbyStates::recoveryPhysicsLev6::Update()
	{
		if(Owner()._hsmIsRecoveryInit)
		{
			Owner()._hsmIsRecovery = Owner()._motionSdk->recoveryRun(RecoveryStyle::Recovery_Physics);
			Owner()._hsmIsExecRecovery = true;

			switch (Owner()._hsmIsRecovery)
			{
			case NodeStatus::RUNNING:
				break;
			case NodeStatus::SUCCESS:
				break;
			case NodeStatus::FAILURE:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 物理被困脱困动作失败, 直接进入待机状态" << endl;
				LOGHSM(INFO) << "脱困模式: 物理被困脱困动作失败, 直接进入待机状态";

				Owner()._planningSdk->completeTask();
				Owner()._motionSdk->completeTask();

				Owner()._hsmIsEnd = true;
				Owner()._hsmIsNeedVoice = false;
				break;
			
			default:
				*(Owner()._FILE) << s_log.logNotes() + "脱困模式: 物理被困脱困动作返回异常值, 状态机阻塞..." << endl;
				LOGHSM(INFO) << "脱困模式: 物理被困脱困动作返回异常值, 状态机阻塞...";
				break;
			}
		}
	}

	void rubbyStates::recoveryPhysicsLev6::OnExit()
	{
		*(Owner()._FILE) << s_log.logNotes() + "退出 Lev6-recoveryPhysicsLev6" << endl;
	}
}