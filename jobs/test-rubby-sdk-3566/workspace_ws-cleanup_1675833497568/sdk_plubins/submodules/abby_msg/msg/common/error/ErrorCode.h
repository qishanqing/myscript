/*
 * InternalErrorCode.h
 *
 * Error code for Indemind.
 *
 *  Created on: Sep 16, 2021
 *      Author: ubuntu
 */

#pragma once

namespace error_msgs {

enum ErrorCode
{
    // 0x0l xxxx05 - self_test
    SELF_TEST_NO_ERROR = 0x05000000,
    SELF_TEST_CLOUD_START_FAILED = 0x05020001,
    SELF_TEST_VCU_START_FAILED = 0x05020002,
    SELF_TEST_CAN_START_FAILED = 0x05020003,
    SELF_TEST_COPROCESSOR_START_FAILED = 0x05020004,
    SELF_TEST_IMSEE_START_FAILED = 0x05020005,
    SELF_TEST_LASER_START_FAILED = 0x05020006,
    SELF_TEST_TOF_START_FAILED = 0x05020007,

    SELF_TEST_ENDCODER_RATE_ABNORMAL = 0x05020011,
    SELF_TEST_VELOCITY_RATE_ABNORMAL = 0x05020012,
    SELF_TEST_IMG_RATE_ABNORMAL = 0x05020013,
    SELF_TEST_IMGSIGMA_RATE_ABNORMAL = 0x05020014,
    SELF_TEST_IMU_RATE_ABNORMAL = 0x05020015,
    SELF_TEST_ULTRA_RATE_ABNORMAL = 0x05020016,
    SELF_TEST_LASERSCAN_RATE_ABNORMAL = 0x05020017,
    SELF_TEST_TOF_RATE_ABNORMAL = 0x05020018,

    SELF_TEST_UI_NOT_CONNECTED = 0x05020020,

    SELF_TEST_DISK_ALMOST_FULL = 0x05020030,

    SELF_TEST_CHASSIS_FAULT = 0x05020040,

    SELF_TEST_MARKER_INIT_PARAM_FAULT = 0x05020050,
    SELF_TEST_DETECTOR_INIT_PARAM_FAULT = 0x05020051,
    SELF_TEST_CALIBR_INIT_PARAM_FAULT = 0x05020052,
    SELF_TEST_DEPTH_INIT_PARAM_FAULT = 0x05020053,
    SELF_TEST_CAMERA_INIT_PARAM_FAULT = 0x05020054,
    
    // self calibration
    SELF_CALIBRATION_SUCCESS = 0x0133004B,
    SELF_CALIBRATION_FAILURE = 0x0133004C,
    SELF_CALIBRATION_NO_MORE_IMAGES = 0x0133004E,
    SELF_CALIBRATION_LESS_CACHE_IMAGES = 0x0132004D,
    SELF_CALIBRATION_NO_REGISTER_CALLBACK_FUNCTION = 0x0133004F,

    //slam
    SLAM_TEST_NO_ERROR =  0x12000000,
    SLAM_ONLINE_CALIB_FAILED = 0x123301F5,
    SLAM_REV_SAME_IMG = 0x12320065,
    SLAM_REV_SAME_IMU = 0x12320079,
    SLAM_REV_SAME_ODM = 0x1232008D,
    SLAM_REV_SAME_LASER = 0x123200A1,

    SLAM_REV_IMU_LOSS = 0x1232007A,
    SLAM_REV_ODM_LOSS = 0x1232008E,
    SLAM_REV_LASER_LOSS = 0x123200A2,

    SLAM_REV_IMU_LOSS_LONG_TIME = 0x1222007B,
    SLAM_REV_ODM_LOSS_LONG_TIME = 0x1222008F,
    SLAM_REV_LASER_LOSS_LONG_TIME = 0x122200A3,

    SLAM_ONLY_IMU = 0x122200C9,
    SLAM_LACK_MEMORY = 12230051,
    SLAM_LACK_CPU = 12230052,

    SLAM_MODULE_PARAM_READ_FAILED = 0x12220001,
    SLAM_ODM_PARAM_READ_FAILED = 0x12220002,
    SLAM_MAP_READ_FAILED = 0x12220033,
    SLAM_MAP_PATH_EMPTY = 0x12220034,

