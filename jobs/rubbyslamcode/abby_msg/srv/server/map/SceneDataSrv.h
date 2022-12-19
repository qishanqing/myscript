/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include <memory>

namespace map_srvs {

struct SceneData
{
    std::uint64_t   id;
    std::uint64_t   sceneId;
    std::string     dataPath;
    std::uint64_t   date;
    std::uint64_t   lastDate;
}; 

struct SceneDataCollect
{
    SceneDataCollect() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        bool success;
    } response;
};

struct SceneDataAdd
{
    SceneDataAdd() = default;

    struct Request
    {
        std::uint64_t timestamp;
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        bool success;
        SceneData data;
    } response;
};

struct SceneDataDel
{
    SceneDataDel() = default;

    struct Request
    {
        std::uint64_t sceneDataId;
    } request;

    struct Response
    {
        bool success;
    } response;
};

struct SceneDataQuery
{
    SceneDataQuery() = default;

    struct Request
    {
        std::uint64_t sceneId;
    } request;

    struct Response
    {
        SceneData data;
        bool success;
    } response;
};

}
