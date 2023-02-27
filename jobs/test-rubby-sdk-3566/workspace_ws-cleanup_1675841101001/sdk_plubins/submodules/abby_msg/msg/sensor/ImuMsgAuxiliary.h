#pragma once

#include <cstdint>

namespace sensor_msgs {
    
struct ImuMsgAuxiliary {
    /** IMU frame sensorId */
    uint32_t frameId;
    /** IMU accelerometer data for 3-axis: X, Y, Z. */
    float accel[3];
    /** IMU gyroscope data for 3-axis: X, Y, Z. */
    float gyro[3];
    /** IMU timeStamp */
    std::uint64_t timeStamp;
};

}

