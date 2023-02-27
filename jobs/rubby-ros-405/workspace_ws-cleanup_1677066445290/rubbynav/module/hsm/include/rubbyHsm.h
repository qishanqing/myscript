#ifndef RUBBY_CONTROL_CORE_
#define RUBBY_CONTROL_CORE_

#include "CommonSdk.h"
#include "motionsdk.h"
#include "planningsdk.h"
#include "hsm.h"
#include <fstream>
#include "Log.h"

using namespace hsm;

namespace rubbyHSM
{
	enum workingModeName
	{
		WORK_INIT = 0,
		WORK_ROUTINECLEAN,   // 常规清扫
		WORK_DELIMITCLEAN,    // 划区清扫
		WORK_AUTOPARTITIONSCLEAN, /*自动分区清扫模式*/
		WORK_RECHARGE,           //  回充模式
		WORK_RECOVERY,            //  脱困模式
		WORK_PAUSE,        /*暂停*/
		WORK_RESUME,       /*取消暂停*/
		NO_WORK                           // 非工作模式
	};

	class rubbySTATICLOG
	{
	public:
		bool countHSM;
		bool count2HSM;
		bool count3HSM;
		bool count4HSM;
	};

	class rubbyArgs
	{
	public:
		PoseStamped _startPoseStamped_ROU;  // 重定位动作完成之后的机器人位姿，即初始点
		BoxInfo _BoxInfo_ROU;   // 清扫框
		vector<PoseStamped> _navPath_ROU;     // 导航路径
		vector<PoseStamped> _bowPath_ROU;    // 弓字形路径
		vector<PoseStamped> _slamPath_ROU;   // 合并后的SLAM路径
		PoseStamped _PoseStamped_ROU;           // 清扫起始点
		PoseStamped _start_ROU;                            // 导航起始点
		PoseStamped _end_ROU;                             // 导航目标点 
		PoseStamped _poseRecord_ROU;              // 搜索清扫起始点返回1之前记录的机器人位姿
		PoseStamped _slamPose_ROU;                  // 获取SLAM路径之时需要返回的端点
		PoseStamped _searchNavPoint_ROU;       // 搜索障碍物点时的导航点
		PoseStamped _searchObsPoint_ROU;       // 搜索障碍物点时的障碍物点
		PoseStamped _obsLeftStart_ROU;             // 左沿边起始点

		PoseStamped _startPoseStamped_DEL;  // 重定位动作完成之后的机器人位姿，即初始点
		BoxInfo _BoxInfo_DEL;   // 清扫框
		vector<PoseStamped> _navPath_DEL;     // 导航路径
		vector<PoseStamped> _bowPath_DEL;    // 弓字形路径
		vector<PoseStamped> _slamPath_DEL;   // 合并后的SLAM路径
		PoseStamped _PoseStamped_DEL;           // 清扫起始点
		PoseStamped _start_DEL;                            // 导航起始点
		PoseStamped _end_DEL;                             // 导航目标点 
		PoseStamped _poseRecord_DEL;              // 搜索清扫起始点返回1之前记录的机器人位姿
		PoseStamped _slamPose_DEL;                  // 获取SLAM路径之时需要返回的端点
		PoseStamped _searchNavPoint_DEL;       // 搜索障碍物点时的导航点
		PoseStamped _searchObsPoint_DEL;       // 搜索障碍物点时的障碍物点
		PoseStamped _obsLeftStart_DEL;             // 左沿边起始点

		PoseStamped _startPoseStamped_REC;  // 重定位动作完成之后的机器人位姿，即初始点
		vector<PoseStamped> _navPath_REC;     // 导航路径
		PoseStamped _start_REC;                            // 导航起始点
		PoseStamped _end_REC;                             // 导航目标点 
	};

	class rubbyHSMArgs
	{
	public:
		rubbyHSM::workingModeName HSMMODE;      // 模式记录

