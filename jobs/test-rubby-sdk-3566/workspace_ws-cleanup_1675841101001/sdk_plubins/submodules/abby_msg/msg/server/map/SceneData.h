#pragma once

// #include <mind_os/serialization/serialization.h>

namespace map_msgs {

struct SceneData
{
    std::uint64_t   id;
    std::uint64_t   sceneId;
    std::string     dataPath;
    std::uint64_t   date;
    std::uint64_t   lastDate;
}; 

struct CollectFinish
{
    SceneData data; 
    bool success;
};

}

// namespace mind_os {
// template<>
// inline void serialize(const map_msgs::SceneData& data, BytesBuffer& buffer) {
//     buffer << data.id;
//     buffer << data.sceneId;
//     buffer << data.dataPath;
//     buffer << data.date;
//     buffer << data.lastDate;
// };

// template<>
// inline void serialize(const map_msgs::CollectFinish& data, BytesBuffer& buffer) {
//     serialize(data.data, buffer);
//     buffer << data.success;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_msgs::SceneData& data) {
//     buffer >> data.id;
//     buffer >> data.sceneId;
//     buffer >> data.dataPath;
//     buffer >> data.date;
//     buffer >> data.lastDate;
// };

// template<>
// inline void deserialize(BytesBuffer& buffer, map_msgs::CollectFinish& data) {
//     deserialize(buffer, data.data);
//     buffer >> data.success;
// };

// } /* namespace mind_os */
