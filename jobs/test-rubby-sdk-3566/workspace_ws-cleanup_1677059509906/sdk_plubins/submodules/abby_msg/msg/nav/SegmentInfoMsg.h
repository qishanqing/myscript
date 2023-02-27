//
// Created by lcy on 23-2-10.
//

#ifndef RUBBYNAV_SEGMENTINFOMSG_H
#define RUBBYNAV_SEGMENTINFOMSG_H

#include <cstdint>
#include <opencv2/highgui/highgui.hpp>
#include <mind_os/serialization/serialization.h>
#include <vector>
#include "../common/base/CvMat.h"
#include "../common/base/CvPoint2f.h"

namespace nav_msgs {

    struct SegmentInfoMsg
    {
        std::uint64_t TimeStamp;

        cv::Mat map;//分区地图
        enum Cell
        {
            Unknow = 0    /*0:未知区域*/
                          /*1-255:分区编号*/
        };

        std::vector< std::vector<cv::Point2f> > border;  // 轮廓列表
        int SegNumber;  // 分区数量
    };

}

namespace mind_os
{
    template <>
    inline void serialize(const nav_msgs::SegmentInfoMsg& data, BytesBuffer& buffer){
        serialize(data.TimeStamp, buffer);
        serialize(data.map, buffer);
        serialize(data.border, buffer);
        serialize(data.SegNumber, buffer);
    }

	template <>
	inline void deserialize(BytesBuffer& buffer, nav_msgs::SegmentInfoMsg& data)
	{
		deserialize(buffer, data.TimeStamp);
        deserialize(buffer, data.map);
        deserialize(buffer, data.border);
        deserialize(buffer, data.SegNumber);
	}
}
#endif //RUBBYNAV_SEGMENTINFOMSG_H