		bool _hsmIsInPileRecharge;          // 是否在桩上回充，默认false;
		bool _hsmTellSLAMStartFlag;       // 与SLAM交互，非INIT状态下告知SLAM NAV start，默认false
		bool _hsmIsStartRelocation;        // 机器人是否监听到SLAM重定位开始状态，默认false

		bool _hsmIsEnd;                              // 是否准备结束工作进入语音播报和待机状态，默认false
		bool _hsmIsNeedVoice;                 // 是否需要需要语音播报，默认true

		bool _hsmIsMergeMapEnd;          // 是否监听到SLAM发送的重定位成功后发出的新位姿标志，默认false
		bool _hsmIsStartPile;                    // 判断是否是从桩上启动，默认false

		/**********绑架与非绑架重定位**********/
		bool _hsmIsKidnapFlag;               //是否执行重定位(绑架)，默认false
		bool _hsmIsKidnapUp;                  // 机器人是否被抬起，默认false

		bool _hsmReLocationInit_NoKidnap;            // 非绑架重定位是否初始化，默认false
		int _hsmReLocation_NoKidnap;                      // 非绑架重定位返回的结果，默认DEFAULT
		bool _hsmIsExecReLocation_NoKidnap;       // 非绑架是否执行了重定位这个动作，默认false

		bool _hsmRelocationMapInit_NoKidnap;                 // 非绑架建图是否初始化，默认false
		bool _hsmExecRelocationMapResult_NoKidnap;   // 是否执行了非绑架建图动作，默认false
		bool _hsmRelocationMapResult_NoKidnap;           // 非绑架建图动作结果，默认false

		bool _hsmReLocationInit_Kidnap;            // 绑架重定位是否初始化，默认false
		int _hsmReLocation_Kidnap;                      // 绑架重定位返回的结果，默认DEFAULT
		bool _hsmIsExecReLocation_Kidnap;       // 绑架是否执行了重定位这个动作，默认false

		bool _hsmRelocationMapInit_Kidnap;                 // 绑架建图是否初始化，默认false
		bool _hsmExecRelocationMapResult_Kidnap;   // 是否执行了绑架建图动作，默认false
		bool _hsmRelocationMapResult_Kidnap;           // 绑架建图动作结果，默认false

		/**********初始化、获取清扫边界、合并SLAM路径**********/
		bool _hsmIsRoutineInit_ROU;                                 // 是否初始化，默认false
		bool _hsmRoutineInitFlag_ROU;                            // 初始化标志位，默认false
		bool _hsmIsExecGetCleanBox_ROU;                     // 是否执行了获取清扫边界功能，默认false
		bool _hsmIsExecGetAndMergeSlamPath_ROU;  // 是否执行过获取与合并SLAM路径这堆动作，默认false

		bool _hsmIsDelimitInit_DEL;                                  // 是否初始化，默认false
		bool _hsmDelimitInitFlag_DEL;                             // 初始化标志位，默认false
		bool _hsmIsExecGetCleanBox_DEL;                     // 是否执行了获取清扫边界功能，默认false
		bool _hsmIsExecGetAndMergeSlamPath_DEL;  // 是否执行过获取与合并SLAM路径这堆动作，默认false

		bool _hsmIsExecMotionAndPlanningInit;    // motion和planning是否初始化过，默认false

		/**************常规清扫每次搜索清扫起始点之前要初始化以下参数********************/
		bool _hsmIsExecGetCleanStartPoint_ROU;        // 获取清扫起始点功能是否执行，默认false
		int _hsmIsGetCleanStartPoint_ROU;                    // 获得清扫起始点的结果，默认FindUncleanPoint_Null

		bool _hsmIsExecNav_ROU;                // 导航是否执行，默认false
		int _hsmIsSucceedNav_ROU;         // 导航是否成功，默认Nav_fail

		bool _hsmIsMotionNavPathInit_ROU;       // motion追踪导航路径是否初始化，，默认false
		int _hsmIsMotionNavPath_ROU;                 // motion追踪导航路径的结果，默认DEFAULT
		bool _hsmIsExecMotionNavPath_ROU;     // motion追踪导航动作是否执行，默认false