    SLAM_REV_IMU_ABNORMAL = 0x1232007C,//跳变
    SLAM_REV_ODM_ABNORMAL = 0x12320090,
    SLAM_REV_LASER_ABNORMAL = 0x123200A4,

    SLAM_LACK_RESULT_CALLBACK_PTR = 0x1222003D,
    SLAM_LACK_MAP_CALLBACK_PTR = 0x1222003E,
    SLAM_LACK_SENTI_MAP_CALLBACK_PTR = 0x1222003F,

    SLAM_INIT_FAILED_NO_MARKER_POSE = 0x12220040,
    SLAM_FINAL_FAILED_NO_MARKER_POSE = 0x12220041,

    SLAM_MODI_MAP_FAILED_LOSS_KEY_DATA = 0x12230040,
    SLAM_REV_MAP_DATA_EMPTY = 0x12230041,

    SLAM_REV_IMU_DELAY = 0x1232007D,
    SLAM_REV_ODM_DELAY = 0x12320091,
    SLAM_REV_LASER_DELAY = 0x123200A5,

    SLAM_REV_IMU_LOSS_ODM_LOSS = 0x1232007E,
    SLAM_REV_IMU_LOSS_LASER_LOSS = 0x1232007F,
    SLAM_REV_ODM_LOSS_LASER_LOSS = 0x12320092,
    SLAM_REV_ODM_LOSS_LASER_LOSS_IMU_LOSS = 0x12320093,

    SLAM_REV_IMU_LOSS_LONG_ODM_LOSS = 0x12320080,
    SLAM_REV_IMU_LOSS_LONG_LASER_LOSS = 0x12320081,
    SLAM_REV_IMU_LOSS_LONG_ODM_LOSS_LONG = 0x12320082,
    SLAM_REV_IMU_LOSS_LONG_LASER_LOSS_LONG = 0x12320083,
    SLAM_REV_IMU_LOSS_LONG_LASER_LOSS_ODM_LOSS = 0x12320084,
    SLAM_REV_IMU_LOSS_LONG_LASER_LOSS_LONG_ODM_LOSS = 0x12320085,
    SLAM_REV_IMU_LOSS_LONG_ODM_LOSS_LONG_LASER_LOSS = 0x12320086,
    SLAM_REV_ODM_LOSS_LONG_LASER_LOSS_LONG = 0x12320094,
    SLAM_REV_ODM_LOSS_LONG_IMU_LOSS = 0x12320095,
    SLAM_REV_ODM_LOSS_LONG_LASER_LOSS = 0x12320096,
    SLAM_REV_ODM_LOSS_LONG_LASER_LOSS_IMU_LOSS = 0x12320097,
    SLAM_REV_ODM_LOSS_LONG_LASER_LOSS_LONG_IMU_LOSS = 0x12320098,
    SLAM_REV_LASER_LOSS_LONG_IMU_LOSS = 0x123200A6,
    SLAM_REV_LASER_LOSS_LONG_ODM_LOSS = 0x123200A7,
    SLAM_REV_LASER_LOSS_LONG_ODM_LOSS_IMU_LOSS = 0x123200A8,
    SLAM_REV_LASER_LOSS_LONG_ODM_LOSS_LONG_IMU_LOSS_LONG= 0x123200A9,


    SLAM_REV_IMU_NOT_ORDER = 0x12320087,
    SLAM_REV_ODM_NOT_ORDER = 0x12320099,
    SLAM_REV_LASER_NOT_ORDER = 0x123200AA,

    SLAM_ROBOT_FALL_DOWN = 0x1223012D,
    SLAM_UNDERGROUND_ANGLE_ERROR = 0x1223012E,

    SLAM_RELOCAL_MODI_TOO_MANY = 0x12230141,
    SLAM_RELOCAL_FAILED_OVER_TIME = 0x12230142,

    SLAM_BIG_EMPTY_SURROUNDINGS = 0x12330155,
    SLAM_LONG_CORRIDOR = 0x12330156,

    SLAM_UNCERTAIN_SURROUNDING_BAD_PERCISION = 0x12230157,
    SLAM_LASER_OVER_EXPOSURE = 0x12230158,

