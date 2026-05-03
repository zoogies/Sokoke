#pragma once

#include <SDL3/SDL_events.h>

namespace sokoke::backends::sdl {

class ISDLEventSink {
public:
    virtual bool ProcessSDLEvent(const SDL_Event& event) = 0;
    virtual ~ISDLEventSink() = default;
};

}