		bool _hsmIsGetBowPath_ROU;                // 是否获得了弓字形路径，默认false
		bool _hsmIsExecGetBowPath_ROU;        // 获得弓字形路径动作是否执行，默认false
		bool _hsmIsGetBowShapeInit_ROU;       // 获得弓字形路径是否初始化，默认false

		bool _hsmIsMotionBowInit_ROU;            // motion追踪弓字形路径是否初始化，默认false
		int _hsmIsMotionBow_ROU;                      // motion追踪弓字形路径返回的结果，默认DEFAULT
		bool _hsmIsExecMotionBow_ROU;          // motion追踪弓字形动作是否执行，默认false

		bool _hsmIsGetSlamPath_ROU;               // 获取SLAM路径返回的结果，默认false
		bool _hsmIsExecGetSlamPath_ROU;       // 获取SLAM路径动作是否执行，默认false

		bool _hsmIsMergeSlamPath_ROU;           // 合并SLAM与导航路径返回的结果，默认false
		bool _hsmIsExecMergeSlamPath_ROU;   // 合并SLAM与导航路径动作是否执行，默认false

		bool _hsmIsObstacleAvoidInit_ROU;      // 避障动作是否初始化， 默认false
		bool _hsmIsExecObstacleAvoid_ROU;    // 避障动作是否执行， 默认false
		int _hsmIsObstacleAvoid_ROU;                // 避障动作的结果，默认DEFAULT

		bool _hsmIsUpperPileInit_ROU;       // 上桩动作是否初始化， 默认false
		bool _hsmIsExecUpperPile_ROU;     // 上桩动作是否执行， 默认false
		int _hsmIsUpperPile_ROU;                 // 上桩动作结果， 默认DEFAULT

		bool _hsmIsUnderPileInit_ROU;       // 下桩动作是否初始化， 默认false
		bool _hsmIsExecUnderPile_ROU;     // 下桩这个动作是否执行， 默认false
		bool _hsmIsUnderPile_ROU;             // 下桩动作结果， 默认false

		bool _hsmIsExecSearchObsPoint_ROU;  // 搜索障碍物点动作是否执行， 默认false
		int _hsmIsSearchObsPoint_ROU;          // 搜索障碍物点的结果，默认FindObs_Null

		bool _hsmIsFollowEdgeRightInit_ROU;   // 右沿边是否初始化，默认false
		bool _hsmIsExecFollowEdgeRight_ROU; // 右沿边动作是否执行， 默认false
		int _hsmIsFollowEdgeRight_ROU;             // 右沿边状态结果，默认DEFAULT

		bool _hsmIsExecGetObsLeftStart_ROU;  // 获取左沿边起始点是否执行，默认fasle
		bool _hsmIsGetObsLeftStart_ROU;          //  左沿边起始点是否获取成功，默认fasle

		bool _hsmIsFollowEdgeLeftInit_ROU;   // 左沿边是否初始化，默认false
		bool _hsmIsExecFollowEdgeLeft_ROU; // 左沿边动作是否执行， 默认false
		int _hsmIsFollowEdgeLeft_ROU;             // 左沿边状态结果，默认DEFAULT

		/**************划区清扫每次搜索清扫起始点之前要初始化以下参数********************/
		bool _hsmIsExecGetCleanStartPoint_DEL;   // 是否执行了获取清扫起始点功能，默认false
		int _hsmIsGetCleanStartPoint_DEL;     // 获得清扫起始点的结果，默认FindUncleanPointDelimit_Null

		bool _hsmIsExecNav_DEL;                      // 是否执行了导航动作，默认false
		int _hsmIsSucceedNav_DEL;               // 导航是否成功，默认Nav_fail