    SLAM_SURROUNDING_EXIT_GLASS = 0x12330159,
    SLAM_SURROUNDING_EXIT_MIRROR = 0x1233015A,

    SLAM_ROBOT_SLIPPING = 0x12330169,
    SLAM_MAP_CHANGE_LOT = 0x1233016A,

    //dl
    DL_INVALID_IMAGE = 0x04220033,
    DL_LACK_IMAGE = 0x04320034,
    DL_LACK_IMAGE_HEAVY = 0x04220035,
    DL_ERROR_IMAGE = 0x04220036,
    DL_LACK_LIDAR = 0x04320065,
    DL_LACK_LIDAR_HEAVY = 0x04220066,
    DL_ERROR_LIDAR = 0x04220067,
    DL_INVALID_CAMERA_PARAM = 0x04220047,
    DL_INVALID_MODEL = 0x04220001,
    DL_LACK_MODEL = 0x04220002,
    DL_INVALID_CONFIG = 0x04220003,
    DL_LACK_CONFIG = 0x04220004,
    DL_NO_GPU = 0x04230015,
    DL_NO_NPU = 0x04230016,
    DL_UNKNOWN_DL = 0x0423001F,
    DL_COLLECT_IMG_BY_CONFIDENCE = 0x04440029,

/*************************FAULT********************************/
//IMSEE   
    //IMSEE-DUALCAME 
    IMSEE_NOT_MOUNT = 0x07110001,
    IMSEE_START_FAULT = 0x07110002,
    IMSEE_HOTPLUGIN_EVENT = 0x07110003,
    IMSEE_DATA_FAULT = 0x07110065,
    IMSEE_IMG_FRAMERATE_FAULT = 0x07110067,
    IMSEE_GET_LOCAL_CALIBRATION_FILE_FAULT = 0x07110068,
    IMSEE_IMG_LOST_CONNECTION_FAULT = 0x0731006E,
    //IMSEE-IMU 
    IMSEE_IMU_NOT_MOUNT = 0x071100C9,   //无此异常
    IMSEE_IMU_START_FAULT = 0x071100CA,
    IMSEE_IMU_FRAMERATE_FAULT = 0x071100CC,
    IMSEE_IMU_LOST_CONNECTION_FAULT = 0x073100D2,

//LASER
    LASER_NOT_MOUNT = 0x09110001,
    LASER_START_FAULT = 0x09110002,
    LASER_DATA_FAULT = 0x09110065,
    LASER_DATA_FRAMERATE_FAULT = 0x09110067,
    
//CO-PROCESSOR
    //COPROCESSOR
    COPROCESSOR_START_FAULT = 0x0B110001,
    COPROCESSOR_DATA_FAULT = 0x0B110002,
    COPROCESSOR_DATA_FRAMERATE_FAULT = 0x0B110004,
    //ULTRA 1
    COPROCESSOR_ULTRA_1_CONNECT_FAULT = 0x0B110097,
    COPROCESSOR_ULTRA_1_DATA_FAULT = 0x0B110098,

    COPROCESSOR_ULTRA_1_DATA_FRAMERATE_FAULT = 0x0B11009B,
    //ULTRA 2
    COPROCESSOR_ULTRA_2_CONNECT_FAULT = 0x0B1100AB,
    COPROCESSOR_ULTRA_2_DATA_FAULT = 0x0B1100AC,

    COPROCESSOR_ULTRA_2_DATA_FRAMERATE_FAULT = 0x0B1100AF,
    //ULTRA 3
    COPROCESSOR_ULTRA_3_CONNECT_FAULT = 0x0B1100BF,
    COPROCESSOR_ULTRA_3_DATA_FAULT = 0x0B1100C0,

    COPROCESSOR_ULTRA_3_DATA_FRAMERATE_FAULT = 0x0B1100C3,
    //ULTRA 4
    COPROCESSOR_ULTRA_4_CONNECT_FAULT = 0x0B1100D3,
    COPROCESSOR_ULTRA_4_DATA_FAULT = 0x0B1100D4,

