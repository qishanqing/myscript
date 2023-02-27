#pragma once

#include <opencv2/highgui.hpp>
#include <mind_os/serialization/serialization.h>

namespace mind_os {
template<>
inline void serialize(BytesBuffer& buffer,const cv::Point2f& data) {
    serialize(buffer,data.x);
    serialize(buffer,data.y);
};

template<>
inline void serialize(const cv::Point2f& data,BytesBuffer& buffer) {
    serialize(buffer,data.x);
    serialize(buffer,data.y);
};

template<>
inline void deserialize(BytesBuffer& buffer, cv::Point2f& data) {
    deserialize(buffer,data.x);
    deserialize(buffer,data.y);
};

template<>
inline void deserialize(cv::Point2f& data,BytesBuffer& buffer) {
    deserialize(buffer,data.x);
    deserialize(buffer,data.y);
};

} /* namespace mind_os */
