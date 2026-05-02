#pragma once

#include <Core/PlatformEvents.hpp>

namespace sokoke::backends::sdl {

class SDLEventPump final : public IPlatformEventPump {
public:
    std::vector<PlatformEvent> PollEvents() override;
};

}