    COPROCESSOR_ULTRA_4_DATA_FRAMERATE_FAULT = 0x0B1100D7,
    //ULTRA 5
    COPROCESSOR_ULTRA_5_CONNECT_FAULT = 0x0B1100E7,
    COPROCESSOR_ULTRA_5_DATA_FAULT = 0x0B1100E8,

    COPROCESSOR_ULTRA_5_DATA_FRAMERATE_FAULT = 0x0B1100EB,
    //ULTRA 6
    COPROCESSOR_ULTRA_6_CONNECT_FAULT = 0x0B1100FB,
    COPROCESSOR_ULTRA_6_DATA_FAULT = 0x0B1100FC,

    COPROCESSOR_ULTRA_6_DATA_FRAMERATE_FAULT = 0x0B1100FF,
    //CONTROLLER
    COPROCESSOR_CTRL_NOT_MOUNT = 0x0B110065,
    COPROCESSOR_CTRL_START_FAULT = 0x0B110066,

    COPROCESSOR_CTRL_DATA_FRAMERATE_FAULT = 0x0B110068,

//TOF
    TOF_NOT_MOUNT = 0x14110001,
    TOF_START_FAULT = 0x14110002,
    TOF_DATA_FAULT = 0x14110033,
    TOF_DATA_FRAMERATE_FAULT = 0x14110035,
    TOF_ILLUMINATION_FAULT = 0x14110036,     //TOF 红外照明 异常
    TOF_LOST_CONNECTION_FAULT = 0x14310015,
    TOF_GET_LOCAL_CALIBRATION_FILE_FAULT = 0x14310065,
    
//CAN
    MCU_CAN_NOT_MOUNT = 0x02110015,
    MCU_CAN_START_FAULT = 0x02110016,
    MCU_REPLACE_UNREGISTERED_FAULT= 0x02110017,
    //CLIFF
    CLIFF_1_DATA_FRAMERATE_FAULT = 0x02110029,
    CLIFF_2_DATA_FRAMERATE_FAULT = 0x0211002A,
    CLIFF_3_DATA_FRAMERATE_FAULT = 0x0211002B,
    CLIFF_4_DATA_FRAMERATE_FAULT = 0x0211002C,
    CLIFF_5_DATA_FRAMERATE_FAULT = 0x0211002D,
    CLIFF_6_DATA_FRAMERATE_FAULT = 0x0211002E,
    CLIFF_7_DATA_FRAMERATE_FAULT = 0x0211002F,
    //WHEEL
    ENCODER_LEFT_DATA_LONGTIME_LOSS_FAULT = 0x0211003D,
    VELOCITY_LEFT_DATA_LONGTIME_LOSS_FAULT = 0x0211003E,
    ENCODER_RIGHT_DATA_LONGTIME_LOSS_FAULT = 0x02110051,
    VELOCITY_RIGHT_DATA_LONGTIME_LOSS_FAULT = 0x02110052,
    //MCU FEEDBACK
    LEFT_WHEEL_FAULT = 0xffffffff,  //协议有修改弃用
    RIGHT_WHEEL_FAULT = 0xffffffff, //协议有修改弃用
    LIGHT_BRUSH_FAULT = 0xffffffff, //协议有修改弃用
    RIGHT_BRUSH_FAULT = 0xffffffff, //协议有修改弃用
    CONTROLLER_OVER_TEMPERATURE = 0xffffffff,   //协议有修改弃用
//  行走电机故障，吸水泵故障，边刷电机故障，主刷电机故障，刷盘线推器故障，水刮线推器故障，急停触发，控制器系统故障。
    DRIVING_WHEEL_FAULT = 0x021A0001,
    SUCTION_PUMP_FAULT = 0x021A0002,
    BESIDE_BRUSH_FAULT = 0x021A0003,
    MAIN_BRUSH_FAULT = 0x021A0004,
    BRUSH_LINE_DRIVER_FAULT = 0x021A0005,
    WATER_SCRAPER_LINE_DRIVER_FAULT = 0x021A0006,
    EMERGENCY_STOP_TRIGGER = 0x021A0007,
    CONTROLLER_SYSTEM_FAULT = 0x021A0008,

    
//VCU
    VCU_START_FAULT = 0x15110001,
    VCU_GET_DATA_FAULT = 0x15110002,
    VCU_REPLACE_UNREGISTERED_FAULT = 0x1511000B,
    
