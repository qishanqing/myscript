#pragma once

#include <mind_os/serialization/serialization.h>

namespace tutorial {
namespace event {

enum EventId {
    ERROR_1 = 0x01,
    ERROR_2_1 = 0x02,
    ERROR_2_2 = 0x03,
    ERROR_3_1 = 0x04,
    ERROR_3_2 = 0x05,
}; /* enum EventId */

struct ErrorEvent {
    EventId eventId;
    bool on;
}; /* message ErrorEvent */

} /* namespace tutorial */
} /* namespace event */

namespace mind_os {
template<>
void serialize(const tutorial::event::ErrorEvent& data, BytesBuffer& buffer) {
    serialize(data.eventId, buffer);
    serialize(data.on, buffer);
}; /* serialization of tutorial::event::ErrorEvent */

template<>
void deserialize(BytesBuffer& buffer, tutorial::event::ErrorEvent& data) {
    deserialize(buffer, data.eventId);
    deserialize(buffer, data.on);
}; /* de-serialization of tutorial::event::ErrorEvent */

} /* namespace mind_os */