		bool _hsmIsMotionNavPathInit_DEL;   // motion追踪导航路径是否初始化，，默认false
		int _hsmIsMotionNavPath_DEL;             // motion追踪导航路径的结果，默认DEFAULT
		bool _hsmIsExecMotionNavPath_DEL;  // 是否执行了motion追踪导航动作，默认false

		bool _hsmIsGetBowPath_DEL;                // 是否获得了弓字形路径，默认false
		bool _hsmIsExecGetBowPath_DEL;        // 是否执行了获得弓字形路径这个动作，默认false
		bool _hsmIsGetBowShapeInit_DEL;       // 获得弓字形路径之前是否初始化，默认false

		bool _hsmIsMotionBowInit_DEL;            // motion追踪弓字形路径是否初始化，默认false
		int _hsmIsMotionBow_DEL;                      // motion追踪弓字形路径返回的结果，默认DEFAULT
		bool _hsmIsExecMotionBow_DEL;          // 是否执行了motion追踪弓字形这个动作，默认false

		bool _hsmIsGetSlamPath_DEL;               // 获取SLAM路径返回的结果，默认false
		bool _hsmIsExecGetSlamPath_DEL;       // 是否执行了获取SLAM路径这个动作，默认false

		bool _hsmIsMergeSlamPath_DEL;           // 合并SLAM与导航路径返回的结果，默认false
		bool _hsmIsExecMergeSlamPath_DEL;   // 是否执行了合并SLAM与导航路径这个动作，默认false

		bool _hsmIsObstacleAvoidInit_DEL;      // 避障动作是否初始化， 默认false
		bool _hsmIsExecObstacleAvoid_DEL;    // 是否执行了避障这个动作， 默认false
		int _hsmIsObstacleAvoid_DEL;                // 避障动作的结果，默认DEFAULT

		bool _hsmIsUpperPileInit_DEL;    // 上桩动作是否初始化， 默认false
		bool _hsmIsExecUpperPile_DEL;   // 是否执行了上桩这个动作， 默认false
		int _hsmIsUpperPile_DEL;          // 上桩动作结果， 默认DEFAULT

		bool _hsmIsUnderPileInit_DEL;    // 下桩动作是否初始化， 默认false
		bool _hsmIsExecUnderPile_DEL;   // 是否执行了下桩这个动作， 默认false
		bool _hsmIsUnderPile_DEL;      // 下桩动作结果， 默认false

		bool _hsmIsExecSearchObsPoint_DEL;  // 搜索障碍物点动作是否执行， 默认false
		int _hsmIsSearchObsPoint_DEL;          // 搜索障碍物点的结果，默认FindObs_Null

		bool _hsmIsFollowEdgeRightInit_DEL;   // 右沿边是否初始化，默认false
		bool _hsmIsExecFollowEdgeRight_DEL; // 右沿边动作是否执行， 默认false
		int _hsmIsFollowEdgeRight_DEL;             // 右沿边状态结果，默认DEFAULT

		bool _hsmIsExecGetObsLeftStart_DEL;  // 获取左沿边起始点是否执行，默认fasle
		bool _hsmIsGetObsLeftStart_DEL;          //  左沿边起始点是否获取成功, 默认fasle

		bool _hsmIsFollowEdgeLeftInit_DEL;   // 左沿边是否初始化，默认false
		bool _hsmIsExecFollowEdgeLeft_DEL; // 左沿边动作是否执行， 默认false
		int _hsmIsFollowEdgeLeft_DEL;             // 左沿边状态结果，默认DEFAULT

		/**************回充模式每次搜索清扫起始点之前要初始化以下参数********************/
		bool _hsmIsExecNav_REC;                      // 是否执行了导航动作，默认false
		int _hsmIsSucceedNav_REC;               // 导航是否成功，默认Nav_fail

		bool _hsmIsMotionNavPathInit_REC;   // motion追踪导航路径是否初始化，，默认false
		int _hsmIsMotionNavPath_REC;             // motion追踪导航路径的结果，默认DEFAULT
		bool _hsmIsExecMotionNavPath_REC;  // 是否执行了motion追踪导航动作，默认false

