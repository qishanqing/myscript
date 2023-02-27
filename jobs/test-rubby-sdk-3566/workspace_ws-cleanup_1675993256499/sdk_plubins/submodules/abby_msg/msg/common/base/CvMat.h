#include <opencv2/core/mat.hpp>
#include <mind_os/serialization/serialization.h>

namespace base_msgs {
    using CvMat = cv::Mat;
}

namespace mind_os {
template<>
inline void serialize(const base_msgs::CvMat& data, BytesBuffer& buffer) {
    buffer << data.rows;
    buffer << data.cols;
    buffer << data.type();
    size_t sizeOfData = data.rows * data.cols * data.elemSize();
    buffer << sizeOfData;
    buffer.AppendData(data.data, sizeOfData);
};

template<>
inline void deserialize(BytesBuffer& buffer, base_msgs::CvMat& data) {
    int rows, cols, type;
    size_t sizeOfData;
    buffer >> rows;
    buffer >> cols;
    buffer >> type;
    buffer >> sizeOfData;
    data.create(rows, cols, type);
    buffer.NextData(data.data, sizeOfData);
};

} /* namespace mind_os */
