include "Header.msg"

namespace tutorial.sensor

message ImuData {
    header: tutorial.base.Header
    frame_id: uint32
    accel: [double]
    gyro: [double]
}