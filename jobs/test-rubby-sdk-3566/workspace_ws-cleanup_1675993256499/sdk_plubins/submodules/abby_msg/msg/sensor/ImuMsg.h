#pragma once

#include <cstdint>

namespace sensor_msgs {
    
struct ImuMsg {
    /** IMU frame sensorId */
    uint32_t frameId;
    /** IMU accelerometer data for 3-axis: X, Y, Z. */
    double accel[3];
    /** IMU gyroscope data for 3-axis: X, Y, Z. */
    double gyro[3];
    double quation[4];
    /** IMU timeStamp */
    std::uint64_t timeStamp;
};

}