		bool _hsmIsObstacleAvoidInit_REC;      // 避障动作是否初始化， 默认false
		bool _hsmIsExecObstacleAvoid_REC;    // 是否执行了避障这个动作， 默认false
		int _hsmIsObstacleAvoid_REC;                // 避障动作的结果，默认DEFAULT

		bool _hsmIsUpperPileInit_REC;    // 上桩动作是否初始化， 默认false
		bool _hsmIsExecUpperPile_REC;   // 是否执行了上桩这个动作， 默认false
		int _hsmIsUpperPile_REC;          // 上桩动作结果， 默认DEFAULT

		/**************脱困模式每次搜索清扫起始点之前要初始化以下参数********************/
		bool _hsmIsRecoveryInit;    // 脱困是否初始化，默认false
		bool _hsmIsExecRecovery;   // 是否执行脱困这个动作，默认false
		NodeStatus _hsmIsRecovery;  // 脱困动作结果，默认DEFAULT

	};

	class rubbyOwner : public rubbyArgs, public rubbyHSMArgs, public rubbySTATICLOG
	{
	private:
		StateMachine rubbyStateMachine;
		SensorInfo* _SensorInfo;
		DataDown* _DataDown;
		workingModeName _hsmMode;
		CommonSdk* _commonSdk = nullptr;
		MotionSdk* _motionSdk = nullptr;
		bool _hsmIsMotionInit;
		PlanningSdk* _planningSdk = nullptr;
		bool _hsmIsPlanningInit;
		std::fstream* _FILE;
		int boxflag;

	public:

		rubbyOwner(SensorInfo* _SensorInfo, DataDown* _DataDown);
		~rubbyOwner();

		/**
       * @brief  更新状态机
	   * @param FILE_ log文件
       */
		void updateStateMachine(std::fstream& FILE_);

		/**
       * @brief  更新地图
       */
		void updateMap();

		/**
       * @brief  待机前初始化所有参数
       */
		void resetAllParams();

		/**
       * @brief  常规清扫模式获取清扫点之前重新初始化参数
       */
		void resetParams_ROU();

		/**
       * @brief  划区清扫模式获取清扫点之前重新初始化参数
       */
		void resetParams_DEL();

		/**
       * @brief  进入回充模式之前重新初始化参数
       */
		void resetParams_REC();

		/**
       * @brief  进入脱困模式之前重新初始化参数
       */
		void resetParams_RECOVERY();

		/**
       * @brief  析构所有指针变量
       */
		void destoryHsm();

	public:
		void setRotateAngle(double angle);
		bool setAutoSegment();
		bool setAutoSegmentOperation();
	public:
		friend struct rubbyStates;
	};

	struct rubbyStates
	{
		struct BaseState : StateWithOwner<rubbyOwner>
		{
		};

	/********************** Lev1 ***********************/

		struct interactive : BaseState
		{
			workingModeName mode_interactive_Lev1;
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		struct TRANS : BaseState
		{
			workingModeName mode_TRANS_Lev1;
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void OnExit();
		};

	/********************** Lev2 ***********************/

		struct standByLev2 : BaseState
		{
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};
		
