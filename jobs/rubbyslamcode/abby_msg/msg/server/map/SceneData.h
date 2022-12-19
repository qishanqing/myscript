#pragma once
#include <iostream>
#include <cstdint>

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