    VCU_DATA_FRAMERATE_LOW_FAULT = 0x15110034,
    VCU_UNABLE_TO_WRITE_FAULT = 0x15110035,
//MULTIMODULE
    //LCD
    MULTIMODULE_LCD_NOT_MOUNT = 0x17110097,
    MULTIMODULE_LCD_START_FAULT = 0x17110098,
    MULTIMODULE_LCD_BACKLIGHT_START_FAULT = 0x17110099,
    MULTIMODULE_LCD_NOSIGNAL_FAULT = 0x1711009B,
    MULTIMODULE_LCD_SIGNAL_OUT_FAULT = 0x1711009C,
    MULTIMODULE_LCD_CANNOT_DISPLAY_FAULT = 0x1711009D,
    MULTIMODULE_LCD_DISPLAY_FAULT = 0x1711009E,
    //SPEAKER
    MULTIMODULE_SPEAKER_NOT_MOUNT = 0x171100C9,
    MULTIMODULE_SPEAKER_START_FAULT = 0x171100CA,
    MULTIMODULE_SPEAKER_OPENCIRCUIT_FAULT = 0x171100CB,
    MULTIMODULE_SPEAKER_SHORTCIRCULT_FAULT = 0x171100CC,




/*************************WARNINGS*********************************/
 //IMSEE
    IMSEE_TEXTURE_LIGHT_WARN = 0x07310069, 
    IMSEE_BRIGHTNESS_LIGHT_WARN = 0x0731006A, 
    IMSEE_BRIGHTNESS_DETECTION_WARN = 0x0731006B,
    
    //UPGRADE
    IMSEE_UPGRADE_FILES_LOSS_WARN = 0X07320033,
//COPROCESSOR
    //ULTRA   
    COPROCESSOR_ULTRA_1_SIGNAL_INTERFERENCE_WARN = 0x0B310099,
    COPROCESSOR_ULTRA_2_SIGNAL_INTERFERENCE_WARN = 0x0B3100AD,
    COPROCESSOR_ULTRA_3_SIGNAL_INTERFERENCE_WARN = 0x0B3100C1,
    COPROCESSOR_ULTRA_4_SIGNAL_INTERFERENCE_WARN = 0x0B3100D5,
    COPROCESSOR_ULTRA_5_SIGNAL_INTERFERENCE_WARN = 0x0B3100E9,
    COPROCESSOR_ULTRA_6_SIGNAL_INTERFERENCE_WARN = 0x0B3100FD,
    //UPGRADE
    // COPROCESSOR_UPGRADE_FILES_LOST_WARN = 0x0B320015,
    // COPROCESSOR_UPGRADE_FILES_OPEN_WARN = 0x0B320016,
    // COPROCESSOR_UPGRADE_FILES_DATA_WARN = 0x0B320017,
    // COPROCESSOR_UPGRADE_FILES_DELET_WARN = 0x0B320018,
//TOF
    TOF_UPGRADE_FILES_LOST_WARN = 0x14320065,
    TOF_UPGRADE_FILES_OPEN_WARN = 0x14320066,
    TOF_UPGRADE_FILES_DATA_WARN = 0x14320067,
    TOF_UPGRADE_FILES_DELET_FAIL_WARN = 0x14320068,
//MULTIMODULE 
    //LCD
    MULTIMODULE_LCD_BACKLIGHT_ADJUST_FAIL_WARN = 0x1731009A,
    //SPEAKER
    MULTIMODULE_SPEAKER_VOLUME_ADJUSTMENT_FAIL_FAULT = 0x173100CD,
    //HOTSPOT
    MULTIMODULE_HOTSPOT_NOT_MOUNT_WARN = 0x173100FB,
    MULTIMODULE_HOTSPOT_HARDWARE_START_FAIL_WARN = 0x173100FC,
    MULTIMODULE_HOTSPOT_SOFTWARE_START_FAIL_WARN = 0x173100FD,
    MULTIMODULE_HOTSPOT_PASSWORD_NOT_MATCH_WARN = 0x173100FE,
    MULTIMODULE_HOTSPOT_ANTENNA_MISSING_WARN = 0x173100FF,
    MULTIMODULE_HOTSPOT_SOFTWARE_CLOSE_FAIL_WARN = 0x17310100,

