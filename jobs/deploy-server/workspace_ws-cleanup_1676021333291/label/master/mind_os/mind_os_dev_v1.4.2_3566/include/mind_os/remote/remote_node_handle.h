#pragma once

#include <type_traits>
#include <functional>
#include <any>
#include <mutex>

#include "remote_message_core.h"
#include "../node_handle.h"
#include "../no_copy.h"

namespace mind_os
{

class RemoteNodeHandle : public NoCopy
{
public:
    // mode - 0 : manual
    RemoteNodeHandle();

    // mode - 1 : automatic
    RemoteNodeHandle(NodeHandle& nh);
    RemoteNodeHandle(NodeHandle* pNh);

    bool startAsServer(RemoteSetting& setting,
            std::function<void(bool)> funcOnConntected = nullptr);

    bool startAsClient(RemoteSetting& setting,
            std::function<void(bool)> funcOnConntected = nullptr,
            std::function<void()> funcOnTimeout = nullptr);

    bool isRunning();

    void stop();

    template <typename MSG>
    Publisher advertise(std::string name)
    {
        if (!running)
        {
            return Publisher();
        }

        std::function<void(std::any&, BytesBuffer&)> funcSerialize =
        [](std::any& msg, BytesBuffer& buffer) {
            serialize(*std::any_cast<ConstPtr<MSG>>(msg), buffer);
        };

        auto pub = impl->advertise(name, funcSerialize, typeid(MSG).name());

        Subscriber sub;
        if (pNh != nullptr) {
            sub = pNh->subscribe<MSG>(name, std::bind([](Publisher pub, ConstPtr<MSG>& msg){
                pub.publish(msg);
            }, pub, std::placeholders::_1));

            auto funcShutdown = std::bind([](Subscriber sub){
                sub.shutdown();
            }, sub);

            pub.funcShutdown = funcShutdown;
        }

        return pub;
    }

    Subscriber subscribe(std::string name, std::function<void()> funcOnMessage)
    {
        if (!running)
        {
            return Subscriber();
        }
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
        if (!running)
        {
            return Subscriber();
        }
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
        if (!running)
        {
            return Subscriber();
        }
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

    template <typename SRV>
    ServiceServer advertiseService(std::string name)
    {
        if (!running)
        {
            return ServiceServer();
        }

        ServiceServer ss;
        if (pNh != nullptr)
        {
            auto funcOnCalled = std::bind([name](NodeHandle* pNh, ConstPtr<typename SRV::Request>& request, Ptr<typename SRV::Response>& response) {
                auto client = pNh->serviceClient<SRV>(name);
                return client.call(request, response);
            }, pNh, std::placeholders::_1, std::placeholders::_2);
            ss = advertiseService<SRV>(name, funcOnCalled);
        }
//        else
//        {
//            auto funcOnCalled = [](ConstPtr<typename SRV::Request>& request, Ptr<typename SRV::Response>& response) {
//                return false;
//            };
//            ss = advertiseService<SRV>(name, funcOnCalled);
//        }
        return ss;
    }

    template <typename SRV, typename FUNC,
        typename TYPE_GUARD = typename std::enable_if<
            !std::is_member_function_pointer_v<FUNC>
                && std::is_invocable_v<FUNC, ConstPtr<typename SRV::Request>, Ptr<typename SRV::Response>>>::type>
    ServiceServer advertiseService(std::string name, FUNC funcOnCalled)
    {
        if (!running)
        {
            return ServiceServer();
        }
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
        if (!running)
        {
            return ServiceServer();
        }
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
        if (!running)
        {
            return ServiceClient();
        }
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


private:
    std::shared_ptr<RemoteMessageCore> impl;
    NodeHandle* pNh;
    std::mutex mtxOpt;
    bool running;
};

}
