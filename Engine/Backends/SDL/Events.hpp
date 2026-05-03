#pragma once

#include <vector>

#include <Core/PlatformEvents.hpp>
#include <Backends/SDL/ISDLEventSink.hpp>

namespace sokoke::backends::sdl {

class SDLEventPump final : public IPlatformEventPump {
public:
    std::vector<PlatformEvent> PollEvents() override;
    void AddEventSink(ISDLEventSink* sink);

private:
    std::vector<ISDLEventSink*> eventSinks;
};

}