    MULTIMODULE_HOTSPOT_STA_MODE_OPEN_FAIL_WARN = 0x17320029,
    MULTIMODULE_HOTSPOT_AP_MODE_OPEN_FAIL_WARN = 0x1732002A,
    MULTIMODULE_HOTSPOT_NAME_FORMAT_WRONG_WARN = 0x1732002B,
    MULTIMODULE_HOTSPOT_PASSWORD_FORMAT_WRONG_WARN = 0x1732002C,
    //POWER

    
    //THREE COLOR LIGHT
    MULTIMODULE_THREECOLORLIGHT_RED_FAIL_WARN = 0X1731015F,
    MULTIMODULE_THREECOLORLIGHT_GREEN_FAIL_WARN = 0X17310160,
    MULTIMODULE_THREECOLORLIGHT_BLUE_FAIL_WARN = 0X17310161,

    //AUDIO
    MULTIMODULE_AUDIO_GET_INPUT_VOLUME_FAIL_WARN = 0X17320015,
    MULTIMODULE_AUDIO_INPUT_VOLUME_FILE_IS_NULL_WARN = 0X17320016,
    MULTIMODULE_AUDIO_INPUT_VOLUME_FILE_CANNOT_PLAY_WARN = 0X17320017,
    MULTIMODULE_PLAY_EARLY_TERMINATED_WARN = 0X17320018,
    MULTIMODULE_PLAY_ONPLYING_CANNOT_PLAY_NEW_FILE_WARN = 0X17320019,



/*************************SERIOUS WARNINGS*************************/
    IMSEE_IMG_DATA_FRAMERATE_WARN = 0x07210066,
    IMSEE_IMU_DATA_FRAMERATE_WARN = 0x072100CB,
    //LASER   
    LASER_IMU_DATA_FRAMERATE_WARN = 0x09210066,

    // COPROCESSOR_DATA_FRAMERATE_WARN = 0x0B210003, 无此异常
    COPROCESSOR_ULTRA_1_DATA_FRAMERATE_WARN = 0x0B21009A,
    COPROCESSOR_ULTRA_2_DATA_FRAMERATE_WARN = 0x0B2100AE,
    COPROCESSOR_ULTRA_3_DATA_FRAMERATE_WARN = 0x0B2100C2,
    COPROCESSOR_ULTRA_4_DATA_FRAMERATE_WARN = 0x0B2100D6,
    COPROCESSOR_ULTRA_5_DATA_FRAMERATE_WARN = 0x0B2100EA,
    COPROCESSOR_ULTRA_6_DATA_FRAMERATE_WARN = 0x0B2100FE,

    COPROCESSOR_CTRL_SPD_FRAMERATE_WARN = 0x0B210067,

    TOF_DATA_FRAMERATE_WARN = 0x14210034,
    VCU_DATA_FRAMERATE_WARN = 0x15210033,
//HOTSPOT
    MULTIMODULE_HOTSPOT_START_FAIL_WARN = 0x17220001,
    MULTIMODULE_HOTSPOT_NO_CONFIG_FILE_WARN = 0x17220002,
    MULTIMODULE_HOTSPOT_CONFIG_FILE_CANNOT_OPEN_WARN = 0x17220003,
    MULTIMODULE_HOTSPOT_CONFIG_FILE_FORMAT_WRONG_WARN = 0x17220004,
    

/******************************************************************/

