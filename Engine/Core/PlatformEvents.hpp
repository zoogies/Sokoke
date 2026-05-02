#pragma once

#include <cstdint>
#include <variant>
#include <vector>

namespace sokoke {

// im going to implement the events only as i need them, for now

typedef uint32_t WindowID;

struct AppQuitRequestedEvent {};

struct WindowCloseRequestedEvent {
    WindowID windowID;
};

using PlatformEvent = std::variant<
    AppQuitRequestedEvent,
    WindowCloseRequestedEvent
>;

/*
    Event Pump / Dispatcher
*/

class IPlatformEventPump {
public:
    virtual std::vector<PlatformEvent> PollEvents() = 0;
};

}
