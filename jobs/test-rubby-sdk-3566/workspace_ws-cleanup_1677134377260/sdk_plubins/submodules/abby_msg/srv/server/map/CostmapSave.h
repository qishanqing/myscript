#pragma once

#include <mind_os/serialization/serialization.h>

namespace map_srvs {

struct CostmapSave
{
    enum ResultEnum{
        Success         = 1,
        SlamAllMapErr   = 2,
        NavDesErr       = 3,
        BeautifyMapErr  = 4,
        SaveMapErr      = 5,
    };
    CostmapSave() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
        std::uint64_t mapId;
    } request;

    struct Response
    {
        std::string savePath;
        ResultEnum result;
    } response;
};

}

namespace mind_os {
template<>
inline void serialize(const map_srvs::CostmapSave::Request& data, BytesBuffer& buffer) {
    serialize(buffer,data.timestamp);
    serialize(buffer,data.sceneId);
    serialize(buffer,data.mapId);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::CostmapSave::Request& data) {
    deserialize(buffer, data.timestamp);
    deserialize(buffer, data.timestamp);
    deserialize(buffer, data.mapId);
};

template<>
inline void serialize(const map_srvs::CostmapSave::Response& data, BytesBuffer& buffer) {
    serialize(buffer,data.savePath);
    serialize(buffer,data.result);
};

template<>
inline void deserialize(BytesBuffer& buffer, map_srvs::CostmapSave::Response& data) {
    deserialize(buffer, data.savePath);
    deserialize(buffer, data.result);
};

} /* namespace mind_os */