		struct informationPubLev2 : BaseState
		{
			workingModeName mode_infor_Lev2 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_infor_Args_Lev2(mode_){}
				workingModeName mode_infor_Args_Lev2;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		struct selectStateLev2 : BaseState
		{
			workingModeName mode_selec_Lev2 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_selec_Args_Lev2(mode_){}
				workingModeName mode_selec_Args_Lev2;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		struct workingModeLev2 : BaseState
		{
			workingModeName mode_work_Lev2 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_work_Args_Lev2(mode_){}
				workingModeName mode_work_Args_Lev2;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

	/********************** Lev3 ***********************/

		struct informationPubLev3 : BaseState
		{
			workingModeName mode_infor_Lev3 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_infor_Args_Lev3(mode_){}
				workingModeName mode_infor_Args_Lev3;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		struct selectStateLev3 : BaseState
		{
			workingModeName mode_selec_Lev3 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_selec_Args_Lev3(mode_){}
				workingModeName mode_selec_Args_Lev3;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

	/********************** Lev4 ***********************/

		struct routineCleanLev4 : BaseState
		{
			workingModeName mode_routi_Lev4 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_routi_Args_Lev4(mode_){}
				workingModeName mode_routi_Args_Lev4;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		struct delimitCleanLev4 : BaseState
		{
			workingModeName mode_delimit_Lev4 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_delimit_Args_Lev4(mode_){}
				workingModeName mode_delimit_Args_Lev4;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		struct reChargeLev4 : BaseState
		{
			workingModeName mode_recharge_Lev4 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_recharge_Args_Lev4(mode_){}
				workingModeName mode_recharge_Args_Lev4;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		struct recoveryLev4 : BaseState
		{
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void OnExit();
		};
		/********************** Lev5 ***********************/

		struct informationPubLev5 : BaseState
		{
			workingModeName mode_infor_Lev5 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_infor_Args_Lev5(mode_){}
				workingModeName mode_infor_Args_Lev5;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		struct selectStateLev5 : BaseState
		{
			workingModeName mode_selec_Lev5 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_selec_Args_Lev5(mode_){}
				workingModeName mode_selec_Args_Lev5;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		struct recoverySelectStateLev5 : BaseState
		{
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void OnExit();
		};

		/********************** Lev6 ***********************/

		/**
       * @brief  Lev6-StateManhine: 导航
       */
		struct navigationLev6 : BaseState
		{
			workingModeName mode_nav_Lev6 = NO_WORK;
			PoseStamped _start_;
			PoseStamped _end_;
			struct Args
			{
				Args(const workingModeName& mode_, const PoseStamped _start_ROU, const PoseStamped _end_ROU) : mode_nav_Args_Lev6(mode_), start_Args_nav_Lev6(_start_ROU), end_Args_nav_Lev6(_end_ROU){}
				workingModeName mode_nav_Args_Lev6;
				PoseStamped start_Args_nav_Lev6;
				PoseStamped end_Args_nav_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 获取弓字形路径
       */
		struct bowShapeLev6 : BaseState
		{
			workingModeName mode_bow_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_nav_Args_Lev6(mode_){}
				workingModeName mode_nav_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 非绑架重定位
       */
		struct reLocation_NoKidnapLev6 : BaseState
		{
			workingModeName mode_reLocat_NoKidnap_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_reLocat_NoKidnap_Args_Lev6(mode_){}
				workingModeName mode_reLocat_NoKidnap_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 非绑架重定位后的建图
       */
		struct mappingAfterRelocation_NoKidnapLev6 : BaseState
		{
			workingModeName mode_mappingAfterRelocation_NoKidnap_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_mappingAfterRelocation_NoKidnap_Args_Lev6(mode_){}
				workingModeName mode_mappingAfterRelocation_NoKidnap_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 获取清扫边界
       */
		struct getCleanBoxLev6 : BaseState
		{
			workingModeName mode_getCleanBox_Lev6 = NO_WORK;
			bool isGetCleanBox;  // 是否获得清扫区域，默认false
			struct Args
			{
				Args(const workingModeName& mode_) : mode_getCleanBox_Args_Lev6(mode_){}
				workingModeName mode_getCleanBox_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 搜索清扫起始点
       */
		struct getCleanStartPointLev6 : BaseState
		{
			workingModeName mode_getCleanStartPoint_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_getCleanStartPoint_Args_Lev6(mode_){}
				workingModeName mode_getCleanStartPoint_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 导航中motion路经追踪
       */
		struct motionNavLev6 : BaseState
		{
			workingModeName mode_motionNav_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_motionNav_Args_Lev6(mode_){}
				workingModeName mode_motionNav_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 弓字形中的路径追踪
       */
		struct motionBowLev6 : BaseState
		{
			workingModeName mode_motionBow_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_motionBow_Args_Lev6(mode_){}
				workingModeName mode_motionBow_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 获得SLAM路径
       */
		struct getSlamPathLev6 : BaseState
		{
			workingModeName mode_getSlamPath_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_getSlamPath_Args_Lev6(mode_){}
				workingModeName mode_getSlamPath_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 合并SLAM与导航路径
       */
		struct mergeSlamPathLev6 : BaseState
		{
			workingModeName mode_mergeSlamPath_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_mergeSlamPath_Args_Lev6(mode_){}
				workingModeName mode_mergeSlamPath_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 避障
       */
		struct obstacleAvoidLev6 : BaseState
		{
			workingModeName mode_obstacleAvoid_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_obstacleAvoid_Args_Lev6(mode_){}
				workingModeName mode_obstacleAvoid_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 绑架重定位
       */
		struct reLocation_KidnapLev6 : BaseState
		{
			workingModeName mode_reLocat_Kidnap_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_reLocat_Kidnap_Args_Lev6(mode_){}
				workingModeName mode_reLocat_Kidnap_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 绑架重定位后的建图
       */
		struct mappingAfterRelocation_KidnapLev6 : BaseState
		{
			workingModeName mode_mappingAfterRelocation_Kidnap_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_mappingAfterRelocation_Kidnap_Args_Lev6(mode_){}
				workingModeName mode_mappingAfterRelocation_Kidnap_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 上桩
       */
		struct upperPileLev6 : BaseState
		{
			workingModeName mode_upperPile_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_upperPile_Args_Lev6(mode_){}
				workingModeName mode_upperPile_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 下桩
       */
		struct underPileLev6 : BaseState
		{
			workingModeName mode_underPile_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_underPile_Args_Lev6(mode_){}
				workingModeName mode_underPile_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 空状态
       */
		struct EMPTYLev6 : BaseState
		{
			workingModeName mode_EMPTY_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_EMPTY_Args_Lev6(mode_){}
				workingModeName mode_EMPTY_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 速度为零状态
       */
		struct SPEEDZEROLev6 : BaseState
		{
			workingModeName mode_SPEEDZERO_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_SPEEDZERO_Args_Lev6(mode_){}
				workingModeName mode_SPEEDZERO_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 搜索障碍物点
       */
		struct searchObsPointLev6 : BaseState
		{
			workingModeName mode_searchObsPoint_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_searchObsPoint_Args_Lev6(mode_){}
				workingModeName mode_searchObsPoint_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 右沿边
       */
		struct followEdgeRightLev6 : BaseState
		{
			workingModeName mode_followEdgeRight_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_followEdgeRight_Args_Lev6(mode_){}
				workingModeName mode_followEdgeRight_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 获取左沿边起始点
       */
		struct getObsLeftStartLev6 : BaseState
		{
			workingModeName mode_getObsLeftStart_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_getObsLeftStart_Args_Lev6(mode_){}
				workingModeName mode_getObsLeftStart_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 左沿边
       */
		struct followEdgeLeftLev6 : BaseState
		{
			workingModeName mode_followEdgeLeft_Lev6 = NO_WORK;
			struct Args
			{
				Args(const workingModeName& mode_) : mode_followEdgeLeft_Args_Lev6(mode_){}
				workingModeName mode_followEdgeLeft_Args_Lev6;
			};
			virtual void OnEnter(const Args& args);
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 禁区脱困
       */
		struct recoveryForbidenLev6 : BaseState
		{
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 膨胀层脱困
       */
		struct recoveryObsLev6 : BaseState
		{
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 包络环绕脱困
       */
		struct recoverySurroundedLev6 : BaseState
		{
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};

		/**
       * @brief  Lev6-StateManhine: 物理脱困
       */
		struct recoveryPhysicsLev6 : BaseState
		{
			virtual void OnEnter();
			virtual Transition GetTransition();
			virtual void Update();
			virtual void OnExit();
		};
	};
}

#endif