//
// Created by guest on 2022/3/21.
//
#pragma once
#include <string>
#include "Object.h"
#include <mind_os/serialization/serialization.h>

namespace perception_msgs
{
    struct SenmaticMap
    {
        enum Status
        {
            NO_UPDATE = 0, UPDATE
        };

        std::vector <Object> objects;
        bool finalUpdate = false;    //  final update befor the task close;
        Status status = NO_UPDATE;
    };
}

namespace mind_os
{
    template <>
    inline void serialize(const perception_msgs::SenmaticMap &data, BytesBuffer& buffer)
    {
        serialize(data.objects, buffer);
        serialize(data.finalUpdate, buffer);
        serialize(data.status, buffer);
    } /* serialization of perception_msgs::SenmaticMap */

    template <>
    inline void deserialize(BytesBuffer& buffer, perception_msgs::SenmaticMap& data)
    {
        deserialize(buffer, data.objects);
        deserialize(buffer, data.finalUpdate);
        deserialize(buffer, data.status);
    } /* de-serialization of perception_msgs::SenmaticMap */
};



