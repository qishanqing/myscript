#pragma once

#include <cstdint>

namespace nav_msgs {

struct CleanInfo
    {
        enum CLEAN_STATUS /*工作状态*/
        {
           CLEANINIT = 0u, /*初始化状态*/
        //     CLEAN = 1u,/*常规清扫状态*/
        //     BACK,/*回充状态*/
        //     CLEANFINISHED,/*清扫完成状态*/
            RETURNSATRTPOINT  /*回到起点*/
        };

        enum RELOCATION_STATUS
        {
            RELOCATIONINIT = 0,  /*重定位动作闲时状态*/
            RUNNING = 1, /*重定位动作正在进行*/
            FINISHEND = 2,  /*重定位动作三角走完结束*/
            NOFINISHEND = 3, /*重定位动作三角未走完结束*/
            STARTSUCCEED = 4  /*start之后发送该值*/
        };

        int Id;/*帧数*/
        int CleanState;/*工作状态*/
        int CleanTime;/*清扫时长*/
        float CleanArea;/*清扫面积*/
        int ReLocationActionState;/*重定位动作执行状态*/
        std::uint64_t TimeStamp;
    };

}
