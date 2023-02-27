include "Header.msg"

namespace tutorial.sensor

message LaserScan {
    header: tutorial.base.Header
    angle_min: float
    angle_max: float
    angle_increment: float
    time_increment: float
    scan_time: float
    range_min: float
    range_max float
    ranges: [float]
    intensities [float]
}
