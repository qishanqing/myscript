/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include <memory>

namespace map_srvs {

struct Scene
{
    std::uint64_t   id;
    std::string     alias;
    std::uint32_t   useCount;
    std::string     sceneImage;
    std::string     sceneThumbnail;
    std::uint64_t   date;
    std::uint64_t   lastDate;
};

struct SceneAdd
{
    SceneAdd() = default;

    struct Request
    {
        std::uint64_t timestamp;
    } request;

    struct Response
    {
        bool success;
        Scene scene;
    } response;
};

struct SceneDel
{
    SceneDel() = default;

    struct Request
    {
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        bool success;
    } response;
};

struct SceneQuery
{
    SceneQuery() = default;

    struct Request
    {
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        Scene scene;
        bool success;
    } response;
};

struct SceneUpdate
{
    SceneUpdate() = default;

    struct Request
    {
        Scene scene;
    } request;

    struct Response
    {
        bool success;
        Scene scene;
    } response;
};

struct SceneDefault
{
    enum MapEnum{
        NewMap,
        Default,
    };

    SceneDefault() = default;

    struct Request
    {
        std::uint64_t timestamp;
    } request;

    struct Response
    {
        MapEnum mapState;
        std::uint64_t sceneId;
    } response;
};

}
