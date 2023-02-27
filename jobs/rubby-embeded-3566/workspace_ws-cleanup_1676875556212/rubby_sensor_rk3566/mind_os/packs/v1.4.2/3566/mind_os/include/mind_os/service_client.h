/*
 * ServiceClient.h
 *
 *  Created on: Aug 10, 2021
 *      Author: ubuntu
 */

#pragma once

#include <functional>
#include <memory>
#include <any>
#include "const_ptr.h"

namespace mind_os
{

class ServiceClient
{
    using CallType =
            std::function<bool(std::any&, std::any&)>;
    CallType funcCall;
    friend class NodeHandle;
public:
    ServiceClient() : funcCall(nullptr) {}
    ServiceClient(CallType funcCall) : funcCall(funcCall) {}

    bool isValid()
    {
        return funcCall != nullptr;
    }

    template <typename T>
    bool call(T& srv)
    {
        if (funcCall != nullptr)
        {
            auto req = std::make_shared<typename T::Request>();
            auto resp = std::make_shared<typename T::Response>();
            *req = srv.request;
            ConstPtr<typename T::Request> constReq = req;
            std::any anyReq = constReq;
            std::any anyResp = resp;
            auto rlt = funcCall(anyReq, anyResp);
            srv.response = *resp;
            return rlt;
        }
        return false;
    }

    template <typename T>
    bool call(ConstPtr<typename T::Request> &req, Ptr<typename T::Response> &resp)
    {
        if (funcCall != nullptr)
        {
            ConstPtr<typename T::Request> constReq = req;
            std::any anyReq = constReq;
            std::any anyResp = resp;
            return funcCall(anyReq, anyResp);
        }
        return false;
    }

    template <typename T>
    bool call(std::shared_ptr<typename T::Request> &req,
        std::shared_ptr<typename T::Response> &resp)
    {
        if (funcCall != nullptr)
        {
            ConstPtr<typename T::Request> constReq = req;
            std::any anyReq = constReq;
            std::any anyResp = resp;
            return funcCall(anyReq, anyResp);
        }
        return false;
    }

    template <typename REQ, typename RESP>
    bool call(ConstPtr<REQ> &req, Ptr<RESP> &resp)
    {
        if (funcCall != nullptr)
        {
            ConstPtr<REQ> constReq = req;
            std::any anyReq = constReq;
            std::any anyResp = resp;
            return funcCall(anyReq, anyResp);
        }
        return false;
    }

    template <typename REQ, typename RESP>
    bool call(std::shared_ptr<REQ> &req,
        std::shared_ptr<RESP> &resp)
    {
        if (funcCall != nullptr)
        {
            ConstPtr<REQ> constReq = req;
            std::any anyReq = constReq;
            std::any anyResp = resp;
            return funcCall(anyReq, anyResp);
        }
        return false;
    }

    bool call(std::any& req, std::any& resp)
    {
        if (funcCall != nullptr)
        {
            return funcCall(req, resp);
        }
        return false;
    }
};

}
