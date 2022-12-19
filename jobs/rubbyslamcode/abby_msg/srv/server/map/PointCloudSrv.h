#pragma once

#include <memory>

namespace map_srvs {

struct PointCloud
{
    std::uint64_t   id;
    std::uint64_t   sceneId;
    std::uint32_t   useCount;
    std::string     image;
    std::string     thumbnail;
    std::uint64_t   date;
    std::uint64_t   lastDate;
    bool valid;
};

struct PointCloudNew
{
    PointCloudNew() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        bool success;
        PointCloud map;
    } response;
};

struct PointCloudDel
{
    PointCloudDel() = default;

    struct Request
    {
        std::uint64_t mapId;
    } request;

    struct Response
    {
        bool success;
    } response;
};

struct PointCloudQuery
{
    PointCloudQuery() = default;

    struct Request
    {
        std::uint64_t mapId;
    } request;

    struct Response
    {
        PointCloud map;
        bool success;
    } response;
};

struct PointCloudUpdate
{
    PointCloudUpdate() = default;

    struct Request
    {
        PointCloud map;
    } request;

    struct Response
    {
        bool success;
        PointCloud map;
    } response;
};

struct PointCloudDefault
{
    enum MapEnum{
        NewMap,
        Default,
    };

    PointCloudDefault() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        MapEnum mapState;
        std::uint64_t mapId;
    } response;
};

}
