include "Header.msg"

namespace tutorial.slam

message SlamResult {
    header: tutorial.base.Header
    s_rotation: [float]
    s_position: [float]
    s_state: float
}