#pragma once

#include <type_traits>
#include "message_core.h"
#include "param/number.h"
#include "serialization/serialization.h"

namespace mind_os
{

class NodeHandle
{
public:
    template <typename MSG>
    Publisher advertise(std::string name)
    {
        std::function<void(std::any&, BytesBuffer&)> funcSerialize =
        [](std::any& msg, BytesBuffer& buffer) {
            serialize(*std::any_cast<ConstPtr<MSG>>(msg), buffer);
        };
        return impl->advertise(name, funcSerialize, typeid(MSG).name());
    }

    Subscriber subscribe(std::string name, std::function<void()> funcOnMessage)
    {
        auto funcDeserialize =
        [](BytesBuffer& buffer, std::any& msg) {
        };
        auto funcPackedOnMessage = [funcOnMessage](std::any& msg)
        {
            funcOnMessage();
        };
        return impl->subscribe(name, funcDeserialize, funcPackedOnMessage, nullptr);
    }

    template <typename MSG, typename FUNC,
        typename TYPE_GUARD = typename std::enable_if<
            !std::is_member_function_pointer_v<FUNC>
                && std::is_invocable_v<FUNC, ConstPtr<MSG>>>::type>
    Subscriber subscribe(std::string name, FUNC funcOnMessage)
    {
        auto funcDeserialize =
        [](BytesBuffer& buffer, std::any& msg) {
            auto pMsg = std::make_shared<MSG>();
            deserialize(buffer, *pMsg);
            ConstPtr<MSG> cMsg = pMsg;
            msg = cMsg;
        };
        auto funcPackedOnMessage = [funcOnMessage](std::any& msg)
        {
            funcOnMessage(std::any_cast<ConstPtr<MSG>>(msg));
        };
        return impl->subscribe(name, funcDeserialize, funcPackedOnMessage, typeid(MSG).name());
    }

    template <typename MSG, typename FUNC, typename OBJECT,
        typename TYPE_GUARD = typename std::enable_if<
            std::is_member_function_pointer_v<FUNC>
                && std::is_class_v<OBJECT>>::type>
    Subscriber subscribe(std::string name, FUNC func, OBJECT* obj)
    {
        auto funcOnMessage = std::bind(func, obj, std::placeholders::_1);
        auto funcDeserialize =
        [](BytesBuffer& buffer, std::any& msg) {
            auto pMsg = std::make_shared<MSG>();
            deserialize(buffer, *pMsg);
            ConstPtr<MSG> cMsg = pMsg;
            msg = cMsg;
        };
        auto funcPackedOnMessage = [funcOnMessage](std::any& msg)
        {
            funcOnMessage(std::any_cast<ConstPtr<MSG>>(msg));
        };
        return impl->subscribe(name, funcDeserialize, funcPackedOnMessage, typeid(MSG).name());
    }

    template <typename SRV, typename FUNC,
        typename TYPE_GUARD = typename std::enable_if<
            !std::is_member_function_pointer_v<FUNC>
                && std::is_invocable_v<FUNC, ConstPtr<typename SRV::Request>, Ptr<typename SRV::Response>>>::type>
    ServiceServer advertiseService(std::string name, FUNC funcOnCalled)
    {
        std::function<void(std::any&, BytesBuffer&)> funcSerializeResponse =
        [](std::any& msg, BytesBuffer& buffer) {
            ConstPtr<typename SRV::Response> pReq = std::any_cast<Ptr<typename SRV::Response>>(msg);
            serialize(*pReq, buffer);
        };
        auto funcDeserializeRequest=
        [](BytesBuffer& buffer, std::any& msg) {
            auto pMsg = std::make_shared<typename SRV::Request>();
            deserialize(buffer, *pMsg);
            ConstPtr<typename SRV::Request> cMsg = pMsg;
            msg = cMsg;
        };
        auto funcPackedOnCalled = [funcOnCalled](std::any& req, std::any& resp)
        {
            if (resp.has_value())
            {
                funcOnCalled(std::any_cast<ConstPtr<typename SRV::Request>>(req),
                    std::any_cast<Ptr<typename SRV::Response>>(resp));
            }
            else
            {
                auto pResp = std::make_shared<typename SRV::Response>();
                resp = pResp;
                funcOnCalled(std::any_cast<ConstPtr<typename SRV::Request>>(req), pResp);
            }
        };
        return impl->advertiseService(name, funcSerializeResponse,
            funcDeserializeRequest, funcPackedOnCalled, typeid(SRV).name());
    }

