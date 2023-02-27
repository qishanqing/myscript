//
// Created by lcy on 23-2-10.
//

#ifndef RUBBYNAV_SEGMENTOPERATIONMSG_H
#define RUBBYNAV_SEGMENTOPERATIONMSG_H

#include <cstdint>
#include <vector>
#include <mind_os/serialization/serialization.h>
#include "../common/base/CvPoint2f.h"

namespace nav_msgs {

    struct SegmentOperationMsg
    {
        std::uint64_t TimeStamp;

        bool flg_merge;//合并标记.true:有合并操作,false:无合并操作
        std::vector<int> merge;//合并

        bool flg_split;//拆分标记.true:有拆分操作,false:无拆分操作
        int PartationId;//拆分目标区域编号
        cv::Point2f p1;//拆分起点世界坐标
        cv::Point2f p2;//拆分终点世界坐标
    };

}

namespace mind_os
{
    template <>
    inline void serialize(const nav_msgs::SegmentOperationMsg& data, BytesBuffer& buffer){
        serialize(data.TimeStamp, buffer);
        serialize(data.flg_merge, buffer);
        serialize(data.merge, buffer);
        serialize(data.flg_split, buffer);
        serialize(data.PartationId, buffer);
        serialize(data.p1, buffer);
        serialize(data.p2, buffer);
    }

	template <>
	inline void deserialize(BytesBuffer& buffer, nav_msgs::SegmentOperationMsg& data)
	{
		deserialize(buffer, data.TimeStamp);
        deserialize(buffer, data.flg_merge);
        deserialize(buffer, data.merge);
        deserialize(buffer, data.flg_split);
        deserialize(buffer, data.PartationId);
        deserialize(buffer, data.p1);
        deserialize(buffer, data.p2);
	}
}

#endif //RUBBYNAV_SEGMENTOPERATIONMSG_H