    // depth
    DEP_IMG_FRAME_LOSS=0x03320097,
    DEP_IMG_FRAME_RATE_IS_TOO_LOW=0x03220098,
    DEP_IMG_FRAME_LOST=0x03220099,
    DEP_IMG_DATA_ABNORMAL=0x0332009A,
    DEP_IMG_DATA_ABNORMAL_CONTINUOUS=0x0322009B,
    DEP_IMG_DELAY_TOO_LONG=0x0322009C,
    DEP_TOF_FRAME_LOSS=0x03320065,
    DEP_TOF_FRAME_RATE_IS_TOO_LOW=0x03220066,
    DEP_TOF_FRAME_LOST=0x03220067,
    DEP_TOF_DATA_ABNORMAL=0x03220068,
    DEP_TOF_DATA_ABNORMAL_CONTINUOUS=0x03220069,
    DEP_TOF_DELAY_TOO_LONG=0x0322006A,
    DEP_POSE_FRAME_LOSS=0x033200C9,
    DEP_POSE_FRAME_LOST=0x033200CA,
    DEP_GET_POSE_FOR_TOO_LONG=0x033200CB,

//    DEP_PROCESSED_IMG_FRAME_RATE_TOO_LOW,
    DEP_VALID_PROCESSED_IMG_FRAME_RATE_TOO_LOW=0x0333000B,
    DEP_PROCESSED_IMG_CALCULATION_TOO_LONG=0x0333000E,
    DEP_PROCESSED_IMG_CALCULATION_TIMEOUT=0x03230011,
//    DEP_PROCESSED_TOF_FRAME_RATE_TOO_LOW,
    DEP_VALID_PROCESSED_TOF_FRAME_RATE_TOO_LOW=0x0333000C,
    DEP_PROCESSED_TOF_CALCULATION_TOO_LONG=0x0333000F,
    DEP_PROCESSED_TOF_CALCULATION_TIMEOUT=0x03230012,
//    DEP_PROCESSED_LASER_FRAME_RATE_TOO_LOW,
    DEP_VALID_PROCESSED_LASER_FRAME_RATE_TOO_LOW=0x0323000D,
    DEP_PROCESSED_LASER_CALCULATION_TOO_LONG=0x03330010,
    DEP_PROCESSED_LASER_CALCULATION_TIMEOUT=0x03230013,