    template <typename SRV, typename FUNC, typename OBJECT,
        typename TYPE_GUARD = typename std::enable_if<
            std::is_member_function_pointer_v<FUNC>
                && std::is_class_v<OBJECT>>::type>
    ServiceServer advertiseService(std::string name, FUNC func, OBJECT* obj)
    {
        auto funcOnCalled = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
        std::function<void(std::any&, BytesBuffer&)> funcSerializeResponse =
        [](std::any& msg, BytesBuffer& buffer) {
            ConstPtr<typename SRV::Response> pReq = std::any_cast<Ptr<typename SRV::Response>>(msg);
            serialize(*pReq, buffer);
        };
        auto funcDeserializeRequest=
        [](BytesBuffer& buffer, std::any& msg) {
            auto pMsg = std::make_shared<typename SRV::Request>();
            deserialize(buffer, *pMsg);
            ConstPtr<typename SRV::Request> cMsg = pMsg;
            msg = cMsg;
        };
        auto funcPackedOnCalled = [funcOnCalled](std::any& req, std::any& resp)
        {
            if (resp.has_value())
            {
                funcOnCalled(std::any_cast<ConstPtr<typename SRV::Request>>(req),
                    std::any_cast<Ptr<typename SRV::Response>>(resp));
            }
            else
            {
                auto pResp = std::make_shared<typename SRV::Response>();
                resp = pResp;
                funcOnCalled(std::any_cast<ConstPtr<typename SRV::Request>>(req), pResp);
            }
        };
        return impl->advertiseService(name, funcSerializeResponse,
            funcDeserializeRequest, funcPackedOnCalled, typeid(SRV).name());
    }

    template <typename SRV>
    ServiceClient serviceClient(std::string name)
    {
        std::function<void(std::any&, BytesBuffer&)> funcSerializeRequest =
        [](std::any& msg, BytesBuffer& buffer) {
             serialize(*std::any_cast<ConstPtr<typename SRV::Request>>(msg), buffer);
        };
        auto funcDeserializeResponse =
        [](BytesBuffer& buffer, std::any& msg) {
             deserialize(buffer, *std::any_cast<Ptr<typename SRV::Response>>(msg));
        };
        return impl->serviceClient(name, funcSerializeRequest, funcDeserializeResponse, typeid(SRV).name());
    }

    template<typename T, typename TYPE_GUARD = typename std::enable_if<std::is_arithmetic_v<T>>::type>
    bool setParam(std::string name, T value)
    {
        return impl->setParam(name, std::to_string(value));
    }

    bool setParam(std::string name, std::string value)
    {
        return impl->setParam(name, value);
    }

    template<typename T, typename TYPE_GUARD = typename std::enable_if<std::is_arithmetic_v<T>>::type>
    bool getParam(std::string name, T& value)
    {
        std::string rawValue;
        auto rlt = impl->getParam(name, rawValue);
        if (!rlt)
        {
            return false;
        }
        stringToNumber(rawValue, value);
        return true;
    }

    bool getParam(std::string name, std::string& value)
    {
        auto rlt = impl->getParam(name, value);
        if (!rlt)
        {
            return false;
        }
        return true;
    }

private:
    inline void spin()
    {
        impl->spin();
    }
    void stop()
    {
        impl->stop();
    }

private:
    MessageCore* impl;
    NodeHandle();
    friend class NodePlugin;
};

}
