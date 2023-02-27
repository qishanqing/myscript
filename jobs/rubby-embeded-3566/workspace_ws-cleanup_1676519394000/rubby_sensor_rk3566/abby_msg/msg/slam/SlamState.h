#pragma once
#include <cstdint>
#include <mind_os/serialization/serialization.h>

namespace slam_msgs {

    /*SLAM状态位*/
    enum eSLAMState
    {
        SYSTEM_INIT = -1,       //系统初始化。--------->机器人静止
        COMPUTE_INIT_ANGLE = 0, //计算初始角度。--------->机器人静止
        RELOCATION = 1,         //重定位。--------->机器人出桩，并360度旋转
        OK = 2,                 //紧耦合正常。--------->机器人正常工作
        ONLY_ODM = 3,           //视觉失败，只有里程计工作。--------->机器人正常工作
        ONLY_VISION = 4,        //里程机打滑，只有视觉工作。--------->机器人正常工作
        KIDNAP = 5,             //绑架。当KIDNAP由0 到1 进入绑架状态，线程不处理。由1到0 进入重定位状态。--------->机器人被抱起
        FAIL = 6,               //里程机打滑，视觉失败。进入重定位状态。
        ERROR = 7,              //错误
        MAPSAVE = 8,            //保存地图
        MAPLOAD = 9,            //加载地图
        FINDLOOP = 10,          //发现回环
        
        SLAM_START = 11,
        MERGE_MAP_START = 12,
        MERGE_MAP_END = 13,
        MAP_RELOCATION_START = 14,    //地图重定位start 
        MAP_RELOCATION_FAIL = 15,     //地图重定位失败
        MAP_RELOCATION_OK = 16,       //地图重定位融合成功
        KINNAPPED = 17,               // 机器人被绑架，被抬起
        KINNAP_RELOCATION_START = 18, //机器人绑架后放在地上，开始重定位
        KINNAP_RELOCATION_FAIL = 19,  //机器人绑架重定位失败
        KINNAP_RELOCATION_OK = 20,    //机器人绑架重定位成功

    };

    struct SlamState
    {
        eSLAMState slamstate;
        int mapId = -1; // mapId < 0为错误的mapId, mapId in [0, ...)
        std::uint64_t timestamp; 
    };

}  // namespace slam_msgs

namespace mind_os{
template <>
inline void serialize(const slam_msgs::SlamState& data, BytesBuffer& buffer){
    serialize(data.slamstate, buffer);
    serialize(data.mapId, buffer);
    serialize(data.timestamp, buffer);
}

template <>
inline void deserialize(BytesBuffer& buffer, slam_msgs::SlamState& data)
{
    deserialize(buffer, data.slamstate);
    deserialize(buffer, data.mapId);
    deserialize(buffer, data.timestamp);
}
}