    DEP_CAMERA_CONTINUOUSLY_OVEREXPOSURE=0x0322009D,
    DEP_TOF_CONTINUOUSLY_OVEREXPOSURE=0x0322006B,
    DEP_NO_CALIBRATION_PARAM,
    DEP_CALIBRATION_PARAM_ERROR=0x03220014,
    DEP_CONFIG_PARAM_DOES_NOT_MATCH=0x03220034,
    DEP_CONFIG_PARAM_ERROR=0x03220035,






/***************NAV-ERROR-CODE***********************/
/**                  Start                         **/
/***************NAV-ERROR-CODE***********************/
    NAV_ERCODE_VELOCITY_LOSE_500ms = 0x0E220065,  //底盘速度500ms丢失
    NAV_ERCODE_VELOCITY_LOSE_1000ms = 0x0E220066, //底盘速度1s丢失
    NAV_ERCODE_LASER_LOSE_500ms     = 0x0E220097, //激光500ms丢失
    NAV_ERCODE_LASER_LOSE_1000ms   = 0x0E220098,  //激光1s丢失
    NAV_ERCODE_ROBOTPOSE_LOSE_100ms =0x0E32012D,  //定位100ms丢失
    NAV_ERCODE_ROBOTPOSE_LOSE_300ms = 0x0E22012E, //定位300ms丢失
    NAV_ERCODE_DEPTH_LOSE_500ms = 0x0E3200C9,     //tof及双目数据500ms丢失
    NAV_ERCODE_DEPTH_LOSE_1000ms = 0x0E2200CA,     //tof及双目数据500ms丢失
    NAV_ERCODE_SAVE_PAHT_FAILED  = 0x0E24000B,  //存取路径失败或者校验失败
    NAV_ERCODE_CLEAN_PAHT_VERIFY_FAILED = 0x0E22000C,//路径校验失败
    NAV_ERCODE_DRIVE_MOTOR_OVERCURRENT = 0x0E2300FB,//侧轮或双轮持续过载
    NAV_ERCODE_TRACKING_ERROR = 0x0E2300FC, //检测到电机跟踪误差超限
    NAV_ERCODE_STOP_DRIVE_MOTOR_FAILED = 0x0E2300FD, //电机无法停止
    NAV_ERCODE_RETURN_HOME_FAILED = 0x0E23028B,//结束后未回到起始点
    NAV_ERCODE_NOT_RECEIVE_FIRST_STATIC_MAP = 0x0E220033,//没有接收到首张地图
    NAV_ERCODE_GLOBAL_COSTMAP_UPDATE_OVERTIME = 0x0E230015,//全局costmap更新超时
    NAV_ERCODE_LOCAL_COSTMAP_UPDATE_OVERTIME = 0x0E230016,//局部costmap更新超时
    NAV_ERCODE_GLOBAL_PLAN_FAILED = 0x0E230033, //全局规划失败
    NAV_ERCODE_GLOBAL_PLAN_OVERTIME=0x0E330034,//全局规划超时
    NAV_ERCODE_LOCAL_PLAN_FAILED=0x0E230047,//局部规划失败
    NAV_ERCODE_LOCAL_PLAN_OVERTIME=0x0E330048,//局部规划超时
    NAV_ERCODE_RETURN_CLOSET_TO_HOMEPOINT = 0x0E330290,//返回到扫码点附近？？
    NAV_ERCODE_BACKING_WARNING = 0x0E3301F5,//机器人倒退警告
    NAV_ERCODE_BACK_CHARGING_WARNING=0x0E3301FF,//回冲警告
    NAV_ERCODE_PAUSE_WARNING = 0x0E330209,//暂停警告
    NAV_ERCODE_LEFT_WHEEL_DROP = 0x0E230105, //左轮悬空
    NAV_ERCODE_RIGHT_WHEEL_DROP = 0x0E23010F,//右轮悬空
    NAV_ERCODE_IN_INFLATION_AREA = 0x0E330227,//设备进入膨胀区
    NAV_ERCODE_FIND_NARROW_AREA = 0x0E330231,//发现狭窄区域
    NAV_ERCODE_IN_NARROW_AREA = 0x0E330232,//进入狭窄区域
    NAV_ERCODE_SURROUND_OB = 0x0E330245, //机器人环绕包围
    NAV_ERCODE_CLEAN_AREA_UNREACH = 0x0E330246,//出现无法到达的清扫区域
    NAV_ERCODE_FIND_NO_PATH_2_MARKER = 0x0E33028E,//无法规划出返回Marker的路线
    NAV_ERCODE_OBSTACLE_IN_FROUNT_MARKER = 0x0E33028D,//Marker前有障碍物无法返回
    NAV_ERCODE_FIND_NO_MARKER = 0x0E33028F,//运行到行驶终点后无法找到Marker
    NAV_ERCODE_UNREACH_PRECISION_AREA_MARKER = 0x0E330291,//运行到行驶终点后设备无法停入高精度区域内部
    NAV_ERCODE_NEAR_PEDESTRIAN = 0x0E3302BD,//行人过近警告
    NAV_ERCODE_NEAR_PEDESTRIAN_CONTINUE = 0x0E3302BE,//行人持续干扰
    NAV_ERCODE_NERA_LIFT = 0x0E3302EF,//发现设备距离电梯过近
    NAV_ERCODE_NERA_CLIFF = 0x0E3302F0,//发现悬崖距离设备过近
    NAV_ERCODE_BUMPER_PRESS_LONG_TIME = 0x0E33010F,//Bumper长时间处于压下状态
    NAV_ERCODE_BUMPER_EVENT_TOO_MANY = 0x0E230110,//Bumper一段时间内响应次数过多
    NAV_ERCODE_CLIFF_PRESS_LONG_TIME = 0x0E330119,//cliff长时间触发
    NAV_ERCODE_CLIFF_EVENT_TOO_MANY = 0x0E23011A,//cliff一段时间内响应次数过多
    NAV_ERCODE_TILT = 0x0E230213,//机器人倾斜达到一定阈值
    NAV_ERCODE_UNFINISH_CLEAN = 0x0E33015F,//清扫未完成
    NAV_ERCODE_TEACHING_OVERSPEED = 0x0E33021E, //教学时推动速度过快
   

/***************NAV-ERROR-CODE***********************/
/**                  End                           **/
/***************NAV-ERROR-CODE***********************/








/*****************其它error code,请在上面定义， 在上面定义， 在上面定义， 在上面定义 ************************/

//无效的error code, 原因：部分传感器的一些异常只需要定义其中一部分， 为了代码编写统一，且方便定义如下无效异常
    DONOT_USE_THIS_ERROR_CODE = 0xFFFFFFFF,
};

}  // namespace error

#define GetErrorLevel(X) ((X & 0x00110000) >> 16)
