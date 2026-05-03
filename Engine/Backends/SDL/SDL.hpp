
#pragma once

#include <memory>
#include <vector>

#include <Core/Backend/IBackend.hpp>
#include <Backends/SDL/Events.hpp>
#include <Backends/SDL/ISDLEventSink.hpp>
#include <Backends/SDL/Window.hpp>

namespace sokoke::backends::sdl {

class SDLBackend final : public IPlatformBackend {
public:
    SDLBackend();
    ~SDLBackend();

    SDLWindow& CreateWindow(const WindowDesc& desc) override;
    SDLEventPump& Events() override;
    TicksMS GetTicks() override;
    void AddEventSink(ISDLEventSink* sink);

private:
    SDLEventPump eventPump;
    std::vector<std::unique_ptr<SDLWindow>> windows;
};

}
