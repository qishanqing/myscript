/*
 * InternalErrorCode.h
 *
 * Error code for Indemind.
 *
 *  Created on: Sep 16, 2021
 *      Author: ubuntu
 */

#pragma once

namespace psl {
enum ErrorCode
{
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
};
}

#define GetErrorLevel(X) ((X & 0x00110000) >> 16)
