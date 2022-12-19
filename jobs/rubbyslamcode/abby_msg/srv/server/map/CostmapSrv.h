/*
 * EventTestSrv.h
 *
 *  Created on: Mar 10, 2022
 *      Author: ubuntu
 */

#pragma once

#include <memory>

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
    } request;

    struct Response
    {
        std::string savePath;
        ResultEnum result;
    } response;
};

}
