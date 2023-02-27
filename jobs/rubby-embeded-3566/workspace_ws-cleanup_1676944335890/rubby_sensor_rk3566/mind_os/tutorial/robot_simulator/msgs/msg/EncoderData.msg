include "Header.msg"

namespace tutorial.sensor

message EncoderData {
    header: tutorial.base.Header
    encoderL: int64
    encoderR: int64
}