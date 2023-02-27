namespace tutorial.event

enum EventId {
    ERROR_1: 0x01 // immediate event
    ERROR_2_1: 0x02 // condition event timeout
    ERROR_2_2: 0x03 // condition event timeout2
    ERROR_3_1: 0x04 // condition event trigger
    ERROR_3_2: 0x05 // condition event trigger2
}

message ErrorEvent {
    eventId: EventId
    on: bool
}