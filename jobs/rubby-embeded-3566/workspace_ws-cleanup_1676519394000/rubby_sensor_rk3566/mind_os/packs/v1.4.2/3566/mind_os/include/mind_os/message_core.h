#pragma once

#include <functional>
#include <any>

#include "service_server.h"
#include "service_client.h"
#include "subscriber.h"
#include "publisher.h"
#include "const_ptr.h"
#include "serialization/bytes_buffer.h"

namespace mind_os
{
class MessageCore
{
public:
    virtual ~MessageCore() {}

    virtual Publisher advertise(std::string name,
            std::function<void(std::any&, BytesBuffer&)> funcSerialize,
            const char* typeName) = 0;

    virtual Subscriber subscribe(std::string name,
            std::function<void(BytesBuffer&, std::any&)> funcDeserialize,
            std::function<void(std::any&)> funcOnMessage,
            const char* typeName) = 0;

    virtual ServiceServer advertiseService(std::string name,
            std::function<void(std::any&, BytesBuffer&)> funcSerializeResponse,
            std::function<void(BytesBuffer&, std::any&)> funcDeserializeRequest,
            std::function<void(std::any&, std::any&)> funcOnCalled,
            const char* typeName) = 0;

    virtual ServiceClient serviceClient(std::string name,
            std::function<void(std::any&, BytesBuffer&)> funcSerializeRequest,
            std::function<void(BytesBuffer&, std::any&)> funcDeserializeResponse,
            const char* typeName) = 0;

    virtual bool getParam(std::string& key, std::string& value) = 0;
    virtual bool setParam(std::string& key, std::string value) = 0;

    virtual void spin() = 0;
    virtual void stop() = 0;
};

}
