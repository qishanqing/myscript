//
// Created by guest on 2022/3/21.
//
#pragma once
#include <mind_os/serialization/serialization.h>

namespace perception_msgs
{
    using LOCATION_TYPE = double;

    struct Location
    {
        LOCATION_TYPE x;
        LOCATION_TYPE y;
        LOCATION_TYPE z;
    };

    struct Object
    {
        enum StatusOperate
        {
            INITED_DL = 0, UPDATE_DL, //  [dl]
            INITED_SLAM, UPDATE_SLAM, //  [slam]
        };
        enum ClassID
        {
            UNKNOWN = 0, SHOES, BIN, PEDESTAL, WIRE, SOCKET, CAT, DOG, DESK_RECT, DESK_CIRCLE
        };

        enum Status
        {
            ENABLE = 0, DISABLE
        };

        int id = -1;    // object ID
        ClassID classID = UNKNOWN; // senmatic id
        float score;
        float detectExtend;
        // convex for object; one point for person, two point for escalator
        std::vector <Location> locationWorld; // convex
        std::uint64_t time;
        StatusOperate statusOperator; // the status of operator
        signed long int nodeID;       // node id for slam [slam]
        bool history = true; // is load from offline map [slam]
        Status status = DISABLE; // for navigation
    };
}

namespace mind_os
{
    template <>
    inline void serialize(const perception_msgs::Location& data, BytesBuffer &buffer)
    {
        serialize(data.x, buffer);
        serialize(data.y, buffer);
        serialize(data.z, buffer);
    } /* serialization of perception_msgs::Location */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_msgs::Location& data)
    {
        deserialize(buffer, data.x);
        deserialize(buffer, data.y);
        deserialize(buffer, data.z);
    } /* de-serialization of perception_msgs::Location */

    template <>
    inline void serialize(const perception_msgs::Object& data, BytesBuffer &buffer)
    {
        serialize(data.id, buffer);
        serialize(data.classID, buffer);
        serialize(data.score, buffer);
        serialize(data.detectExtend, buffer);
        serialize(data.locationWorld, buffer);
        serialize(data.time, buffer);
        serialize(data.statusOperator, buffer);
        serialize(data.nodeID, buffer);
        serialize(data.history, buffer);
        serialize(data.status, buffer);
    } /* serialization of perception_msgs::Object */

    template <>
    inline void deserialize(BytesBuffer &buffer, perception_msgs::Object& data)
    {
        deserialize(buffer, data.id);
        deserialize(buffer, data.classID);
        deserialize(buffer, data.score);
        deserialize(buffer, data.detectExtend);
        deserialize(buffer, data.locationWorld);
        deserialize(buffer, data.time);
        deserialize(buffer, data.statusOperator);
        deserialize(buffer, data.nodeID);
        deserialize(buffer, data.history);
        deserialize(buffer, data.status);
    } /* de-serialization of perception_msgs::Object */
};